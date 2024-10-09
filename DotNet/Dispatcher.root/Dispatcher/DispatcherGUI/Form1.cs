using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Linq;
using System.Reflection;
using System.ServiceProcess;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DispatcherGUI
{
    public partial class Form1 : Form
    {
        private Thread mStateThread;
        private ServiceController mMyService;
        private bool IsAlive;
        private string mServerIp;
        private string IsDebugLog;
        private readonly string DEF_DISP_NAME = "DispatcherService";


        public Form1()
        {
            InitializeComponent();

            IsAlive = true;

            //DispatcherSetting.configを読み込む
            mServerIp = ConfigurationManager.AppSettings["ServerIp"];
            IsDebugLog = ConfigurationManager.AppSettings["DebugLog"];

            timer1.Start();
        }


        #region 共有メモリから状態取得

        //----------------------------------------------
        // 共有メモリから状態取得
        //----------------------------------------------

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

        public enum Status
        {
            Disconnect = 0,
            ToConnect = 1,
            Connect = 2,
        }

        /// <summary>
        /// 共有メモリから状態取得(Main)
        /// </summary>
        private void ReadDispStatus()
        {
            byte[] data = new byte[Enum.GetNames(typeof(StatusNo)).Length];

            while (IsAlive)
            {
                try
                {
                    using (var mmf = MemoryMappedFile.OpenExisting("Global\\scdispa.bin"))
                    {
                        using (var accessor = mmf.CreateViewAccessor())
                        {
                            accessor.ReadArray<byte>(0, data, 0, data.Length);
                        }
                    }

                    this.Invoke((MethodInvoker)delegate()
                    {
                        this.textBox50001.Text = Enum.GetName(typeof(Status), data[(int)StatusNo.TCP50001]);
                        this.textBox50002.Text = Enum.GetName(typeof(Status), data[(int)StatusNo.TCP50002]);
                        this.textBox50003.Text = Enum.GetName(typeof(Status), data[(int)StatusNo.TCP50003]);
                        this.textBoxPipeState.Text = Enum.GetName(typeof(Status), data[(int)StatusNo.Pipe]);
                        this.textBoxPipeCount.Text = data[(int)StatusNo.PipeCount].ToString();
                        string ips = string.Format("{0}.{1}.{2}.{3}", 
                            data[(int)StatusNo.Ip1],
                            data[(int)StatusNo.Ip2],
                            data[(int)StatusNo.Ip3],
                            data[(int)StatusNo.Ip4]);
                        this.textBoxSvIp.Text = ips;
                        string vsstr = string.Format("{0}.{1}.{2}.{3}",
                            data[(int)StatusNo.VerMajor],
                            data[(int)StatusNo.VerMinor],
                            data[(int)StatusNo.VerBuild],
                            data[(int)StatusNo.VerRev]);
                        this.textBoxVersion.Text = vsstr;

                        SetBackColor(this.textBox50001);
                        SetBackColor(this.textBox50002);
                        SetBackColor(this.textBox50003);
                        SetBackColor(this.textBoxPipeState);


                    });
                }
                catch
                {
                    this.Invoke((MethodInvoker)delegate ()
                    {
                        this.textBox50001.Text = "-----";
                        this.textBox50002.Text = "-----";
                        this.textBox50003.Text = "-----";
                        this.textBoxPipeState.Text = "-----";
                        this.textBoxPipeCount.Text = "-----";
                        this.textBoxSvIp.Text = "-----";
                    });
                }

                Thread.Sleep(500);
            }
        }

        private void SetBackColor(TextBox tbox)
        {
            if (tbox.Text == "Connect")
            {
                tbox.BackColor = Color.SkyBlue;
            }
            else
            {
                tbox.BackColor = Color.Pink;
            }
        }


        #endregion


        private void ToolStripMenuItemSetting_Click(object sender, EventArgs e)
        {

           
            FormSet frm = new FormSet(mServerIp, IsDebugLog);
            if(frm.ShowDialog() == DialogResult.OK)
            {
                Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                //mServerIp = config.AppSettings.Settings["ServerIp"].Value;
                //IsDebugLog = config.AppSettings.Settings["DebugLog"].Value;
                config.AppSettings.Settings["ServerIp"].Value = frm.ServerIp;
                config.AppSettings.Settings["DebugLog"].Value = frm.IsDebugLog;
                config.Save();

                mServerIp = frm.ServerIp;
                IsDebugLog = frm.IsDebugLog;

                this.textBoxSvIp.Text = "-----";
            }
        }

        private void ToolStripMenuItemStart_Click(object sender, EventArgs e)
        {
            if(mMyService.Status == ServiceControllerStatus.Stopped)
            {
                mMyService.Start();
                mMyService.WaitForStatus(ServiceControllerStatus.Running);

                GetServiceStatus();

            }
        }

        private void ToolStripMenuItemStop_Click(object sender, EventArgs e)
        {
            if (mMyService.Status == ServiceControllerStatus.Running)
            {
                mMyService.Stop();
                mMyService.WaitForStatus(ServiceControllerStatus.Stopped);

                GetServiceStatus();

            }

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
           if( MessageBox.Show("終了しますか？","Close",MessageBoxButtons.YesNo)== System.Windows.Forms.DialogResult.No)
           {
               e.Cancel = true;
           }else
           {
               IsAlive = false;
           }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            GetServiceStatus();

            mStateThread = new Thread(new ThreadStart(ReadDispStatus));
            mStateThread.Start();

        }

        private void GetServiceStatus()
        {
            System.ServiceProcess.ServiceController[] controllerList;
            controllerList = ServiceController.GetServices();

            bool ans = false;
            foreach (var ser in controllerList)
            {
                if (ser.ServiceName == DEF_DISP_NAME)
                {
                    ans = true;
                    break;
                }
            }

            if (ans == false)
            {
                labelServiceText.Text = "サービスがインストールされていません";
                this.ToolStripMenuItemStart.Enabled = false;
                this.ToolStripMenuItemStop.Enabled = false;
                this.ToolStripMenuItemIns.Enabled = true;
                this.ToolStripMenuItemUnins.Enabled = false;

                this.textBox50001.Text = "-----";
                this.textBox50002.Text = "-----";
                this.textBox50003.Text = "-----";
                this.textBoxPipeState.Text = "-----";
                this.textBoxPipeCount.Text = "-----";
                this.textBoxSvIp.Text = "-----";
            }
            else
            {
                mMyService = new System.ServiceProcess.ServiceController(DEF_DISP_NAME, ".");

                if (mMyService.Status == ServiceControllerStatus.Running)
                {
                    labelServiceText.Text = "サービスは開始";

                    this.ToolStripMenuItemSetting.Enabled = false;
                    this.ToolStripMenuItemStart.Enabled = false;
                    this.ToolStripMenuItemStop.Enabled = true;
                    this.ToolStripMenuItemIns.Enabled = false;
                    this.ToolStripMenuItemUnins.Enabled = false;
                }
                else if (mMyService.Status == ServiceControllerStatus.Stopped ||
                    mMyService.Status == ServiceControllerStatus.StopPending)
                {
                    labelServiceText.Text = "サービスは停止";

                    this.ToolStripMenuItemSetting.Enabled = true;
                    this.ToolStripMenuItemStart.Enabled = true;
                    this.ToolStripMenuItemStop.Enabled = false;
                    this.ToolStripMenuItemIns.Enabled = false;
                    this.ToolStripMenuItemUnins.Enabled = true;

                    this.textBox50001.Text = "-----";
                    this.textBox50002.Text = "-----";
                    this.textBox50003.Text = "-----";
                    this.textBoxPipeState.Text = "-----";
                    this.textBoxPipeCount.Text = "-----";
                    this.textBoxSvIp.Text = "-----";

                }
                else
                {
                    labelServiceText.Text = "";

                    this.ToolStripMenuItemSetting.Enabled = true;
                    this.ToolStripMenuItemStart.Enabled = false;
                    this.ToolStripMenuItemStop.Enabled = false;
                    this.ToolStripMenuItemIns.Enabled = false;
                    this.ToolStripMenuItemUnins.Enabled = false;

                    this.textBox50001.Text = "-----";
                    this.textBox50002.Text = "-----";
                    this.textBox50003.Text = "-----";
                    this.textBoxPipeState.Text = "-----";
                    this.textBoxPipeCount.Text = "-----";
                    this.textBoxSvIp.Text = "-----";

                }

            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            this.Refresh();
        }

        private void ToolStripMenuItemIns_Click(object sender, EventArgs e)
        {
            Assembly myAsse = Assembly.GetEntryAssembly();
            string path = Path.GetDirectoryName(myAsse.Location);
            path = path + @"\DispatcherService.exe";

            StringBuilder cmd = new StringBuilder();
            cmd.Append(string.Format("create \"{0}\"", DEF_DISP_NAME));
            cmd.Append(" displayname= \"Station control dispatcher\" start= \"auto\"");
            cmd.Append(string.Format(" binPath= \"{0}\"", path));

            using (Process process = new Process())
            {
                process.StartInfo.FileName = @"sc.exe";
                process.StartInfo.Arguments = cmd.ToString();
                process.StartInfo.CreateNoWindow = true;
                process.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                process.Start();

                process.WaitForExit(10000);
            }

            GetServiceStatus();
        }

        private void ToolStripMenuItemUnins_Click(object sender, EventArgs e)
        {
            if(mMyService == null)
            {
                return;
            }

            if (mMyService.Status == ServiceControllerStatus.Running)
            {
                mMyService.Stop();
                mMyService.WaitForStatus(ServiceControllerStatus.Stopped);
            }

            using (Process process = new Process())
            {
                process.StartInfo.FileName = @"sc.exe";
                process.StartInfo.Arguments = "delete \"DispatcherService\"";
                process.StartInfo.CreateNoWindow = true;
                process.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                process.Start();

                process.WaitForExit(10000);
            }

            GetServiceStatus();
        }
        

    }
}
