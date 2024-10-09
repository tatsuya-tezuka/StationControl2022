using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Dispatcher
{
    public class DsLog
    {
        private static ConcurrentQueue<string> mLogMsg;
        private bool IsAlive;
        private string FileName;
        private StringBuilder mSb;
        private int RecordCnt;
        private readonly int REC_COUNT = 10000;
        
        private Encoding mEnc;
        private string LogDir;
        private bool IsDebug = false;


        public DsLog()
        {
            IsDebug = false;
            IsAlive = true;
            RecordCnt = 0;
            mEnc = Encoding.GetEncoding("shift_jis");

            Assembly myAsse = Assembly.GetEntryAssembly();
            LogDir = Path.GetDirectoryName(myAsse.Location);

            mLogMsg = new ConcurrentQueue<string>();

            mSb = new StringBuilder();
            mSb.Clear();

            if (!Directory.Exists(LogDir + @"\Log\"))
            {
                Directory.CreateDirectory(LogDir + @"\Log\");
            }
            
            FileName = CreateFileName();

            Task.Factory.StartNew(() =>
                {
                    DeleteFile(LogDir + @"\Log\");

                    while (IsAlive)
                    {
                        try
                        {
                            if (RecordCnt > REC_COUNT)
                            {
                                DeleteFile(LogDir + @"\Log\");

                                RecordCnt = 0;
                                FileName = CreateFileName();
                            }

                            using (StreamWriter mStWrite = new StreamWriter(FileName, true, mEnc))
                            {
                                string msg;
                                if (mLogMsg.TryDequeue(out msg))
                                {
                                    mStWrite.WriteLine(msg);
                                    RecordCnt++;
                                }
                                else
                                {
                                    Thread.Sleep(100);
                                }
                            }
                        }
                        catch
                        {
                            Thread.Sleep(100);
                        }
                    }
                });

        }

        public void DeleteFile(string dir)
        {
            DateTime dt = DateTime.Now;
            List<string> fileList = Directory.GetFiles(dir).ToList();

            fileList.ForEach(s =>
            {
                try
                {
                    if ((dt - DateTime.ParseExact(Path.GetFileName(s).Substring(8, 14), "yyyyMMddHHmmss", null)).TotalDays >= 7)
                    {
                        File.Delete(s);
                    }
                }
                catch (ThreadAbortException)
                {
                    throw;
                }
                catch (Exception)
                { }
            });
        }

        public void SetDebugFlag(bool flag)
        {
            IsDebug = flag;
        }

        public void WriteLog_Debug(string name, string msg)
        {
            if (IsDebug)
            {
                lock (lockObj)
                {
                    mSb.Clear();
                    mSb.Append(DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff"));
                    mSb.Append("\t");
                    mSb.Append(name);
                    mSb.Append("\t");
                    mSb.Append(msg);

                    mLogMsg.Enqueue(mSb.ToString());
                }
            }
        }

        object lockObj = new object();
        public void WriteLog(string name, string msg)
        {
            lock (lockObj)
            {
                mSb.Clear();
                mSb.Append(DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff"));
                mSb.Append("\t");
                mSb.Append(name);
                mSb.Append("\t");
                mSb.Append(msg);

                mLogMsg.Enqueue(mSb.ToString());
            }
        }


        public void Close()
        {
            IsAlive = false;
        }


        private string CreateFileName()
        {
            return LogDir + @"\Log\" + "DispaLog" + DateTime.Now.ToString("yyyyMMddHHmmss") + ".txt";
        }


    }
}
