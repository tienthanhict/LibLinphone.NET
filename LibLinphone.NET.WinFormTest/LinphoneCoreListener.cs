using System;
using Linphone.Core;

namespace LibLinphone.NET.WinFormTest
{
    public delegate void WriteLogDelegate(string format, params object[] args);

    /// <summary>
    /// Utility class to handle most of the LinphoneCore (and more globally the C++/CX API) methods calls.
    /// </summary>
    public sealed class LinphoneCoreListener : ILinphoneCoreListener
    {
        public event WriteLogDelegate WriteLog;

        private LinphoneCore _lc;
        public LinphoneCore Lc
        {
            get { return this._lc; }
            set { this._lc = value; }
        }
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
            if (state == LinphoneCallState.IncomingReceived)
            {
                this.Lc.AcceptCall(call);
            }
        }

        public void CallStatsUpdated(LinphoneCall call, LinphoneCallStats stats)
        {
            WriteLog("CallStatsUpdated: U:{0} | D:{1}", stats.UploadBandwidth, stats.DownloadBandwidth);
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
    }
}
