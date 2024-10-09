using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;

namespace Dispatcher
{
    static class Program
    {

        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        static void Main()
        {

            //デバック用
            if (Environment.UserInteractive)
            {
                DispMain service1 = new DispMain();
                service1.TestStartupAndStop(new string[] { "args" });
                return;
            }

            ServiceBase[] ServicesToRun;
            ServicesToRun = new ServiceBase[] 
                { 
                    new DispMain() 
                };

            ServiceBase.Run(ServicesToRun);
        }

    }
}
