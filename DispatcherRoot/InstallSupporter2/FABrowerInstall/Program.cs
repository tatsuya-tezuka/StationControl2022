using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using Install.Core;

namespace FABrowerInstall
{
    static class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main()
        {
            string[] cmds = System.Environment.GetCommandLineArgs();
            string appPath;
            if (cmds.Length < 2)
            {
                appPath = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            }
            else
            {
                appPath = cmds[1];
            }

            appPath = appPath.Trim('\"');
            appPath = appPath.Trim();

            string OutputPath = appPath;
            string LogFile = "FABrowerInstall.log";

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new FormSelect("FABrower", OutputPath, Properties.Resources.ZipFile, LogFile));

        }
    }
}
