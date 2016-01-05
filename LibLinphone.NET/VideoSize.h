/*
VideoSize.h
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

#include "Utils.h"

namespace Linphone
{
    namespace Core
	{
		ref class LinphoneCoreFactory;
		ref class LinphoneCore;

		/// <summary>
		/// Class describing a video size.
		/// </summary>
		public ref class VideoSize sealed
		{
		public:
            VideoSize(int width, int height);
            VideoSize(int width, int height, System::String^ name);

			/// <summary>
			/// The video size width (eg. 640).
			/// </summary>
			property int Width
			{
				int get();
				void set(int value);
			}

			/// <summary>
			/// The video size height (eg. 480).
			/// </summary>
			property int Height
			{
				int get();
				void set(int value);
			}

			/// <summary>
			/// The video size name (eg. vga).
			/// </summary>
			property System::String^ Name
			{
				System::String^ get();
				void set(System::String^ value);
			}

		private:
			int width;
			int height;
			System::String^ name;
		};
	}
}