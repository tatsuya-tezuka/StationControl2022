using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ConvertToExcel
{
    static class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main()
        {
            string errMag = string.Empty;

            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new Form1());
            }
            catch (NoActionException)
            {
                //何もしないで終了
            }
            catch (ErrorException ex)
            {
                errMag = ex.Message + "\n";
                if(!string.IsNullOrEmpty(ex.StackTrace))
                {
                    errMag += ex.StackTrace;
                }
            }
            catch (WarnningException ex)
            {
                errMag = ex.Message + "\n";
                if (!string.IsNullOrEmpty(ex.StackTrace))
                {
                    errMag += ex.StackTrace;
                }
            }
            catch (Exception ex)
            {
                errMag = ex.Message + "\n";
                if (!string.IsNullOrEmpty(ex.StackTrace))
                {
                    errMag += ex.StackTrace;
                }
            }

            if(!string.IsNullOrEmpty(errMag))
            {
                Log.ExWrite(errMag);
                FormResult fr = new FormResult(errMag);
                fr.ShowDialog();
            }

            #region Mutex廃止
            //string mutexname = "SC_ToExcel";
            //System.Threading.Mutex myTex = new System.Threading.Mutex(false, mutexname);
            //bool texHondle = false;
            //MessageBoxIcon myIcon = MessageBoxIcon.Information;
            //string msg = "";

            //try
            //{
            //    try
            //    {
            //        //ミューテックスの所有権を要求する
            //        texHondle = myTex.WaitOne(60000, false);
            //    }
            //    catch (System.Threading.AbandonedMutexException)
            //    {
            //        //別のアプリケーションがミューテックスを解放しないで終了した時
            //        texHondle = true;
            //    }

            //    try
            //    {
            //        Application.EnableVisualStyles();
            //        Application.SetCompatibleTextRenderingDefault(false);
            //        Application.Run(new Form1());
            //    }
            //    catch (NoActionException)
            //    {
            //        //何もしないで終了
            //    }
            //    catch (ErrorException ex)
            //    {
            //        myIcon = MessageBoxIcon.Error;
            //        msg = ex.Message;
            //    }
            //    catch (WarnningException ex)
            //    {
            //        myIcon = MessageBoxIcon.Warning;
            //        msg = ex.Message;
            //    }
            //    catch (InfoException ex)
            //    {
            //        myIcon = MessageBoxIcon.Information;
            //        msg = ex.Message;
            //    }
            //    catch (Exception ex)
            //    {
            //        myIcon = MessageBoxIcon.Error;
            //        msg = ex.Message;
            //    }

            //}
            //finally
            //{
            //    if (texHondle)
            //    {
            //        //ミューテックスを解放する
            //        myTex.ReleaseMutex();
            //    }
            //    myTex.Close();
            //}

            //if (!string.IsNullOrEmpty(msg))
            //{
            //    MessageBox.Show(msg, "帳票作成ツール", MessageBoxButtons.OK, myIcon);
            //}
            #endregion

        }
    }
}
