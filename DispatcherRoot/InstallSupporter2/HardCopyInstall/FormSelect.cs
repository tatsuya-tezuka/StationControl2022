using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Install.Core
{
    public partial class FormSelect : Form
    {
        private string FileName;
        private string ZipName;

        private string ZipPath;
        private string LogFile;
        private string OutPath;

        public FormSelect(string name, string outPath, string zipName, string logFile)
        {
            InitializeComponent();

            FileName = name;
            ZipName = zipName;
            LogFile = logFile;

            label4.Text = outPath;
            OutPath = outPath;
            label1.Text = name + "のZipファイルを展開します。";
            this.Text = name;
        }

        public FormSelect(string name, string outPath, string zipName, string logFile, string displayOut)
        {
            InitializeComponent();

            FileName = name;
            ZipName = zipName;
            LogFile = logFile;

            label4.Text = displayOut;
            OutPath = outPath;
            label1.Text = name + "のZipファイルを展開します。";
            this.Text = name;
        }

        private void textBox1_DragDrop(object sender, DragEventArgs e)
        {
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop, false);
            if (files == null || files.Length <= 0)
            {
                return;
            }

            textBox1.Text = files[0];
        }

        private void textBox1_DragEnter(object sender, DragEventArgs e)
        {
            //ファイルがドラッグされている場合、カーソルを変更する
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Copy;
            }
        }

        private void button_ok_Click(object sender, EventArgs e)
        {
            ZipPath = textBox1.Text;

            Core core = new Core(OutPath, LogFile, ZipPath);
            core.Executing();

            this.Close();
        }

        private void button_skip_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(textBox1.Text))
            {
                button_ok.Enabled = false;
            }
            else
            {
                button_ok.Enabled = true;
            }
        }

        private void FormSelect_Load(object sender, EventArgs e)
        {

            string cName = System.Globalization.CultureInfo.CurrentCulture.Name;
            Encoding ed = Encoding.GetEncoding(932);
            if (cName.Contains("en-"))
            {
                ed = Encoding.GetEncoding(437);
            }

            string currentDir = "";
            try
            {
                //カレントディレクトリ
                StreamReader sr = new StreamReader(@"C:\~faist.txt", ed);
                currentDir = sr.ReadLine();
                sr.Close();
                currentDir = currentDir.Trim();
            }
            catch (Exception)
            {}

            if (!string.IsNullOrEmpty(currentDir))
            {
                string zipPath = Path.Combine(currentDir, ZipName);
                ZipPath = zipPath;
            }
            else
            {
                ZipPath = "";
                button_ok.Enabled = false;
            }

            textBox1.Text = ZipPath;

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //OpenFileDialogクラスのインスタンスを作成
            OpenFileDialog ofd = new OpenFileDialog();

            //はじめのファイル名を指定する
            ofd.FileName = ZipName;
            //はじめに表示されるフォルダを指定する
            //指定しない（空の文字列）の時は、現在のディレクトリが表示される
            ofd.InitialDirectory = @"C:\";
            //[ファイルの種類]に表示される選択肢を指定する
            //指定しないとすべてのファイルが表示される
            ofd.Filter = "Zipファイル(*.zip)|*.zip";
            //[ファイルの種類]ではじめに選択されるものを指定する
            //2番目の「すべてのファイル」が選択されているようにする
            ofd.FilterIndex = 1;
            //タイトルを設定する
            ofd.Title = "開くファイルを選択してください";
            //ダイアログボックスを閉じる前に現在のディレクトリを復元するようにする
            ofd.RestoreDirectory = true;
            //存在しないファイルの名前が指定されたとき警告を表示する
            //デフォルトでTrueなので指定する必要はない
            ofd.CheckFileExists = true;
            //存在しないパスが指定されたとき警告を表示する
            //デフォルトでTrueなので指定する必要はない
            ofd.CheckPathExists = true;

            //ダイアログを表示する
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                ZipPath = ofd.FileName;
                //OKボタンがクリックされたとき、選択されたファイル名を表示する
                textBox1.Text = ZipPath;
            }

        }
    }
}
