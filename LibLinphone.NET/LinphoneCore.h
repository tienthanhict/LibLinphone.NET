/*
LinphoneCore.h
Copyright (C) 2015  Belledonne Communications, Grenoble, France
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#pragma once
// Do not treat doxygen documentation as XML
#pragma warning(push)
#pragma warning(disable : 4635)

#include "linphone\linphonecore.h"
#include "linphone\linphonecore_utils.h"
//#include "linphone\linphone_tunnel.h"
#include "linphone\lpconfig.h"

#include "DataTypes.h"
#include "Enums.h"
#include "ILinphoneCoreListener.h"
#include "Utils.h"

#pragma warning(pop)

namespace Linphone
{
    namespace Core
    {
        ref class LinphoneCoreFactory;
        ref class LinphoneCore;
        ref class LinphoneProxyConfig;
        ref class LinphoneAuthInfo;
        ref class LinphoneAddress;
        ref class LinphoneCall;
        ref class LinphoneCallParams;
        ref class LinphoneCallLog;
        ref class LinphoneCallStats;
        ref class LinphoneChatRoom;
        ref class PayloadType;
        ref class LpConfig;
        ref class Transports;
        //ref class Tunnel;
        ref class VideoPolicy;
        ref class VideoSize;

        /// <summary>
        /// Main object created by LinphoneCoreFactory::CreateLinphoneCore.
        /// </summary>
        public ref class LinphoneCore sealed
        {
        public:
            LinphoneCore(ILinphoneCoreListener^ coreListener);
            LinphoneCore(ILinphoneCoreListener^ coreListener, LpConfig^ config);
            void Init();

            /// <summary>
            /// Sets the global log level.
            /// </summary>
            static property OutputTraceLevel LogLevel
            {
                OutputTraceLevel get();
                void set(OutputTraceLevel value);
            }

            /// <summary>
            /// Declares how many CPUs (cores) are available on the platform.
            /// </summary>
            static property int CPUCount
            {
                int get();
                void set(int value);
            }

            /// <summary>
            /// Resets the log collection by removing the log files.
            /// </summary>
            static void ResetLogCollection();

            /// <summary>
            /// Removes all the proxy configs from LinphoneCore.
            /// </summary>
            void ClearProxyConfigs();

            /// <summary>
            /// Adds a proxy config.
            /// This will start the registration of the proxy if registration is enabled.
            /// </summary>
            /// <param name="proxyCfg">The proxy config to be added</param>
            void AddProxyConfig(LinphoneProxyConfig^ proxyCfg);

            /// <summary>
            /// Sets the default proxy.
            /// This default proxy must be part of the list of already entered LinphoneProxyConfig.
            /// Toggling it as default will make LinphoneCore use this identity associated with the proxy config in all incoming and outgoing calls.
            /// </summary>
            property LinphoneProxyConfig^ DefaultProxyConfig
            {
                LinphoneProxyConfig^ get();
                void set(LinphoneProxyConfig^ value);
            }

            /// <summary>
            /// Creates an empty proxy config.
            /// </summary>
            /// <returns>An empty proxy config</returns>
            LinphoneProxyConfig^ CreateProxyConfig();

            /// <summary>
            /// Gets the list of the current proxy configs.
            /// </summary>
            property System::Collections::Generic::IList<System::Object^>^ ProxyConfigList
            {
                System::Collections::Generic::IList<System::Object^>^ get();
            }

            /// <summary>
            /// Removes all the auth infos from LinphoneCore.
            /// </summary>
            void ClearAuthInfos();

            /// <summary>
            /// Adds authentication information to the LinphoneCore.
            /// This information will be used during all SIP transactions which requieres authentication.
            /// </summary>
            /// <param name="info">The authentication information to be added</param>
            void AddAuthInfo(LinphoneAuthInfo^ info);

            /// <summary>
            /// Creates an empty auth info.
            /// </summary>
            /// <param name="username">The authentication username</param>
            /// <param name="userid">The authentication userid</param>
            /// <param name="password">The authentication password</param>
            /// <param name="ha1">The authentication ha1</param>
            /// <param name="realm">The authentication realm</param>
            /// <param name="domain">The authentication domain</param>
            LinphoneAuthInfo^ CreateAuthInfo(System::String^ username, System::String^ userid, System::String^ password, System::String^ ha1, System::String^ realm, System::String^ domain);

            /// <summary>
            /// Gets the current auth infos.
            /// </summary>
            /// <returns>The list of authentication informations</returns>
            property System::Collections::Generic::IList<System::Object^>^ AuthInfos
            {
                System::Collections::Generic::IList<System::Object^>^ get();
            }

            /// <summary>
            /// Destroys LinphoneCore and free all underlying resources.
            /// </summary>
            void Destroy();

            /// <summary>
            /// Builds an address according to the current proxy config.
            /// In case destination is not a sip address, the default proxy domain is automatically appended.
            /// </summary>
            /// <param name="destination">Either a SIP address or a username to which the default proxy domain will be appended</param>
            /// <returns>The created LinphoneAddress</returns>
            LinphoneAddress^ InterpretURL(System::String^ destination);

            /// <summary>
            /// Initiates an outgoing call given a destination as a System::String.
            /// Internally calls InterpretURL then Invite
            /// </summary>
            /// <param name="destination">Either a SIP address or a username to which the default proxy domain will be appended</param>
            /// <returns>The LinphoneCall that has just been initiated</returns>
            /// <seealso cref="InviteAddress(LinphoneAddress^)"/>
            /// <seealso cref="InviteAddressWithParams(LinphoneAddress^, LinphoneCallParams^)"/>
            LinphoneCall^ Invite(System::String^ destination);

            /// <summary>
            /// Initiates an outgoing call given a destination as a System::String.
            /// Internally calls InterpretURL then Invite
            /// </summary>
            /// <param name="destination">Either a SIP address or a username to which the default proxy domain will be appended</param>
            /// <param name="params">The LinphoneCallParams to be used</param>
            /// <returns>The LinphoneCall that has just been initiated</returns>
            /// <seealso cref="Invite(System::String^)"/>
            /// <seealso cref="InviteAddress(LinphoneAddress^)"/>
            /// <seealso cref="InviteAddressWithParams(LinphoneAddress^, LinphoneCallParams^)"/>
            LinphoneCall^ InviteWithParams(System::String^ destination, LinphoneCallParams^ params);

            /// <summary>
            /// Initiates an outgoing call given a destination LinphoneAddress.
            /// The LinphoneAddress can be constructed directly using LinphoneCoreFactory::CreateLinphoneAddress or InterpretURL.
            /// </summary>
            /// <param name="destination">The LinphoneAddress of the destination to call</param>
            /// <returns>The LinphoneCall that has just been initiated</returns>
            /// <seealso cref="Invite(System::String^)"/>
            /// <seealso cref="InviteAddressWithParams(LinphoneAddress^, LinphoneCallParams^)"/>
            LinphoneCall^ InviteAddress(LinphoneAddress^ destination);

            /// <summary>
            /// Initiates an outgoing call given a destination LinphoneAddress and the LinphoneCallParams to be used.
            /// The LinphoneAddress can be constructed directly using LinphoneCoreFactory::CreateLinphoneAddress or InterpretURL.
            /// </summary>
            /// <param name="destination">The LinphoneAddress of the destination to call</param>
            /// <param name="params">The LinphoneCallParams to be used</param>
            /// <returns>The LinphoneCall that has just been initiated</returns>
            /// <seealso cref="Invite(System::String^)"/>
            /// <seealso cref="InviteAddress(LinphoneAddress^)"/>
            LinphoneCall^ InviteAddressWithParams(LinphoneAddress^ destination, LinphoneCallParams^ params);

            /// <summary>
            /// Terminates the given call if running.
            /// </summary>
            /// <param name="call">The call to be terminated</param>
            void TerminateCall(LinphoneCall^ call);

            /// <summary>
            /// Declines an incoming call with a specific reason.
            /// </summary>
            void DeclineCall(LinphoneCall^ call, Reason reason);

            /// <summary>
            /// Gets the current active call.
            /// If there is only one ongoing call that is in the paused state, then there is no current call.
            /// </summary>
            property LinphoneCall^ CurrentCall
            {
                LinphoneCall^ get();
            }

            /// <summary>
            /// Tells whether there is at least one call running.
            /// </summary>
            property System::Boolean InCall
            {
                System::Boolean get();
            }

            /// <summary>
            /// Tells whether there is an incoming call invite pending.
            /// </summary>
            /// <returns>true if there is an incoming call invite pending, else returns false</returns>
            property System::Boolean IncomingInvitePending
            {
                System::Boolean get();
            }

            /// <summary>
            /// Accepts an incoming call.
            /// Basically the app is notified of incoming calls within the ILinphoneCoreListener::CallState listener method.
            /// The application can later respond positively to the call using this method.
            /// </summary>
            /// <param name="call">The incoming call to accept</param>
            /// <seealso cref="AcceptCallWithParams(LinphoneCall^, LinphoneCallParams^)"/>
            void AcceptCall(LinphoneCall^ call);

			/// <summary>
			/// Performs a simple call transfer to the specified destination.
			/// </summary>
			/// <param name="call">The incoming call to accept</param>
			/// <seealso cref="AcceptCallWithParams(LinphoneCall^, LinphoneCallParams^)"/>
			/// <param name="referTo">number call center</param>

			int TransferCall(LinphoneCall^ call, System::String^ referTo);

            /// <summary>
            /// Accepts an incoming call.
            /// Basically the app is notified of incoming calls within the ILinphoneCoreListener::CallState listener method.
            /// The application can later respond positively to the call using this method.
            /// </summary>
            /// <param name="call">The incoming call to accept</param>
            /// <param name="params">The LinphoneCallParams to use for the accepted call</param>
            /// <seealso cref="AcceptCall(LinphoneCall^)"/>
            void AcceptCallWithParams(LinphoneCall^ call, LinphoneCallParams^ params);

            /// <summary>
            /// Accepts call modifications initiated by other end.
            /// </summary>
            /// <param name="call">The incoming call to accept</param>
            /// <param name="params">The new local LinphoneCallParams to use</param>
            void AcceptCallUpdate(LinphoneCall^ call, LinphoneCallParams^ params);

            /// <summary>
            /// Prevent LinphoneCore from performing an automatic answer when receiving call modifications from the other end of the call.
            /// </summary>
            /// <param name="call">The call for which a modification from the other end has been notified</param>
            void DeferCallUpdate(LinphoneCall^ call);

            /// <summary>
            /// Updates the given call with the given params if the remote agrees.
            /// </summary>
            /// <param name="call">The call to update</param>
            /// <param name="params">The new LinphoneCallParams to propose to the remote peer</param>
            void UpdateCall(LinphoneCall^ call, LinphoneCallParams^ params);

            /// <summary>
            /// Gets a default set of LinphoneCallParams.
            /// </summary>
            /// <returns>The default set of LinphoneCallParams</returns>
            LinphoneCallParams^ CreateDefaultCallParameters();

            /// <summary>
            /// Gets the list of call logs of the LinphoneCore.
            /// </summary>
            /// <returns>A list of LinphoneCallLog objects as Object</returns>
            property System::Collections::Generic::IList<System::Object^>^ CallLogs
            {
                System::Collections::Generic::IList<System::Object^>^ get();
            }

            /// <summary>
            /// Removes all call logs from the LinphoneCore.
            /// </summary>
            void ClearCallLogs();

            /// <summary>
            /// Removes a specific log from the LinphoneCore.
            /// </summary>
            /// <param name="log">The call log to be removed</param>
            void RemoveCallLog(LinphoneCallLog^ log);

            /// <summary>
            /// This method is called by the application to notify the Linphone.Core library when network is reachable.
            /// Calling this method with true triggers Linphone to initiate a registration process for all proxy configs with parameter register set to enable.
            /// This method disables the automatic registration mode. It means you must call this method after each network state changes.
            /// </summary>
            property System::Boolean NetworkReachable
            {
                System::Boolean get();
                void set(System::Boolean value);
            }

            /// <summary>
            /// Sets the microphone gain in dB.
            /// </summary>
            property float MicGainDb
            {
                float get();
                void set(float value);
            }

            /// <summary>
            /// Allow to control play level before entering the sound card in dB.
            /// </summary>
            property float PlaybackGainDb
            {
                float get();
                void set(float value);
            }

            /// <summary>
            /// Sets the playback level in a scale from 0 to 100.
            /// </summary>
            property int PlayLevel
            {
                int get();
                void set(int value);
            }

            /// <summary>
            /// Mutes or unmutes the local microphone.
            /// </summary>
            property System::Boolean MicMuted
            {
                System::Boolean get();
                void set(System::Boolean value);
            }

            /// <summary>
            /// Sends a DTMF signal to the remote party if in call.
            /// Playing the DTMF locally is done with PlayDTMF(char16, int).
            /// </summary>
            /// <param name="number">The DTMF digit to be sent</param>
            /// <seealso cref="PlayDTMF(char16, int)"/>
            void SendDTMF(char16 number);

            /// <summary>
            /// Plays a DTMF signal to the speaker if not in call.
            /// Sending of the DTMF is done with SendDTMF(char16).
            /// </summary>
            /// <param name="number">The DTMF digit to be played</param>
            /// <param name="duration">The duration of the DTMF digit in ms, -1 for unlimited</param>
            /// <seealso cref="SendDTMF(char16)"/>
            void PlayDTMF(char16 number, int duration);

            /// <summary>
            /// Stops the current playing DTMF.
            /// </summary>
            void StopDTMF();

            /// <summary>
            /// Tries to return the PayloadType matching the given MIME type, clock rate and number of channels.
            /// </summary>
            /// <param name="mime">The MIME type to search a payload type for</param>
            /// <param name="clockRate">The clock rate to search a payload type for</param>
            /// <param name="channels">The number of channels to search a payload type for</param>
            /// <returns>The PayloadType matching the parameters or null if not found</returns>
            PayloadType^ FindPayloadType(System::String^ mime, int clockRate, int channels);

            /// <summary>
            /// Tries to return the PayloadType matching the given MIME type and clock rate.
            /// </summary>
            /// <param name="mime">The MIME type to search a payload type for</param>
            /// <param name="clockRate">The clock rate to search a payload type for</param>
            /// <returns>The PayloadType matching the parameters or null if not found</returns>
            PayloadType^ FindPayloadType(System::String^ mime, int clockRate);

            /// <summary>
            /// Tells whether a payload type is enabled or not.
            /// </summary>
            /// <param name="pt">The PayloadType that is to be checked</param>
            /// <returns>A System::Boolean value telling whether the PayloadType is enabled</returns>
            bool PayloadTypeEnabled(PayloadType^ pt);

            /// <summary>
            /// Enables or disables a payload type.
            /// The payload type to enable/disable can be retrieved using FindPayloadType(System::String, int, int).
            /// </summary>
            /// <param name="pt">The PayloadType to enable/disable</param>
            /// <param name="enable">A System::Boolean value telling whether to enable or disable the PayloadType</param>
            void EnablePayloadType(PayloadType^ pt, System::Boolean enable);

            /// <summary>
            /// Gets the currently supported audio codecs, as PayloadType elements.
            /// </summary>
            /// <returns>A list of PayloadType objects as Object</returns>
            property System::Collections::Generic::IList<System::Object^>^ AudioCodecs
            {
                System::Collections::Generic::IList<System::Object^>^ get();
            }

            /// <summary>
            /// Enables or disables the echo cancellation.
            /// </summary>
            property System::Boolean EchoCancellationEnabled
            {
                System::Boolean get();
                void set(System::Boolean value);
            }

            /// <summary>
            /// Starts an echo calibration of the sound devices, in order to find adequate settings for the echo canceller automatically.
            /// Status is notified to ILinphoneCoreListener::EcCalibrationStatus.
            /// </summary>
            void StartEchoCalibration();

            /// <summary>
            /// Enables or disables the echo limiter.
            /// </summary>
            property System::Boolean EchoLimiterEnabled
            {
                System::Boolean get();
                void set(System::Boolean value);
            }

            /// <summary>
            /// Sets the ports to be used for each transport (UDP, TCP, TLS).
            /// </summary>
            property Transports^ SipTransports
            {
                Transports^ get();
                void set(Transports^ value);
            }

            /// <summary>
            /// Enables or disables IPv6 support.
            /// </summary>
            property System::Boolean IPv6Enabled
            {
                System::Boolean get();
                void set(System::Boolean value);
            }

            /// <summary>
            /// Sets the user presence status.
            /// </summary>
            /// <param name="minuteAway">How long in away</param>
            /// <param name="alternativeContact">SIP URI to redirect call if the status is OnlineStatus.StatusMoved</param>
            /// <param name="status">The new presence status</param>
            void SetPresenceInfo(int minuteAway, System::String^ alternativeContact, OnlineStatus status);

            /// <summary>
            /// Specifies a STUN server to help firewall traversal, such as stun.linphone.org or stun.linphone.org:3478
            /// </summary>
            property System::String^ StunServer
            {
                System::String^ get();
                void set(System::String^ value);
            }

            /// <summary>
            /// Sets policy regarding workarounding NATs.
            /// </summary>
            property Linphone::Core::FirewallPolicy FirewallPolicy
            {
                Linphone::Core::FirewallPolicy get();
                void set(Linphone::Core::FirewallPolicy value);
            }

            /// <summary>
            /// Sets the file or folder containing trusted root CAs.
            /// </summary>
            property System::String^ RootCA
            {
                System::String^ get();
                void set(System::String^ value);
            }

            /// <summary>
            /// Sets maximum available upload bandwidth.
            /// This is IP bandwidth, in kbit/s (0 for infinite).
            /// This information is used by liblinphone together with remote side available bandwidth signaled in SDP messages
            /// to properly configure audio and video codec's output bitrate.
            /// </summary>
            property int UploadBandwidth
            {
                int get();
                void set(int value);
            }

            /// <summary>
            /// Maximum available download bandwidth.
            /// This is IP bandwidth, in kbit/s (0 for infinite).
            /// This information is used signaled to other parties during calls (within SDP messages) so that the remote end
            /// can have sufficient knowledge to properly configure its audio and video codec output bitrate to not overflow available bandwidth.
            /// </summary>
            property int DownloadBandwidth
            {
                int get();
                void set(int value);
            }

            /// <summary>
            /// Sets audio packetization interval suggested for remote end (in milliseconds).
            /// A value of zero means that ptime is not specified.
            /// </summary>
            property int DownloadPTime
            {
                int get();
                void set(int value);
            }

            /// <summary>
            /// Sets audio packetization interval sent to remote end (in milliseconds).
            /// A value of zero means that ptime is not specified.
            /// </summary>
            property int UploadPTime
            {
                int get();
                void set(int value);
            }

            /// <summary>
            /// Enables signaling keep alive. Small UDP packet sent periodically to keep UDP NAT association.
            /// </summary>
            property System::Boolean KeepAliveEnabled
            {
                System::Boolean get();
                void set(System::Boolean value);
            }

            /// <summary>
            /// Sets the file to be played when putting a call on hold.
            /// </summary>
            property System::String^ PlayFile
            {
                void set(System::String^ value);
            }

            /// <summary>
            /// Pauses a currently active call.
            /// </summary>
            /// <param name="call">The call to be paused</param>
            /// <returns>A System::Boolean value telling whether the call has successfully been paused</returns>
            System::Boolean PauseCall(LinphoneCall^ call);

            /// <summary>
            /// Resumes a currently paused call.
            /// </summary>
            /// <param name="call">The call to bbe resumed</param>
            /// <returns>A System::Boolean value telling whether the call has successfully been resumed</returns>
            System::Boolean ResumeCall(LinphoneCall^ call);

            /// <summary>
            /// Pauses all the calls.
            /// </summary>
            /// <returns>A System::Boolean value telling whether the calls have successfully been paused</returns>
            System::Boolean PauseAllCalls();

            /// <summary>
            /// Tells whether a conference is currently ongoing.
            /// </summary>
            property System::Boolean InConference
            {
                System::Boolean get();
            }

            /// <summary>
            /// Makes the local participant to join the conference. 
            /// Typically, the local participant is by default always part of the conference when joining an active call into a conference.
            /// However, by calling LeaveConference() and EnterConference() the application can decide to temporarily
            /// move out and in the local participant from the conference.
            /// </summary>
            /// <returns>A System::Boolean value telling whether the conference has successfully been entered</returns>
            System::Boolean EnterConference();

            /// <summary>
            /// Moves the local participant out of the conference.
            /// When the local participant is out of the conference, the remote participants can continue to talk normally.
            /// </summary>
            void LeaveConference();

            /// <summary>
            /// Merges a call into a conference.
            /// If this is the first call that enters the conference, the virtual conference will be created automatically.
            /// If the local user was actively part of the call (ie not in paused state), then the local user is automatically entered into the conference.
            /// If the call was in paused state, then it is automatically resumed when entering into the conference.
            /// </summary>
            /// <param name="call">An established call, either in StreamsRunning or Paused state</param>
            void AddToConference(LinphoneCall^ call);

            /// <summary>
            /// Adds all calls into a conference.
            /// Merge all established calls (either in StreamsRunning or Paused) into a conference.
            /// </summary>
            void AddAllToConference();

            /// <summary>
            /// Removes a call from the conference.
            /// After removing the remote participant belonging to the supplied call, the call becomes a normal call in paused state.
            /// If one single remote participant is left alone together with the local user in the conference after the removal, then the conference is
            /// automatically transformed into a simple call in StreamsRunning state.
            /// The conference's resources are then automatically destroyed.
            /// In other words, unless LeaveConference() is explicitely called, the last remote participant of a conference is automatically
            /// put in a simple call in running state.
            /// </summary>
            /// <param name="call">A call that has been previously merged into the conference.</param>
            void RemoveFromConference(LinphoneCall^ call);

            /// <summary>
            /// Terminates the conference and the calls associated with it.
            /// All the calls that were merged to the conference are terminated, and the conference resources are destroyed.
            /// </summary>
            void TerminateConference();

            /// <summary>
            /// Returns the number of participants to the conference, including the local participant.
            /// Typically, after merging two calls into the conference, there is total of 3 participants:
            /// the local participant (or local user), and two remote participants that were the destinations of the two previously establised calls.
            /// </summary>
            property int ConferenceSize
            {
                int get();
            }

            /// <summary>
            /// Terminates all the calls.
            /// </summary>
            void TerminateAllCalls();

            /// <summary>
            /// Gets the current list of calls.
            /// </summary>
            property System::Collections::Generic::IList<System::Object^>^ Calls
            {
                System::Collections::Generic::IList<System::Object^>^ get();
            }

            /// <summary>
            /// Gets the number of calls.
            /// </summary>
            property int CallsNb
            {
                int get();
            }

            /// <summary>
            /// Searches from the list of current calls if a remote address match uri.
            /// </summary>
            /// <param name="uri">URI to search for in the list of current calls</param>
            /// <returns>The matching LinphoneCall if found, null otherwise</returns>
            LinphoneCall^ FindCallFromUri(System::String^ uri);

            /// <summary>
            /// Sets the maximum number of simultaneous calls the LinphoneCore can manage at a time. All new call above this limit are declined with a busy answer.
            /// </summary>
            property int MaxCalls
            {
                int get();
                void set(int value);
            }

            /// <summary>
            /// Tells whether a URI corresponds to my identity.
            /// </summary>
            /// <returns>true if the URI corresponds to my identity, false otherwise</returns>
            System::Boolean IsMyself(System::String^ uri);

            /// <summary>
            /// Use this method to check calls state and forbid proposing actions which could result in an active call.
            /// Eg: don't start a new call if one is in outgoing ringing.
            /// Eg: don't merge to conference either as it could result in two active calls (conference and accepted call).
            /// </summary>
            property System::Boolean SoundResourcesLocked
            {
                System::Boolean get();
            }

            /// <summary>
            /// Tells whether a media encryption scheme is supported by the LinphoneCore engine.
            /// </summary>
            /// <param name="menc">The media encryption to check</param>
            /// <returns>true if supported, false otherwise</returns>
            System::Boolean IsMediaEncryptionSupported(Linphone::Core::MediaEncryption menc);

            /// <summary>
            /// Chooses the media encryption policy to be used for RTP packets.
            /// </summary>
            property Linphone::Core::MediaEncryption MediaEncryption
            {
                Linphone::Core::MediaEncryption get();
                void set(Linphone::Core::MediaEncryption value);
            }

            /// <summary>
            /// Defines Linphone behaviour when encryption parameters negociation fails on outgoing call.
            /// </summary>
            property System::Boolean MediaEncryptionMandatory
            {
                System::Boolean get();
                void set(System::Boolean value);
            }

            /// <summary>
            /// Tells whether tunnel support is available.
            /// </summary>
            property System::Boolean TunnelAvailable
            {
                System::Boolean get();
            }

            ///// <summary>
            ///// Gets the tunnel instance if available.
            ///// </summary>
            //property Linphone::Core::Tunnel^ Tunnel
            //{
            //    Linphone::Core::Tunnel^ get();
            //}

            /// <summary>
            /// Sets the user agent System::String used in SIP messages.
            /// </summary>
            /// <param name="name">The user agent name to set</param>
            /// <param name="version">The user agent version to set</param>
            void SetUserAgent(System::String^ name, System::String^ version);

            /// <summary>
            /// Gets the number of missed calls since last reset.
            /// </summary>
            property int MissedCallsCount
            {
                int get();
            }

            /// <summary>
            /// Reset the count of missed calls.
            /// </summary>
            void ResetMissedCallsCount();

            /// <summary>
            /// Re-initiates registration if network is up.
            /// </summary>
            void RefreshRegisters();

            /// <summary>
            /// Gets liblinphone's version as a System::String.
            /// </summary>
            property System::String^ Version
            {
                System::String^ get();
            }

            /// <summary>
            /// Sets the UDP port used for audio streaming.
            /// </summary>
            property int AudioPort
            {
                int get();
                void set(int value);
            }

            /// <summary>
            /// Sets the UDP port range from which to randomly select the port used for audio streaming.
            /// </summary>
            /// <param name="minP">The lower value of the UDP port range</param>
            /// <param name="maxP">The upper value of the UDP port range</param>
            void SetAudioPortRange(int minP, int maxP);

            /// <summary>
            /// Sets the incoming call timeout in seconds.
            /// If an incoming call isn't answered for this timeout period, it is automatically declined.
            /// </summary>
            property int IncTimeout
            {
                int get();
                void set(int value);
            }

            /// <summary>
            /// Sets the call timeout in seconds.
            /// Once this time is elapsed (ringing included), the call is automatically hung up.
            /// </summary>
            property int InCallTimeout
            {
                int get();
                void set(int value);
            }

            /// <summary>
            /// Set username and display name to use if no LinphoneProxyConfig is configured.
            /// </summary>
            /// <param name="displayName">The display name to use</param>
            /// <param name="userName">The username to use</param>
            void SetPrimaryContact(System::String^ displayName, System::String^ userName);

            /// <summary>
            /// Enables/Disables the use of SIP INFO to send DTMFs.
            /// </summary>
            property System::Boolean UseInfoForDtmf
            {
                System::Boolean get();
                void set(System::Boolean value);
            }

            /// <summary>
            /// Enables/Disables the use of RFC2833 to send DTMFs.
            /// </summary>
            property System::Boolean UseRfc2833ForDtmf
            {
                System::Boolean get();
                void set(System::Boolean value);
            }

            /// <summary>
            /// Gets the LpConfig object to read/write to the config file: useful if you wish to extend the config file with your own sections.
            /// </summary>
            property Linphone::Core::LpConfig^ Config
            {
                Linphone::Core::LpConfig^ get();
            }

            /// <summary>
            /// Tells whether video support has been compiled.
            /// </summary>
            property System::Boolean VideoSupported
            {
                System::Boolean get();
            }

            /// <summary>
            /// Sets the policy for video.
            /// </summary>
            property Linphone::Core::VideoPolicy^ VideoPolicy
            {
                Linphone::Core::VideoPolicy^ get();
                void set(Linphone::Core::VideoPolicy^ value);
            }

            /// <summary>
            /// Gets the list of video sizes that are supported.
            /// </summary>
            property System::Collections::Generic::IList<System::Object^>^ SupportedVideoSizes
            {
                System::Collections::Generic::IList<System::Object^>^ get();
            }

            /// <summary>
            /// Gets the preferred video size name.
            /// </summary>
            /// <returns>The preferred video size name</returns>
            System::String^ GetPreferredVideoSizeName();

            /// <summary>
            /// Sets the preferred video size.
            /// This applies only to the stream that is captured and sent to the remote party,
            /// since we accept all standard video size on the receive path.
            /// </summary>
            property Linphone::Core::VideoSize^ PreferredVideoSize
            {
                Linphone::Core::VideoSize^ get();
                void set(Linphone::Core::VideoSize^ value);
            }

            /// <summary>
            /// Sets the preferred video size.
            /// This applies only to the stream that is captured and sent to the remote party,
            /// since we accept all standard video size on the receive path.
            /// </summary>
            /// <param name="width">The width of the preferred video size</param>
            /// <param name="height">The height of the preferred video size</param>
            void SetPreferredVideoSize(int width, int height);

            /// <summary>
            /// Sets the preferred video size by telling its name.
            /// This applies only to the stream that is captured and sent to the remote party,
            /// since we accept all standard video size on the receive path.
            /// </summary>
            /// <param name="sizeName">The name of the preferred video size (eg. "vga")</param>
            void SetPreferredVideoSizeByName(System::String^ sizeName);

			/// <summary>
			/// Reload the list of video devices.
			/// </summary>
			void ReloadVideoDevices();

			/// <summary>
			/// Gets the list of video devices.
			/// </summary>
			property System::Collections::Generic::IList<System::Object^>^ VideoDevices
			{
				System::Collections::Generic::IList<System::Object^>^ get();
			}

            /// <summary>
            /// Sets the active video device.
            /// </summary>
            property System::String^ VideoDevice
            {
                System::String^ get();
                void set(System::String^ value);
            }

			/// <summary>
			/// Reload the list of sound devices.
			/// </summary>
			void ReloadSoundDevices();

			/// <summary>
			/// Returns true if the specified sound device can capture sound.
			/// </summary>
			bool SoundDeviceCanCapture(System::String^ soundDeviceId);

			/// <summary>
			/// Gets the list of sound devices.
			/// </summary>
			property System::Collections::Generic::IList<System::Object^>^ SoundDevices
			{
				System::Collections::Generic::IList<System::Object^>^ get();
			}

			/// <summary>
			/// Sets the active sound capture device.
			/// </summary>
			property System::String^ SoundCaptureDevice
			{
				System::String^ get();
				void set(System::String^ value);
			}

            /// <summary>
            /// Gets the currently supported video codecs, as PayloadType elements.
            /// </summary>
            property System::Collections::Generic::IList<System::Object^>^ VideoCodecs
            {
                System::Collections::Generic::IList<System::Object^>^ get();
            }

            /// <summary>
            /// Deprecated! 
            /// Tells whether video is enabled or not.
            /// </summary>
            property System::Boolean VideoEnabled
            {
                System::Boolean get();
            }

            /// <summary>
            /// Deprecated!
            /// Enables video.
            /// This method does not have any effect during calls. It just indicates LinphoneCore to
            /// initiate future calls with video or not. The two System::Boolean parameters indicate in which
            /// direction video is enabled. Setting both to false disables video entirely.
            /// </summary>
            /// <param name="enableCapture">Indicates whether video capture is enabled</param>
            /// <param name="enableDisplay">Indicates whether video display should be shown</param>
            void EnableVideo(System::Boolean enableCapture, System::Boolean enableDisplay);

            /// <summary>
            /// Enable or disable video capture.
            ///This function does not have any effect during calls. It just indicates the LinphoneCore to initiate future calls with video capture or not.
            /// </summary>
            property System::Boolean VideoCaptureEnabled
            {
                System::Boolean get();
                void set(System::Boolean value);
            }

            /// <summary>
            /// Enable or disable video display.
            /// This function does not have any effect during calls. It just indicates the LinphoneCore to initiate future calls with video display or not.
            /// </summary>
            property System::Boolean VideoDisplayEnabled
            {
                System::Boolean get();
                void set(System::Boolean value);
            }

            /// <summary>
            /// Gets the native video window id.
            /// </summary>
            property ulong NativeVideoWindowId
            {
                ulong get();
                void set(ulong windowId);
            }

			/// <summary>
			/// Gets the preview native video window id.
			/// </summary>
			property ulong NativePreviewVideoWindowId
			{
				ulong get();
				void set(ulong windowId);
			}

            /// <summary>
            /// Gets the camera sensor rotation in degrees.
            /// </summary>
            property int CameraSensorRotation
            {
                int get();
            }

            /// <summary>
            /// Enables or disables self view during calls.
            /// Self-view refers to having local webcam image inserted in corner of the video window during calls.
            /// </summary>
            property System::Boolean SelfViewEnabled
            {
                System::Boolean get();
                void set(System::Boolean value);
            }

			/// <summary>
			/// Controls video preview enablement.
			/// Video preview refers to the action of displaying the local webcam image to the user while not in call.
			/// </summary>
			property System::Boolean VideoPreviewEnabled
			{
				System::Boolean get();
				void set(System::Boolean value);
			}

            /// <summary>
            /// Get a chat room whose peer is the supplied address. If it does not exist yet, it will be created.
            /// </summary>
            /// <param name="address">A LinphoneAddress</param>
            Linphone::Core::LinphoneChatRoom^ GetChatRoom(Linphone::Core::LinphoneAddress^ address);

            /// <summary>
            /// Get a chat room for messaging from a sip uri like sip:joe@sip.linphone.org. If it does not exist yet, it will be created.
            /// </summary>
            /// <param name="to">The destination address for messages</param>
            Linphone::Core::LinphoneChatRoom^ GetChatRoomFromUri(System::String^ to);

            /// <summary>
            /// Sets the log collection upload server URL.
            /// </summary>
            property System::String^ LogCollectionUploadServerUrl
            {
                void set(System::String^ value);
            }

            /// <summary>
            /// Starts the upload of the log collection.
            /// </summary>
            void UploadLogCollection();

            /// <summary>
            /// Tells the core the device current orientation. This can be used by capture filters
            /// on mobile devices to select between portrait / landscape mode and to produce properly
            ///	oriented images.The exact meaning of the value in rotation if left to each device
            ///	specific implementations.
            /// </summary>
            property int DeviceRotation
            {
                int get();
                void set(int value);
            }

            /// <summary>
            /// Notifies the system that the call needs to be muted/unmuted.
            /// </summary>
            /// <param name="isMuted">The new mute state</param>
            void NotifyMute(bool isMuted);

            /// <summary>
            /// Sets the path to the database file used to store chat messages
            /// </summary>
            property System::String^ ChatDatabasePath
            {
                void set(System::String^ value);
            }

            /// <summary>
            /// Gets the list of the created chatrooms
            /// </summary>
            property System::Collections::Generic::IList<System::Object^>^ ChatRooms
            {
                System::Collections::Generic::IList<System::Object^>^ get();
            }

            /// <summary>
            /// The ILinphoneCoreListener that handles the events coming from the core.
            /// </summary>
            property ILinphoneCoreListener^ CoreListener
            {
                ILinphoneCoreListener^ get();
                void set(ILinphoneCoreListener^ listener);
            }

            /// <summary>
            /// Set it to true to start the iterate, set it to false to stop it. 
            /// Is disabled by default.
            /// </summary>
            property System::Boolean IterateEnabled
            {
                System::Boolean get();
                void set(System::Boolean value);
            }
			/// <summary>
			/// Get playback sound level in 0-100 scale.
			/// </summary>
			property int SoundPlayback
			{
				int get();
				void set(int value);
			}

			/// <summary>
			/// Sets the sound device used for playback.
			/// </summary>
			property System::String^ SoundPlaybackDevice
			{
				System::String^ get();
				void set(System::String^ value);
			}



        private:
            ~LinphoneCore();
            void IterateThreadProc();

            ::LinphoneCore *lc;
            ILinphoneCoreListener^ listener;
            LpConfig^ config;
            //Windows::Foundation::IAsyncAction^ IterateWorkItem;
            System::Threading::Thread^ IterateWorkItem;
            System::Threading::ManualResetEvent^ IterateWorkEvent = gcnew System::Threading::ManualResetEvent(false);
            volatile System::Boolean stopIterate = false;
            System::Boolean isIterateEnabled;
            static OutputTraceLevel logLevel = OutputTraceLevel::Error;
        };
    }
}


