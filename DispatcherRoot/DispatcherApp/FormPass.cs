using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Dispatcher
{
    public partial class FormPass : Form
    {
        private const string PASS = "kfkf";

        public FormPass()
        {
            InitializeComponent();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            if (this.textBox1.Text == PASS)
            {
                DialogResult = DialogResult.OK;
            }
            else
            {
                MessageBox.Show("パスワードが正しくありません。", "パスワード入力結果", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                DialogResult = DialogResult.Cancel;
            }
        }
    }
}
