using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.IO;
using System.Threading;

namespace Dispatcher
{
    public class TcpManager : IDisposable
    {
        public enum Status
        {
            Disconnect = 0,
            ToConnect = 1,
            Connect = 2,
        }
        public Status TcpStatus;

        private readonly int RecvMaxSize = 1024 * 1024 * 10;
        private readonly int Recv16KbSize = 1024 * 16;

        private string Ip;
        private int mPort;
        private TcpClient mClient;
        private NetworkStream mNetStream;
        private bool IsAlive = true;

        private int headerSize;
        private byte[] headerBuff;

        public delegate void RecvTcpCmdEventHandler(object sender, TcpEventArg e);
        public event RecvTcpCmdEventHandler RecvTcpCmdEvent;
        public delegate void RecvTcpErrorEventHandler(object sender, TcpErrorEventArg e);
        public event RecvTcpErrorEventHandler RecvTcpExcepEvent;

        public event EventHandler TcpCloseEvent;
        public event EventHandler TcpOpenEvent;

        public TcpManager(string ip, int port)
        {
            Ip = ip;
            mPort = port;

            headerSize = sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int);
            headerBuff = new byte[headerSize];

            TcpStatus = Status.Disconnect;
        }

        protected virtual void Dispose(bool disposing)
        {
            if(disposing)
            {
                Close();
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        public void SoketRecv()
        {
            Task.Factory.StartNew(() =>
            {
                while (IsAlive)
                {
                    if (TcpStatus == Status.Disconnect)
                    {
                        try
                        {
                            //Serverと接続
                            mClient = new TcpClient(Ip, mPort);

                            TcpStatus = Status.ToConnect;
                            //接続待ち
                            mNetStream = mClient.GetStream();

                            TcpStatus = Status.Connect;

                            OnConnect();
                        }
                        catch(Exception ex)
                        {
                            OnException(ex.Message);
                            toDisconnect();
                            Thread.Sleep(300);
                        }
                    }
                    else if (TcpStatus == Status.Connect)
                    {
                        //受信処理
                        int recvSize = 0;
                        bool FlagSend = true;
                        {
                            try
                            {
                                recvSize = mNetStream.Read(headerBuff, 0, headerBuff.Length);

                                if(recvSize == 0)
                                {
                                    OnException("[Error]header read error. size = 0");
                                    toDisconnect();
                                    continue;
                                }
                                else if (recvSize != headerBuff.Length)
                                {
                                    //一回で取得できないとき
                                    OnException("[Warning]header read. size =" + recvSize.ToString());

                                    int r_size = recvSize;
                                    int h_length = headerBuff.Length - r_size;
                                    using (MemoryStream hms = new MemoryStream())
                                    {
                                        hms.Write(headerBuff, 0, r_size);

                                        do
                                        {
                                            r_size = mNetStream.Read(headerBuff, 0, h_length);
                                            if (r_size == 0)
                                            {
                                                //切れた
                                                OnException("header read error.");
                                                toDisconnect();
                                                throw new ApplicationException("header read error. size=0");
                                            }

                                            hms.Write(headerBuff, 0, r_size);
                                            h_length = h_length - r_size;
                                        }
                                        while (h_length > 0);

                                        if (h_length < 0)
                                        {
                                            OnException("header size is Mismatch.");
                                            toDisconnect();
                                            continue;
                                        }

                                        headerBuff = hms.ToArray();

                                    }
                                }

                                int bodySize = BitConverter.ToInt32(headerBuff, 0);
                                int commandId = BitConverter.ToInt32(headerBuff, 4);
                                int kyokuId = BitConverter.ToInt32(headerBuff, 8);

                                if (bodySize > RecvMaxSize)
                                {
                                    OnException("body size is 10 Mbyte over. size =" + bodySize.ToString());
                                    toDisconnect();
                                    continue;
                                }
                                else if (bodySize > Recv16KbSize)
                                {
                                    OnException("body size is 16 Kbyte over. recv continue. size =" + bodySize.ToString());
                                    FlagSend = false;
                                }

                                using (MemoryStream ms = new MemoryStream())
                                {
                                    ms.Write(headerBuff, 0, headerBuff.Length);

                                    //ボディが0サイズもあり得る
                                    if (bodySize > 0)
                                    {
                                        //本体
                                        byte[] msgBuff = new byte[bodySize];

                                        do
                                        {
                                            recvSize = mNetStream.Read(msgBuff, 0, bodySize);
                                            if (recvSize == 0)
                                            {
                                                //切れた
                                                OnException("body Read error.");
                                                toDisconnect();
                                                throw new ApplicationException("body read error. size=0");
                                            }

                                            ms.Write(msgBuff, 0, recvSize);
                                            bodySize = bodySize - recvSize;
                                        }
                                        while (bodySize > 0);

                                        if (bodySize < 0)
                                        {
                                            OnException("body size is Mismatch " + bodySize.ToString());
                                            //ありえないが保険
                                            FlagSend = false;
                                        }

                                        msgBuff = null;
                                    }
                                    byte[] msg = ms.ToArray();

                                    OnRecvCmd(msg, FlagSend, commandId, kyokuId);
                                }
                            }
                            catch(Exception ex)
                            {
                                OnException(ex.Message);
                                toDisconnect();
                            }
                        }
                    }
                }

            });
        }


        public void SoketCommand(byte[] msg)
        {
            try
            {
                //要求・応答
                mNetStream.Write(msg, 0, msg.Length);
            }
            catch(Exception ex)
            {
                OnException(ex.Message);
            }
        }

        public void Close(bool isStop = true)
        {
            if (isStop)
            {
                IsAlive = false;
            }

            if (mNetStream != null)
            {
                try
                {
                    mNetStream.Close();
                }
                catch { }
            }
            if (mClient != null)
            {
                try
                {
                    mClient.Close();
                }
                catch { }
            }
        }

        private void toDisconnect()
        {
            if (TcpStatus != Status.Disconnect)
            {
                OnDisconnect();
            }
            TcpStatus = Status.Disconnect;

        }

        private void OnRecvCmd(byte[] msg, bool sendFlag, int cmdId, int kyokuId)
        {
            if (RecvTcpCmdEvent != null)
            {
                RecvTcpCmdEvent(this, new TcpEventArg(msg, sendFlag, cmdId, kyokuId));
            }
        }

        private void OnDisconnect()
        {
            if (TcpCloseEvent != null)
            {
                TcpCloseEvent(this, new EventArgs());
            }
        }

        private void OnConnect()
        {
            if (TcpOpenEvent != null)
            {
                TcpOpenEvent(this, new EventArgs());
            }
        }

        private void OnException(string msg)
        {
            if (RecvTcpExcepEvent != null)
            {
                RecvTcpExcepEvent(this, new TcpErrorEventArg(msg));
            }
        }

    }

    public class TcpEventArg : EventArgs
    {
        public DispaMessageCls DisMessage;
        public bool SendFlag;

        public TcpEventArg(byte[] msg, bool sendFlag, int cmdId, int kyokuId)
            : base()
        {
            DisMessage = new DispaMessageCls(msg, cmdId, kyokuId);
            SendFlag = sendFlag;
        }
    }

    public class TcpErrorEventArg: EventArgs
    {
        public string Msg;

        public TcpErrorEventArg(string msg)
            : base()
        {
            Msg = msg;
        }

    }

    public class DispaMessageCls
    {
        public byte[] MessageBin;
        public int KyokuId;
        public int CommandId;

        public DispaMessageCls(byte[] msg, int cmdId, int kyokuId)
        {
            MessageBin = new byte[msg.Length];
            msg.CopyTo(MessageBin, 0);
            KyokuId = kyokuId;
            CommandId = cmdId;
        }
    }


}
