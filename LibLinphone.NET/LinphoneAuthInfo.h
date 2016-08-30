/*
LinphoneAuthInfo.h
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

#include "LinphoneCore.h"
#include "ApiLock.h"

namespace Linphone
{
	namespace Core
	{
		/// <summary>
		/// Object holding authentication information.
		/// In most cases, authentication information consists in an username and a password.
		/// Sometimes, a userid is required by the proxy, and realm can be useful to discriminate different SIP domains.
		/// This object is instanciated using LinphoneCoreFactory::CreateAuthInfo.
		/// Once created and filled, a LinphoneAuthInfo must be added to the LinphoneCore in order to become known and used automatically when needed.
		/// The LinphoneCore object can take the initiative to request authentication information when needed in the application through the ILinphoneCoreListener::AuthInfoRequested listener.
		/// The application can respond to this information request later using LinphoneCore::AddAuthInfo.
		/// This will unblock all pending authentication transactions and retry them with authentication headers.
		/// </summary>
		public ref class LinphoneAuthInfo sealed
		{
		public:
            LinphoneAuthInfo(System::String^ username, System::String^ userid, System::String^ password, System::String^ ha1, System::String^ realm, System::String^ domain);
            LinphoneAuthInfo(::LinphoneAuthInfo *auth_info);

			/// <summary>
			/// The authentication username.
			/// </summary>
			property System::String^ Username
			{
				System::String^ get();
				void set(System::String^ value);
			}

			/// <summary>
			/// The authentication userid.
			/// </summary>
			property System::String^ UserId
			{
				System::String^ get();
				void set(System::String^ value);
			}

			/// <summary>
			/// The authentication password.
			/// </summary>
			property System::String^ Password
			{
				System::String^ get();
				void set(System::String^ value);
			}

			/// <summary>
			/// The authentication realm.
			/// </summary>
			property System::String^ Realm
			{
				System::String^ get();
				void set(System::String^ value);
			}

            /// <summary>
            /// The authentication domain.
            /// </summary>
            property System::String^ Domain
            {
                System::String^ get();
                void set(System::String^ value);
            }

			/// <summary>
			/// The authentication ha1.
			/// </summary>
			property System::String^ Ha1
			{
				System::String^ get();
				void set(System::String^ value);
			}

            /// <summary>
            /// Get inner authentication info pointer
            /// </summary>
            ::LinphoneAuthInfo* GetAuthInfoPtr();

		private:
			~LinphoneAuthInfo();

			::LinphoneAuthInfo *auth_info;
		};
	}
}