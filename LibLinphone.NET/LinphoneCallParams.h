/*
LinphoneCallParams.h
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

#using <System.Drawing.dll>

#include "LinphoneCore.h"
#include "ApiLock.h"

namespace Linphone
{
	namespace Core
	{
		ref class PayloadType;
        ref class VideoSize;

		/// <summary>
		/// This object contains various call related parameters.
		/// It can be used to retrieve parameters from a currently running call or modify the call's caracteristics dynamically.
		/// </summary>
		public ref class LinphoneCallParams sealed
		{
		public:
            LinphoneCallParams(::LinphoneCallParams* params);
			/// <summary>
			/// Sets the audio bandwidth in kbits/s (0 to disable limitation).
			/// </summary>
			property int AudioBandwidthLimit
			{
				void set(int value);
			}

			/// <summary>
			/// Returns the MediaEncryption of the call (None, SRTP or ZRTP).
			/// </summary>
			property Linphone::Core::MediaEncryption MediaEncryption
			{
				Linphone::Core::MediaEncryption get();
				void set(Linphone::Core::MediaEncryption value);
			}

			/// <summary>
			/// Returns the PayloadType currently in use for the audio stream.
			/// </summary>
			property PayloadType^ UsedAudioCodec
			{
				PayloadType^ get();
			}

			/// <summary>
			/// Indicates low bandwidth mode.
			/// Configuring a call to low bandwidth mode will result in the core to activate several settings for the call in order to ensure that bitrate usage is lowered to the minimum possible.
			/// Tyically, ptime (packetization time) will be increased, audio codecs's output bitrate will be targetted to 20kbits/s provided that it is achievable by the codec selected after SDP handshake.
			/// Video is automatically disabled.
			/// </summary>
			property System::Boolean LowBandwidthEnabled
			{
				System::Boolean get();
				void set(System::Boolean value);
			}

			/// <summary>
			/// Enable or disable video.
			/// </summary>
			property System::Boolean VideoEnabled
			{
				System::Boolean get();
				void set(System::Boolean value);
			}

			/// <summary>
			/// Returns the PayloadType currently in use for the video stream.
			/// </summary>
			property PayloadType^ UsedVideoCodec
			{
				PayloadType^ get();
			}

			/// <summary>
			/// Returns the size of the video being sent.
			/// </summary>
			property VideoSize^ SentVideoSize
			{
                VideoSize^ get();
			}

			/// <summary>
			/// Returns the size of the video being received.
			/// </summary>
            property VideoSize^ ReceivedVideoSize
			{
                VideoSize^ get();
			}

			/// <summary>
			/// Set the audio stream direction.
			/// </summary>
			property MediaDirection AudioDirection
			{
				MediaDirection get();
				void set(MediaDirection value);
			}

			/// <summary>
			/// Set the video stream direction.
			/// </summary>
			property MediaDirection VideoDirection
			{
				MediaDirection get();
				void set(MediaDirection value);
			}

            /// <summary>
            /// Set a path to file where the call will be recorded. 
            /// Actual start of the recording is controlled by LinphoneCall.StartRecording().
            /// </summary>
            property System::String^ RecordFile
            {
                void set(System::String^ value);
            }

            /// <summary>
            /// Add a custom header to be used for the call for which these call params are used.
            /// </summary>
            /// <params name="name">header name</params>
            /// <params name="value">header value</params>
            void AddCustomHeader(System::String^ name, System::String^ value);

            /// <summary>
            /// Returns the value of a custom header given its name. If no header with that name exists, then null is returned.
            /// </summary>
            /// <params name="name">header name</params>
            /// <return>value for the header, or null if it doesn't exist.</return>
            System::String^ GetCustomHeader(System::String^ name);

            /// <summary>
            /// Get inner CallParams pointer
            /// </summary>
            ::LinphoneCallParams* GetCallParamsPtr();

		private:			
			~LinphoneCallParams();

			::LinphoneCallParams *params;
		};
	}
}