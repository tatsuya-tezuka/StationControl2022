using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.IO.Compression;
using System.Threading.Tasks;


namespace Install.Core
{
    public class Core
    {
        StreamWriter log = null;
        string OutputPath;
        string LogFile;
        
        string ZipPath;

        public Core(string outPath, string logName, string zipPath)
        {
            OutputPath = outPath;
            LogFile = logName;
            
            ZipPath = zipPath;
        }

        public void Executing()
        {
            try
            {

                if (!Directory.Exists(OutputPath))
                {
                    Directory.CreateDirectory(OutputPath);
                }

                string path = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData) + @"\NEC\StationControl";
                if (!Directory.Exists(path))
                {
                    Directory.CreateDirectory(path);
                }

                log = new StreamWriter(Path.Combine(path, LogFile), true);
                log.WriteLine(DateTime.Now.ToString() + " ****** Start ******");

                string cName = System.Globalization.CultureInfo.CurrentCulture.Name;
                Encoding ed = Encoding.GetEncoding(932);
                if (cName.Contains("en-"))
                {
                    ed = Encoding.GetEncoding(437);
                }
                log.WriteLine(DateTime.Now.ToString() + " CurrentCulture =" + cName);
                log.WriteLine(DateTime.Now.ToString() + " OutPut =" + OutputPath);

                //Zipが存在するか
                if (!File.Exists(ZipPath))
                {
                    log.WriteLine(DateTime.Now.ToString() + " Zip file does not exist. I will not install it.");
                    log.WriteLine(DateTime.Now.ToString() + " ****** End ******");
                    return;
                }
                //Zip情報を取得
                File.GetCreationTime(ZipPath);
                log.WriteLine(DateTime.Now.ToString() + " [ZIP INFO]CreationTime = " + File.GetCreationTime(ZipPath));
                log.WriteLine(DateTime.Now.ToString() + " [ZIP INFO]LastAccessTime = " + File.GetLastAccessTime(ZipPath));

                using (ZipArchive archive = ZipFile.OpenRead(ZipPath))
                {
                    ExtractToDirectory(archive, OutputPath, true);
                }
                log.WriteLine(DateTime.Now.ToString() + " ****** End ******");
            }
            catch (Exception ex)
            {
                log.WriteLine(DateTime.Now.ToString() + " ERROR:" + ex.Message);
            }
            finally
            {
                if (log != null)
                {
                    log.Close();
                }
            }
        }

        /// <summary>
        /// エントリーがディレクトリかどうか取得する。
        /// </summary>
        /// <param name="entry">ZIPアーカイブエントリー</param>
        /// <returns></returns>
        public bool IsDirectory(ZipArchiveEntry entry)
        {
            return string.IsNullOrEmpty(entry.Name);
        }

        /// <summary>
        /// ZIPアーカイブ内のすべてのファイルを特定のフォルダに解凍する。
        /// </summary>
        /// <param name="source">ZIPアーカイブ</param>
        /// <param name="destinationDirectoryName">解凍先ディレクトリ。</param>
        /// <param name="overwrite">上書きフラグ。ファイルの上書きを行う場合はtrue。</param>
        public void ExtractToDirectory(ZipArchive source, string destinationDirectoryName, bool overwrite)
        {
            foreach (var entry in source.Entries)
            {
                var fullPath = Path.Combine(destinationDirectoryName, entry.FullName);
                if (IsDirectory(entry))
                {
                    if (!Directory.Exists(fullPath))
                    {
                        Directory.CreateDirectory(fullPath);
                    }
                }
                else
                {
                    if (overwrite)
                    {
                        entry.ExtractToFile(fullPath, true);
                        log.WriteLine(DateTime.Now.ToString() + " ExtractToFile:" + fullPath);
                    }
                    else
                    {
                        if (!File.Exists(fullPath))
                        {
                            entry.ExtractToFile(fullPath, true);
                            log.WriteLine(DateTime.Now.ToString() + " ExtractToFile:" + fullPath);
                        }
                    }
                }
            }
        }
    }
}
