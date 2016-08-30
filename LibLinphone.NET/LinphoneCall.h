/*
LinphoneCall.h
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

#include "Enums.h"
#include "LinphoneCore.h"

namespace Linphone
{
	namespace Core
	{
		ref class LinphoneAddress;
		ref class LinphoneCallLog;
		ref class LinphoneCallStats;
		ref class LinphoneCallParams;
		ref class LinphoneCore;

		/// <summary>
		/// Object representing a call.
		/// Calls are create using LinphoneCore::Invite or passed to the application by the listener ILinphoneCoreListener::CallState.
		/// </summary>
		public ref class LinphoneCall sealed
		{
		public:
            LinphoneCall(::LinphoneCall *call);
            ~LinphoneCall();

			/// <summary>
			/// Gets the LinphoneCallState of the call (StreamRunning, IncomingReceived, OutgoingProgress, ...).
			/// </summary>
			property LinphoneCallState State
			{
				LinphoneCallState get();
			}

			/// <summary>
			/// Gets the remote LinphoneAddress.
			/// </summary>
			property LinphoneAddress^ RemoteAddress
			{
				LinphoneAddress^ get();
			}

			/// <summary>
			/// Returns the CallDirection (Outgoing or incoming).
			/// </summary>
			property CallDirection Direction
			{
				CallDirection get();
			}

			/// <summary>
			/// Gets the LinphoneCallLog associated with this call.
			/// </summary>
			property LinphoneCallLog^ CallLog
			{
				LinphoneCallLog^ get();
			}

			/// <summary>
			/// Gets the audio statistics associated with this call.
			/// </summary>
			property LinphoneCallStats^ AudioStats
			{
				LinphoneCallStats^ get();
			}

			/// <summary>
			/// Gets the call parameters given by the remote peer.
			/// This is useful for example to know if far end supports video or encryption.
			/// </summary>
			property LinphoneCallParams^ RemoteParams
			{
				LinphoneCallParams^ get();
			}

			/// <summary>
			/// Gets a copy of the current local call parameters.
			/// </summary>
			/// <returns>A copy of the current local call parameters</returns>
			LinphoneCallParams^ GetCurrentParamsCopy();

			/// <summary>
			/// Enable or disable the echo cancellation.
			/// </summary>
			property System::Boolean EchoCancellationEnabled
			{
				System::Boolean get();
				void set(System::Boolean value);
			}

			/// <summary>
			/// Enable or disable the echo limiter.
			/// </summary>
			property System::Boolean EchoLimiterEnabled
			{
				System::Boolean get();
				void set(System::Boolean value);
			}

			/// <summary>
			/// Gets the current duration of the call in seconds.
			/// </summary>
			property int Duration
			{
				int get();
			}

			/// <summary>
			/// Obtain real time quality rating of the call.
			/// Based on local RTP statistics and RTCP feedback, a quality rating is computed and updated during all the duration of the call.
			/// This function returns its value at the time of the function call.
			/// It is expected that the rating is updated at least every 5 seconds or so.
			/// The rating is a floating point number comprised between 0 and 5.
			/// 4-5 = good quality
			/// 3-4 = average quality
			/// 2-3 = poor quality
			/// 1-2 = very poor quality
			/// 0-1 = can't be worse, mostly unusable
			/// </summary>
			/// <returns>
			/// -1 if no quality mesurement is available, for example if no active audio stream exists. Otherwise returns the quality rating.
			/// </returns>
			property float CurrentQuality
			{
				float get();
			}

			/// <summary>
			/// Returns call quality averaged over all the duration of the call.
			/// See GetCurrentQuality for more details about quality mesurement.
			/// </summary>
			property float AverageQuality
			{
				float get();
			}

			/// <summary>
			/// Used by ZRTP encryption mechanism.
			/// </summary>
			/// <returns>SAS associated to the main stream [voice]</returns>
			property System::String^ AuthenticationToken
			{
				System::String^ get();
			}

			/// <summary>
			/// Used by ZRTP mechanism.
			/// SAS can verified manually by the user or automatically using a previously shared secret.
			/// </summary>
			property System::Boolean AuthenticationTokenVerified
			{
				System::Boolean get();
				void set(System::Boolean value);
			}

			/// <summary>
			/// Tells whether the call is in conference or not.
			/// </summary>
			property System::Boolean InConference
			{
				System::Boolean get();
			}

			/// <summary>
			/// Gets the measured sound volume played locally (received from remote).
			/// It is expressed in dbm0.
			/// </summary>
			property float PlayVolume
			{
				float get();
			}

			/// <summary>
			/// Gets the far end's user agent description string, if available.
			/// </summary>
			property System::String^ RemoteUserAgent
			{
				System::String^ get();
			}

			/// <summary>
			/// Gets the far end's sip contact as a string, if available.
			/// </summary>
			property System::String^ RemoteContact
			{
				System::String^ get();
			}

			/// <summary>
			/// Uses the CallContext object (native VoipPhoneCall) to get the DateTimeOffset at which the call started
			/// </summary>
			property System::Object^ CallStartTimeFromContext
			{
				System::Object^ get();
			}

			/// <summary>
			/// Tells whether video captured from the camera is sent to the remote party.
			/// </summary>
			property System::Boolean CameraEnabled
			{
				System::Boolean get();
				void set(System::Boolean value);
			}

			/// <summary>
			/// Gets the video statistics associated with this call.
			/// </summary>
			property LinphoneCallStats^ VideoStats
			{
				LinphoneCallStats^ get();
			}

			/// <summary>
			/// Requests remote side to send us a Video Fast Update.
			/// </summary>
			void SendVFURequest();

            // TODO: Must implement
			///// <summary>
			///// Gets the CallContext object (native VoipPhoneCall)
			///// </summary>
			//property Windows::Phone::Networking::Voip::VoipPhoneCall^ CallContext
   //         {
			//	Windows::Phone::Networking::Voip::VoipPhoneCall^ get();
			//	void set(Windows::Phone::Networking::Voip::VoipPhoneCall^ cc);
   //         }

			/// <summary>
			/// Gets the reason for a call termination (either error or normal termination)
			/// </summary>
			property Linphone::Core::Reason Reason
			{
				Linphone::Core::Reason get();
			}

            /// <summary>
            /// Get inner call pointer
            /// </summary>
            ::LinphoneCall* GetCallPtr();

		private:
            // TODO: Must implement 
			/*Windows::Phone::Networking::Voip::VoipPhoneCall^ callContext;*/
			::LinphoneCall *call;
		};
	}
}