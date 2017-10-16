/*
Globals.cpp
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

#include "Globals.h"
#include "ApiLock.h"
#include "BackgroundModeLogger.h"

using namespace Linphone::Core;

// Maximum number of characters required to contain the string version of an unsigned integer
#define MAX_CHARS_IN_UINT_AS_STRING ((sizeof(unsigned int) * 4) + 1)

//Globals^ Globals::singleton = nullptr;

Globals^ Globals::Instance::get()
{
    return Globals::singleton;
}

LinphoneCoreFactory^ Globals::LinphoneCoreFactory::get()
{
	if (this->linphoneCoreFactory == nullptr)
    {
        // Make sure only one API call is in progress at a time
		API_LOCK;

        if (this->linphoneCoreFactory == nullptr)
        {
            this->linphoneCoreFactory = gcnew Linphone::Core::LinphoneCoreFactory();
        }
    }

	return this->linphoneCoreFactory;
}

Linphone::Core::LinphoneCore^ Globals::LinphoneCore::get()
{
	return this->linphoneCoreFactory->LinphoneCore;
}

Linphone::Core::BackgroundModeLogger^ Globals::BackgroundModeLogger::get()
{
	if (this->backgroundModeLogger == nullptr)
	{
		// Make sure only one API call is in progress at a time
		API_LOCK;

		if (this->backgroundModeLogger == nullptr)
		{
			this->backgroundModeLogger = gcnew Linphone::Core::BackgroundModeLogger();
		}
	}

	return this->backgroundModeLogger;
}

Globals::Globals() :
    started(false),
	linphoneCoreFactory(nullptr),
	backgroundModeLogger(nullptr)
{

}

Globals::~Globals()
{
    // The destructor of this singleton object is called when the process is shutting down.
}
