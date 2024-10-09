using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConvertToExcel
{
    #region ファイルタイプ
    public enum FileType
    {
        /// <summary>
        /// アンテナ予報値データ(近地球用)
        /// </summary>
        ANT_YOHO_KinChikyu,
        /// <summary>
        /// アンテナ予報値データ(深宇宙用)
        /// </summary>
        ANT_YOHO_ShinUchu,
        /// <summary>
        /// 運用制御情報(近地球用)
        /// </summary>
        UNYO_SEIGYO_KinChikyu,
        /// <summary>
        /// 運用制御情報(深宇宙用)
        /// </summary>
        UNYO_SEIGYO_ShinUchu,
        /// <summary>
        /// 局運用計画
        /// </summary>
        KYOKU_UNNYO_KYOKU,
        /// <summary>
        /// 無線業務日誌
        /// </summary>
        MUSEN_NISSHI,
        /// <summary>
        /// RF情報ファイル
        /// </summary>
        RF_INFO,
        /// <summary>
        /// パス履歴
        /// </summary>
        PATH_RIREKI,
        /// <summary>
        /// 運用履歴
        /// </summary>
        UNYO_RIREKI,
        /// <summary>
        /// 制御履歴
        /// </summary>
        SEIGYO_RIREKI,
        /// <summary>
        /// 設備状態履歴
        /// </summary>
        SETSUBI_RIREKI,
        /// <summary>
        /// 設備監視蓄積データ
        /// </summary>
        KANSHI_CHIKUSEKI,
        /// <summary>
        /// アンテナ設備駆動履歴ファイル
        /// </summary>
        ANT_KUDO_RIREKI,
        /// <summary>
        /// アンテナ設備運用履歴ファイル
        /// </summary>
        ANT_UNYO_RIREKI,
        /// <summary>
        /// イベント履歴
        /// </summary>
        EVENT_RIREKI,
        /// <summary>
        /// 追跡データファイル
        /// </summary>
        TRK_HIST,
        /// <summary>
        /// 送信周波数情報データファイル
        /// </summary>
        SEND_FREQ_INFO,
        /// <summary>
        /// RF情報ファイル2
        /// </summary>
        RF_INFO2,
        /// <summary>
        /// 運用制御情報(深宇宙用) udsc54
        /// </summary>
        UNYO_SEIGYO_ShinUchu_udsc54,
        /// <summary>
        /// パス履歴 udsc54
        /// </summary>
        PATH_RIREKI_udsc54,
        /// <summary>
        /// 局運用計画 udsc54
        /// </summary>
        KYOKU_UNNYO_KYOKU_udsc54,
        /// <summary>
        /// アンテナ設備駆動履歴ファイル udsc54
        /// </summary>
        ANT_KUDO_RIREKI_udsc54,


    }
    #endregion

    //public class InfoException : Exception
    //{
    //    public InfoException() { }

    //    public InfoException(string message) : base(message) { }

    //    public InfoException(string message, Exception inner) : base(message) { }
    //}

    public class WarnningException : Exception
    {
        public WarnningException() { }

        public WarnningException(string message) : base(message) { }

        public WarnningException(string message, Exception inner) : base(message) { }
    }

    public class ErrorException : Exception
    {
        public ErrorException() { }

        public ErrorException(string message) : base(message) { }

        public ErrorException(string message, Exception inner) : base(message) { }
    }

    public class NoActionException : Exception
    {
        public NoActionException() { }

        public NoActionException(string message) : base(message) { }

        public NoActionException(string message, Exception inner) : base(message) { }
    }

    public class CombOnj
    {
        public FileType ExcelType;
        public string Name;
        public string TempFileName;
        public bool IsMatomeru = false;
        public bool IsDisplay = true;

        public CombOnj()
        { }


        public string FullTempFile()
        {
            //Exeの直下に"TempFile"を置くこと
            string appPath = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            return Path.Combine(appPath, "TempFile", TempFileName);
        }

        public override string ToString()
        {
            return Name;
        }
    }

    public class CItem
    {
        public string Group;
        public string Line;
        public string NamaLine;
        public string Key;
        public string ItemValue;
    }


    public static class Log
    {
        public static void ExWrite(string msg)
        {
            try
            {
                string path = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData) + @"\NEC\StationControl";

                string date = DateTime.Now.ToString("yyyyMMddHHmmss");
                StreamWriter log = new StreamWriter(Path.Combine(path, "ToExcelLog" + date + ".txt"), true);
                log.WriteLine(date + " " + msg);
                log.Close();
            }
            catch { }
        }

    }

}
