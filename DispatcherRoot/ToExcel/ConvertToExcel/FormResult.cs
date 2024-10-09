using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ConvertToExcel
{
    public partial class FormResult : Form
    {
        private string ErrMsg = string.Empty;

        public FormResult(string msg)
        {
            InitializeComponent();
            ErrMsg = msg;
        }

        private void buttonOk_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
        }

        private void buttonDetails_Click(object sender, EventArgs e)
        {
            MessageBox.Show(ErrMsg, "詳細", MessageBoxButtons.OK);
        }
    }
}
