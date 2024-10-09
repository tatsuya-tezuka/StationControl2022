using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading.Tasks;
using System.Threading;
using System.Diagnostics;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        
        NamedPipeClientStream streamIn;
        NamedPipeClientStream streamOut;
        string PipeInName;
        string PipeOutName;

        public Form1()
        {
            InitializeComponent();

            Version vs = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version;

            this.Text += "  version " + vs.ToString();
            this.comboBox1.SelectedIndex = 0;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            PipeInName = this.comboBox1.Items[this.comboBox1.SelectedIndex].ToString() + "_IN";
            PipeOutName = this.comboBox1.Items[this.comboBox1.SelectedIndex].ToString() + "_OUT";

            this.button1.Enabled = false;
            Task.Factory.StartNew(() =>
                {
                    try
                    {
                        streamIn = new NamedPipeClientStream(".", PipeInName, PipeDirection.Out);
                        streamOut = new NamedPipeClientStream(".", PipeOutName, PipeDirection.In);

                        streamIn.Connect();
                        streamOut.Connect();

                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }

                    byte[] recv = new byte[16];
                    int count = 0;
                    int recvSize = 0;
                    Stopwatch sw = new Stopwatch();
                    while (true)
                    {
                        sw.Start();

                        recvSize = streamOut.Read(recv, 0, recv.Length);

                        if(recvSize == 0)
                        {
                            break;
                        }
                        else if(recvSize != 16)
                        {
                            int r_size = recvSize;
                            int h_length = 16 - r_size;
                            using (MemoryStream hms = new MemoryStream())
                            {
                                hms.Write(recv, 0, r_size);

                                do
                                {
                                    r_size = streamOut.Read(recv, 0, h_length);
                                    if (r_size == 0)
                                    {
                                        break;
                                    }

                                    hms.Write(recv, 0, r_size);
                                    h_length = h_length - r_size;
                                }
                                while (h_length > 0);

                                recv = hms.ToArray();
                            }
                        }

                        int bodySize = BitConverter.ToInt32(recv, 0);

                        using (MemoryStream ms = new MemoryStream())
                        {
                            ms.Write(recv, 0, recv.Length);

                            //本体
                            byte[] msgBuff = new byte[bodySize];
                            do
                            {
                                recvSize = streamOut.Read(msgBuff, 0, bodySize);
                                if (recvSize == 0)
                                {
                                    break;
                                }
                                ms.Write(msgBuff, 0, recvSize);
                                bodySize = bodySize - recvSize;
                            }
                            while (bodySize > 0);

                            sw.Stop();

                            byte[] logf = ms.ToArray();
                            getRecv(DateTime.Now.ToString("HH:mm:ss.fff") + " "+ sw.Elapsed.TotalMilliseconds.ToString() + "\tlen=" + logf.Length.ToString() + "\t" + BitConverter.ToString(logf, 0, 16));

                            if (this.checkBox1.Checked)
                            {                                
                                LogOutput(logf, count);
                            }
                        }

                        count++;
                        if(count >= 100)
                        {
                            count = 0;

                        }

                        sw.Reset();
                        Thread.Sleep(0);
                    }


                });
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            byte[] tmp = new byte[] { 0x0a, 0x0a, 0x0a, 0x0a };
            ConnectCmdMessage ccm = new ConnectCmdMessage(tmp);

            byte[] msg = ccm.ToByte();

            streamIn.Write(msg, 0, msg.Length);
            streamIn.Flush();
         
        }

        private void getRecv(string s)
        {
            this.Invoke((MethodInvoker)delegate()
            {
                if(listBox1.Items.Count > 2000)
                {
                    this.listBox1.Items.Clear();
                }
                this.listBox1.Items.Add(s);
                this.listBox1.TopIndex = this.listBox1.Items.Count - 1;
            });
        }

        private void button4_Click(object sender, EventArgs e)
        {
            TestEvtMessage evt = new TestEvtMessage();

            byte[] msg = evt.ToByte();

            streamIn.Write(msg, 0, msg.Length);
            streamIn.Flush();
        }

        private void LogOutput(byte[] msg, int count)
        {
            Task.Factory.StartNew(() =>
            {
                try
                {
                    string f = string.Format("Recv-Pipe-{0}-{1}", DateTime.Now.ToString("MMddHHmmssfff"), count.ToString("D2"));
                    File.WriteAllBytes(f, msg);
                }
                catch { }
            });
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
                ms.Write(BitConverter.GetBytes(MessageLength), 0, sizeof(int));

                ms.Write(BitConverter.GetBytes(CommnadId), 0, sizeof(int));

                ms.Write(BitConverter.GetBytes(KyokuId), 0, sizeof(int));

                ms.Write(BitConverter.GetBytes(MatchingId), 0, sizeof(int));

                ms.Write(IpAddress, 0, IpAddress.Length);

                ms.Close();

                return ms.ToArray();
            }

        }

    }

    public class TestEvtMessage
    {
        public const int DEF_REQ = 201;

        public int MessageLength;
        public int CommnadId;
        public int KyokuId;
        public int MatchingId;

        public byte[] Body;

        public TestEvtMessage()
        {
            Body = new byte[16 * 1024];
        }

        public byte[] ToByte()
        {

            MessageLength = Body.Length;
            CommnadId = DEF_REQ;
            KyokuId = 0;
            MatchingId = 0xFF;

            using (MemoryStream ms = new MemoryStream())
            {
                ms.Write(BitConverter.GetBytes(MessageLength), 0, sizeof(int));

                ms.Write(BitConverter.GetBytes(CommnadId), 0, sizeof(int));

                ms.Write(BitConverter.GetBytes(KyokuId), 0, sizeof(int));

                ms.Write(BitConverter.GetBytes(MatchingId), 0, sizeof(int));

                ms.Write(Body, 0, Body.Length);

                ms.Close();

                return ms.ToArray();
            }

        }

    }
}
