using System;
using System.Configuration;
using System.Windows.Forms;
using Linphone.Core;

namespace LibLinphone.NET.WinFormTest
{
    public partial class Form1 : Form, ILinphoneCoreListener
    {
        const string AppConfig_FSRegisterInfo = "FS Register Info";
        const string AppConfig_FSCalleeInfo = "FS Call Center Info";
        const string Template_FSRegisterInfo = "sip:<user_name>@<host>:[port]";
        const string Template_FSCallee = "sip:<callee_user_name>@<host>:[port]";

        private LinphoneCoreListener listener = new LinphoneCoreListener();
        private LinphoneCore lc;

        private bool SipRegistered
        {
            get 
            { 
                return lc != null 
                    && lc.DefaultProxyConfig != null 
                    && lc.DefaultProxyConfig.State == Linphone.Core.RegistrationState.RegistrationOk;
            }
        }

        public Form1()
        {
            InitializeComponent();
            
            listener = new LinphoneCoreListener();
            listener.WriteLog += listener_WriteLog;
        }

        void listener_WriteLog(string format, params object[] args)
        {
            this.UIThreadBeginInvoke(() => WriteLog(format, args));
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            localAddress.Text = string.IsNullOrWhiteSpace(ConfigurationManager.AppSettings[AppConfig_FSRegisterInfo]) ? 
                Template_FSRegisterInfo : ConfigurationManager.AppSettings[AppConfig_FSRegisterInfo];
            calleeAddress.Text = string.IsNullOrWhiteSpace(ConfigurationManager.AppSettings[AppConfig_FSCalleeInfo]) ?
                Template_FSCallee : ConfigurationManager.AppSettings[AppConfig_FSCalleeInfo];

            InitLinphoneCore();
            //Register();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                Unregister();

                //System.Threading.Thread.Sleep(2000);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void WriteLog(string format, params object[] args)
        {
            this.UIThreadBeginInvoke(delegate
            {
                string log = string.Format(format, args);
                this.consoleLog.AppendText(Environment.NewLine + log);
            });
        }

        private void InitLinphoneCore()
        {
            //Globals.Instance.LinphoneCoreFactory.CreateLinphoneCore(listener);
            Globals.Instance.LinphoneCoreFactory.CreateLinphoneCore(this);
            lc = Globals.Instance.LinphoneCore;
            lc.NativeVideoWindowId = this.videoViewerPanel.Handle.ToInt64();
            listener.Lc = lc;
        }

        private void Register()
        {
            string localUri = this.localAddress.Text;
            string password = this.password.Text;
            if (string.IsNullOrWhiteSpace(localUri))
                return;

            var localAddress = Globals.Instance.LinphoneCoreFactory.CreateLinphoneAddress(localUri);
            if (localAddress == null)
            {
                MessageBox.Show("Your SIP address is invalid");
                return;
            }
            var proxyCfg = new LinphoneProxyConfig();
            proxyCfg.SetIdentity(localUri);
            proxyCfg.RegisterEnabled = true;
            proxyCfg.Expires = 3600;            
            proxyCfg.ServerAddr = localAddress.Domain;

            lc.ClearProxyConfigs();
            lc.AddProxyConfig(proxyCfg);
            lc.DefaultProxyConfig = proxyCfg;

            var authInfo = new LinphoneAuthInfo(localAddress.UserName, string.Empty, password, string.Empty, string.Empty, string.Empty);
            lc.AddAuthInfo(authInfo);

            lc.IterateEnabled = true;
            while (lc.DefaultProxyConfig.State != Linphone.Core.RegistrationState.RegistrationOk)
            {
                WriteLog("Registering...");
                System.Threading.Thread.Sleep(1000);
                if (lc.DefaultProxyConfig.State == Linphone.Core.RegistrationState.RegistrationFailed)
                {
                    WriteLog("Reason: {0}", lc.DefaultProxyConfig.Error);
                    lc.IterateEnabled = false;
                    return;
                }
            }

            this.btnLogin.Text = "Logout";
        }

        private void Unregister()
        {
            if (!this.SipRegistered)
            {
                return;
            }
            // De-activate registering
            var defaultCfg = lc.DefaultProxyConfig;
            defaultCfg.Edit();
            defaultCfg.RegisterEnabled = false;
            defaultCfg.Done();
            while (lc.DefaultProxyConfig.State != Linphone.Core.RegistrationState.RegistrationCleared)
            {
                WriteLog("UnRegistering...");
            }            
            lc.IterateEnabled = false;
            lc.ClearProxyConfigs();

            WriteLog("UnRegistered");
            this.btnLogin.Text = "Login";
        }

        private void MakeCall()
        {
            if (!SipRegistered)
            {
                MessageBox.Show("You must register SIP before making a call");
            }

            string calleeUri = this.calleeAddress.Text;
            if (string.IsNullOrWhiteSpace(calleeUri))
                return;

            WriteLog("Calling to [{0}] ...", calleeUri);

            var callParams = lc.CreateDefaultCallParameters();
            //callParams.VideoEnabled = false;
            var call = lc.InviteWithParams(calleeUri, callParams);

            if (call.State != LinphoneCallState.Connected)
            {
                WriteLog("Waiting accept call...");
            }

            this.btnCall.Text = "Hangup";
        }

        private void AcceptCall(LinphoneCall call, string message)
        {
            this.lc.AcceptCall(call);
        }

        private void EndCall()
        {
            lc.TerminateCall(lc.CurrentCall);
            //this.btnCall.Text = "Call";
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            try
            {
                if (!SipRegistered)
                {
                    Register();
                }
                else
                {
                    Unregister();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnCall_Click(object sender, EventArgs e)
        {
            if (!SipRegistered) return;

            try
            {
                if (!lc.InCall)
                {
                    MakeCall();
                }
                else
                {
                    EndCall();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        #region ILinphoneCoreListener interface methods
        public void AuthInfoRequested(string realm, string username, string domain)
        {
            WriteLog("AuthInfoRequested");
        }

        public void CallEncryptionChanged(LinphoneCall call, bool encrypted, string authenticationToken)
        {
            WriteLog("CallEncryptionChanged");
        }

        public void CallState(LinphoneCall call, LinphoneCallState state, string message)
        {
            WriteLog("CallState: {0} | {1}", state, message);
            switch (state)
            {
                case LinphoneCallState.IncomingReceived:
                    this.UIThreadInvoke(() => AcceptCall(call, message));
                    break;
                case LinphoneCallState.OutgoingRinging:
                    this.UIThreadInvoke(() => WriteLog("Ringing"));
                    break;
                case LinphoneCallState.Connected:                    
                    this.UIThreadInvoke(() => WriteLog("Talking"));
                    this.UIThreadInvoke(() => this.btnCall.Text = "Hangup");
                    break;
                case LinphoneCallState.CallEnd:
                    this.UIThreadInvoke(() => WriteLog("Call ended"));
                    this.UIThreadInvoke(() => this.btnCall.Text = "Call" );
                    break;
                default:
                    break;
            }
        }

        public void CallStatsUpdated(LinphoneCall call, LinphoneCallStats stats)
        {
            //WriteLog("CallStatsUpdated: U:{0} | D:{1}", stats.UploadBandwidth, stats.DownloadBandwidth);
        }

        public void ComposingReceived(LinphoneChatRoom room)
        {
            WriteLog("ComposingReceived");
        }

        public void DTMFReceived(LinphoneCall call, char dtmf)
        {
            WriteLog("DTMFReceived");
        }

        public void EcCalibrationStatus(EcCalibratorStatus status, int delayMs)
        {
            WriteLog("EcCalibrationStatus");
        }

        public void FileTransferProgressIndication(LinphoneChatMessage message, int offset, int total)
        {
            WriteLog("FileTransferProgressIndication");
        }

        public void GlobalState(GlobalState state, string message)
        {
            WriteLog("GlobalState: {0} | {1}", state, message);
        }

        public void LogUploadProgressIndication(int offset, int total)
        {
            WriteLog("LogUploadProgressIndication");
        }

        public void LogUploadStatusChanged(LinphoneCoreLogCollectionUploadState state, string info)
        {
            WriteLog("LogUploadStatusChanged");
        }

        public void MessageReceived(LinphoneChatMessage message)
        {
            WriteLog("MessageReceived");
        }

        public void RegistrationState(LinphoneProxyConfig config, RegistrationState state, string message)
        {
            WriteLog("RegistrationState: {0} | ", state, message);
        }
        #endregion
    }
}
