#define CALL

using Linphone.Core;
using System;

namespace TestLibLinphoneCLIConsole
{
    class Program
    {
        static void Main(string[] args)
        {
            var listener = new LinphoneManager();
            Globals.Instance.LinphoneCoreFactory.CreateLinphoneCore(listener);
            LinphoneCore lc = Globals.Instance.LinphoneCore;
            listener.Lc = lc;

            var proxyCfg = new LinphoneProxyConfig(lc);
            proxyCfg.SetIdentity("test 1000 <sip:1000@192.168.6.241>");
            proxyCfg.ServerAddr = "192.168.6.241";
            proxyCfg.RegisterEnabled = true;
            proxyCfg.Expires = 60000;
            lc.ClearProxyConfigs();
            lc.AddProxyConfig(proxyCfg);
            lc.DefaultProxyConfig = proxyCfg;

            var authInfo = new LinphoneAuthInfo("1000", string.Empty, "12345", string.Empty, string.Empty, string.Empty);
            lc.AddAuthInfo(authInfo);
            
            lc.IterateEnabled = true;
            while (lc.DefaultProxyConfig.State != RegistrationState.RegistrationOk)
            {
                Console.WriteLine("Registering...");
                System.Threading.Thread.Sleep(1000);
                if (lc.DefaultProxyConfig.State == RegistrationState.RegistrationFailed)
                {
                    Console.WriteLine("Reason: {0}", lc.DefaultProxyConfig.Error);
                    lc.IterateEnabled = false;
                    Console.ReadKey();
                    return;
                }
            }
            Console.WriteLine("Registered");
            Console.ReadKey();

#if CALL
            Console.WriteLine("Calling to sip:1001@192.168.6.241 ...");
            
            var callParams = lc.CreateDefaultCallParameters();
            callParams.VideoEnabled = false;
            //var callAddr = Globals.Instance.LinphoneCoreFactory.CreateLinphoneAddress("sip:1001@192.168.6.241");
            //var call = lc.InviteAddressWithParams(callAddr, callParams);
            var call = lc.InviteWithParams("sip:1001@192.168.6.241", callParams);
            
            if (call.State != LinphoneCallState.Connected)
            {
                Console.WriteLine("Waiting accept call...");
                System.Threading.Thread.Sleep(2000);
            }
            Console.ReadKey();
            
            lc.TerminateAllCalls();
#endif
            // De-activate registering
            var defaultCfg = lc.DefaultProxyConfig;
            defaultCfg.Edit();
            defaultCfg.RegisterEnabled = false;
            defaultCfg.Done();
            while (lc.DefaultProxyConfig.State != RegistrationState.RegistrationCleared)
            {
                Console.WriteLine("UnRegistering...");
                System.Threading.Thread.Sleep(1000);
            }
            Console.WriteLine("UnRegistered");
            lc.IterateEnabled = false;
            Console.ReadKey();
        }
    }
}
