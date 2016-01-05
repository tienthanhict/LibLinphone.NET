/*
Globals.h
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

namespace Linphone
{
    namespace Core
    {
		ref class LinphoneCoreFactory;
		ref class LinphoneCore;
		ref class BackgroundModeLogger;
        
		/// <summary>
		/// Singleton container used to hold global singletons and background process-wide objects.
		/// </summary>
        public ref class Globals sealed
        {
        public:
			/// <summary>
			/// Gets the instance of the Globals class, used to directly access any C++/CX objects from C#
			/// </summary>
            static property Globals^ Instance
            {
                Globals^ get();
            }

			/// <summary>
			/// Gets the current instance of LinphoneCoreFactory
			/// </summary>
			property LinphoneCoreFactory^ LinphoneCoreFactory
            {
                Linphone::Core::LinphoneCoreFactory^ get();
            }

			/// <summary>
			/// Gets the current instance of LinphoneCore from LinphoneCoreFactory
			/// </summary>
			property LinphoneCore^ LinphoneCore
            {
                Linphone::Core::LinphoneCore^ get();
            }

			/// <summary>
			/// Gets the current instance of the logger
			/// </summary>
			property BackgroundModeLogger^ BackgroundModeLogger
			{
				Linphone::Core::BackgroundModeLogger^ get();
			}


        private:
            Globals();
            ~Globals();

            static initonly Globals^ singleton = gcnew Globals();

            bool started;

			Linphone::Core::LinphoneCoreFactory^ linphoneCoreFactory;
			Linphone::Core::BackgroundModeLogger^ backgroundModeLogger;

        };
    }
}
