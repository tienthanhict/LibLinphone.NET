using Linphone.Core;
using System;

namespace TestLibLinphoneCLIConsole
{
    /// <summary>
    /// Utility class to handle most of the LinphoneCore (and more globally the C++/CX API) methods calls.
    /// </summary>
    public sealed class LinphoneManager : LinphoneCoreListener
    {
        private LinphoneCore _lc;
        public LinphoneCore Lc
        {
            get { return this._lc; }
            set { this._lc = value; }
        }
        public void AuthInfoRequested(string realm, string username, string domain)
        {
            Console.WriteLine("AuthInfoRequested");
        }

        public void CallEncryptionChanged(LinphoneCall call, bool encrypted, string authenticationToken)
        {
            Console.WriteLine("CallEncryptionChanged");
        }

        public void CallState(LinphoneCall call, LinphoneCallState state, string message)
        {
            Console.WriteLine("CallState: {0} | {1}", state, message);
            if (state == LinphoneCallState.IncomingReceived)
            {
                this.Lc.AcceptCall(call);
            }
        }

        public void CallStatsUpdated(LinphoneCall call, LinphoneCallStats stats)
        {
            Console.WriteLine("CallStatsUpdated: U:{0} | D:{1}", stats.UploadBandwidth, stats.DownloadBandwidth);
        }

        public void ComposingReceived(LinphoneChatRoom room)
        {
            Console.WriteLine("ComposingReceived");
        }

        public void DTMFReceived(LinphoneCall call, char dtmf)
        {
            Console.WriteLine("DTMFReceived");
        }

        public void EcCalibrationStatus(EcCalibratorStatus status, int delayMs)
        {
            Console.WriteLine("EcCalibrationStatus");
        }

        public void FileTransferProgressIndication(LinphoneChatMessage message, int offset, int total)
        {
            Console.WriteLine("FileTransferProgressIndication");
        }

        public void GlobalState(GlobalState state, string message)
        {
            Console.WriteLine("GlobalState: {0} | {1}", state, message);
        }

        public void LogUploadProgressIndication(int offset, int total)
        {
            Console.WriteLine("LogUploadProgressIndication");
        }

        public void LogUploadStatusChanged(LinphoneCoreLogCollectionUploadState state, string info)
        {
            Console.WriteLine("LogUploadStatusChanged");
        }

        public void MessageReceived(LinphoneChatMessage message)
        {
            Console.WriteLine("MessageReceived");
        }

        public void RegistrationState(LinphoneProxyConfig config, RegistrationState state, string message)
        {
            Console.WriteLine("RegistrationState: {0} | ", state, message);
        }
    }
}
