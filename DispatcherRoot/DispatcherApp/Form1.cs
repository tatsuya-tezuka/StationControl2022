using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.ServiceProcess;

namespace Dispatcher
{
    public partial class Form1 : Form
    {
        private readonly string DEF_DISP_NAME = "DispatcherService";

        TcpManager MyTcpManager1;
        TcpManager MyTcpManager2;
        TcpManager MyTcpManager3;
        IPAddress MyIp;
        bool IsAlive;
        SynchronizedCollection<PipeServer> MyPipeManager = new SynchronizedCollection<PipeServer>();
        private ConcurrentQueue<DispaMessageCls> SafeQueue;
        private Task mPipeSend;
        private uint TotalCount;
        CancellationTokenSource TraceCancel;
        Task task;

        public Form1()
        {
            InitializeComponent();

            IsAlive = true;
            TotalCount = 0;

            SafeQueue = new ConcurrentQueue<DispaMessageCls>();

            #region Debug画面専用
            //---Debug画面用---
            string host = Dns.GetHostName();
            IPAddress[] ip = Dns.GetHostAddresses(host);

            for (int i = 0; i < ip.Length; i++)
            {
                if (ip[i].AddressFamily == AddressFamily.InterNetwork)
                {
                    MyIp = ip[i];
                    break;
                }
            }
            Version vs = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version;

            this.textBox1.Text = Properties.Settings.Default.ServerIp;

            this.Text += "  (My IpAddress is " + MyIp.ToString() + ")    version " + vs.ToString();
            
            if(GetDispaServiceStatus())
            {
                MessageBox.Show("サービスを停止してから起動してください", "注意", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                this.Close();
                return;
            }
            
            
            #endregion

            MyTcpManager1 = new TcpManager(this.textBox1.Text, 50001);
            MyTcpManager1.RecvTcpCmdEvent += MyTcpManager1_RecvTcpCmdEvent;
            MyTcpManager1.TcpCloseEvent += MyTcpManager1_TcpCloseEvent;
            MyTcpManager1.TcpOpenEvent += MyTcpManager1_TcpOpenEvent;
            MyTcpManager1.RecvTcpExcepEvent += MyTcpManager1_RecvTcpExcepEvent;
            MyTcpManager1.SoketRecv();

            MyTcpManager2 = new TcpManager(this.textBox1.Text, 50002);
            MyTcpManager2.RecvTcpCmdEvent += MyTcpManager2_RecvTcpCmdEvent;
            MyTcpManager2.TcpCloseEvent += MyTcpManager2_TcpCloseEvent;
            MyTcpManager2.TcpOpenEvent += MyTcpManager2_TcpOpenEvent;
            MyTcpManager2.RecvTcpExcepEvent += MyTcpManager2_RecvTcpExcepEvent;
            MyTcpManager2.SoketRecv();

            MyTcpManager3 = new TcpManager(this.textBox1.Text, 50003);
            MyTcpManager3.RecvTcpCmdEvent += MyTcpManager3_RecvTcpCmdEvent;
            MyTcpManager3.TcpCloseEvent += MyTcpManager3_TcpCloseEvent;
            MyTcpManager3.TcpOpenEvent += MyTcpManager3_TcpOpenEvent;
            MyTcpManager3.RecvTcpExcepEvent += MyTcpManager3_RecvTcpExcepEvent;
            MyTcpManager3.SoketRecv();

            this.button1.Enabled = false;
            this.button2.Enabled = true;
            this.button3.Enabled = true;

            //パイプメッセージキューを送信する処理
            mPipeSend = Task.Factory.StartNew(() =>
                {
                    while(IsAlive)
                    {
                        DispaMessageCls msgData;
                        if (SafeQueue.TryDequeue(out msgData))
                        {
                            lock (MyPipeManager)
                            {
                                for (int i = 0; i < MyPipeManager.Count; i++)
                                {
                                    MyPipeManager[i].SendMessage(msgData.MessageBin);
                                }
                            }
                        }else
                        {
                            Thread.Sleep(1);
                        }
                    }
                });

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
                MyPipeManager.Add(pipe);
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
                        PipeServer pipe = new PipeServer(pipename, TotalCount, int.MaxValue, kyoku_id , PipeServer.RecvMessageType.All);
                        pipe.PipeConnectEvent += pipe_PipeConnectEvent;
                        pipe.RecvCommandEvent += pipe_RecvCommandEvent;
                        pipe.PipeDisconnectEvent += pipe_PipeDisconnectEvent;
                        pipe.RecvPipeExcepEvent += pipe_RecvPipeExcepEvent;
                        pipe.WaitConnect();
                        MyPipeManager.Add(pipe);
                    }
                    else
                    {
                        Thread.Sleep(1000);
                    }
                }
            });

        }



        #region 50003（制御コマンド専用ポート）
        //----------------------------------------------------------
        // 50003（制御コマンド専用ポート）
        //----------------------------------------------------------

        void MyTcpManager3_TcpOpenEvent(object sender, EventArgs e)
        {
            writeString("connect", this.listBox3);
            writeStatus("connect",this.labelStatus50003);

            SessionPing();
                        
        }

        void MyTcpManager3_TcpCloseEvent(object sender, EventArgs e)
        {
            writeString("disconnect", this.listBox3);
            writeStatus("disconnect", this.labelStatus50003);

            SessionPingClose();

            if (MyTcpManager3 != null)
            {
                MyTcpManager3.Close(false);
            }

        }

        void MyTcpManager3_RecvTcpCmdEvent(object sender, TcpEventArg e)
        {
            if (e.DisMessage.MessageBin.Length == 0)
            {
                return;
            }

            writeMsg(e.DisMessage.MessageBin, this.listBox3, "Cnt", false);

            if (e.SendFlag)
            {
                uint cmdId = BitConverter.ToUInt32(e.DisMessage.MessageBin, 4);
                if (SessionPingMessage.DEF_ACK == cmdId ||
                    SessionPingMessage.DEF_REQ == cmdId)
                {
                    //生存確認通信は端末に送信しない
                    writeString("[SessionPingAck] recv.", this.listBox3);
                }
                else
                {
                    SafeQueue.Enqueue(e.DisMessage);
                    writeString("[Enqueue] QueueCount=" + SafeQueue.Count.ToString(), this.listBox3);
                }
            }
            else
            {
                writeString("do not send to tarminal. 16kb over.", this.listBox3);
            }
        }

        void MyTcpManager3_RecvTcpExcepEvent(object sender, TcpErrorEventArg e)
        {
            writeString("[Info]" + e.Msg, this.listBox3);
        }

        #endregion

        #region 50002（履歴通知専用ポート）
        //----------------------------------------------------------
        // 50002（履歴通知専用ポート）
        //----------------------------------------------------------

        void MyTcpManager2_TcpOpenEvent(object sender, EventArgs e)
        {
            writeString("connect", this.listBox2);
            writeStatus("connect", this.labelStatus50002);
        }

        void MyTcpManager2_TcpCloseEvent(object sender, EventArgs e)
        {
            writeString("disconnect", this.listBox2);
            writeStatus("disconnect", this.labelStatus50002);

            if (MyTcpManager2 != null)
            {
                MyTcpManager2.Close(false);
            }
        }

        void MyTcpManager2_RecvTcpCmdEvent(object sender, TcpEventArg e)
        {
            if (e.DisMessage.MessageBin.Length == 0)
            {
                return;
            }

            writeMsg(e.DisMessage.MessageBin, this.listBox2, "Rireki", checkBox2.Checked);

            if (e.SendFlag)
            {
                SafeQueue.Enqueue(e.DisMessage);
                writeString("[Enqueue] QueueCount=" + SafeQueue.Count.ToString(), this.listBox2);
            }
            else
            {
                writeString("do not send to tarminal. 16kb over.", this.listBox2);
            }

        }

        void MyTcpManager2_RecvTcpExcepEvent(object sender, TcpErrorEventArg e)
        {
            writeString("[Info]" + e.Msg, this.listBox2);
        }
        #endregion

        #region 50001（監視データ専用ポート）
        //----------------------------------------------------------
        // 50001（監視データ専用ポート）
        //----------------------------------------------------------


        void MyTcpManager1_TcpOpenEvent(object sender, EventArgs e)
        {
            writeString("connect", this.listBox1);
            writeStatus("connect", this.labelStatus50001);

        }

        void MyTcpManager1_TcpCloseEvent(object sender, EventArgs e)
        {
            writeString("disconnect", this.listBox1);
            writeStatus("disconnect", this.labelStatus50001);

            if (MyTcpManager1 != null)
            {
                MyTcpManager1.Close(false);
            }
        }

        void MyTcpManager1_RecvTcpCmdEvent(object sender, TcpEventArg e)
        {
            if (e.DisMessage.MessageBin.Length == 0)
            {
                return;
            }

            writeMsg(e.DisMessage.MessageBin, this.listBox1, "Kanshi", checkBox1.Checked);

            if (e.SendFlag)
            {
                SafeQueue.Enqueue(e.DisMessage);
                writeString("[Enqueue] QueueCount=" + SafeQueue.Count.ToString(), this.listBox1);
            }
            else
            {
                writeString("do not send to tarminal. 16kb over.", this.listBox1);
            }
            
        }

        void MyTcpManager1_RecvTcpExcepEvent(object sender, TcpErrorEventArg e)
        {
            writeString("[Info]" + e.Msg, this.listBox1);
        }
        #endregion
        
        #region パイプ通信のイベント
        //-----------------------------------------------
        // パイプ通信のイベント
        //-----------------------------------------------
        void pipe_PipeConnectEvent(object sender, EventArgs e)
        {
            PipeServer ps = sender as PipeServer;
            string s = ps.PipeNameIn + ":ConnectCount=" + (MyPipeManager.Count + 1).ToString();
            writeString(s, this.listBox4);
        }

        void pipe_PipeDisconnectEvent(object sender, EventArgs e)
        {
            PipeServer ps = sender as PipeServer;
            string pipeName = ps.NamaPipeName;
            int kyokuid = ps.KyokuID;
            PipeServer.RecvMessageType type = ps.Type;
            MyPipeManager.Remove((PipeServer)sender);

            string s = ps.PipeNameIn + ":Disconnect";
            writeString(s, this.listBox4);

            //再接続する
            if (type == PipeServer.RecvMessageType.KanshiOnly)
            {
                CreatePipeTaskFa(pipeName, kyokuid);
            }

        }

        void pipe_RecvCommandEvent(object sender, PipeEventArg e)
        {
            PipeServer ps = sender as PipeServer;

            writeMsg(e.recvMessage, this.listBox4, "Pipe", false);

            //オウム返し
            ps.SendMessage(e.recvMessage);
        }

        void pipe_RecvPipeExcepEvent(object sender, PipeErrorEventArg e)
        {
            writeString("[Info]" + e.Msg, this.listBox4);
        }

        #endregion
        

        //-----------------------------------------------
        // ボタンイベント
        //-----------------------------------------------

        private void button1_Click(object sender, EventArgs e)
        {
            MyTcpManager1 = new TcpManager(this.textBox1.Text, 50001);
            MyTcpManager1.RecvTcpCmdEvent += MyTcpManager1_RecvTcpCmdEvent;
            MyTcpManager1.TcpCloseEvent += MyTcpManager1_TcpCloseEvent;
            MyTcpManager1.TcpOpenEvent += MyTcpManager1_TcpOpenEvent;
            MyTcpManager1.RecvTcpExcepEvent += MyTcpManager1_RecvTcpExcepEvent;
            MyTcpManager1.SoketRecv();

            MyTcpManager2 = new TcpManager(this.textBox1.Text, 50002);
            MyTcpManager2.RecvTcpCmdEvent += MyTcpManager2_RecvTcpCmdEvent;
            MyTcpManager2.TcpCloseEvent += MyTcpManager2_TcpCloseEvent;
            MyTcpManager2.TcpOpenEvent += MyTcpManager2_TcpOpenEvent;
            MyTcpManager2.RecvTcpExcepEvent += MyTcpManager2_RecvTcpExcepEvent;
            MyTcpManager2.SoketRecv();

            MyTcpManager3 = new TcpManager(this.textBox1.Text, 50003);
            MyTcpManager3.RecvTcpCmdEvent += MyTcpManager3_RecvTcpCmdEvent;
            MyTcpManager3.TcpCloseEvent += MyTcpManager3_TcpCloseEvent;
            MyTcpManager3.TcpOpenEvent += MyTcpManager3_TcpOpenEvent;
            MyTcpManager3.RecvTcpExcepEvent += MyTcpManager3_RecvTcpExcepEvent;
            MyTcpManager3.SoketRecv();

            this.button1.Enabled = false;
            this.button2.Enabled = true;
            this.button3.Enabled = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {

            SessionPingClose();

            if (MyTcpManager1 != null)
                MyTcpManager1.Close();
            if (MyTcpManager2 != null)
                MyTcpManager2.Close();
            if (MyTcpManager3 != null)
                MyTcpManager3.Close();

            this.button1.Enabled = true;
            this.button2.Enabled = false;
            this.button3.Enabled = false;
        }
        
 
        private void button3_Click(object sender, EventArgs e)
        {
            ConnectCmdMessage ccm = new ConnectCmdMessage(MyIp.GetAddressBytes());

            byte[] msg = ccm.ToByte();

            writeSendMsg(msg, listBox3);

            MyTcpManager3.SoketCommand(msg);

        }

        private void button4_Click(object sender, EventArgs e)
        {
            DisconnectCmdMessage ccm = new DisconnectCmdMessage(MyIp.GetAddressBytes());

            byte[] msg = ccm.ToByte();

            writeSendMsg(msg, listBox3);

            MyTcpManager3.SoketCommand(msg);
        }

        private void button6_Click(object sender, EventArgs e)
        {
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

        private void button5_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "開くファイルを選択してください";

            //ダイアログを表示する
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                byte[] msg = File.ReadAllBytes(ofd.FileName);
                int cmdId = BitConverter.ToInt32(msg, 4);
                int kyoId = BitConverter.ToInt32(msg, 8);

                bool isAll = checkBox3.Checked;
                lock (MyPipeManager)
                {
                    for (int i = 0; i < MyPipeManager.Count; i++)
                    {
                        if (isAll)
                        {
                            MyPipeManager[i].SendMessage(msg);
                        }
                        else
                        {
                            MyPipeManager[i].SendMessageEnqueue(new DispaMessageCls(msg, cmdId, kyoId));
                        }
                    }
                }
            }
        }

        private void button7_Click(object sender, EventArgs e)
        {
            //FolderBrowserDialogクラスのインスタンスを作成
            FolderBrowserDialog fbd = new FolderBrowserDialog();

            //上部に表示する説明テキストを指定する
            fbd.Description = "フォルダを指定してください。";
            //ルートフォルダを指定する
            //デフォルトでDesktop
            fbd.RootFolder = Environment.SpecialFolder.Desktop;
            //ダイアログを表示する
            if (fbd.ShowDialog(this) == DialogResult.OK)
            {
                //選択されたフォルダを表示する
                DirectoryInfo di = new DirectoryInfo(fbd.SelectedPath);
                FileInfo[] fis = di.GetFiles("Recv-*");

                var ans = from n in fis orderby n.FullName ascending select n.FullName;
                var list = ans.ToList();

                bool isAll = checkBox3.Checked;
                Task.Factory.StartNew(() =>
                {
                    for (int i = 0; i < list.Count; i++)
                    {
                        byte[] msg = File.ReadAllBytes(list[i]);
                        int cmdId = BitConverter.ToInt32(msg, 4);
                        int kyoId = BitConverter.ToInt32(msg, 8);

                        lock (MyPipeManager)
                        {
                            for (int j = 0; j < MyPipeManager.Count; j++)
                            {
                                if(isAll)
                                {
                                    MyPipeManager[j].SendMessage(msg);
                                }
                                else
                                {
                                    MyPipeManager[j].SendMessageEnqueue(new DispaMessageCls(msg, cmdId, kyoId));
                                }
                            }
                        }
                        Thread.Sleep(500);
                    }
                });
            }

        }
       
        //-----------------------------------------------
        // Privateな関数群
        //-----------------------------------------------
        
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
                    byte [] msg = mins.ToByte();
                    this.MyTcpManager3.SoketCommand(msg);

                    writeString("[SessionPingReq] Send --->", this.listBox3);
                
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
                    Task.WaitAll(task);
                    task = null;
                }
                catch { }
            }
        }
        
        
        private int uniId = 0;

        private void writeMsg(byte[] msg, ListBox lb, string id, bool isEnable)
        {
            if (isEnable)
            {
                uniId++;
                if (uniId > 100)
                {
                    uniId = 0;
                }
                try
                {
                    string f = string.Format("Recv-{0}-{1}-{2}", id, DateTime.Now.ToString("MMddHHmmssfff"), uniId.ToString("D2"));
                    File.WriteAllBytes(f, msg);
                }
                catch { }
            }

            string s = BitConverter.ToString(msg);
            if (s.Length > 1024)
            {
                s = s.Substring(0, 1024) + "...";
            }

            try
            {
                this.Invoke((MethodInvoker)delegate()
                {
                    if (msg.Length > 0)
                    {
                        if (lb.Items.Count > 200)
                        {
                            lb.Items.Clear();
                        }
                        lb.Items.Add("Recv-->:" + s);
                    }
                    else
                    {
                        lb.Items.Add("Recv-->ZERO");
                    }
                    lb.TopIndex = lb.Items.Count - 1;
                });
            }
            catch { }
        }

        private void writeSendMsg(byte[] msg, ListBox lb)
        {
            string s = BitConverter.ToString(msg);
            if (s.Length > 1024)
            {
                s = s.Substring(0, 1024) + "...";
            }

            try
            {
                this.Invoke((MethodInvoker)delegate()
                {
                    if (lb.Items.Count > 200)
                    {
                        lb.Items.Clear();
                    }
                    lb.Items.Add("Send<--:" + s);
                    lb.TopIndex = lb.Items.Count - 1;
                });
            }
            catch { }
        }

        private void writePipeSendMsg(byte[] msg, uint id, ListBox lb)
        {
            string s = BitConverter.ToString(msg);
            if (s.Length > 1024)
            {
                s = s.Substring(0, 1024) + "...";
            }

            try
            {
                this.Invoke((MethodInvoker)delegate()
                {
                    if (lb.Items.Count > 200)
                    {
                        lb.Items.Clear();
                    }
                    lb.Items.Add("[" + id.ToString() + "] Send-->:" + s);
                    lb.TopIndex = lb.Items.Count - 1;
                });
            }
            catch { }
        }

        private void writeString(string msg, ListBox lb)
        {
            try
            {
                this.Invoke((MethodInvoker)delegate()
                {
                    if (lb.Items.Count > 200)
                    {
                        lb.Items.Clear();
                    }
                    lb.Items.Add(DateTime.Now.ToString("HH:mm:ss.fff") + " " + msg);
                    lb.TopIndex = lb.Items.Count - 1;
                });
            }
            catch { }
        }

        private void writeStatus(string msg, Label lb)
        {
            try
            {
                this.Invoke((MethodInvoker)delegate()
                {
                    lb.Text = msg;
                    if (msg.Equals("disconnect"))
                    {
                        lb.ForeColor = Color.Red;
                    }
                    else
                    {
                        lb.ForeColor = Color.Blue;
                    }

                });
            }
            catch { }
        }
        

        private void label7_Click(object sender, EventArgs e)
        {
            if(this.button5.Visible == true)
            {
                this.button5.Visible = false;
                this.button7.Visible = false;
                groupBox3.Visible = false;
            }
            else
            {
                this.button5.Visible = true;
                this.button7.Visible = true;
                groupBox3.Visible = true;
            }
        }

        private bool GetDispaServiceStatus()
        {
            System.ServiceProcess.ServiceController[] controllerList;
            controllerList = ServiceController.GetServices();

            bool ans = false;
            foreach (var ser in controllerList)
            {
                if (ser.ServiceName == DEF_DISP_NAME)
                {
                    if (ser.Status == ServiceControllerStatus.Running)
                    {
                        ans = true;
                    }
                    break;
                }
            }
            return ans;
        }



        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            SessionPingClose();

            if (MyTcpManager1 != null)
                MyTcpManager1.Close();
            if (MyTcpManager2 != null)
                MyTcpManager2.Close();
            if (MyTcpManager3 != null)
                MyTcpManager3.Close();

        }

        bool ubnFlag = false;

        private void button8_Click(object sender, EventArgs e)
        {
            if(!string.IsNullOrEmpty(textBoxPath.Text))
            {
                //選択されたフォルダを表示する
                DirectoryInfo di = new DirectoryInfo(textBoxPath.Text);
                FileInfo[] fis = di.GetFiles("Recv-*");

                var ans = from n in fis orderby n.FullName ascending select n.FullName;
                var list = ans.ToList();

                ubnFlag = true;
                int sleep = (int)this.numericUpDown1.Value;

                bool isAll = checkBox3.Checked;
                Task.Factory.StartNew(() =>
                {
                    while (ubnFlag)
                    {
                        for (int i = 0; i < list.Count; i++)
                        {
                            byte[] msg = File.ReadAllBytes(list[i]);
                            int cmdId = BitConverter.ToInt32(msg, 4);
                            int kyoId = BitConverter.ToInt32(msg, 8);

                            lock (MyPipeManager)
                            {
                                for (int j = 0; j < MyPipeManager.Count; j++)
                                {
                                    if (isAll)
                                    {
                                        MyPipeManager[j].SendMessage(msg);
                                    }
                                    else
                                    {
                                        MyPipeManager[j].SendMessageEnqueue(new DispaMessageCls(msg, cmdId, kyoId));
                                    }
                                    writeString("DummrySend! -> " + BitConverter.ToString(msg, 0, 16), this.listBox4);
                                }
                            }
                            Thread.Sleep(sleep);
                            if(!ubnFlag)
                            {
                                break;
                            }
                        }
                        writeString("************", this.listBox4);
                        if(this.checkBoxOnlyOnce.Checked)
                        {
                            break;
                        }
                    }
                });
            }
        }

        private void button9_Click(object sender, EventArgs e)
        {
            ubnFlag = false;
        }

        private void button10_Click(object sender, EventArgs e)
        {
            //FolderBrowserDialogクラスのインスタンスを作成
            FolderBrowserDialog fbd = new FolderBrowserDialog();

            //上部に表示する説明テキストを指定する
            fbd.Description = "フォルダを指定してください。";
            //ルートフォルダを指定する
            //デフォルトでDesktop
            fbd.RootFolder = Environment.SpecialFolder.Desktop;
            //ダイアログを表示する
            if (fbd.ShowDialog(this) == DialogResult.OK)
            {
                this.textBoxPath.Text = fbd.SelectedPath;
            }
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

    public class ConnectCmdMessage
    {
        public const int DEF_REQ = 101;
        public const int DEF_ACK = 102;

        public int MessageLength;
        public int CommnadId;
        public int KyokuId;
        public int MatchingId;

        public byte[] IpAddress;

        public ConnectCmdMessage(byte[] ip)
        {
            IpAddress = new byte[ip.Length];
            ip.CopyTo(IpAddress, 0);
        }

        public byte[] ToByte()
        {

            MessageLength = IpAddress.Length;
            CommnadId = DEF_REQ;
            KyokuId = 0;
            MatchingId = 0xFF;

            using (MemoryStream ms = new MemoryStream())
            {
                ms.Write(BitConverter.GetBytes(MessageLength),0, sizeof(int));

                ms.Write(BitConverter.GetBytes(CommnadId), 0, sizeof(int));

                ms.Write(BitConverter.GetBytes(KyokuId), 0, sizeof(int));

                ms.Write(BitConverter.GetBytes(MatchingId), 0, sizeof(int));

                ms.Write(IpAddress, 0, IpAddress.Length);

                return ms.ToArray();
            }

        }

    }

    public class DisconnectCmdMessage
    {
        public const int DEF_REQ = 103;
        public const int DEF_ACK = 104;

        public int MessageLength;
        public int CommnadId;
        public int KyokuId;
        public int MatchingId;

        public byte[] IpAddress;

        public DisconnectCmdMessage(byte[] ip)
        {
            IpAddress = new byte[ip.Length];
            ip.CopyTo(IpAddress, 0);
        }

        public byte[] ToByte()
        {

            MessageLength = IpAddress.Length;
            CommnadId = DEF_REQ;
            KyokuId = 0;
            MatchingId = 0xAA;

            using (MemoryStream ms = new MemoryStream())
            {
                ms.Write(BitConverter.GetBytes(MessageLength), 0, sizeof(int));

                ms.Write(BitConverter.GetBytes(CommnadId), 0, sizeof(int));

                ms.Write(BitConverter.GetBytes(KyokuId), 0, sizeof(int));

                ms.Write(BitConverter.GetBytes(MatchingId), 0, sizeof(int));

                ms.Write(IpAddress, 0, IpAddress.Length);

                return ms.ToArray();
            }

        }

    }
}
