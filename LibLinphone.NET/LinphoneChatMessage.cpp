/*
LinphoneChatMessage.cpp
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

#include "LinphoneChatMessage.h"
#include "LinphoneAddress.h"
#include "ApiLock.h"

using namespace System;

String^ Linphone::Core::LinphoneChatMessage::Text::get()
{
	API_LOCK;
	return Linphone::Core::Utils::cctops(linphone_chat_message_get_text(this->message));
}

Linphone::Core::LinphoneAddress^ Linphone::Core::LinphoneChatMessage::PeerAddress::get()
{
	API_LOCK;
	return (Linphone::Core::LinphoneAddress^) Linphone::Core::Utils::CreateLinphoneAddress((void*)linphone_chat_message_get_peer_address(this->message));
}

Linphone::Core::LinphoneAddress^ Linphone::Core::LinphoneChatMessage::From::get()
{
	API_LOCK;
	return (Linphone::Core::LinphoneAddress^) Linphone::Core::Utils::CreateLinphoneAddress((void*)linphone_chat_message_get_from(this->message));
}

String^ Linphone::Core::LinphoneChatMessage::ExternalBodyUrl::get()
{
	API_LOCK;
	return Linphone::Core::Utils::cctops(linphone_chat_message_get_external_body_url(this->message));
}

void Linphone::Core::LinphoneChatMessage::ExternalBodyUrl::set(String^ url)
{
	API_LOCK;
	const char* body = Linphone::Core::Utils::pstoccs(url);
	linphone_chat_message_set_external_body_url(this->message, body);
	delete(body);
}

int64 Linphone::Core::LinphoneChatMessage::Time::get()
{
	API_LOCK;
	return linphone_chat_message_get_time(this->message);
}

Linphone::Core::LinphoneChatMessageState Linphone::Core::LinphoneChatMessage::State::get()
{
	API_LOCK;
	return (Linphone::Core::LinphoneChatMessageState) linphone_chat_message_get_state(this->message);
}

System::Boolean Linphone::Core::LinphoneChatMessage::IsOutgoing::get()
{
	API_LOCK;
	return (linphone_chat_message_is_outgoing(this->message) == TRUE);
}

System::Boolean Linphone::Core::LinphoneChatMessage::IsRead::get()
{
	API_LOCK;
	return (linphone_chat_message_is_read(this->message) == TRUE);
}

String^ Linphone::Core::LinphoneChatMessage::FileTransferName::get()
{
	API_LOCK;
	String^ fileName;
	const LinphoneContent *content = linphone_chat_message_get_file_transfer_information(this->message);
	if (content) 
	{
		fileName = Linphone::Core::Utils::cctops(linphone_content_get_name(content));
	}
	return fileName;
}

String^ Linphone::Core::LinphoneChatMessage::FileTransferFilePath::get()
{
	API_LOCK;
	return Linphone::Core::Utils::cctops(linphone_chat_message_get_file_transfer_filepath(this->message));
}

String^ Linphone::Core::LinphoneChatMessage::AppData::get()
{
	API_LOCK;
	return Linphone::Core::Utils::cctops(linphone_chat_message_get_appdata(this->message));
}

void Linphone::Core::LinphoneChatMessage::AppData::set(String^ appData)
{
	API_LOCK;
	linphone_chat_message_set_appdata(this->message, Linphone::Core::Utils::pstoccs(appData));
}

static void status_cb(::LinphoneChatMessage* msg, ::LinphoneChatMessageState state, void* ud)
{
	Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatMessageListener^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatMessageListener^> *>(ud);
	Linphone::Core::LinphoneChatMessageListener^ listener = (proxy) ? proxy->Ref() : nullptr;

	if (listener != nullptr) {
		Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatMessage^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatMessage^> *>(linphone_chat_message_get_user_data(msg));
		Linphone::Core::LinphoneChatMessage^ lChatMessage = (proxy) ? proxy->Ref() : nullptr;
		if (lChatMessage == nullptr) {
			lChatMessage = (Linphone::Core::LinphoneChatMessage^)Linphone::Core::Utils::CreateLinphoneChatMessage(msg);
		}

		listener->MessageStateChanged(lChatMessage, (Linphone::Core::LinphoneChatMessageState) state);
	}
}

void Linphone::Core::LinphoneChatMessage::StartFileDownload(Linphone::Core::LinphoneChatMessageListener^ listener, String^ filepath)
{
	API_LOCK;
	const char *cfilepath = Utils::pstoccs(filepath);
	linphone_chat_message_set_file_transfer_filepath(this->message, cfilepath);
    linphone_chat_message_download_file(this->message);
	delete(cfilepath);
}

::LinphoneChatMessage* Linphone::Core::LinphoneChatMessage::GetMessagePtr()
{
    return this->message;
}

Linphone::Core::LinphoneChatMessage::LinphoneChatMessage(::LinphoneChatMessage *cm) :
	message(cm)
{
	API_LOCK;
	message = linphone_chat_message_ref(message);
	RefToPtrProxy<LinphoneChatMessage^> *chat_message = new RefToPtrProxy<LinphoneChatMessage^>(this);
	linphone_chat_message_set_user_data(this->message, chat_message);
}

Linphone::Core::LinphoneChatMessage::~LinphoneChatMessage()
{
	API_LOCK;
	linphone_chat_message_unref(message);
	RefToPtrProxy<LinphoneChatMessage^> *chat_message = reinterpret_cast< RefToPtrProxy<LinphoneChatMessage^> *>(linphone_chat_message_get_user_data(this->message));
	delete chat_message;
}