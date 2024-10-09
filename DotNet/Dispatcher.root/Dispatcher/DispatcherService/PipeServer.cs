using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.IO.Pipes;
using System.Threading.Tasks;
using System.Collections.Concurrent;
using System.Threading;

namespace Dispatcher
{
    public class PipeServer : IDisposable
    {
        public static readonly int MaxCount = 64;

        /// <summary>
        /// 監視データのメッセージID(201)
        /// </summary>
        public readonly int Kanshi_Message_ID = 201;

        public enum Status
        {
            Disconnect = 0,
            ToConnect = 1,
            Connect = 2,
        }

        public enum RecvMessageType
        {
            All,
            KanshiOnly,
        }

        public Status PipeStatus;
        private string mPipeNameIn;
        private string mPipeNameOut;
        private uint mId;
        private int mMaxQueueCount;

        public string PipeNameIn
        {
            get { return mPipeNameIn; }
        }
        public string PipeNameOut
        {
            get { return mPipeNameOut; }
        }

        public uint Id
        {
            get { return mId; }
        }

        // 名前付きパイプ操作用のオブジェクト（クライアント用）を生成
        private NamedPipeServerStream pipeServerIn;
        private NamedPipeServerStream pipeServerOut;
        private Stream mStream;

        private int mHeaderSize;
        private byte[] mHeaderBuf;

        /// <summary>
        /// 局ID
        /// </summary>
        private int mKyoku_ID;
        public int KyokuID
        {
            get { return mKyoku_ID; }
        }

        /// <summary>
        /// 扱うメッセージタイプ
        /// </summary>
        private RecvMessageType mType;
        public RecvMessageType Type
        {
            get { return mType; }
        }

        private string mNamaPipeName;
        public string NamaPipeName
        {
            get { return mNamaPipeName; }
        }


        /// <summary>
        /// ログに書く閾値
        /// </summary>
        int threshhold = 100;
        /// <summary>
        /// ログ書くフラグ
        /// </summary>
        bool enQueueFlag = false;


        /// <summary>
        /// イベント類
        /// </summary>
        public delegate void RecvCmdEventHandler(object sender, PipeEventArg e);
        public event RecvCmdEventHandler RecvCommandEvent;
        public delegate void RecvPipeErrorEventHandler(object sender, PipeErrorEventArg e);
        public event RecvPipeErrorEventHandler RecvPipeExcepEvent;

        public event EventHandler PipeDisconnectEvent;
        public event EventHandler PipeConnectEvent;

        private ConcurrentQueue<DispaMessageCls> SendQueue;
        private bool IsAlive;

        public PipeServer(string pipeName, uint count, int MaxQueue, int kyoku_id, RecvMessageType type)
        {
            mId = count;
            mMaxQueueCount = MaxQueue;
            mKyoku_ID = kyoku_id;
            mType = type;

            mNamaPipeName = pipeName;
            mPipeNameIn = pipeName + "IN";
            mPipeNameOut = pipeName + "OUT";
            PipeStatus = Status.Disconnect;

            mHeaderSize = sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int); ;
            mHeaderBuf = new byte[mHeaderSize];

            SendQueue = new ConcurrentQueue<DispaMessageCls>();
            IsAlive = true;
        }

        ~PipeServer()
        {
            Dispose(false);
        }
        
        public void Dispose()
        {
            Dispose(true);
        }

        protected virtual void Dispose(bool disposing)
        {
            Close();
        }


        /// <summary>
        /// 接続待ち状態
        /// 接続したら関数が帰る
        /// </summary>
        public void WaitConnect()
        {
            pipeServerIn = new NamedPipeServerStream(mPipeNameIn, PipeDirection.In, MaxCount, PipeTransmissionMode.Byte, PipeOptions.Asynchronous);
            pipeServerOut = new NamedPipeServerStream(mPipeNameOut, PipeDirection.Out, MaxCount, PipeTransmissionMode.Byte, PipeOptions.Asynchronous);

            PipeStatus = Status.ToConnect;

            Task t1 = Task.Factory.StartNew(() =>
            {
                pipeServerIn.WaitForConnection();
            });
            Task t2 = Task.Factory.StartNew(() =>
            {
                pipeServerOut.WaitForConnection();
            });

            Task.WaitAll(t1, t2);

            mStream = pipeServerIn;

            PipeStatus = Status.Connect;
            OnPipeOpen();

            //パイプメッセージキューを送信する処理            
            Task.Factory.StartNew(() =>
            {
                while (IsAlive)
                {
                    DispaMessageCls msgData;
                    if (SendQueue.TryDequeue(out msgData))
                    {
                        SendMessage(msgData.MessageBin);
                    }
                    else
                    {
                        Thread.Sleep(1);
                    }
                }
            });

            Task.Factory.StartNew(() =>
            {
                RecvMessage();
            });
        }

        private void RecvMessage()
        {
            try
            {
                while (IsAlive)
                {
                    //メッセージサイズ取得
                    if (mStream.Read(mHeaderBuf, 0, mHeaderSize) == 0)
                    {
                        //切れた
                        PipeClose();
                        break;
                    }

                    int bodySize = BitConverter.ToInt32(mHeaderBuf, 0);

                    using (MemoryStream ms = new MemoryStream())
                    {
                        ms.Write(mHeaderBuf, 0, mHeaderSize);

                        //本体
                        byte[] msgBuff = new byte[bodySize];
                        if (mStream.Read(msgBuff, 0, bodySize) == 0)
                        {
                            //切れた
                            PipeClose();
                            break;
                        }

                        ms.Write(msgBuff, 0, bodySize);

                        OnRecv(ms.ToArray());
                    }
                }
            }
            catch(Exception ex)
            {
                string innEx = string.Empty;
                if(ex.InnerException != null)
                {
                    innEx = ex.InnerException.ToString();
                }

                OnPipeException(ex.Message, innEx, ex.Source);
                PipeClose();
            }
        }


        public void Close()
        {
            PipeStatus = Status.Disconnect;
            IsAlive = false;
            try
            {
                pipeServerIn.Close();
            }
            catch { }
            try
            {
                pipeServerOut.Close();
            }
            catch { }
        }

        public void SendMessageEnqueue(DispaMessageCls msg)
        {
            //RecvMessageType.KanshiOnlyはFA用
            if (mType == RecvMessageType.KanshiOnly)
            {
                if (msg.CommandId == Kanshi_Message_ID && msg.KyokuId == mKyoku_ID)
                {
                    //Queue最大を超えたら送らずスルー
                    if (SendQueue.Count < mMaxQueueCount)
                    {
                        SendQueue.Enqueue(msg);
                    }
                    enQueueFlag = true;
                }
            }
            else if (mType == RecvMessageType.All)
            {
                //局別のパイプ名にQueueする。100と101は全局にQueueする。
                if (msg.KyokuId == mKyoku_ID || msg.KyokuId == 100 || msg.KyokuId == 101)
                {
                    SendQueue.Enqueue(msg);
                    enQueueFlag = true;
                }
            }

            //Countをログに書き込む
            if (enQueueFlag)
            {
                int count = SendQueue.Count;
                if (count > threshhold)
                {
                    string log = string.Format("*** 2nd Queue count is {0} over. **** Pipe={1}", count, mPipeNameIn);
                    OnPipeException(log, null, null);
                    threshhold += 100;
                }
                enQueueFlag = false;
            }
        }

        object lockObj = new object();
        public void SendMessage(byte[] msg)
        {
            lock (lockObj)
            {
                try
                {
                    pipeServerOut.Write(msg, 0, msg.Length);
                    pipeServerOut.Flush();
                }
                catch (Exception ex)
                {
                    string log = string.Format(" ID={1}", mId);
                    string innEx = string.Empty;
                    if (ex.InnerException != null)
                    {
                        innEx = ex.InnerException.ToString();
                    }
                    OnPipeException(ex.Message + log, innEx, ex.Source);
                }
            }
        }

        private void PipeClose()
        {
            if (PipeStatus != Status.Disconnect)
            {
                OnPipeClose();
            }
            PipeStatus = Status.Disconnect;
            IsAlive = false;
        }

        private void OnRecv(byte[] msg)
        {
            if (this.RecvCommandEvent != null)
            {
                RecvCommandEvent(this, new PipeEventArg(msg));
            }
        }

        private void OnPipeClose()
        {
            if (PipeDisconnectEvent != null)
            {
                PipeDisconnectEvent(this, new EventArgs());
            }
        }

        private void OnPipeOpen()
        {
            if (PipeConnectEvent != null)
            {
                PipeConnectEvent(this, new EventArgs());
            }
        }

        private void OnPipeException(string msg, string innEx, string source)
        {
            if(RecvPipeExcepEvent != null)
            {
                RecvPipeExcepEvent(this, new PipeErrorEventArg(msg, innEx, source));
            }
        }

    }


    public class PipeEventArg : EventArgs
    {
        public byte[] recvMessage;

        public PipeEventArg(byte[] msg)
            : base()
        {
            recvMessage = new byte[msg.Length];
            msg.CopyTo(recvMessage, 0);
        }
    }

    public class PipeErrorEventArg : EventArgs
    {
        public string Msg;
        public string InnerExceptionMsg;
        public string Soruce;


        public PipeErrorEventArg(string msg, string innExcMsg, string source)
            : base()
        {
            Msg = msg;

            if(innExcMsg == null)
            {
                InnerExceptionMsg = string.Empty;
            }

            if(source == null)
            {
                Soruce = string.Empty;
            }
        }
    }

}
