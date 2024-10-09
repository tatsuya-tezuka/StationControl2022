using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace RegistSendTo
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                //Debug
                //for (int i = 0; i < args.Length; i++)
                //{
                //    Console.WriteLine(args[i]);
                //}
                //Console.ReadLine();

                //カレントディレクトリを取得
                string appPath;
                if(args.Length < 1)
                {
                    appPath = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                }
                else
                {
                    appPath = args[0];
                }
                //作成するショートカットのパス
                string shortcutPath = System.IO.Path.Combine(appPath, @"Excel帳票作成.lnk");
                //ショートカットのリンク先
                string targetPath = System.IO.Path.Combine(appPath, @"Excel帳票作成.exe");

                //WshShellを作成
                IWshRuntimeLibrary.WshShell shell = new IWshRuntimeLibrary.WshShell();
                //ショートカットのパスを指定して、WshShortcutを作成
                IWshRuntimeLibrary.IWshShortcut shortcut =
                    (IWshRuntimeLibrary.IWshShortcut)shell.CreateShortcut(shortcutPath);
                //リンク先
                shortcut.TargetPath = targetPath;
                //コマンドパラメータ 「リンク先」の後ろに付く
                shortcut.Arguments = "";
                //作業フォルダ
                shortcut.WorkingDirectory = appPath;
                //ショートカットキー（ホットキー）
                shortcut.Hotkey = "";
                //実行時の大きさ 1が通常、3が最大化、7が最小化
                shortcut.WindowStyle = 1;
                //コメント
                shortcut.Description = "";
                //アイコンのパス 自分のEXEファイルのインデックス0のアイコン
                //shortcut.IconLocation = Application.ExecutablePath + ",0";

                //ショートカットを作成
                shortcut.Save();

                //後始末
                System.Runtime.InteropServices.Marshal.FinalReleaseComObject(shortcut);
                System.Runtime.InteropServices.Marshal.FinalReleaseComObject(shell);

                string Sendto = System.Environment.GetFolderPath(Environment.SpecialFolder.SendTo);
                Sendto = Path.Combine(Sendto, "Excel帳票作成.lnk");

                File.Copy(shortcutPath, Sendto, true);

                File.Delete(shortcutPath);
            }
            catch { }
        }
    }
}
