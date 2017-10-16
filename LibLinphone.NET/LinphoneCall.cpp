/*
LinphoneCall.cpp
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
#include "LinphoneCall.h"
#include "LinphoneAddress.h"
#include "LinphoneCallStats.h"
#include "LinphoneCallLog.h"
#include "LinphoneCallParams.h"
#include "ApiLock.h"
#include "LinphoneCoreFactory.h"

using namespace System;

Linphone::Core::LinphoneCallState Linphone::Core::LinphoneCall::State::get()
{
	API_LOCK;
	return (Linphone::Core::LinphoneCallState)linphone_call_get_state(this->call);
}

Linphone::Core::LinphoneAddress^ Linphone::Core::LinphoneCall::RemoteAddress::get()
{
	API_LOCK;
	const ::LinphoneAddress *addr = linphone_call_get_remote_address(this->call);
	return (Linphone::Core::LinphoneAddress^)Linphone::Core::Utils::CreateLinphoneAddress((void*)addr);
}

Linphone::Core::CallDirection Linphone::Core::LinphoneCall::Direction::get()
{
	API_LOCK;
	return (Linphone::Core::CallDirection)linphone_call_get_dir(this->call);
}

Linphone::Core::LinphoneCallLog^ Linphone::Core::LinphoneCall::CallLog::get()
{
	API_LOCK;
	return (Linphone::Core::LinphoneCallLog^) Linphone::Core::Utils::CreateLinphoneCallLog(linphone_call_get_call_log(this->call));
}

Linphone::Core::LinphoneCallStats^ Linphone::Core::LinphoneCall::AudioStats::get()
{
	API_LOCK;
	return (Linphone::Core::LinphoneCallStats^) Linphone::Core::Utils::CreateLinphoneCallStats(this->call, (int)Linphone::Core::MediaType::Audio);
}

Linphone::Core::LinphoneCallParams^ Linphone::Core::LinphoneCall::RemoteParams::get()
{
	API_LOCK;
	Linphone::Core::LinphoneCallParams^ params = nullptr;
	if (linphone_call_get_remote_params(this->call) != nullptr) {
		params = (Linphone::Core::LinphoneCallParams^) Linphone::Core::Utils::CreateLinphoneCallParams(linphone_call_params_copy(linphone_call_get_remote_params(this->call)));
	}
	return params;
}

Linphone::Core::LinphoneCallParams^ Linphone::Core::LinphoneCall::GetCurrentParamsCopy()
{
	API_LOCK;
	return (Linphone::Core::LinphoneCallParams^) Linphone::Core::Utils::CreateLinphoneCallParams(linphone_call_params_copy(linphone_call_get_current_params(this->call)));
}

void Linphone::Core::LinphoneCall::EchoCancellationEnabled::set(System::Boolean enable)
{
	API_LOCK;
	linphone_call_enable_echo_cancellation(this->call, enable);
}

System::Boolean Linphone::Core::LinphoneCall::EchoCancellationEnabled::get()
{
	API_LOCK;
	return (linphone_call_echo_cancellation_enabled(this->call) == TRUE);
}

void Linphone::Core::LinphoneCall::EchoLimiterEnabled::set(System::Boolean enable)
{
	API_LOCK;
	linphone_call_enable_echo_limiter(this->call, enable);
}

System::Boolean Linphone::Core::LinphoneCall::EchoLimiterEnabled::get()
{
	API_LOCK;
	return (linphone_call_echo_limiter_enabled(this->call) == TRUE);
}

int Linphone::Core::LinphoneCall::Duration::get()
{
	API_LOCK;
	return linphone_call_get_duration(this->call);
}

float Linphone::Core::LinphoneCall::CurrentQuality::get()
{
	API_LOCK;
	return linphone_call_get_current_quality(this->call);
}

float Linphone::Core::LinphoneCall::AverageQuality::get()
{
	API_LOCK;
	return linphone_call_get_average_quality(this->call);
}

String^ Linphone::Core::LinphoneCall::AuthenticationToken::get()
{
	API_LOCK;
	return Linphone::Core::Utils::cctops(linphone_call_get_authentication_token(this->call));
}

System::Boolean Linphone::Core::LinphoneCall::AuthenticationTokenVerified::get()
{
	API_LOCK;
	return (linphone_call_get_authentication_token_verified(this->call) == TRUE);
}

void Linphone::Core::LinphoneCall::AuthenticationTokenVerified::set(System::Boolean verified)
{
	API_LOCK;
	linphone_call_set_authentication_token_verified(this->call, verified);
}

System::Boolean Linphone::Core::LinphoneCall::InConference::get()
{
	API_LOCK;
	return (linphone_call_get_conference(this->call) != NULL);
}

float Linphone::Core::LinphoneCall::PlayVolume::get()
{
	API_LOCK;
	return linphone_call_get_play_volume(this->call);
}

String^ Linphone::Core::LinphoneCall::RemoteUserAgent::get()
{
	API_LOCK;
	return Linphone::Core::Utils::cctops(linphone_call_get_remote_user_agent(this->call));
}

String^ Linphone::Core::LinphoneCall::RemoteContact::get()
{
	API_LOCK;
	return Linphone::Core::Utils::cctops(linphone_call_get_remote_contact(this->call));
}

// TODO: Must implement  
//void Linphone::Core::LinphoneCall::CallContext::set(Windows::Phone::Networking::Voip::VoipPhoneCall^ cc)
//{
//	API_LOCK;
//	this->callContext = cc;
//}

System::Object^ Linphone::Core::LinphoneCall::CallStartTimeFromContext::get()
{
	API_LOCK;
	System::Object^ result = nullptr;
    // TODO: Must implement    
	//try {
	//	if (this->callContext != nullptr) {
	//		result = this->callContext->StartTime;
	//	}
	//}
	//catch (Platform::COMException^ ex) {
	//}
	return result;
}

System::Boolean Linphone::Core::LinphoneCall::CameraEnabled::get()
{
	API_LOCK;
	return (linphone_call_camera_enabled(this->call) == TRUE);
}

void Linphone::Core::LinphoneCall::CameraEnabled::set(System::Boolean enable)
{
	API_LOCK;
	linphone_call_enable_camera(this->call, enable);
}

Linphone::Core::LinphoneCallStats^ Linphone::Core::LinphoneCall::VideoStats::get()
{
	API_LOCK;
	return (Linphone::Core::LinphoneCallStats^) Linphone::Core::Utils::CreateLinphoneCallStats(this->call, (int)Linphone::Core::MediaType::Video);
}

void Linphone::Core::LinphoneCall::SendVFURequest()
{
	API_LOCK;
	linphone_call_send_vfu_request(this->call);
}

// TODO: Must implement  
//Windows::Phone::Networking::Voip::VoipPhoneCall^ Linphone::Core::LinphoneCall::CallContext::get()
//{
//	return this->callContext;
//}

Linphone::Core::Reason Linphone::Core::LinphoneCall::Reason::get()
{
	API_LOCK;
	return (Linphone::Core::Reason)linphone_call_get_reason(this->call);
}

::LinphoneCall* Linphone::Core::LinphoneCall::GetCallPtr()
{
    return this->call;
}

Linphone::Core::LinphoneCall::LinphoneCall(::LinphoneCall *call) :
	call(call)
{
	API_LOCK;
	RefToPtrProxy<LinphoneCall^> *proxy = new RefToPtrProxy<LinphoneCall^>(this);
	linphone_call_set_user_data(this->call, proxy);
}

Linphone::Core::LinphoneCall::~LinphoneCall()
{
	API_LOCK;
	linphone_call_unref(call);
	RefToPtrProxy<LinphoneCall^> *proxy = reinterpret_cast< RefToPtrProxy<LinphoneCall^> *>(linphone_call_get_user_data(this->call));
	delete proxy;
}
