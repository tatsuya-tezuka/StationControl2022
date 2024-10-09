using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DispatcherGUI
{
    public partial class FormSet : Form
    {
        private string mServerIp;
        public string ServerIp
        {
            get { return mServerIp; }
        }

        private string mIsDebugLog;
        public string IsDebugLog
        {
            get { return mIsDebugLog; }
        }


        public FormSet(string serverIp, string isdebug)
        {
            InitializeComponent();

            mServerIp = serverIp;
            textBoxSvIp.Text = serverIp;

            mIsDebugLog = isdebug;
            if (mIsDebugLog.ToUpper() == "TRUE")
            {
                this.comboBox1.SelectedIndex = 1;
            }
            else
            {
                this.comboBox1.SelectedIndex = 0;
            }
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            if (this.comboBox1.SelectedItem.ToString() == "Enable")
            {
                mIsDebugLog = "true";
            }
            else
            {
                mIsDebugLog = "false";
            }

            string ip = textBoxSvIp.Text;
            IPAddress okIp;
            if (IPAddress.TryParse(ip, out okIp))
            {
                if (MessageBox.Show("保存しますか？", "確認", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    mServerIp = okIp.ToString();
                    this.DialogResult = System.Windows.Forms.DialogResult.OK;
                }
            }
            else
            {
                MessageBox.Show("IPアドレスが有効ではありません", "注意", MessageBoxButtons.OK);
            }
        }

        private void buttonConcel_Click(object sender, EventArgs e)
        {
            this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
        }
    }
}
