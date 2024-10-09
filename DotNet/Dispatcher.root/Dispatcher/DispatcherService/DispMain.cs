using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Security.Principal;
using System.ServiceProcess;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Dispatcher
{
    public partial class DispMain : ServiceBase , IDisposable
    {
        public static readonly string SERVICE_NAME = "DispatcherService";
                
        TcpManager MyTcpManager1;
        TcpManager MyTcpManager2;
        TcpManager MyTcpManager3;
        bool IsAlive;
        SynchronizedCollection<PipeServer> MyPipeManager = new SynchronizedCollection<PipeServer>();
        private ConcurrentQueue<DispaMessageCls> SafeQueue;
        private Task mPipeSend;
        MemoryMappedFile mMmFile = null;
        private DsLog mLog;
        private string ServerIp;
        private bool IsDebug;
        private uint TotalCount;
        CancellationTokenSource TraceCancel;
        Task task;
        int threshhold = 100;

        /// <summary>
        /// 各状態の番号
        /// </summary>
        enum StatusNo
        {
            Pipe = 0,
            PipeCount,
            TCP50001,
            TCP50002,
            TCP50003,
            Ip1,
            Ip2,
            Ip3,
            Ip4,
            VerMajor,
            VerMinor,
            VerBuild,
            VerRev,
        }

        /// <summary>
        /// 共有メモリのデータ
        /// </summary>
        byte[] mLogData;


        /// <summary>
        /// コンストラクタ
        /// </summary>
        public DispMain()
        {
            InitializeComponent();

            IsDebug = false;
            IsAlive = true;
            TotalCount = 0;

            SafeQueue = new ConcurrentQueue<DispaMessageCls>();

            //ログ用
            mLog = new DsLog();
            mLogData = new byte[Enum.GetNames(typeof(StatusNo)).Length];

            try
            {
                //バージョン取得            
                Version vs = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version;
                string[] verStr = vs.ToString().Split('.');
                mLogData[(int)StatusNo.VerMajor] = byte.Parse(verStr[0]);
                mLogData[(int)StatusNo.VerMinor] = byte.Parse(verStr[1]);
                mLogData[(int)StatusNo.VerBuild] = byte.Parse(verStr[2]);
                mLogData[(int)StatusNo.VerRev] = byte.Parse(verStr[3]);

                //DispatcherSetting.configを読み込む
                ServerIp = ConfigurationManager.AppSettings["ServerIp"];
                string IsDebugLog = ConfigurationManager.AppSettings["DebugLog"];
                mLog.WriteLog("DispMain", "***** Start *****");
                mLog.WriteLog("DispMain", "read ConfigurationManager. ip=" + ServerIp + " debug=" + IsDebugLog);

                if (IsDebugLog.ToUpper() == "TRUE")
                {
                    IsDebug = true;
                    mLog.SetDebugFlag(true);
                }

                //サーバーIPアドレス取得
                IPAddress adr = IPAddress.Parse(ServerIp);
                byte[] adrb = adr.GetAddressBytes();
                mLogData[(int)StatusNo.Ip1] = adrb[0];
                mLogData[(int)StatusNo.Ip2] = adrb[1];
                mLogData[(int)StatusNo.Ip3] = adrb[2];
                mLogData[(int)StatusNo.Ip4] = adrb[3];

                WriteLogMemory(mLogData);
            }
            catch(Exception ex)
            {
                mLog.WriteLog("DispMain", ex.Message);
                throw;
            }
        }


        bool StopMessageFlag = false;

        //----------------------------------------
        // protected
        //----------------------------------------
        /// <summary>
        /// OnStart
        /// </summary>
        protected override void OnStart(string[] args)
        {
            MyTcpManager3 = new TcpManager(ServerIp, 50003);
            MyTcpManager3.RecvTcpCmdEvent += MyTcpManager3_RecvTcpCmdEvent;
            MyTcpManager3.TcpCloseEvent +=MyTcpManager3_TcpCloseEvent;
            MyTcpManager3.TcpOpenEvent += MyTcpManager3_TcpOpenEvent;
            MyTcpManager3.RecvTcpExcepEvent += MyTcpManager3_RecvTcpExcepEvent;
            MyTcpManager3.SoketRecv();

            //パイプメッセージキューを送信する処理            
            mPipeSend = Task.Factory.StartNew(() =>
            {
                while (IsAlive)
                {
                    if(StopMessageFlag)
                    {
                        continue;
                    }
                    DispaMessageCls msgData;
                    if (SafeQueue.TryDequeue(out msgData))
                    {
                        lock (MyPipeManager)
                        {
                            for (int i = 0; i < MyPipeManager.Count; i++)
                            {
                                MyPipeManager[i].SendMessageEnqueue(msgData);
                            }
                        }
                    }
                    else
                    {
                        Thread.Sleep(1);
                    }
                }
            });

            //パイプ受信待ち処理
            
            //パイプ受信待ち処理(EX)
            CreatePipeTask("STATIONEX01_", 1);
            CreatePipeTask("STATIONEX02_", 2);
            CreatePipeTask("STATIONEX03_", 3);
            CreatePipeTask("STATIONEX04_", 4);
            //パイプ受信待ち処理
            CreatePipeTaskFa("STATION01_", 1);
            CreatePipeTaskFa("STATION02_", 2);
            CreatePipeTaskFa("STATION03_", 3);
            CreatePipeTaskFa("STATION04_", 4);
        }

        private void CreatePipeTaskFa(string pipename, int kyoku_id)
        {
            //パイプ受信待ち処理
            Task.Factory.StartNew(() =>
            {
                TotalCount++;
                PipeServer pipe = new PipeServer(pipename, TotalCount, 500, kyoku_id, PipeServer.RecvMessageType.KanshiOnly);
                pipe.PipeConnectEvent += pipe_PipeConnectEvent;
                pipe.RecvCommandEvent += pipe_RecvCommandEvent;
                pipe.PipeDisconnectEvent += pipe_PipeDisconnectEvent;
                pipe.RecvPipeExcepEvent += pipe_RecvPipeExcepEvent;
                pipe.WaitConnect();
                StopMessageFlag = true;
                MyPipeManager.Add(pipe);
                StopMessageFlag = false;
            });
        }

        /// <summary>
        /// パイプ受信待ち処理(EX)
        /// </summary>
        /// <param name="pipename"></param>
        /// <param name="kyoku_id"></param>
        private void CreatePipeTask(string pipename, int kyoku_id)
        {
            Task.Factory.StartNew(() =>
            {
                while (IsAlive)
                {
                    int count = MyPipeManager.Count;
                    if (count < PipeServer.MaxCount)
                    {
                        TotalCount++;
                        PipeServer pipe = new PipeServer(pipename, TotalCount, int.MaxValue, kyoku_id, PipeServer.RecvMessageType.All);
                        pipe.PipeConnectEvent += pipe_PipeConnectEvent;
                        pipe.RecvCommandEvent += pipe_RecvCommandEvent;
                        pipe.PipeDisconnectEvent += pipe_PipeDisconnectEvent;
                        pipe.RecvPipeExcepEvent += pipe_RecvPipeExcepEvent;
                        pipe.WaitConnect();
                        StopMessageFlag = true;
                        MyPipeManager.Add(pipe);
                        StopMessageFlag = false;
                    }
                    else
                    {
                        Thread.Sleep(1000);
                    }
                }
            });

        }


        /// <summary>
        /// OnStop
        /// </summary>
        protected override void OnStop()
        {
            mLog.WriteLog("System", "*****OnStop*****");

            SessionPingClose();

            if (MyTcpManager1 != null)
                MyTcpManager1.Close();
            if (MyTcpManager2 != null)
                MyTcpManager2.Close();
            if (MyTcpManager3 != null)
                MyTcpManager3.Close();

            //強制的に切断状態にする
            mLogData[(int)StatusNo.TCP50001] = (byte)TcpManager.Status.Disconnect;
            mLogData[(int)StatusNo.TCP50002] = (byte)TcpManager.Status.Disconnect;
            mLogData[(int)StatusNo.TCP50003] = (byte)TcpManager.Status.Disconnect;
            WriteLogMemory(mLogData);

            IsAlive = false;

            lock (MyPipeManager)
            {
                for (int i = 0; i < MyPipeManager.Count; i++)
                {
                    MyPipeManager[i].Close();
                }
            }

            mMmFile.Dispose();

            mLog.Close();

            MyPipeManager.Clear();
        }

        /// <summary>
        /// OnShutdown
        /// </summary>
        protected override void OnShutdown()
        {
            mLog.WriteLog("DispMain", "OnShutdown");

            OnStop();
        }


        #region パイプ通信処理
        //----------------------------------------
        // パイプ通信処理
        //----------------------------------------

        /// <summary>
        /// 接続Event
        /// </summary>
        void pipe_PipeConnectEvent(object sender, EventArgs e)
        {
            PipeServer ps = sender as PipeServer;

            //接続状態になった
            int count = MyPipeManager.Count + 1;
            mLogData[(int)StatusNo.Pipe] = (byte)PipeServer.Status.Connect;
            mLogData[(int)StatusNo.PipeCount] = (byte)(count);
            WriteLogMemory(mLogData);

            mLog.WriteLog("Pipe", "Connect count=" + count + " name=" + ps.PipeNameIn);
        }

        /// <summary>
        /// 切断Event
        /// </summary>
        void pipe_PipeDisconnectEvent(object sender, EventArgs e)
        {
            PipeServer ps = sender as PipeServer;
            string pipeName = ps.NamaPipeName;
            int kyokuid = ps.KyokuID;
            PipeServer.RecvMessageType type = ps.Type;
            MyPipeManager.Remove(ps);

            //Memoryに書き込み
            int count = MyPipeManager.Count;
            if (count == 0)
            {
                mLogData[(int)StatusNo.Pipe] = (byte)PipeServer.Status.Disconnect;
            }
            mLogData[(int)StatusNo.PipeCount] = (byte)(count);
            WriteLogMemory(mLogData);

            mLog.WriteLog("Pipe", "disconnect count=" + count);

            //再接続する
            if (type == PipeServer.RecvMessageType.KanshiOnly)
            {
                CreatePipeTaskFa(pipeName, kyokuid);
            }
        }

        /// <summary>
        /// 要求・応答Message
        /// </summary>
        void pipe_RecvCommandEvent(object sender, PipeEventArg e)
        {
            PipeServer ps = sender as PipeServer;

            MsgLog("Pipe ID:" + ps.Id.ToString(), e.recvMessage);

            //オウム返し
            ps.SendMessage(e.recvMessage);
        }

        /// <summary>
        /// 異常系Event
        /// </summary>
        void pipe_RecvPipeExcepEvent(object sender, PipeErrorEventArg e)
        {
            mLog.WriteLog("Pipe", e.Msg);

            if (!string.IsNullOrEmpty(e.InnerExceptionMsg))
            {
                mLog.WriteLog("Pipe", "[Inner]" + e.InnerExceptionMsg);
            }

            if (!string.IsNullOrEmpty(e.Soruce))
            {
                mLog.WriteLog("Pipe", "[Soruce]" + e.Soruce);
            }
        }


        #endregion

        #region TCP通信処理
        //----------------------------------------
        // TCP通信処理
        //----------------------------------------

        /// <summary>
        /// ポート50003が接続
        /// </summary>
        void MyTcpManager3_TcpOpenEvent(object sender, EventArgs e)
        {
            //if (MyTcpManager1 != null)
            //{
            //    MyTcpManager1.Close();
            //}
            //if (MyTcpManager2 != null)
            //{
            //    MyTcpManager2.Close();
            //}

            mLogData[(int)StatusNo.TCP50003] = (byte)TcpManager.Status.Connect;
            WriteLogMemory(mLogData);

            mLog.WriteLog("TCP50003", "connect");

            MyTcpManager1 = new TcpManager(ServerIp, 50001);
            MyTcpManager1.RecvTcpCmdEvent += MyTcpManager1_RecvTcpCmdEvent;
            MyTcpManager1.TcpCloseEvent += MyTcpManager1_TcpCloseEvent;
            MyTcpManager1.TcpOpenEvent += MyTcpManager1_TcpOpenEvent;
            MyTcpManager1.RecvTcpExcepEvent += MyTcpManager1_RecvTcpExcepEvent;
            MyTcpManager1.SoketRecv();

            MyTcpManager2 = new TcpManager(ServerIp, 50002);
            MyTcpManager2.RecvTcpCmdEvent += MyTcpManager2_RecvTcpCmdEvent;
            MyTcpManager2.TcpCloseEvent += MyTcpManager2_TcpCloseEvent;
            MyTcpManager2.TcpOpenEvent += MyTcpManager2_TcpOpenEvent;
            MyTcpManager2.RecvTcpExcepEvent += MyTcpManager2_RecvTcpExcepEvent;
            MyTcpManager2.SoketRecv();

            SessionPing();
        }

        /// <summary>
        /// ポート50003が切断
        /// </summary>
        void MyTcpManager3_TcpCloseEvent(object sender, EventArgs e)
        {
            //制御コマンドが切断した
            mLogData[(int)StatusNo.TCP50003] = (byte)TcpManager.Status.Disconnect;
            WriteLogMemory(mLogData);

            mLog.WriteLog("TCP50003", "disconnect");

            allPortClose();
        }

        /// <summary>
        /// ポート50003で転送
        /// </summary>
        void MyTcpManager3_RecvTcpCmdEvent(object sender, TcpEventArg e)
        {
            if (e.DisMessage.MessageBin.Length == 0)
            {
                return;
            }

            MsgLog("TCP50003", e.DisMessage.MessageBin);

            if (e.SendFlag)
            {
                uint cmdId = (uint)e.DisMessage.CommandId;
                if (SessionPingMessage.DEF_ACK == cmdId ||
                    SessionPingMessage.DEF_REQ == cmdId)
                {
                    //生存確認通信は端末に送信しない
                    mLog.WriteLog_Debug("TCP50003", "[SessionPingAck] recv.");
                }
                else
                {
                    SafeQueue.Enqueue(e.DisMessage);
                    WriteQueueCountLog("TCP50003");
                }
            }
            else
            {
                mLog.WriteLog("TCP50003", "do not send to tarminal. 16kb over.");
            }
        }

        /// <summary>
        /// ポート50003の異常系Event
        /// </summary>
        void MyTcpManager3_RecvTcpExcepEvent(object sender, TcpErrorEventArg e)
        {
            mLog.WriteLog("TCP50003", e.Msg);
        }

        /// <summary>
        /// ポート50002が接続
        /// </summary>
        void MyTcpManager2_TcpOpenEvent(object sender, EventArgs e)
        {
            //接続状態になった
            mLogData[(int)StatusNo.TCP50002] = (byte)TcpManager.Status.Connect;
            WriteLogMemory(mLogData);

            mLog.WriteLog("TCP50002", "connect");
        }

        /// <summary>
        /// ポート50002が切断
        /// </summary>
        void MyTcpManager2_TcpCloseEvent(object sender, EventArgs e)
        {
            //切断状態になった
            mLogData[(int)StatusNo.TCP50002] = (byte)TcpManager.Status.Disconnect;
            WriteLogMemory(mLogData);

            mLog.WriteLog("TCP50002", "disconnect");

            allPortClose();
        }

        /// <summary>
        /// ポート50002で転送
        /// </summary>
        void MyTcpManager2_RecvTcpCmdEvent(object sender, TcpEventArg e)
        {
            if (e.DisMessage.MessageBin.Length == 0)
            {
                return;
            }

            MsgLog("TCP50002", e.DisMessage.MessageBin);
            //mLog.WriteLog("TCP50002", string.Format("cmd={0} kyoku={1}", e.DisMessage.CommandId, e.DisMessage.KyokuId));

            if (e.SendFlag)
            {
                SafeQueue.Enqueue(e.DisMessage);
                WriteQueueCountLog("TCP50002");
            }
            else
            {
                mLog.WriteLog("TCP50002", "do not send to tarminal. 16kb over.");
            }
        }

        /// <summary>
        /// ポート50002の異常系Event
        /// </summary>
        void MyTcpManager2_RecvTcpExcepEvent(object sender, TcpErrorEventArg e)
        {
            mLog.WriteLog("TCP50002", e.Msg);
        }

        /// <summary>
        /// ポート50001が接続
        /// </summary>
        void MyTcpManager1_TcpOpenEvent(object sender, EventArgs e)
        {
            //接続状態になった
            mLogData[(int)StatusNo.TCP50001] = (byte)TcpManager.Status.Connect;
            WriteLogMemory(mLogData);

            mLog.WriteLog("TCP50001", "connect");
        }

        /// <summary>
        /// ポート50001が切断
        /// </summary>
        void MyTcpManager1_TcpCloseEvent(object sender, EventArgs e)
        {
            //切断状態になった
            mLogData[(int)StatusNo.TCP50001] = (byte)TcpManager.Status.Disconnect;
            WriteLogMemory(mLogData);

            mLog.WriteLog("TCP50001", "disconnect");

            allPortClose();

        }

        /// <summary>
        /// ポート50001で転送
        /// </summary>
        void MyTcpManager1_RecvTcpCmdEvent(object sender, TcpEventArg e)
        {
            if (e.DisMessage.MessageBin.Length == 0)
            {
                return;
            }

            MsgLog("TCP50001", e.DisMessage.MessageBin);
            //mLog.WriteLog("TCP50001", string.Format("cmd={0} kyoku={1}", e.DisMessage.CommandId, e.DisMessage.KyokuId));

            if (e.SendFlag)
            {
                SafeQueue.Enqueue(e.DisMessage);
                WriteQueueCountLog("TCP50001");
            }
            else
            {
                mLog.WriteLog("TCP50001", "do not send to tarminal. 16kb over.");
            }
        }

        /// <summary>
        /// ポート50001の異常系Event
        /// </summary>
        void MyTcpManager1_RecvTcpExcepEvent(object sender, TcpErrorEventArg e)
        {
            mLog.WriteLog("TCP50001", e.Msg);
        }

        private void allPortClose()
        {
            SessionPingClose();

            if (MyTcpManager1 != null)
            {
                MyTcpManager1.Close();
            }
            if (MyTcpManager2 != null)
            {
                MyTcpManager2.Close();
            }
            if (MyTcpManager3 != null)
            {
                MyTcpManager3.Close(false);
            }
        }

        private void WriteQueueCountLog(string name)
        {
            int count = SafeQueue.Count;
            if(count > threshhold)
            {
                mLog.WriteLog(name, "***1st Queue count " + threshhold.ToString() + "[count] over!");
                threshhold += 100;
            }

            if (IsDebug)
            {
                try
                {
                    //int count = SafeQueue.Count;
                    mLog.WriteLog_Debug(name, "MainQueueCount=" + count.ToString());

                    //忙しいときはデバッグログをOffにする
                    if (count > 200)
                    {
                        mLog.WriteLog_Debug(name, "***Because the queue is accumulated, turn off the debug log. [true]->[false]***");
                        IsDebug = false;
                    }
                }
                catch (Exception ex)
                {
                    mLog.WriteLog(name, "[Debug Exception]mainQueueCountFunc() :" + ex.Message);
                }
            }
        }

        #endregion


        /// <summary>
        /// 定期的に生存確認を送信する
        /// </summary>
        private void SessionPing()
        {
            TraceCancel = new CancellationTokenSource();
            task = Task.Factory.StartNew(() =>
            {
                while (true)
                {
                    //5秒間隔
                    for (int i = 0; i < 10; i++)
                    {
                        Thread.Sleep(500);
                        TraceCancel.Token.ThrowIfCancellationRequested();
                    }

                    //生存確認通信を送信
                    SessionPingMessage mins = new SessionPingMessage();
                    byte[] msg = mins.ToByte();
                    this.MyTcpManager3.SoketCommand(msg);

                    mLog.WriteLog_Debug("TCP50003", "[SessionPingReq] Send --->");

                }
            }, TraceCancel.Token);

        }

        /// <summary>
        /// 定期的に生存確認を停止する
        /// </summary>
        private void SessionPingClose()
        {
            if (task != null)
            {
                try
                {
                    TraceCancel.Cancel();
                    task.Wait(1000);
                    task = null;
                }
                catch { }
            }
        }



        /// <summary>
        /// メッセージダンプ用にログを加工
        /// </summary>
        private void MsgLog(string name, byte[] msg, bool isSend = false)
        {
            if (IsDebug)
            {
                //ヘッダーのみログに書く
                string s = BitConverter.ToString(msg, 0, 16);

                if (isSend)
                {
                    mLog.WriteLog_Debug(name, "Send " + s);
                }
                else
                {
                    mLog.WriteLog_Debug(name, "Recv " + s);
                }
            }
        }


        bool isCreate = false;
        /// <summary>
        /// 共有メモリに状態書き込み
        /// </summary>
        private void WriteLogMemory(byte[] bdata)
        {
            mLog.WriteLog_Debug("WriteLogMemory", "Write");

            Task.Factory.StartNew(() =>
                {
                    try
                    {
                        try
                        {
                            if (isCreate == false)
                            {
                                //初回だけ
                                MemoryMappedFileSecurity mms = new MemoryMappedFileSecurity();
                                SecurityIdentifier everyoneSid = new SecurityIdentifier(WellKnownSidType.WorldSid, null);
                                mms.AddAccessRule(new System.Security.AccessControl.AccessRule<MemoryMappedFileRights>(everyoneSid, MemoryMappedFileRights.FullControl, System.Security.AccessControl.AccessControlType.Allow));

                                mMmFile = MemoryMappedFile.CreateNew("Global\\scdispa.bin", 64, MemoryMappedFileAccess.ReadWrite, MemoryMappedFileOptions.None, mms, HandleInheritability.Inheritable);
                            }
                        }
                        catch
                        {
                            isCreate = true;
                        }

                        if (isCreate)
                        {
                            mMmFile = MemoryMappedFile.OpenExisting("Global\\scdispa.bin", MemoryMappedFileRights.ReadWrite);
                        }

                        using (var accessor = mMmFile.CreateViewAccessor())
                        {
                            accessor.WriteArray<byte>(0, bdata, 0, bdata.Length);
                        }
                    }
                    catch (Exception ex)
                    {
                        mLog.WriteLog("WriteLogMemory", ex.Message);
                    }
                });
        }

        /// <summary>
        /// デバック用関数
        /// </summary>
        internal void TestStartupAndStop(string[] args)
        {
            this.OnStart(args);
            Console.ReadLine();
            this.OnStop();
        }

    }

    public class SessionPingMessage
    {
        public const int DEF_REQ = 254;
        public const int DEF_ACK = 255;

        public int MessageLength;
        public int CommnadId;
        public int KyokuId;
        public int MatchingId;
        public int ReplyCode;

        public SessionPingMessage()
        {
        }

        public byte[] ToByte()
        {
            MessageLength = 0;
            CommnadId = DEF_REQ;
            KyokuId = 0;
            MatchingId = 0xFF;

            using (MemoryStream ms = new MemoryStream())
            {
                ms.Write(BitConverter.GetBytes(MessageLength), 0, sizeof(int));
                ms.Write(BitConverter.GetBytes(CommnadId), 0, sizeof(int));
                ms.Write(BitConverter.GetBytes(KyokuId), 0, sizeof(int));
                ms.Write(BitConverter.GetBytes(MatchingId), 0, sizeof(int));
                return ms.ToArray();
            }
        }
    }
}
