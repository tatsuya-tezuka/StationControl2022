using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Reflection;
using System.Threading;
using System.Runtime.InteropServices;

namespace ConvertToExcel
{
    public partial class Form1 : Form
    {
        private bool Is_Debug = false;

        /// <summary>
        /// Excelを最終的に閉じるかどうか。
        /// </summary>
        private bool Is_Close = false;

        private string _InputFile;
        private bool _IsDisplay = true;
        private string _OutputFile;
        private string _OutputDir = Environment.GetFolderPath(Environment.SpecialFolder.Personal);

        private CombOnj SelecedObj = null;
        private List<CombOnj> FileTypeList = new List<CombOnj>();
        private string KyokuName = string.Empty;
        private string KyokuNameJp = string.Empty;
        private string Option = string.Empty;

        private const string DEF_UDSC54 = "UDSC54";

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public Form1()
        {
            InitializeComponent();

            if (Properties.Settings.Default.IsDebug == "true")
            {
                Is_Debug = true;
                this.button1.Visible = true;
            }
            ArgsAnalyzer(System.Environment.GetCommandLineArgs());

            //局名取得
            try
            {
                //if (_InputFile.Contains(@"\share\"))
                int start = _InputFile.IndexOf(@"\share\", StringComparison.OrdinalIgnoreCase);
                if (start >= 0)
                {
                    start += @"\share\".Length;
                    int end = _InputFile.IndexOf(@"\", start);
                    KyokuName = _InputFile.Substring(start, end - start).ToUpper();
                    KyokuNameJp = ToJpKyokuName(KyokuName);
                }
            }
            catch
            { }


            this.progressBar1.Minimum = 0;
            this.progressBar1.Maximum = 100;
            this.progressBar1.Value = 0;
            this.backgroundWorker1.WorkerReportsProgress = true;
            this.progressBar1.MarqueeAnimationSpeed = 0;

            this.backgroundWorker1.DoWork += BackgroundWorker1_DoWork;
            this.backgroundWorker1.ProgressChanged += BackgroundWorker1_ProgressChanged;
            this.backgroundWorker1.RunWorkerCompleted += BackgroundWorker1_RunWorkerCompleted;

            FileTypeList.Add(new CombOnj() { Name = "アンテナ予報値データ(近地球)", ExcelType = FileType.ANT_YOHO_KinChikyu, TempFileName = "アンテナ予報値データ(近地球用).xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "アンテナ予報値データ(深宇宙)", ExcelType = FileType.ANT_YOHO_ShinUchu, TempFileName = "アンテナ予報値データ(深宇宙用).xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "運用制御情報(近地球)", ExcelType = FileType.UNYO_SEIGYO_KinChikyu, TempFileName = "運用制御情報(近地球用).xlsx", IsDisplay = true });
            FileTypeList.Add(new CombOnj() { Name = "運用制御情報(深宇宙)", ExcelType = FileType.UNYO_SEIGYO_ShinUchu, TempFileName = "運用制御情報(深宇宙用).xlsx", IsDisplay = true });
            FileTypeList.Add(new CombOnj() { Name = "局運用計画", ExcelType = FileType.KYOKU_UNNYO_KYOKU, TempFileName = "局運用計画.xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "無線業務日誌", ExcelType = FileType.MUSEN_NISSHI, TempFileName = "無線業務日誌.xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "RF情報", ExcelType = FileType.RF_INFO, TempFileName = "RF情報ファイル.xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "パス履歴", ExcelType = FileType.PATH_RIREKI, TempFileName = "パス履歴.xlsx", IsDisplay = true });
            FileTypeList.Add(new CombOnj() { Name = "運用履歴", ExcelType = FileType.UNYO_RIREKI, TempFileName = "運用履歴.xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "制御履歴", ExcelType = FileType.SEIGYO_RIREKI, TempFileName = "制御履歴.xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "設備状態履歴", ExcelType = FileType.SETSUBI_RIREKI, TempFileName = "設備状態履歴.xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "設備監視蓄積データ", ExcelType = FileType.KANSHI_CHIKUSEKI, TempFileName = "設備監視蓄積データ.xlsx", IsDisplay = true });
            FileTypeList.Add(new CombOnj() { Name = "アンテナ設備駆動履歴ファイル", ExcelType = FileType.ANT_KUDO_RIREKI, TempFileName = "アンテナ設備駆動履歴ファイル.xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "アンテナ設備運用履歴ファイル", ExcelType = FileType.ANT_UNYO_RIREKI, TempFileName = "アンテナ設備運用履歴ファイル.xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "イベント履歴", ExcelType = FileType.EVENT_RIREKI, TempFileName = "イベント履歴.xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "追跡データファイル", ExcelType = FileType.TRK_HIST, TempFileName = "追跡データファイル.xlsx", IsDisplay = true });
            FileTypeList.Add(new CombOnj() { Name = "送信周波数情報データファイル", ExcelType = FileType.SEND_FREQ_INFO, TempFileName = "送信周波数情報データファイル.xlsx", IsDisplay = true });
            FileTypeList.Add(new CombOnj() { Name = "RF情報2", ExcelType = FileType.RF_INFO2, TempFileName = "RF情報ファイル2.xlsx", IsDisplay = false });
            //udsc54対応
            FileTypeList.Add(new CombOnj() { Name = "運用制御情報(深宇宙) udsc54", ExcelType = FileType.UNYO_SEIGYO_ShinUchu_udsc54, TempFileName = "運用制御情報(深宇宙用)udsc54.xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "パス履歴 udsc54", ExcelType = FileType.PATH_RIREKI_udsc54, TempFileName = "パス履歴udsc54.xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "局運用計画 udsc54", ExcelType = FileType.KYOKU_UNNYO_KYOKU_udsc54, TempFileName = "局運用計画udsc54.xlsx", IsDisplay = false });
            FileTypeList.Add(new CombOnj() { Name = "アンテナ設備駆動履歴ファイル udsc54", ExcelType = FileType.ANT_KUDO_RIREKI_udsc54, TempFileName = "アンテナ設備駆動履歴ファイルudsc54.xlsx", IsDisplay = false });

            for (int i = 0; i < FileTypeList.Count; i++)
            {
                if (FileTypeList[i].IsDisplay)
                {
                    this.comboBox1.Items.Add(FileTypeList[i]);
                }
            }

            SelecedObj = IsOrignalCSV();
            if (SelecedObj != null)
            {
                //非表示
                _IsDisplay = false;
            }
            else
            {
                this.comboBox1.SelectedIndex = 0;
            }


            _OutputFile = CreateOutputFileName();

            textBoxSelect.Text = _InputFile;
            displayFilePath(pictureBox1, _InputFile);
            textBoxOutput.Text = _OutputFile;

            if (_IsDisplay == false)
            {
                this.backgroundWorker1.RunWorkerAsync();
            }
        }

        /// <summary>
        /// 進捗状態表示
        /// </summary>
        private void BackgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            this.progressBar1.MarqueeAnimationSpeed = e.ProgressPercentage;
        }

        /// <summary>
        /// 進捗結果
        /// </summary>
        private void BackgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                string msg = "帳票作成失敗\n";
                if (SelecedObj != null)
                {
                    msg += SelecedObj.Name + "\n";
                }
                msg += e.Error.Message + "\n\n";
                if (!string.IsNullOrEmpty(e.Error.StackTrace))
                {
                    msg += e.Error.StackTrace;
                }
                Log.ExWrite(msg);

                FormResult fr = new FormResult(msg);
                fr.ShowDialog();
            }
            else
            {
                //MessageBox.Show("完了", "帳票作成ツール", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            this.Close();
        }

        /// <summary>
        /// バックグランドで実行される
        /// </summary>
        private void BackgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker bw = (BackgroundWorker)sender;
            bw.ReportProgress(100, null);

            ConvExcel();

            bw.ReportProgress(0, null);
        }

        /// <summary>
        /// 引数解析
        /// </summary>
        private void ArgsAnalyzer(string[] args)
        {

            //出力ファイル名
            try
            {
                var dir = Array.Find(args, cmd => cmd.Contains("/O")).Substring("/O".Length);
                if (!string.IsNullOrEmpty(dir))
                {
                    _OutputDir = dir;
                }
            }
            catch
            {
                _OutputDir = string.Empty;
            }

            //入力ファイル名
            var ans = from n in args where !n.Contains("/O") && !n.Contains(".exe") && !n.Contains("/P") select n;
            if (ans.Any())
            {
                var list = ans.ToList();
                if (list.Count > 1)
                {
                    throw new NoActionException(Properties.Resources.ERROR_ARGS);
                }

                _InputFile = list.First();

                if (_OutputDir == string.Empty)
                {
                    _OutputDir = Path.GetDirectoryName(_InputFile);
                }
            }
            else
            {
                if (!Is_Debug)
                {
                    throw new NoActionException(Properties.Resources.ERROR_ARGS);
                }
            }

            //オプション
            try
            {
                var dir = Array.Find(args, cmd => cmd.Contains("/P0"));
                if (!string.IsNullOrEmpty(dir))
                {
                    //アンテナ予報値(近地球)
                    Option = "P0";
                }
            }
            catch
            {}
            try
            {
                var dir = Array.Find(args, cmd => cmd.Contains("/P1"));
                if (!string.IsNullOrEmpty(dir))
                {
                    //アンテナ予報値(深宇宙)
                    Option = "P1";
                }
            }
            catch
            { }

            return;
        }

        /// <summary>
        /// 端末で生成したファイルかどうか
        /// </summary>
        /// <returns></returns>
        private CombOnj IsOrignalCSV()
        {
            string ext = Path.GetExtension(_InputFile);
            string file = Path.GetFileNameWithoutExtension(_InputFile);

            if(_InputFile==null)
            {
                return null;
            }

            if(ext == ".vrxp")
            {
                //局運用計画
                if (KyokuName == DEF_UDSC54)
                {
                    return FileTypeList[(int)FileType.KYOKU_UNNYO_KYOKU_udsc54];
                }
                return FileTypeList[(int)FileType.KYOKU_UNNYO_KYOKU];
            }
            else if(ext == ".vrxd")
            {
                //無線業務日誌
                return FileTypeList[(int)FileType.MUSEN_NISSHI];
            }
            else if (ext == ".vrxr")
            {
                //RF情報
                return FileTypeList[(int)FileType.RF_INFO];
            }
            else if (ext == ".vrxr2")
            {
                //RF情報
                return FileTypeList[(int)FileType.RF_INFO2];
            }
            else if (ext == ".vrxo")
            {
                //運用履歴
                return FileTypeList[(int)FileType.UNYO_RIREKI];
            }
            else if (ext == ".vrxc")
            {
                //制御履歴
                return FileTypeList[(int)FileType.SEIGYO_RIREKI];
            }
            else if (ext == ".vrxm")
            {
                //設備状態履歴
                return FileTypeList[(int)FileType.SETSUBI_RIREKI];
            }
            else if (ext == ".vrxe")
            {
                //イベント履歴
                return FileTypeList[(int)FileType.EVENT_RIREKI];
            }
            else if (Option == "P0")
            {
                //アンテナ予報値（近地球）
                return FileTypeList[(int)FileType.ANT_YOHO_KinChikyu];
            }
            else if (Option == "P1")
            {
                //アンテナ予報値（深宇宙）
                return FileTypeList[(int)FileType.ANT_YOHO_ShinUchu];
            }
            else if(file.StartsWith("kdu") && ext == ".csv")
            {
                if (KyokuName == DEF_UDSC54)
                {
                    //アンテナ設備駆動履歴
                    return FileTypeList[(int)FileType.ANT_KUDO_RIREKI_udsc54];
                }
                //アンテナ設備駆動履歴
                return FileTypeList[(int)FileType.ANT_KUDO_RIREKI];
            }
            else if (file.StartsWith("uny") && ext == ".csv")
            {
                //アンテナ設備運用履歴
                return FileTypeList[(int)FileType.ANT_UNYO_RIREKI];
            }
#if false
            else if (Path.GetExtension(file) == ".trk" && ext == ".csv")
            {
                //追跡データファイル
                return FileTypeList[(int)FileType.TRK_HIST];
            }
            else if (Path.GetExtension(file) == ".frq" && ext == ".csv")
            {
                //送信周波数情報ファイル
                return FileTypeList[(int)FileType.SEND_FREQ_INFO];
            }
#endif

            return null;
        }

        /// <summary>
        /// 出力ファイル名の生成
        /// </summary>
        /// <returns></returns>
        private string CreateOutputFileName()
        {
            string file = Path.GetFileNameWithoutExtension(SelecedObj.TempFileName);
            string ext = Path.GetExtension(SelecedObj.TempFileName);

            //テンプレートのファイル名は違うが、出力ファイル名は合わせたい時はここで変換
            file = file.Replace("RF情報ファイル2", "RF情報ファイル");
            file = file.Replace("udsc54", "");

            return Path.Combine(_OutputDir, file + DateTime.Now.ToString("-yyyyMMddHHmmssfff") + ext);
        }

        /// <summary>
        /// PictureBoxに文字を表示する。（枠を超えた場合は省略文字）
        /// </summary>
        /// <param name="_picturebox"></param>
        /// <param name="_msg"></param>
        private void displayFilePath(PictureBox _picturebox, string _msg)
        {
            Bitmap canv = new Bitmap(_picturebox.Width, _picturebox.Height);
            Graphics g = Graphics.FromImage(canv);

            TextRenderer.DrawText(g, _msg, this.Font, new Rectangle(0, 3, _picturebox.Width, _picturebox.Height), Color.Black, TextFormatFlags.PathEllipsis);
            g.Dispose();

            _picturebox.Image = canv;
        }

        /// <summary>
        /// 画面が表示される直前
        /// </summary>
        private void Form1_Activated(object sender, EventArgs e)
        {
            if (_IsDisplay == false)
            {
                //this.Hide();
                this.buttonSelectDir.Enabled = false;
                this.buttonStart.Enabled = false;
                this.comboBox1.Enabled = false;
            }
        }


        private void comboBox1_TextChanged(object sender, EventArgs e)
        {
            SelecedObj = (CombOnj)this.comboBox1.SelectedItem;

            if (KyokuName == DEF_UDSC54)
            {
                if (SelecedObj.ExcelType == FileType.UNYO_SEIGYO_ShinUchu)
                {
                    SelecedObj = FileTypeList[(int)FileType.UNYO_SEIGYO_ShinUchu_udsc54];
                }
                else if (SelecedObj.ExcelType == FileType.PATH_RIREKI)
                {
                    SelecedObj = FileTypeList[(int)FileType.PATH_RIREKI_udsc54];
                }
            }

            _OutputFile = CreateOutputFileName();
            textBoxOutput.Text = _OutputFile;

        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            this.buttonStart.Enabled = false;
            this.backgroundWorker1.RunWorkerAsync();
        }


        private void ConvExcel()
        {
            CombOnj obj = SelecedObj;
            //添付ファイルをコピーする。
            if(!File.Exists(_OutputFile))
            File.Copy(obj.FullTempFile(), _OutputFile);

            switch (obj.ExcelType)
            {
                case FileType.ANT_YOHO_KinChikyu:
                case FileType.ANT_YOHO_ShinUchu:
                    ANT_YOHO();
                    break;
                case FileType.UNYO_SEIGYO_KinChikyu:
                case FileType.UNYO_SEIGYO_ShinUchu:
                case FileType.UNYO_SEIGYO_ShinUchu_udsc54:
                    UNYO_SEIGYO();
                    break;
                case FileType.PATH_RIREKI:
                case FileType.PATH_RIREKI_udsc54:
                    PATH_RIREKI();
                    break;
                case FileType.UNYO_RIREKI:
                case FileType.SEIGYO_RIREKI:
                case FileType.SETSUBI_RIREKI:
                case FileType.EVENT_RIREKI:
                    RIREKI();
                    break;
                case FileType.KANSHI_CHIKUSEKI:
                    KANSHI_CHIKUSEKI();
                    break;
                case FileType.ANT_KUDO_RIREKI:
                case FileType.ANT_KUDO_RIREKI_udsc54:
                    ANT_KUDO_RIREKI();
                    break;
                case FileType.ANT_UNYO_RIREKI:
                    ANT_UNYO_RIREKI();
                    break;
                case FileType.MUSEN_NISSHI:
                    MUSEN_NISSHI();
                    break;
                case FileType.RF_INFO:
                case FileType.RF_INFO2:
                    RF_INFO();
                    break;
                case FileType.KYOKU_UNNYO_KYOKU:
                case FileType.KYOKU_UNNYO_KYOKU_udsc54:
                    KYOKU_UNNYO_KYOKU();
                    break;
                case FileType.TRK_HIST:
                    TRK_HIST();
                    break;
                case FileType.SEND_FREQ_INFO:
                    SEND_FREQ_INFO();
                    break;
                default:
                    break;
            }

            try
            {
                if (obj.IsDisplay == false)
                {
                    //消してはだめなファイル
                    if(obj.ExcelType != FileType.ANT_YOHO_KinChikyu &&
                       obj.ExcelType != FileType.ANT_YOHO_ShinUchu &&
                       obj.ExcelType != FileType.ANT_KUDO_RIREKI &&
                       obj.ExcelType != FileType.ANT_KUDO_RIREKI_udsc54 &&
                       obj.ExcelType != FileType.ANT_UNYO_RIREKI &&
                       obj.ExcelType != FileType.TRK_HIST &&
                       obj.ExcelType != FileType.SEND_FREQ_INFO &&
                       obj.ExcelType != FileType.UNYO_SEIGYO_ShinUchu_udsc54 &&
                       obj.ExcelType != FileType.PATH_RIREKI_udsc54 )
                    File.Delete(this._InputFile);
                }
            }
            catch { }
        }

        private string[] ReadInputFile()
        {
            Encoding enc = Encoding.GetEncoding("shift_jis");

            //テキストファイルの中身をすべて読み込む
            string[] lines = File.ReadAllLines(_InputFile, enc);

            return lines;
        }

        private string[] ReadInputFileEUC()
        {
            Encoding enc = Encoding.GetEncoding("euc-jp");

            //テキストファイルの中身をすべて読み込む
            string[] lines = File.ReadAllLines(_InputFile, enc);

            return lines;
        }

        private char[] spWord = new char[] { ' ' };

#region アンテナ予報値データ
        /// <summary>
        /// アンテナ予報値データ
        /// </summary>
        private void ANT_YOHO()
        {
            string[] lines = ReadInputFile();
            List<string[]> data = new List<string[]>();
            string eisei_name = "";
            string kyoku_name = "";
            string[] HEAD = null;
            string[] AOS = null;
            string[] LOS = null;
            string[] MAX = null;

            //データの先頭を検索
            int StartIndex = 0;
            for (int i = 0; i < lines.Length; i++)
            {
                string[] tmp = lines[i].Split(spWord, StringSplitOptions.RemoveEmptyEntries);

                if (tmp[0] == "#!HEAD:")
                {
                    eisei_name = tmp[6];
                    kyoku_name = KyokuNameJp;
                    HEAD = tmp.ToArray();
                }
                else if (tmp[0] == "VIS-AOS")
                {
                    AOS = tmp.ToArray();
                }
                else if (tmp[0] == "VIS-LOS")
                {
                    LOS = tmp.ToArray();

                }
                else if (tmp[0] == "VIS-MAX")
                {
                    MAX = tmp.ToArray();
                }

                DateTime dt;
                if (DateTime.TryParseExact(tmp[0], "yyyyMMddHHmmss",
                    System.Globalization.DateTimeFormatInfo.InvariantInfo,
                    System.Globalization.DateTimeStyles.None,
                    out dt))
                {
                    if (StartIndex == 0)
                    {
                        StartIndex = i;
                    }
                    tmp[0] = dt.ToString("yyyy-MM-dd HH:mm:ss");
                    data.Add(tmp);
                }
            }
            if (data.Count == 0)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            object[,] exc = new object[data.Count, data[0].Length];
            for (int row = 0; row < data.Count; row++)
            {
                for (int col = 0; col < data[0].Length; col++)
                {
                    exc[row, col] = data[row][col];
                }
            }

            Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
            //エクセルを非表示
            ExcelApp.Visible = !Is_Close;

            //エクセルファイルのオープンと
            //ワークブックをの作成
            Microsoft.Office.Interop.Excel.Workbook WorkBook = ExcelApp.Workbooks.Open(_OutputFile);

            //1シート目の選択
            Microsoft.Office.Interop.Excel.Worksheet sheet = WorkBook.Sheets["Sheet1"];
            sheet.Select();

            try
            {
                //表紙
                sheet.Range["I12"].Value = HEAD[2].Replace("/", "-") + " " + HEAD[3];
                sheet.Range["I14"].Value = eisei_name;
                sheet.Range["I15"].Value = kyoku_name;
                sheet.Range["I16"].Value = HEAD[7];
                sheet.Range["I17"].Value = DateTimeStringConv(AOS[2]);
                sheet.Range["I18"].Value = AOS[3];
                sheet.Range["I19"].Value = AOS[4];
                sheet.Range["I20"].Value = DateTimeStringConv(LOS[2]);
                sheet.Range["I21"].Value = LOS[3];
                sheet.Range["I22"].Value = LOS[4];
                sheet.Range["I23"].Value = DateTimeStringConv(MAX[2]);
                sheet.Range["I24"].Value = MAX[3];
                sheet.Range["I25"].Value = MAX[4];

                //2シート目の選択
                sheet = WorkBook.Sheets["Sheet2"];
                sheet.Select();

                sheet.Range["D3"].Value = eisei_name;
                sheet.Range["D4"].Value = kyoku_name;
                sheet.Range["D5"].Value = HEAD[7];

                if (SelecedObj.ExcelType == FileType.ANT_YOHO_KinChikyu)
                {
                    //格子を書く
                    sheet.Range["B7:H" + (data.Count + 6).ToString()].Borders.LineStyle = 1;
                    //データを書く
                    sheet.Range["B7:H" + (data.Count + 6).ToString()].Value2 = exc;
                }
                else
                {
                    //格子を書く
                    sheet.Range["B7:K" + (data.Count + 6).ToString()].Borders.LineStyle = 1;
                    //データを書く
                    sheet.Range["B7:K" + (data.Count + 6).ToString()].Value2 = exc;
                }

                // 作成者、前回保存者、会社のプロパティは削除する
                dynamic properties = WorkBook.BuiltinDocumentProperties;
                dynamic property = properties.Item("Author");
                property.Value = "";
                property = properties.Item("Last author");
                property.Value = "";
                property = properties.Item("Company");
                property.Value = "";

                WorkBook.Save();

                if (Is_Close)
                {
                    //ワークブックを閉じる
                    WorkBook.Close();
                    //エクセルを閉じる
                    ExcelApp.Quit();
                }

            }
            finally
            {
                if (sheet != null)
                {
                    Marshal.ReleaseComObject(sheet);
                    sheet = null;
                }
                if (WorkBook != null)
                {
                    Marshal.ReleaseComObject(WorkBook);
                    WorkBook = null;
                }

                if (ExcelApp != null)
                {
                    Marshal.ReleaseComObject(ExcelApp);
                    ExcelApp = null;
                }
                GC.Collect();
            }

        }
#endregion

        private string DateTimeStringConv(string moji)
        {
            DateTime dt = DateTime.ParseExact(moji, "yyyyMMddHHmmss", null);
            return dt.ToString("yyyy-MM-dd HH:mm:ss");
        }


#region 運用制御
        /// <summary>
        /// 運用制御
        /// </summary>
        private void UNYO_SEIGYO()
        {
            string[] lines = ReadInputFile();
            List<string[]> data = new List<string[]>();
            string eisei_name = "";
            string kyoku_name = "";
            string[] HEAD = null;
            string[] AOS = null; 
            string[] LOS = null; 
            string[] MAX = null; 

            //データの先頭を検索
            int StartIndex = 0;
            for (int i = 0; i < lines.Length; i++)
            {
                string[] tmp = lines[i].Split(spWord, StringSplitOptions.RemoveEmptyEntries);

                if (tmp[0] == "#!HEAD:")
                {
                    eisei_name = tmp[6];
                    kyoku_name = KyokuNameJp;
                    HEAD = tmp.ToArray();
                }
                else if (tmp[0] == "VIS-AOS")
                {
                    AOS = tmp.ToArray();
                }
                else if (tmp[0] == "VIS-LOS")
                {
                    LOS = tmp.ToArray();

                }
                else if (tmp[0] == "VIS-MAX")
                {
                    MAX = tmp.ToArray();
                }

                DateTime dt;
                if (DateTime.TryParseExact(tmp[0], "yyyy-MM-dd",
                    System.Globalization.DateTimeFormatInfo.InvariantInfo,
                    System.Globalization.DateTimeStyles.None,
                    out dt))
                {
                    if (StartIndex == 0)
                    {
                        StartIndex = i;
                    }
                    data.Add(tmp);
                }
            }
            if (data.Count == 0)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            object[,] exc = new object[data.Count, data[0].Length];
            for (int row = 0; row < data.Count; row++)
            {
                for (int col = 0; col < data[0].Length; col++)
                {
                    exc[row, col] = data[row][col];
                }
            }

            Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
            //エクセルを非表示
            ExcelApp.Visible = !Is_Close;

            //エクセルファイルのオープンと
            //ワークブックをの作成
            Microsoft.Office.Interop.Excel.Workbook WorkBook = ExcelApp.Workbooks.Open(_OutputFile);

            //1シート目の選択
            string sheetNaem = string.Empty;
            if(SelecedObj.ExcelType == FileType.UNYO_SEIGYO_KinChikyu)
            {
                sheetNaem = "OPE_CONT_INFO_NR_EARTH";
            }
            else
            {
                sheetNaem = "OPE_CONT_INFO_OP_SPACE";
            }


            Microsoft.Office.Interop.Excel.Worksheet sheet = WorkBook.Sheets[sheetNaem];
            sheet.Select();

            try
            {
                //ヘッダー
                sheet.Range["D2"].Value = Path.GetFileName(_InputFile);
                sheet.Range["G2"].Value = HEAD[2] + " " + HEAD[3];
                sheet.Range["D3"].Value = eisei_name;
                sheet.Range["G3"].Value = kyoku_name;
                sheet.Range["I3"].Value = HEAD[7];
                sheet.Range["D4"].Value = DateTimeStringConv(AOS[2]);
                sheet.Range["G4"].Value = AOS[3];
                sheet.Range["I4"].Value = AOS[4];
                sheet.Range["D5"].Value = DateTimeStringConv(LOS[2]);
                sheet.Range["G5"].Value = LOS[3];
                sheet.Range["I5"].Value = LOS[4];
                sheet.Range["D6"].Value = DateTimeStringConv(MAX[2]);
                sheet.Range["G6"].Value = MAX[3];
                sheet.Range["I6"].Value = MAX[4];

                //2シート目の選択
                if (SelecedObj.ExcelType == FileType.UNYO_SEIGYO_KinChikyu)
                {
                    //格子を書く
                    sheet.Range["B10:X" + (data.Count + 9).ToString()].Borders.LineStyle = 1;
                    //データを書く
                    sheet.Range["B10:X" + (data.Count + 9).ToString()].Value2 = exc;
                }
                else
                {
                    //格子を書く
                    sheet.Range["B10:AA" + (data.Count + 9).ToString()].Borders.LineStyle = 1;
                    //データを書く
                    sheet.Range["B10:AA" + (data.Count + 9).ToString()].Value2 = exc;
                }

                // 作成者、前回保存者、会社のプロパティは削除する
                dynamic properties = WorkBook.BuiltinDocumentProperties;
                dynamic property = properties.Item("Author");
                property.Value = "";
                property = properties.Item("Last author");
                property.Value = "";
                property = properties.Item("Company");
                property.Value = "";

                WorkBook.Save();

                if (Is_Close)
                {
                    //ワークブックを閉じる
                    WorkBook.Close();
                    //エクセルを閉じる
                    ExcelApp.Quit();
                }
            }
            finally
            {
                if (sheet != null)
                {
                    Marshal.ReleaseComObject(sheet);
                    sheet = null;
                }
                if (WorkBook != null)
                {
                    Marshal.ReleaseComObject(WorkBook);
                    WorkBook = null;
                }

                if (ExcelApp != null)
                {
                    Marshal.ReleaseComObject(ExcelApp);
                    ExcelApp = null;
                }
                GC.Collect();
            }

        }
#endregion

#region パス履歴
        /// <summary>
        /// パス履歴
        /// </summary>
        private void PATH_RIREKI()
        {
            string[] lines = ReadInputFileEUC();
            List<CItem> data = new List<CItem>();
            string eisei_name = "";
            string eisei_num = "";

            string kyoku_name = "";
            string[] HEAD = null;
            string[] AOS = null;
            string[] LOS = null;
            string[] MAX = null;

            //データの先頭を検索
            string groupname = "";
            for (int i = 0; i < lines.Length; i++)
            {
                string[] tmp = lines[i].Split(spWord, StringSplitOptions.RemoveEmptyEntries);

                if(tmp.Length==0)
                {
                    continue;
                }

                if (tmp[0] == "#!HEAD:")
                {
                    eisei_name = tmp[6];
                    eisei_num = tmp[5];
                    HEAD = tmp.ToArray();
                }
                else if (tmp[0].Contains("-----"))
                {
                    groupname = tmp[1].Trim();
                }else
                {
                    //data.Add(new CItem() { Group = groupname, Line = lines[i].Replace(" ", "").Replace("　", ""), NamaLine = lines[i] });
                    data.Add(new CItem() { Group = groupname, Line = lines[i].Replace("　", ""), NamaLine = lines[i] });
                }
            }
            if (data.Count == 0)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            string pathnum = GetPathData(data, "パス情報", "パス番号");


            Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
            //エクセルを非表示
            ExcelApp.Visible = !Is_Close;

            //エクセルファイルのオープンと
            //ワークブックをの作成
            Microsoft.Office.Interop.Excel.Workbook WorkBook = ExcelApp.Workbooks.Open(_OutputFile);

            //1シート目の選択
            Microsoft.Office.Interop.Excel.Worksheet sheet = WorkBook.Sheets["Sheet1"];
            sheet.Select();

            try
            {

                //ヘッダー
                sheet.Range["I12"].Value = HEAD[2] + " " + HEAD[3];
                sheet.Range["I13"].Value = pathnum;
                sheet.Range["I14"].Value = HEAD[4];
                sheet.Range["I15"].Value = eisei_num;
                sheet.Range["I16"].Value = eisei_name;

                //2シート目の選択
                sheet = WorkBook.Sheets["Sheet2"];
                sheet.Select();

                sheet.Range["F4"].Value = pathnum;
                sheet.Range["F5"].Value = eisei_name;
                sheet.Range["F6"].Value = eisei_num;
                sheet.Range["F7"].Value = GetPathData(data, "パス情報", "パス開始時刻");
                sheet.Range["F8"].Value = GetPathData(data, "パス情報", "パス終了時刻");

                if (KyokuName == DEF_UDSC54)
                {
                    sheet.Range["N4"].Value = GetPathData(data, "パス情報", "Ｘ-ＴＸ送信周波数");
                    sheet.Range["N5"].Value = GetPathData(data, "パス情報", "Ｋａ-ＲＸ予報周波数");
                    sheet.Range["N6"].Value = GetPathData(data, "パス情報", "Ｘ-ＲＸ予報周波数");
                }
                else
                {
                    sheet.Range["N4"].Value = GetPathData(data, "パス情報", "Ｓ-ＴＸ送信周波数");
                    sheet.Range["N5"].Value = GetPathData(data, "パス情報", "Ｓ-ＲＸ予報周波数");
                    sheet.Range["N6"].Value = GetPathData(data, "パス情報", "Ｘ-ＲＸ予報周波数");
                }
                sheet.Range["U4"].Value = GetPathData(data, "アンテナ最大ＥＬ情報", "時刻");
                sheet.Range["U5"].Value = GetPathData(data, "アンテナ最大ＥＬ情報", "ＥＬ");
                sheet.Range["U6"].Value = GetPathData(data, "アンテナ最大ＥＬ情報", "ＡＺ");

                if (KyokuName == DEF_UDSC54)
                {
                    sheet.Range["F15"].Value = GetPathData(data, "Ｘ－ＲＸ　ＡＧＣ情報", "最大ＡＧＣレベル");
                    sheet.Range["F16"].Value = GetPathData(data, "Ｘ－ＲＸ　ＡＧＣ情報", "アンテナＡＺ");
                    sheet.Range["F17"].Value = GetPathData(data, "Ｘ－ＲＸ　ＡＧＣ情報", "アンテナＥＬ");

                    sheet.Range["N15"].Value = GetPathData(data, "Ｋａ－ＲＸ　ＡＧＣ情報", "最大ＡＧＣレベル");
                    sheet.Range["N16"].Value = GetPathData(data, "Ｋａ－ＲＸ　ＡＧＣ情報", "アンテナＡＺ");
                    sheet.Range["N17"].Value = GetPathData(data, "Ｋａ－ＲＸ　ＡＧＣ情報", "アンテナＥＬ");

                    sheet.Range["U14"].Value = GetPathData(data, "ＲＮＧ情報", "Ｘ／Ｋａ帯従来型総レンジング回数");
                    sheet.Range["U15"].Value = GetPathData(data, "ＲＮＧ情報", "Ｘ／Ｋａ帯再生型総レンジング回数");
                    sheet.Range["U16"].Value = GetPathData(data, "ＲＮＧ情報", "Ｘ／Ｘ帯従来型総レンジング回数");
                    sheet.Range["U17"].Value = GetPathData(data, "ＲＮＧ情報", "Ｘ／Ｘ帯再生型総レンジング回数");
                }
                else
                {
                    sheet.Range["F15"].Value = GetPathData(data, "Ｓ－ＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル");
                    sheet.Range["F16"].Value = GetPathData(data, "Ｓ－ＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル時ＡＺ");
                    sheet.Range["F17"].Value = GetPathData(data, "Ｓ－ＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル時ＥＬ");

                    sheet.Range["F20"].Value = GetPathData(data, "Ｓ－ＲＸ　ＡＧＣ情報", "Ｂｃｈ最大ＡＧＣレベル");
                    sheet.Range["F21"].Value = GetPathData(data, "Ｓ－ＲＸ　ＡＧＣ情報", "Ｂｃｈ最大ＡＧＣレベル時ＡＺ");
                    sheet.Range["F22"].Value = GetPathData(data, "Ｓ－ＲＸ　ＡＧＣ情報", "Ｂｃｈ最大ＡＧＣレベル時ＥＬ");

                    sheet.Range["N15"].Value = GetPathData(data, "Ｘ－ＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル");
                    sheet.Range["N16"].Value = GetPathData(data, "Ｘ－ＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル時ＡＺ");
                    sheet.Range["N17"].Value = GetPathData(data, "Ｘ－ＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル時ＥＬ");

                    sheet.Range["N20"].Value = GetPathData(data, "Ｘ－ＲＸ　ＡＧＣ情報", "Ｂｃｈ最大ＡＧＣレベル");
                    sheet.Range["N21"].Value = GetPathData(data, "Ｘ－ＲＸ　ＡＧＣ情報", "Ｂｃｈ最大ＡＧＣレベル時ＡＺ");
                    sheet.Range["N22"].Value = GetPathData(data, "Ｘ－ＲＸ　ＡＧＣ情報", "Ｂｃｈ最大ＡＧＣレベル時ＥＬ");

                    sheet.Range["U15"].Value = GetPathData(data, "Ｓ－ＡＣＱＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル");
                    sheet.Range["U16"].Value = GetPathData(data, "Ｓ－ＡＣＱＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル時ＡＺ");
                    sheet.Range["U17"].Value = GetPathData(data, "Ｓ－ＡＣＱＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル時ＥＬ");

                    sheet.Range["U20"].Value = GetPathData(data, "Ｓ－ＡＣＱＲＸ　ＡＧＣ情報", "Ｂｃｈ最大ＡＧＣレベル");
                    sheet.Range["U21"].Value = GetPathData(data, "Ｓ－ＡＣＱＲＸ　ＡＧＣ情報", "Ｂｃｈ最大ＡＧＣレベル時ＡＺ");
                    sheet.Range["U22"].Value = GetPathData(data, "Ｓ－ＡＣＱＲＸ　ＡＧＣ情報", "Ｂｃｈ最大ＡＧＣレベル時ＥＬ");

                    sheet.Range["F29"].Value = GetPathData(data, "Ｘ－ＡＣＱＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル");
                    sheet.Range["F30"].Value = GetPathData(data, "Ｘ－ＡＣＱＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル時ＡＺ");
                    sheet.Range["F31"].Value = GetPathData(data, "Ｘ－ＡＣＱＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル時ＥＬ");

                    sheet.Range["F34"].Value = GetPathData(data, "Ｘ－ＡＣＱＲＸ　ＡＧＣ情報", "Ｂｃｈ最大ＡＧＣレベル");
                    sheet.Range["F35"].Value = GetPathData(data, "Ｘ－ＡＣＱＲＸ　ＡＧＣ情報", "Ｂｃｈ最大ＡＧＣレベル時ＡＺ");
                    sheet.Range["F36"].Value = GetPathData(data, "Ｘ－ＡＣＱＲＸ　ＡＧＣ情報", "Ｂｃｈ最大ＡＧＣレベル時ＥＬ");

                    sheet.Range["N29"].Value = GetPathData(data, "Ｘ－ＨＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル");
                    sheet.Range["N30"].Value = GetPathData(data, "Ｘ－ＨＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル時ＡＺ");
                    sheet.Range["N31"].Value = GetPathData(data, "Ｘ－ＨＲＸ　ＡＧＣ情報", "Ａｃｈ最大ＡＧＣレベル時ＥＬ");

                    sheet.Range["U28"].Value = GetPathData(data, "ＲＮＧ情報", "Ｓ／Ｓ帯総レンジング回数");
                    sheet.Range["U29"].Value = GetPathData(data, "ＲＮＧ情報", "Ｓ／Ｘ帯総レンジング回数");

                }

                int startRow = 30;
                if (KyokuName == DEF_UDSC54)
                {
                    startRow = 19;
                }

                string[] ldata = GetPathData2(data, "イベント情報");
                for (int j = 0; j < ldata.Length; j++)
                {
                    int row = startRow + j;
                    sheet.Range["U" + row.ToString()].Value = ldata[j];
                }

                // 作成者、前回保存者、会社のプロパティは削除する
                dynamic properties = WorkBook.BuiltinDocumentProperties;
                dynamic property = properties.Item("Author");
                property.Value = "";
                property = properties.Item("Last author");
                property.Value = "";
                property = properties.Item("Company");
                property.Value = "";

                WorkBook.Save();

                if (Is_Close)
                {
                    //ワークブックを閉じる
                    WorkBook.Close();
                    //エクセルを閉じる
                    ExcelApp.Quit();
                }
            }
            finally
            {
                if (sheet != null)
                {
                    Marshal.ReleaseComObject(sheet);
                    sheet = null;
                }
                if (WorkBook != null)
                {
                    Marshal.ReleaseComObject(WorkBook);
                    WorkBook = null;
                }

                if (ExcelApp != null)
                {
                    Marshal.ReleaseComObject(ExcelApp);
                    ExcelApp = null;
                }
                GC.Collect();
            }

        }

        private string GetPathData(List<CItem> data, string groupName, string itemName)
        {
            string result = "";
            var ans = from n in data where n.Group == groupName && n.Line.Contains(itemName) select n.Line;
            if(ans.Any())
            {
                string line = ans.First();

                line = line.Substring(line.IndexOf(itemName));

                int start = line.IndexOf(":");
                string ans1 = line.Substring(start + 1);

                int end = ans1.IndexOf("[");
                if (end == -1)
                {
                    result = ans1.Trim();
                }
                else
                {
                    ans1 = ans1.Substring(0, end);
                    result = ans1.Trim();
                }
            }
            return result;
        }

        private string[] GetPathData2(List<CItem> data, string groupName)
        {
            var ans = from n in data where n.Group == groupName select n.NamaLine;

            return ans.ToArray();
        }
#endregion


        string[] delimiter = { "\",\"" };

#region 運用履歴
        /// <summary>
        /// 運用履歴
        /// </summary>
        private void RIREKI()
        {
            string[] lines = ReadInputFile();
            List<string[]> data = new List<string[]>();
            List<string> header = new List<string>();

            //データの先頭を検索
            for (int i = 0; i < lines.Length; i++)
            {
                if (lines[i].Contains("出力開始時刻="))
                {
                    header.Add(lines[i].Replace("\"", ""));
                }
                else if (lines[i].Contains("出力終了時刻="))
                {
                    header.Add(lines[i].Replace("\"", ""));
                }
                else if (lines[i].Contains("フィルター（局）="))
                {
                    header.Add(lines[i].Replace("\"", ""));
                }
                else if (lines[i].Contains("フィルター（送信元装置）="))
                {
                    header.Add(lines[i].Replace("\"", ""));
                }
                else if (lines[i].Contains("フィルター（重要度）="))
                {
                    header.Add(lines[i].Replace("\"", ""));
                }
                else if (lines[i].Contains("フィルター（メッセージ番号）="))
                {
                    header.Add(lines[i].Replace("\"", ""));
                }
                else if (lines[i].Contains("フィルター（ステータス）="))
                {
                    header.Add(lines[i].Replace("\"", ""));
                }
                else
                {
                    string[] d = lines[i].Split(delimiter, StringSplitOptions.None);
                    for (int h = 0; h < d.Length; h++)
                    {
                        d[h] = d[h].Replace("\"", "");
                    }

                    if(d.Length < 7)
                    {
                        continue;
                    }

                    string[] col = new string[] {
                        d[1],
                        d[0],
                        d[2],
                        d[3],
                        d[4],
                        d[5],
                        d[6]};
                    data.Add(col);
                }
            }
            if (data.Count == 0)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            object[,] exc = new object[data.Count, data[0].Length];
            for (int row = 0; row < data.Count; row++)
            {
                for (int col = 0; col < data[0].Length; col++)
                {
                    exc[row, col] = data[row][col];
                }
            }

            Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
            //エクセルを非表示
            ExcelApp.Visible = !Is_Close;

            //エクセルファイルのオープンと
            //ワークブックをの作成
            Microsoft.Office.Interop.Excel.Workbook WorkBook = ExcelApp.Workbooks.Open(_OutputFile);

            string SheetName = "";
            if (SelecedObj.ExcelType == FileType.UNYO_RIREKI)
            {
                SheetName = "OPE_HIST";
            }
            else if (SelecedObj.ExcelType == FileType.SEIGYO_RIREKI)
            {
                SheetName = "CONT_HIST";
            }
            else if (SelecedObj.ExcelType == FileType.SETSUBI_RIREKI)
            {
                SheetName = "FACILITY_STS_HIST";
            }
            else if (SelecedObj.ExcelType == FileType.EVENT_RIREKI)
            {
                SheetName = "EVENT_HIST";
            }

            //1シート目の選択
            Microsoft.Office.Interop.Excel.Worksheet sheet = WorkBook.Sheets[SheetName];
            sheet.Select();

            try
            {
                //ヘッダー
                sheet.Range["C2"].Value = getHeaderItem(header, "出力開始時刻=");
                sheet.Range["C3"].Value = getHeaderItem(header, "出力終了時刻=");
                sheet.Range["C4"].Value = getHeaderItem(header, "フィルター（局）=");
                sheet.Range["C5"].Value = getHeaderItem(header, "フィルター（送信元装置）=");
                sheet.Range["C6"].Value = getHeaderItem(header, "フィルター（重要度）=");
                sheet.Range["C7"].Value = getHeaderItem(header, "フィルター（ステータス）=");
                sheet.Range["C8"].Value = getHeaderItem(header, "フィルター（メッセージ番号）=");

                //格子を書く
                sheet.Range["B11:H" + (data.Count + 10).ToString()].Borders.LineStyle = 1;
                //データを書く
                sheet.Range["B11:H" + (data.Count + 10).ToString()].Value2 = exc;

                // 作成者、前回保存者、会社のプロパティは削除する
                dynamic properties = WorkBook.BuiltinDocumentProperties;
                dynamic property = properties.Item("Author");
                property.Value = "";
                property = properties.Item("Last author");
                property.Value = "";
                property = properties.Item("Company");
                property.Value = "";

                WorkBook.Save();

                if (Is_Close)
                {
                    //ワークブックを閉じる
                    WorkBook.Close();
                    //エクセルを閉じる
                    ExcelApp.Quit();
                }

            }
            finally
            {
                if (sheet != null)
                {
                    Marshal.ReleaseComObject(sheet);
                    sheet = null;
                }
                if (WorkBook != null)
                {
                    Marshal.ReleaseComObject(WorkBook);
                    WorkBook = null;
                }

                if (ExcelApp != null)
                {
                    Marshal.ReleaseComObject(ExcelApp);
                    ExcelApp = null;
                }
                GC.Collect();
            }


        }
#endregion

        private char[] spWord2 = new char[] { ' ', '：' };

        private char[] spWord3 = new char[] { ',' };
#region 設備監視蓄積データ
        /// <summary>
        /// 設備監視蓄積データ
        /// </summary>
        private void KANSHI_CHIKUSEKI()
        {
            string[] lines = ReadInputFileEUC();
            List<string[]> data = new List<string[]>();

            //データの先頭を検索
            for (int i = 0; i < lines.Length; i++)
            {
                string[] tmp = lines[i].Split(spWord3, StringSplitOptions.RemoveEmptyEntries);

                if(tmp.Length < 2)
                {
                    continue;
                }

                if (tmp[0] == "通算日")
                {
                    data.Add(tmp);
                }

                DateTime dt;
                if (DateTime.TryParseExact(tmp[1], "HH:mm:ss",
                    System.Globalization.DateTimeFormatInfo.InvariantInfo,
                    System.Globalization.DateTimeStyles.None,
                    out dt))
                {
                    data.Add(tmp);
                }
            }
            if (data.Count == 0)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            string stattime = data[1][0] + " " + data[1][1];
            string endtime = data[data.Count - 1][0] + " "+ data[data.Count - 1][1];

            object[,] exc = new object[data.Count, data[0].Length];
            for (int row = 0; row < data.Count; row++)
            {
                for (int col = 0; col < data[0].Length; col++)
                {
                    exc[row, col] = data[row][col];
                }
            }

            Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
            //エクセルを非表示
            ExcelApp.Visible = !Is_Close;

            //エクセルファイルのオープンと
            //ワークブックをの作成
            Microsoft.Office.Interop.Excel.Workbook WorkBook = ExcelApp.Workbooks.Open(_OutputFile);

            //1シート目の選択
            Microsoft.Office.Interop.Excel.Worksheet sheet = WorkBook.Sheets["FACILITY_MON_STORAGE_DATA"];
            sheet.Select();

            try
            {
                //ヘッダー
                sheet.Range["C2"].Value = stattime;
                sheet.Range["C3"].Value = endtime;
                sheet.Range["C4"].Value = KyokuNameJp;

                //格子を書く
                int col2 = data[0].Length;

                sheet.Range["B7:" + GetColumnName(col2) + (data.Count + 6).ToString()].Borders.LineStyle = 1;
                //データを書く
                sheet.Range["B7:" + GetColumnName(col2) + (data.Count + 6).ToString()].Value2 = exc;

                // 作成者、前回保存者、会社のプロパティは削除する
                dynamic properties = WorkBook.BuiltinDocumentProperties;
                dynamic property = properties.Item("Author");
                property.Value = "";
                property = properties.Item("Last author");
                property.Value = "";
                property = properties.Item("Company");
                property.Value = "";

                WorkBook.Save();

                if (Is_Close)
                {
                    //ワークブックを閉じる
                    WorkBook.Close();
                    //エクセルを閉じる
                    ExcelApp.Quit();
                }

            }
            finally
            {
                if (sheet != null)
                {
                    Marshal.ReleaseComObject(sheet);
                    sheet = null;
                }
                if (WorkBook != null)
                {
                    Marshal.ReleaseComObject(WorkBook);
                    WorkBook = null;
                }

                if (ExcelApp != null)
                {
                    Marshal.ReleaseComObject(ExcelApp);
                    ExcelApp = null;
                }
                GC.Collect();
            }

        }


        private string GetColumnName(int index)
        {
            string str = "";
            do
            {
                str = Convert.ToChar(index % 26 + 0x41) + str;
            } while ((index = index / 26 - 1) != -1);

            return str;
        }
#endregion

#region アンテナ設備駆動履歴
        /// <summary>
        /// アンテナ設備駆動履歴
        /// </summary>
        private void ANT_KUDO_RIREKI()
        {
            string[] lines = ReadInputFileEUC();
            List<string[]> data = new List<string[]>();

            int ColCnt = 12;
            if(KyokuName == DEF_UDSC54)
            {
                ColCnt = 14;
            }

            //データの先頭を検索
            for (int i = 0; i < lines.Length; i++)
            {
                string[] tmp = lines[i].Split(spWord3, StringSplitOptions.None);

                if(tmp.Length < ColCnt)
                {
                    continue;
                }
                string[] day = tmp[0].Split(' ');
                if(day.Length < 2)
                {
                    continue;
                }

                string[] col = new string[tmp.Length + 1];
                col[0] = tmp[0];
                col[1] = KyokuNameJp;

                Array.Copy(tmp, 1, col, 2, tmp.Length - 1);

                data.Add(col);
            }
            if (data.Count == 0)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            string stattime = data[0][0];
            string endtime = data[data.Count - 1][0];

            object[,] exc = new object[data.Count, data[0].Length];
            for (int row = 0; row < data.Count; row++)
            {
                for (int col = 0; col < data[0].Length; col++)
                {
                    exc[row, col] = data[row][col];
                }
            }

            Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
            //エクセルを非表示
            ExcelApp.Visible = !Is_Close;

            //エクセルファイルのオープンと
            //ワークブックをの作成
            Microsoft.Office.Interop.Excel.Workbook WorkBook = ExcelApp.Workbooks.Open(_OutputFile);

            //1シート目の選択
            Microsoft.Office.Interop.Excel.Worksheet sheet = WorkBook.Sheets["ANT_DRV_HIST"];
            sheet.Select();

            try
            {
                //ヘッダー
                sheet.Range["C2"].Value = stattime;
                sheet.Range["C3"].Value = endtime;
                sheet.Range["C6"].Value = KyokuNameJp;

                //格子を書く
                int col2 = data[0].Length;

                sheet.Range["B11:" + GetColumnName(col2) + (data.Count + 10).ToString()].Borders.LineStyle = 1;
                //データを書く
                sheet.Range["B11:" + GetColumnName(col2) + (data.Count + 10).ToString()].Value2 = exc;

                // 作成者、前回保存者、会社のプロパティは削除する
                dynamic properties = WorkBook.BuiltinDocumentProperties;
                dynamic property = properties.Item("Author");
                property.Value = "";
                property = properties.Item("Last author");
                property.Value = "";
                property = properties.Item("Company");
                property.Value = "";

                WorkBook.Save();

                if (Is_Close)
                {
                    //ワークブックを閉じる
                    WorkBook.Close();
                    //エクセルを閉じる
                    ExcelApp.Quit();
                }
            }
            finally
            {
                if (sheet != null)
                {
                    Marshal.ReleaseComObject(sheet);
                    sheet = null;
                }
                if (WorkBook != null)
                {
                    Marshal.ReleaseComObject(WorkBook);
                    WorkBook = null;
                }

                if (ExcelApp != null)
                {
                    Marshal.ReleaseComObject(ExcelApp);
                    ExcelApp = null;
                }
                GC.Collect();
            }
        }
        #endregion


        #region アンテナ設備運用履歴
        /// <summary>
        /// アンテナ設備運用履歴
        /// </summary>
        private void ANT_UNYO_RIREKI()
        {
            string[] lines = ReadInputFileEUC();
            List<string[]> data = new List<string[]>();

            //データの先頭を検索
            for (int i = 0; i < lines.Length; i++)
            {
                if (KyokuName == DEF_UDSC54)
                {
                    string[] tmp = lines[i].Split(spWord3, StringSplitOptions.RemoveEmptyEntries);
                    if (tmp.Length < 2)
                    {
                        continue;
                    }
                    string[] day = tmp[0].Split(' ');
                    if (day.Length < 2)
                    {
                        continue;
                    }

                    string[] col = new string[] { tmp[0] , KyokuNameJp, tmp[1] };
                    data.Add(col);
                }
                else
                {
                    string[] tmp = lines[i].Split(spWord3, StringSplitOptions.RemoveEmptyEntries);
                    if (tmp.Length < 3)
                    {
                        continue;
                    }

                    string[] col = new string[] { tmp[0] + " " + tmp[1], KyokuNameJp, tmp[2] };
                    data.Add(col);
                }
            }
            if (data.Count == 0)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            string stattime = data[0][0];
            string endtime = data[data.Count - 1][0];

            object[,] exc = new object[data.Count, data[0].Length];
            for (int row = 0; row < data.Count; row++)
            {
                for (int col = 0; col < data[0].Length; col++)
                {
                    exc[row, col] = data[row][col];
                }
            }

            Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
            //エクセルを非表示
            ExcelApp.Visible = !Is_Close;

            //エクセルファイルのオープンと
            //ワークブックをの作成
            Microsoft.Office.Interop.Excel.Workbook WorkBook = ExcelApp.Workbooks.Open(_OutputFile);

            //1シート目の選択
            Microsoft.Office.Interop.Excel.Worksheet sheet = WorkBook.Sheets["ANT_OPE_HIST"];
            sheet.Select();

            try
            {
                //ヘッダー
                sheet.Range["C2"].Value = stattime;
                sheet.Range["C3"].Value = endtime;
                sheet.Range["C4"].Value = KyokuNameJp;

                //格子を書く
                int col2 = data[0].Length;

                sheet.Range["B9:" + GetColumnName(col2) + (data.Count + 8).ToString()].Borders.LineStyle = 1;
                //データを書く
                sheet.Range["B9:" + GetColumnName(col2) + (data.Count + 8).ToString()].Value2 = exc;

                // 作成者、前回保存者、会社のプロパティは削除する
                dynamic properties = WorkBook.BuiltinDocumentProperties;
                dynamic property = properties.Item("Author");
                property.Value = "";
                property = properties.Item("Last author");
                property.Value = "";
                property = properties.Item("Company");
                property.Value = "";

                WorkBook.Save();

                if (Is_Close)
                {
                    //ワークブックを閉じる
                    WorkBook.Close();
                    //エクセルを閉じる
                    ExcelApp.Quit();
                }
            }
            finally
            {
                if (sheet != null)
                {
                    Marshal.ReleaseComObject(sheet);
                    sheet = null;
                }
                if (WorkBook != null)
                {
                    Marshal.ReleaseComObject(WorkBook);
                    WorkBook = null;
                }

                if (ExcelApp != null)
                {
                    Marshal.ReleaseComObject(ExcelApp);
                    ExcelApp = null;
                }
                GC.Collect();
            }

        }
#endregion


        /// <summary>
        /// 無線業務日誌
        /// </summary>
        private void MUSEN_NISSHI()
        {
            //無線業務日誌だけは表示しない v1.4.0.0 
            //複数個同時にコールされるため
            Is_Close = true;

            string[] lines = ReadInputFile();
            List<string[]> data = new List<string[]>();
            List<string> header = new List<string>();

            bool isHeader = false;
            //データの先頭を検索
            for (int i = 0; i < lines.Length; i++)
            {
                if(lines[i].Contains("#HEADER"))
                {
                    isHeader = true;
                }

                if (isHeader)
                {
                    header.Add(lines[i]);
                }
                else
                {
                    string[] tmp = lines[i].Split(spWord3, StringSplitOptions.None);
                    if (tmp.Length > 1)
                    {
                        data.Add(tmp);
                    }
                }
            }
            if (data.Count == 0)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            bool bFormat = false;
            bool bMusenTitle = false;
            bool bEntsuTimeTitle = false;
            bool bStationNameTitme = false;
            bool bFrequencyLineStart = false;
            bool bFrequencyLineEnd = false;
            bool bFrequencyTitle = false;
            bool bDenpaTitle = false;
            bool bKuchuTitle = false;
            bool bMenNumberTitle = false;
            bool bMenNinTitle = false;

            List<string> freq = new List<string>();
            List<string> denpa = new List<string>();
            List<string> kuchu = new List<string>();
            string menNumber = "";
            string menNin = "";

            foreach (var he in header)
            {
                // 表紙の"FORMAT"文字列を見つける
                if (he.Contains("FORMAT"))
                {
                    bFormat = true;
                }

                // 表紙の日誌タイトルを見つける
                if (he.Contains("無　線　業　務　日　誌"))
                {
                    bMusenTitle = true;
                }

                // 表紙の延通信時間を見つける
                if (he.Contains("延通信時間"))
                {
                    bEntsuTimeTitle = true;
                }

                // 表紙の局名を見つける
                if (he.Contains("局    名"))
                {
                    bStationNameTitme = true;
                }

                // 表紙の局名の欄の終わり行を見つける
                if (bFormat == false || 
                    bMusenTitle == false || 
                    bEntsuTimeTitle == false || 
                    bStationNameTitme == false)
                {
                    continue;
                }
                // 周波数、電波、空中線の欄の始まり行を見つける
                if (bFrequencyLineStart == false && he.Contains("+--------------"))
                {
                    // 局名の欄の終わりが見つかった
                    // 次の行から周波数、電波、空中線の欄の始まり
                    bFrequencyLineStart = true;
                    continue;
                }
                // 周波数、電波、空中線の欄の終わり行を見つける
                if (bFrequencyLineStart == true && bFrequencyLineEnd == false && he.Contains("+--------------"))
                {
                    // 局名の欄の終わりが見つかった
                    bFrequencyLineEnd = true;
                    // 周波数、電波、空中線の欄の終わり
                    continue;
                }

                // 周波数、電波、空中線の欄の場合
                if (bFrequencyLineStart == true && bFrequencyLineEnd == false)
                {
                    // 周波数、電波、空中線の分割
                    string[] tmp = he.Split('|');
                    for (int s = 0; s < tmp.Length; s++)
                    {
                        switch (s)
                        {
                            case 0:
                                // 行の左端
                                break;

                            case 1:
                                // 周波数タイトル
                                if (tmp[s].Trim() == "周波数")
                                {
                                    bFrequencyTitle = true;
                                }
                                break;

                            case 2:
                                // 周波数値
                                if (tmp[s].Trim() != "")
                                {
                                    freq.Add(tmp[s].Trim());
                                }
                                break;

                            case 3:
                                // 電波の
                                if (tmp[s].Trim() == "電波の")
                                {
                                    bDenpaTitle = true;
                                }
                                break;

                            case 4:
                                // 電波の値
                                if (tmp[s].Trim() != "")
                                {
                                    denpa.Add(tmp[s].Trim());
                                }
                                break;

                            case 5:
                                // 空中線
                                if (tmp[s].Trim() == "空中線")
                                {
                                    bKuchuTitle = true;
                                }
                                break;

                            case 6:
                                // 空中線の値
                                if (tmp[s].Trim() != "")
                                {
                                    kuchu.Add(tmp[s].Trim());
                                }
                                break;

                            default:
                                break;
                        }
                    }
                }
                // 表紙の免許の番号を見つける
                if (he.Contains("免許の番号"))
                {
                    bMenNumberTitle = true;
                    string[] tmp = he.Split('|');
                    menNumber = tmp[2].Trim();
                }

                // 表紙の免許人を見つける
                if (he.Contains("免許人"))
                {
                    bMenNinTitle = true;
                    string[] tmp = he.Split('|');
                    menNin = tmp[2].Trim();
                }
            }

            if (bFormat == false || 
                bMusenTitle == false || 
                bEntsuTimeTitle == false || 
                bStationNameTitme == false || 
                bFrequencyTitle == false || 
                bDenpaTitle == false || 
                bKuchuTitle == false ||
                bMenNumberTitle == false ||
                bMenNinTitle == false)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            //            //ヘッダー部
            //            string frequency = "";
            //            foreach(var he in header)
            //            {
            //                if(he.Contains("周波数"))
            //                {
            //                    string[] tmp = he.Split('|');
            //                    for (int s = 0; s < tmp.Length; s++)
            //                    {
            //                        if(tmp[s].Trim() == "周波数")
            //                        {
            //                            frequency = tmp[s + 1];
            //                            break;
            //                        }
            //                    }
            //                    break;
            //                }
            //            }
            //
            //            List<string> denpa = new List<string>();
            //            bool isdenpa = false;
            //            foreach (var he in header)
            //            {
            //                if (he.Contains("電波の"))
            //                {
            //                    isdenpa = true;
            //                }
            //                if(isdenpa)
            //                {
            //                    if(he.Contains("+--------------"))
            //                    {
            //                        break;
            //                    }
            //                    string[] tmp = he.Split('|');
            //                    denpa.Add(tmp[4].Trim());
            //                }
            //            }
            //
            //            List<string> kuchu = new List<string>();
            //            bool iskuchu = false;
            //            int itemIndex = 0;
            //            foreach (var he in header)
            //            {
            //                if (he.Contains("空中線"))
            //                {
            //                    iskuchu = true;
            //                }
            //                if (iskuchu)
            //                {
            //                    if (he.Contains("+--------------"))
            //                    {
            //                        break;
            //                    }
            //                    string[] tmp = he.Split('|');
            //                    if (itemIndex == 0)
            //                    {
            //                        for (int s = 0; s < tmp.Length; s++)
            //                        {
            //                            if (tmp[s].Trim() == "空中線")
            //                            {
            //                                itemIndex = s;
            //                                break;
            //                            }
            //                        }
            //                    }
            //                    if (tmp[itemIndex+1].Trim() != "")
            //                    {
            //                        kuchu.Add(tmp[itemIndex + 1].Trim());
            //                    }
            //                }
            //            }
            //
            //string menNumber = "";
            //foreach (var he in header)
            //{
            //    if (he.Contains("免許の番号"))
            //    {
            //        string[] tmp = he.Split('|');
            //        menNumber = tmp[2].Trim();
            //        break;
            //    }
            //}
            //
            //string menNin = "";
            //foreach (var he in header)
            //{
            //    if (he.Contains("免許人"))
            //    {
            //        string[] tmp = he.Split('|');
            //        menNin = tmp[2].Trim();
            //        break;
            //    }
            //}


            //データ部
            object[,] exc = new object[data.Count, data[0].Length];
            for (int row = 0; row < data.Count; row++)
            {
                for (int col = 0; col < data[0].Length; col++)
                {
                    if (col < data[row].Length)
                    {
                        exc[row, col] = data[row][col];
                    }
                    else
                    {
                        exc[row, col] = "";
                    }
                }
            }

            Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
            //エクセルを非表示
            ExcelApp.Visible = !Is_Close;

            //エクセルファイルのオープンと
            //ワークブックをの作成
            Microsoft.Office.Interop.Excel.Workbook WorkBook = ExcelApp.Workbooks.Open(_OutputFile);

            //1シート目の選択
            Microsoft.Office.Interop.Excel.Worksheet sheet = WorkBook.Sheets["Sheet1"];
            sheet.Select();

            try
            {
                //ヘッダー
                sheet.Range["I10"].Value = getHeaderItem(header, "PERIOD=");
                sheet.Range["I11"].Value = getHeaderItem(header, "STATION=");
                sheet.Range["I12"].Value = getHeaderItem(header, "SATELLITE=");
                sheet.Range["I13"].Value = getHeaderItem(header, "TIME=");
//                sheet.Range["I14"].Value = frequency;
//                sheet.Range["I14"].Value = string.Join(", ", freq.ToArray());
//                sheet.Range["I15"].Value = string.Join(", ", denpa.ToArray());
//                sheet.Range["I16"].Value = string.Join(", ", kuchu.ToArray());
                sheet.Range["I14"].Value = (string.Join(" ", freq.ToArray())).Trim();
                sheet.Range["I15"].Value = (string.Join(" ", denpa.ToArray())).Trim();
                sheet.Range["I16"].Value = (string.Join(" ", kuchu.ToArray())).Trim();
                sheet.Range["I17"].Value = menNumber;
                sheet.Range["I18"].Value = menNin;


                //1シート目の選択
                sheet = WorkBook.Sheets["Sheet2"];
                sheet.Select();

                sheet.Range["D2"].Value = getHeaderItem(header, "PERIOD=");
                sheet.Range["D3"].Value = getHeaderItem(header, "STATION=");
                sheet.Range["D4"].Value = getHeaderItem(header, "SATELLITE=");

                int a = Convert.ToInt32(Math.Ceiling((double)data.Count / 30));
                for (int i = 1; i < a; i++)
                {
                    int start = (30 * i) + 6;
                    sheet.Range["B6:K35"].Copy(sheet.Range["B" + start.ToString()]);
                }

                //データを書く
                sheet.Range["B6:K" + (data.Count + 5).ToString()].Value2 = exc;

                // 作成者、前回保存者、会社のプロパティは削除する
                dynamic properties = WorkBook.BuiltinDocumentProperties;
                dynamic property = properties.Item("Author");
                property.Value = "";
                property = properties.Item("Last author");
                property.Value = "";
                property = properties.Item("Company");
                property.Value = "";

                WorkBook.Save();

                if (Is_Close)//強制Close
                {
                    //ワークブックを閉じる
                    WorkBook.Close();
                    //エクセルを閉じる
                    ExcelApp.Quit();
                }
            }
            finally
            {
                if (sheet != null)
                {
                    Marshal.ReleaseComObject(sheet);
                    sheet = null;
                }
                if (WorkBook != null)
                {
                    Marshal.ReleaseComObject(WorkBook);
                    WorkBook = null;
                }

                if (ExcelApp != null)
                {
                    Marshal.ReleaseComObject(ExcelApp);
                    ExcelApp = null;
                }
                GC.Collect();
            }
        }

        private string getHeaderItem(List<string> data, string keyword)
        {
            var ans = from n in data where n.Contains(keyword) select n;
            if(ans.Any())
            {
                string tmp = ans.First();
                tmp = tmp.Replace(keyword, "");
                return tmp;
            }

            return "";
        }

        private string getHeaderItemStartsWith(List<string> data, string keyword)
        {
            var ans = from n in data where n.StartsWith(keyword) select n;
            if (ans.Any())
            {
                string tmp = ans.First();
                tmp = tmp.Replace(keyword, "");
                return tmp;
            }

            return "";
        }

        /// <summary>
        /// RF情報
        /// </summary>
        private void RF_INFO()
        {
            string[] lines = ReadInputFile();
            List<string[]> data = new List<string[]>();
            List<string> header = new List<string>();

            bool isHeader = false;
            //データの先頭を検索
            for (int i = 0; i < lines.Length; i++)
            {
                if (lines[i].Contains("PERIOD="))
                {
                    header.Add(lines[i]);
                }
                else if(lines[i].Contains("SATELLITE="))
                {
                    header.Add(lines[i]);
                }
                else
                {
                    string[] tmp = lines[i].Split(spWord3, StringSplitOptions.None);
                    if (tmp.Length > 1)
                    {
                        data.Add(tmp);
                    }
                }
            }
            if (data.Count == 0)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            //データ部
            object[,] exc = new object[data.Count, data[0].Length];
            for (int row = 0; row < data.Count; row++)
            {
                for (int col = 0; col < data[0].Length; col++)
                {
                    if (col < data[row].Length)
                    {
                        exc[row, col] = data[row][col];
                    }
                    else
                    {
                        exc[row, col] = "";
                    }
                }
            }

            Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
            //エクセルを非表示
            ExcelApp.Visible = !Is_Close;

            //エクセルファイルのオープンと
            //ワークブックをの作成
            Microsoft.Office.Interop.Excel.Workbook WorkBook = ExcelApp.Workbooks.Open(_OutputFile);

            //1シート目の選択
            Microsoft.Office.Interop.Excel.Worksheet sheet = WorkBook.Sheets["Sheet1"];
            sheet.Select();

            try
            {
                //ヘッダー
                sheet.Range["I12"].Value = getHeaderItem(header, "PERIOD=");
                sheet.Range["I13"].Value = getHeaderItem(header, "SATELLITE=");

                //1シート目の選択
                sheet = WorkBook.Sheets["Sheet2"];
                sheet.Select();

                sheet.Range["D2"].Value = getHeaderItem(header, "PERIOD=");
                sheet.Range["D3"].Value = getHeaderItem(header, "SATELLITE=");

                //格子を書く
                int col2 = data[0].Length;
                sheet.Range["B7:" + GetColumnName(col2) + (data.Count + 6).ToString()].Borders.LineStyle = 1;

                //データを書く
                sheet.Range["B7:" + GetColumnName(col2) + (data.Count + 6).ToString()].Value2 = exc;

                // 作成者、前回保存者、会社のプロパティは削除する
                dynamic properties = WorkBook.BuiltinDocumentProperties;
                dynamic property = properties.Item("Author");
                property.Value = "";
                property = properties.Item("Last author");
                property.Value = "";
                property = properties.Item("Company");
                property.Value = "";

                WorkBook.Save();

                if (Is_Close)
                {
                    //ワークブックを閉じる
                    WorkBook.Close();
                    //エクセルを閉じる
                    ExcelApp.Quit();
                }
            }
            finally
            {
                if (sheet != null)
                {
                    Marshal.ReleaseComObject(sheet);
                    sheet = null;
                }
                if (WorkBook != null)
                {
                    Marshal.ReleaseComObject(WorkBook);
                    WorkBook = null;
                }

                if (ExcelApp != null)
                {
                    Marshal.ReleaseComObject(ExcelApp);
                    ExcelApp = null;
                }
                GC.Collect();
            }

        }


        private void KYOKU_UNNYO_KYOKU()
        {
            string[] lines = ReadInputFile();
            List<string[]> data = new List<string[]>();
            List<string> header = new List<string>();
            List<string> summary = new List<string>();

            int categry = 0;// 1:HEADER 2:SUMMARY 3:DETAILS
            //データの先頭を検索
            for (int i = 0; i < lines.Length; i++)
            {
                if(lines[i].Contains("#HEADER"))
                {
                    categry = 1;
                    continue;
                }
                else if (lines[i].Contains("#SUMMARY"))
                {
                    categry = 2;
                    continue;
                }
                else if (lines[i].Contains("#DETAILS"))
                {
                    categry = 3;
                    continue;
                }

                if(categry == 1)
                {
                    header.Add(lines[i]);
                }
                else if(categry == 2)
                {
                    summary.Add(lines[i]);
                }
                else if(categry == 3)
                {
                    string[] tmp = CanmmSplit(lines[i]);
                    if (tmp.Length > 1)
                    {
                        data.Add(tmp);
                    }
                }
            }
            if (data.Count == 0)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            //データ部
            object[,] exc = new object[data.Count, data[0].Length];
            for (int row = 0; row < data.Count; row++)
            {
                for (int col = 0; col < data[0].Length; col++)
                {
                    if (col < data[row].Length)
                    {
                        exc[row, col] = data[row][col];
                    }
                    else
                    {
                        exc[row, col] = "";
                    }
                }
            }

            Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
            //エクセルを非表示
            ExcelApp.Visible = !Is_Close;

            //エクセルファイルのオープンと
            //ワークブックをの作成
            Microsoft.Office.Interop.Excel.Workbook WorkBook = ExcelApp.Workbooks.Open(_OutputFile);

            //1シート目の選択
            Microsoft.Office.Interop.Excel.Worksheet sheet = WorkBook.Sheets["Sheet1"];
            sheet.Select();

            try
            {
                //ヘッダー
                sheet.Range["I10"].Value = getHeaderItem(summary, "パス番号=");
                sheet.Range["I11"].Value = getHeaderItem(header, "SATELLITE=");
                sheet.Range["I12"].Value = getHeaderItem(header, "P-START=");
                sheet.Range["I13"].Value = getHeaderItem(header, "P-END=");

                //2シート目の選択
                sheet = WorkBook.Sheets["Sheet2"];
                sheet.Select();

                sheet.Range["E4"].Value = getHeaderItem(summary, "衛星名=");
                sheet.Range["E5"].Value = getHeaderItem(summary, "パス番号=");
                sheet.Range["E6"].Value = getHeaderItem(summary, "INIT=");
                sheet.Range["E7"].Value = getHeaderItem(summary, "AUTO_ACQ=");
                sheet.Range["E8"].Value = getHeaderItem(summary, "OFFSET=");
                sheet.Range["E9"].Value = getHeaderItemStartsWith(summary, "P-START=");
                sheet.Range["E10"].Value = getHeaderItemStartsWith(summary, "P-END=");
                //E11は改行
                sheet.Range["E12"].Value = getHeaderItem(summary, "RNG_BAND=");
                sheet.Range["E13"].Value = getHeaderItem(summary, "RNG_TYPE=");
                sheet.Range["E14"].Value = getHeaderItemStartsWith(summary, "R-START=");
                sheet.Range["E15"].Value = getHeaderItemStartsWith(summary, "R-END=");
                //----------
                if (KyokuName == DEF_UDSC54)
                {
                    sheet.Range["J4"].Value = getHeaderItem(summary, "BITRATE-KA=");
                }
                else
                {
                    sheet.Range["J4"].Value = getHeaderItem(summary, "BITRATE-S=");
                }
                sheet.Range["J5"].Value = getHeaderItem(summary, "BITRATE-X=");
                string j6 = getHeaderItem(summary, "BITRATE-XQPSK=");
                if (j6 == string.Empty)
                {
                    j6 = getHeaderItem(summary, "BITRATE-XHRX=");
                }
                sheet.Range["J6"].Value = j6;
                string j7 = getHeaderItemStartsWith(summary, "CMD3-BITRATE=");
                if (j7 == string.Empty)
                {
                    j7 = getHeaderItemStartsWith(summary, "CMD-BITRATE=");
                }
                if (j7 == string.Empty)
                {
                    j7 = getHeaderItemStartsWith(summary, "XCMD-BITRATE=");
                }
                sheet.Range["J7"].Value = j7;
                //J8は改行
                sheet.Range["J9"].Value = getHeaderItem(summary, "AOS_FLAG=");
                sheet.Range["J10"].Value = getHeaderItem(summary, "LOS_FLAG=");
                sheet.Range["J11"].Value = getHeaderItem(summary, "TERM_ANT_MODE=");
                sheet.Range["J12"].Value = getHeaderItem(summary, "CMD_MOD_ON=");
                sheet.Range["J13"].Value = getHeaderItem(summary, "RNG_MOD_ON=");
                sheet.Range["J14"].Value = getHeaderItem(summary, "TX_RAMP=");
                sheet.Range["J15"].Value = getHeaderItem(summary, "RX_RAMP=");
                //----------
                sheet.Range["O4"].Value = getHeaderItem(summary, "ACQ_ROUTE=");
                sheet.Range["O5"].Value = getHeaderItem(summary, "ANT_STOW=");
                sheet.Range["O6"].Value = getHeaderItem(summary, "FALSE_LOCK=");
                sheet.Range["O7"].Value = getHeaderItem(summary, "POL_CHK=");
                sheet.Range["O8"].Value = getHeaderItem(summary, "POL_ENT=");
                sheet.Range["O9"].Value = getHeaderItem(summary, "POL_EXIT=");

                //3シート目の選択
                sheet = WorkBook.Sheets["Sheet3"];
                sheet.Select();

                //格子を書く
                sheet.Range["B5:E" + (data.Count + 4).ToString()].Borders.LineStyle = 1;

                //データを書く
                sheet.Range["B5:E" + (data.Count + 4).ToString()].Value2 = exc;

                // 作成者、前回保存者、会社のプロパティは削除する
                dynamic properties = WorkBook.BuiltinDocumentProperties;
                dynamic property = properties.Item("Author");
                property.Value = "";
                property = properties.Item("Last author");
                property.Value = "";
                property = properties.Item("Company");
                property.Value = "";

                WorkBook.Save();

                if (Is_Close)
                {
                    //ワークブックを閉じる
                    WorkBook.Close();
                    //エクセルを閉じる
                    ExcelApp.Quit();
                }
            }
            finally
            {
                if (sheet != null)
                {
                    Marshal.ReleaseComObject(sheet);
                    sheet = null;
                }
                if (WorkBook != null)
                {
                    Marshal.ReleaseComObject(WorkBook);
                    WorkBook = null;
                }

                if (ExcelApp != null)
                {
                    Marshal.ReleaseComObject(ExcelApp);
                    ExcelApp = null;
                }
                GC.Collect();
            }

        }

#region 追跡データファイル
        /// <summary>
        /// 追跡データファイル
        /// </summary>
        private void TRK_HIST()
        {
            string[] lines = ReadInputFile();
            List<string[]> data = new List<string[]>();
            List<string> summary = new List<string>();

            //データの先頭を検索
            for (int i = 0; i < lines.Length; i++)
            {
                string[] tmp = lines[i].Split(spWord3, StringSplitOptions.None);

                if (i < 4)
                {
                    // サマリー情報
                    summary.Add(tmp[0]);
                }
                else
                {
                    // データ
                    data.Add(tmp);
                }
            }
            if (data.Count == 0)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            object[,] exc = new object[data.Count, 17];
            for (int row = 0; row < data.Count; row++)
            {
                for (int col = 0; col < 17; col++)
                {
                    if (col < data[row].Length)
                    {
                        exc[row, col] = data[row][col];
                    }
                    else
                    {
                        exc[row, col] = "";
                    }
                }
            }

            Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
            //エクセルを非表示
            ExcelApp.Visible = !Is_Close;

            //エクセルファイルのオープンと
            //ワークブックをの作成
            Microsoft.Office.Interop.Excel.Workbook WorkBook = ExcelApp.Workbooks.Open(_OutputFile);

            //[TRK_HIST]シートの選択
            Microsoft.Office.Interop.Excel.Worksheet sheet = WorkBook.Sheets["TRK_HIST"];
            sheet.Select();

            try
            {
                sheet.Range["D2"].Value = summary[0];                   // ファイル名
                sheet.Range["D3"].Value = summary[1];                   // パス番号
                sheet.Range["D4"].Value = summary[2];                   // 衛星名
                sheet.Range["D5"].Value = summary[3] + "追跡データ";    // 計測ルート

                //格子を書く
                sheet.Range["B9:B" + (data.Count + 8).ToString()].Borders[Microsoft.Office.Interop.Excel.XlBordersIndex.xlEdgeLeft].LineStyle = Microsoft.Office.Interop.Excel.XlLineStyle.xlContinuous;
                sheet.Range["B9:B" + (data.Count + 8).ToString()].Borders[Microsoft.Office.Interop.Excel.XlBordersIndex.xlEdgeRight].LineStyle = Microsoft.Office.Interop.Excel.XlLineStyle.xlContinuous;
                sheet.Range["B9:B" + (data.Count + 8).ToString()].Borders[Microsoft.Office.Interop.Excel.XlBordersIndex.xlEdgeBottom].LineStyle = Microsoft.Office.Interop.Excel.XlLineStyle.xlContinuous;

                sheet.Range["C9:R" + (data.Count + 8).ToString()].Borders[Microsoft.Office.Interop.Excel.XlBordersIndex.xlEdgeLeft].LineStyle = Microsoft.Office.Interop.Excel.XlLineStyle.xlContinuous;
                sheet.Range["C9:R" + (data.Count + 8).ToString()].Borders[Microsoft.Office.Interop.Excel.XlBordersIndex.xlEdgeRight].LineStyle = Microsoft.Office.Interop.Excel.XlLineStyle.xlContinuous;
                sheet.Range["C9:R" + (data.Count + 8).ToString()].Borders[Microsoft.Office.Interop.Excel.XlBordersIndex.xlEdgeBottom].LineStyle = Microsoft.Office.Interop.Excel.XlLineStyle.xlContinuous;

                //データを書く
                sheet.Range["B9:R" + (data.Count + 8).ToString()].Value2 = exc;

                // 作成者、前回保存者、会社のプロパティは削除する
                dynamic properties = WorkBook.BuiltinDocumentProperties;
                dynamic property = properties.Item("Author");
                property.Value = "";
                property = properties.Item("Last author");
                property.Value = "";
                property = properties.Item("Company");
                property.Value = "";

                WorkBook.Save();

                if (Is_Close)
                {
                    //ワークブックを閉じる
                    WorkBook.Close();
                    //エクセルを閉じる
                    ExcelApp.Quit();
                }
            }
            finally
            {
                if (sheet != null)
                {
                    Marshal.ReleaseComObject(sheet);
                    sheet = null;
                }
                if (WorkBook != null)
                {
                    Marshal.ReleaseComObject(WorkBook);
                    WorkBook = null;
                }

                if (ExcelApp != null)
                {
                    Marshal.ReleaseComObject(ExcelApp);
                    ExcelApp = null;
                }
                GC.Collect();
            }
        }
#endregion

#region 送信周波数情報データファイル
        /// <summary>
        /// 送信周波数情報データファイル
        /// </summary>
        private void SEND_FREQ_INFO()
        {
            string[] lines = ReadInputFile();
            List<string[]> data = new List<string[]>();
            List<string> summary = new List<string>();
            int data_count = 0;

            //データの先頭を検索
            for (int i = 0; i < lines.Length; i++)
            {
                string[] tmp = lines[i].Split(spWord3, StringSplitOptions.None);

                if (i < 7)
                {
                    // サマリー情報
                    summary.Add(tmp[0]);
                }
                else
                {
                    // データ
                    data.Add(tmp);
                }
            }
            if (data.Count == 0)
            {
                throw new ErrorException("ファイルフォーマットが異なります。選択ファイルを確認してください。選択帳票種：" + SelecedObj.Name);
            }

            object[,] exc = new object[data.Count, 17];
            for (int row = 0; row < data.Count; row++)
            {
                for (int col = 0; col < 17; col++)
                {
                    if (col < data[row].Length)
                    {
                        exc[row, col] = data[row][col];
                        if (col != 0)
                        {
                            data_count++;
                        }
                    }
                    else
                    {
                        exc[row, col] = "";
                    }
                }
            }

            Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
            //エクセルを非表示
            ExcelApp.Visible = !Is_Close;

            //エクセルファイルのオープンと
            //ワークブックをの作成
            Microsoft.Office.Interop.Excel.Workbook WorkBook = ExcelApp.Workbooks.Open(_OutputFile);

            //[SEND_FREQ_INFO]シートの選択
            Microsoft.Office.Interop.Excel.Worksheet sheet = WorkBook.Sheets["SEND_FREQ_INFO"];
            sheet.Select();

            try
            {
                sheet.Range["E2"].Value = KyokuName + "送信周波数情報";     // ファイル種別
                sheet.Range["E3"].Value = data_count;                       // データ部容量(byte)
                sheet.Range["E4"].Value = summary[4];                       // 衛星番号
                sheet.Range["E5"].Value = "";

                sheet.Range["N2"].Value = summary[1];                       // 作成日時
                sheet.Range["N3"].Value = summary[3];                       // パス番号
                sheet.Range["N4"].Value = summary[5];                       // 衛星名

                //格子を書く
                sheet.Range["B9:B" + (data.Count + 8).ToString()].Borders[Microsoft.Office.Interop.Excel.XlBordersIndex.xlEdgeLeft].LineStyle = Microsoft.Office.Interop.Excel.XlLineStyle.xlContinuous;
                sheet.Range["B9:B" + (data.Count + 8).ToString()].Borders[Microsoft.Office.Interop.Excel.XlBordersIndex.xlEdgeRight].LineStyle = Microsoft.Office.Interop.Excel.XlLineStyle.xlContinuous;
                sheet.Range["B9:B" + (data.Count + 8).ToString()].Borders[Microsoft.Office.Interop.Excel.XlBordersIndex.xlEdgeBottom].LineStyle = Microsoft.Office.Interop.Excel.XlLineStyle.xlContinuous;

                sheet.Range["C9:R" + (data.Count + 8).ToString()].Borders[Microsoft.Office.Interop.Excel.XlBordersIndex.xlEdgeLeft].LineStyle = Microsoft.Office.Interop.Excel.XlLineStyle.xlContinuous;
                sheet.Range["C9:R" + (data.Count + 8).ToString()].Borders[Microsoft.Office.Interop.Excel.XlBordersIndex.xlEdgeRight].LineStyle = Microsoft.Office.Interop.Excel.XlLineStyle.xlContinuous;
                sheet.Range["C9:R" + (data.Count + 8).ToString()].Borders[Microsoft.Office.Interop.Excel.XlBordersIndex.xlEdgeBottom].LineStyle = Microsoft.Office.Interop.Excel.XlLineStyle.xlContinuous;

                //データを書く
                sheet.Range["B9:R" + (data.Count + 8).ToString()].Value2 = exc;

                // 作成者、前回保存者、会社のプロパティは削除する
                dynamic properties = WorkBook.BuiltinDocumentProperties;
                dynamic property = properties.Item("Author");
                property.Value = "";
                property = properties.Item("Last author");
                property.Value = "";
                property = properties.Item("Company");
                property.Value = "";

                WorkBook.Save();

                if (Is_Close)
                {
                    //ワークブックを閉じる
                    WorkBook.Close();
                    //エクセルを閉じる
                    ExcelApp.Quit();
                }
            }
            finally
            {
                if (sheet != null)
                {
                    Marshal.ReleaseComObject(sheet);
                    sheet = null;
                }
                if (WorkBook != null)
                {
                    Marshal.ReleaseComObject(WorkBook);
                    WorkBook = null;
                }

                if (ExcelApp != null)
                {
                    Marshal.ReleaseComObject(ExcelApp);
                    ExcelApp = null;
                }
                GC.Collect();
            }

        }
#endregion

        private readonly string CanmaIns = "<CANM>";

        private string[] CanmmSplit(string str)
        {
            int start = str.IndexOf('\"', 0);
            int end = str.IndexOf('\"', start + 1);

            if (start == -1 || end == -1)
            {
                return str.Split(spWord3, StringSplitOptions.None);
            }

            string cTmp = str.Substring(start, end - start + 1);

            string rTmp = cTmp.Replace(",", CanmaIns);
            rTmp = rTmp.Replace("\"", "");

            str = str.Replace(cTmp, rTmp);

            string[] data = str.Split(',');

            for (int i = 0; i < data.Length; i++)
            {
                data[i] = data[i].Replace(CanmaIns, ",");
            }

            return data;
        }



        private void buttonSelectDir_Click(object sender, EventArgs e)
        {
            //FolderBrowserDialogクラスのインスタンスを作成
            FolderBrowserDialog fbd = new FolderBrowserDialog();

            //上部に表示する説明テキストを指定する
            fbd.Description = "フォルダを指定してください。";
            //ルートフォルダを指定する
            //デフォルトでDesktop
            //fbd.RootFolder = Environment.SpecialFolder.Personal;
            //最初に選択するフォルダを指定する
            //RootFolder以下にあるフォルダである必要がある
            fbd.SelectedPath = @"C:\Windows";
            //ユーザーが新しいフォルダを作成できるようにする
            //デフォルトでTrue
            fbd.ShowNewFolderButton = true;

            //ダイアログを表示する
            if (fbd.ShowDialog(this) == DialogResult.OK)
            {
                _OutputDir = fbd.SelectedPath;
                _OutputFile = CreateOutputFileName();
                this.textBoxOutput.Text = _OutputFile;

            }

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //OpenFileDialogクラスのインスタンスを作成
            OpenFileDialog ofd = new OpenFileDialog();

            //はじめのファイル名を指定する
            //はじめに「ファイル名」で表示される文字列を指定する
            ofd.FileName = "*.*";
            //はじめに表示されるフォルダを指定する
            //指定しない（空の文字列）の時は、現在のディレクトリが表示される
            ofd.InitialDirectory = @"C:\";
            //[ファイルの種類]に表示される選択肢を指定する
            //指定しないとすべてのファイルが表示される
            ofd.Filter = "すべてのファイル(*.*)|*.*";

            //[ファイルの種類]ではじめに選択されるものを指定する
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
                _InputFile = ofd.FileName;
                textBoxSelect.Text = _InputFile;
                displayFilePath(pictureBox1, _InputFile);

                //局名取得
                try
                {
                    int start = _InputFile.IndexOf(@"\share\", StringComparison.OrdinalIgnoreCase);
                    start += @"\share\".Length;
                    int end = _InputFile.IndexOf(@"\", start);
                    KyokuName = _InputFile.Substring(start, end - start).ToUpper();
                    KyokuNameJp = ToJpKyokuName(KyokuName);
                }
                catch
                {}
            }

        }


        private string ToJpKyokuName(string folname)
        {
            if (folname == "UDSC54")
            {
                return "臼田54m局";
            }
            else if (folname == "UDSC64")
            {
                return "臼田64m局";
            }
            else if (folname == "USC34")
            {
                return "内之浦34m局";
            }
            else if (folname == "USC20")
            {
                return "内之浦20m局";
            }

            return "";
        }
    }
}
