/*
LinphoneCore.cpp
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

#include "LinphoneCore.h"
#include "LinphoneAddress.h"
#include "LinphoneAuthInfo.h"
#include "LinphoneCall.h"
#include "LinphoneCallLog.h"
#include "LinphoneCallParams.h"
#include "LinphoneProxyConfig.h"
#include "ILinphoneCoreListener.h"
#include "LinphoneChatRoom.h"
#include "LpConfig.h"
#include "PayloadType.h"
#include "Transports.h"
#include "VideoPolicy.h"
#include "VideoSize.h"
#include "Enums.h"
#include "ApiLock.h"
#include "Globals.h"

using namespace System;
using namespace Collections::Generic;
using namespace Threading;


//Linphone::Core::OutputTraceLevel Linphone::Core::LinphoneCore::logLevel = OutputTraceLevel::Error;

Linphone::Core::OutputTraceLevel Linphone::Core::LinphoneCore::LogLevel::get()
{
	return Linphone::Core::LinphoneCore::logLevel;
}

void Linphone::Core::LinphoneCore::LogLevel::set(OutputTraceLevel logLevel)
{
	API_LOCK;
	int coreLogLevel = 0;
	if (logLevel == OutputTraceLevel::Error) {
		coreLogLevel = ORTP_ERROR | ORTP_FATAL;
	}
	else if (logLevel == OutputTraceLevel::Warning) {
		coreLogLevel = ORTP_WARNING | ORTP_ERROR | ORTP_FATAL;
	}
	else if (logLevel == OutputTraceLevel::Message) {
		coreLogLevel = ORTP_MESSAGE | ORTP_WARNING | ORTP_ERROR | ORTP_FATAL;
	}
	else if (logLevel == OutputTraceLevel::Debug) {
		coreLogLevel = ORTP_DEBUG | ORTP_MESSAGE | ORTP_WARNING | ORTP_ERROR | ORTP_FATAL;
	}
	Utils::LinphoneCoreSetLogLevel(coreLogLevel);
}

void Linphone::Core::LinphoneCore::CPUCount::set(int count)
{
	API_LOCK;
    MSFactory* msFactory = ms_factory_new();
    ms_factory_set_cpu_count(msFactory, count);
}

int Linphone::Core::LinphoneCore::CPUCount::get()
{
	API_LOCK;
    MSFactory* msFactory = ms_factory_new();
	return ms_factory_get_cpu_count(msFactory);
}

void Linphone::Core::LinphoneCore::ResetLogCollection()
{
	API_LOCK;
	linphone_core_reset_log_collection();
}

void Linphone::Core::LinphoneCore::ClearProxyConfigs()
{
	API_LOCK;
	linphone_core_clear_proxy_config(this->lc);
}

void Linphone::Core::LinphoneCore::AddProxyConfig(Linphone::Core::LinphoneProxyConfig^ proxyCfg)
{
	API_LOCK;
	linphone_core_add_proxy_config(this->lc, proxyCfg->GetProxyConfigPtr());
}

void Linphone::Core::LinphoneCore::DefaultProxyConfig::set(Linphone::Core::LinphoneProxyConfig^ proxyCfg)
{
	API_LOCK;
	linphone_core_set_default_proxy_config(this->lc, proxyCfg->GetProxyConfigPtr());
}

Linphone::Core::LinphoneProxyConfig^ Linphone::Core::LinphoneCore::DefaultProxyConfig::get()
{
	API_LOCK;
	LinphoneProxyConfig^ defaultProxy = nullptr;
	::LinphoneProxyConfig *proxy = linphone_core_get_default_proxy_config(this->lc);
	if (proxy != nullptr) {
		defaultProxy = gcnew Linphone::Core::LinphoneProxyConfig(proxy);
	}
	return defaultProxy;
}

Linphone::Core::LinphoneProxyConfig^ Linphone::Core::LinphoneCore::CreateProxyConfig()
{
	API_LOCK;
	LinphoneProxyConfig^ proxyConfig = nullptr;
	::LinphoneProxyConfig *proxy = linphone_core_create_proxy_config(this->lc);
	if (proxy != nullptr) {
		proxyConfig = gcnew Linphone::Core::LinphoneProxyConfig(proxy);
	}
	return proxyConfig;
}

static void AddProxyConfigToVector(void *vProxyConfig, void *vector)
{
	::LinphoneProxyConfig *pc = (LinphoneProxyConfig *)vProxyConfig;
	Linphone::Core::RefToPtrProxy<IList<Object^>^> *list = reinterpret_cast< Linphone::Core::RefToPtrProxy<IList<Object^>^> *>(vector);
	IList<Object^>^ proxyconfigs = (list) ? list->Ref() : nullptr;

	Linphone::Core::LinphoneProxyConfig^ proxyConfig = (Linphone::Core::LinphoneProxyConfig^)Linphone::Core::Utils::CreateLinphoneProxyConfig(pc);
	proxyconfigs->Add(proxyConfig);
}

IList<Object^>^ Linphone::Core::LinphoneCore::ProxyConfigList::get() 
{
	API_LOCK;
	IList<Object^>^ proxyconfigs = gcnew List<Object^>();
	const MSList *configList = linphone_core_get_proxy_config_list(this->lc);
	RefToPtrProxy<IList<Object^>^> *proxyConfigPtr = new RefToPtrProxy<IList<Object^>^>(proxyconfigs);
    bctbx_list_for_each2(configList, AddProxyConfigToVector, proxyConfigPtr);
	return proxyconfigs;
}

void Linphone::Core::LinphoneCore::ClearAuthInfos() 
{
	API_LOCK;
	linphone_core_clear_all_auth_info(this->lc);
}

void Linphone::Core::LinphoneCore::AddAuthInfo(Linphone::Core::LinphoneAuthInfo^ info) 
{
	API_LOCK;
	linphone_core_add_auth_info(this->lc, info->GetAuthInfoPtr());
}

Linphone::Core::LinphoneAuthInfo^ Linphone::Core::LinphoneCore::CreateAuthInfo(String^ username, String^ userid, String^ password, String^ ha1, String^ realm, String^ domain)
{
	API_LOCK;
	Linphone::Core::LinphoneAuthInfo^ authInfo = gcnew Linphone::Core::LinphoneAuthInfo(username, userid, password, ha1, realm, domain);
	return authInfo;
}

static void AddAuthInfoToVector(void *vAuthInfo, void *vector)
{
	::LinphoneAuthInfo *ai = (LinphoneAuthInfo *)vAuthInfo;
	Linphone::Core::RefToPtrProxy<IList<Object^>^> *list = reinterpret_cast< Linphone::Core::RefToPtrProxy<IList<Object^>^> *>(vector);
	IList<Object^>^ authInfos = (list) ? list->Ref() : nullptr;

	Linphone::Core::LinphoneAuthInfo^ authInfo = (Linphone::Core::LinphoneAuthInfo^)Linphone::Core::Utils::CreateLinphoneAuthInfo(ai);
	authInfos->Add(authInfo);
}

IList<Object^>^ Linphone::Core::LinphoneCore::AuthInfos::get()
{
	API_LOCK;
	IList<Object^>^ authInfos = gcnew Collections::Generic::List<Object^>();
	const MSList *authlist = linphone_core_get_auth_info_list(this->lc);
	RefToPtrProxy<IList<Object^>^> *authInfosPtr = new RefToPtrProxy<IList<Object^>^>(authInfos);
    bctbx_list_for_each2(authlist, AddAuthInfoToVector, authInfosPtr);
	return authInfos;
}

void Linphone::Core::LinphoneCore::Destroy() 
{
	API_LOCK;
    linphone_core_unref(this->lc);
	IterateEnabled = false;
}

Linphone::Core::LinphoneAddress^ Linphone::Core::LinphoneCore::InterpretURL(String^ destination) 
{
	API_LOCK;
	const char* url = Linphone::Core::Utils::pstoccs(destination);
	Linphone::Core::LinphoneAddress^ addr = (Linphone::Core::LinphoneAddress^) Linphone::Core::Utils::CreateLinphoneAddress(linphone_core_interpret_url(this->lc, url));
	delete(url);
	return addr;
}

Linphone::Core::LinphoneCall^ Linphone::Core::LinphoneCore::Invite(String^ destination) 
{
	API_LOCK;
	Linphone::Core::LinphoneCall^ lCall = nullptr;
	const char *cc = Linphone::Core::Utils::pstoccs(destination);
	::LinphoneCall *call = linphone_core_invite(this->lc, cc);
	call = linphone_call_ref(call);
	delete(cc);
	if (call != NULL) {
		Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *>(linphone_call_get_user_data(call));
		lCall = (proxy) ? proxy->Ref() : nullptr;
		if (lCall == nullptr)
			lCall = (Linphone::Core::LinphoneCall^)Linphone::Core::Utils::CreateLinphoneCall(call);
	}
	return lCall;
}

Linphone::Core::LinphoneCall^ Linphone::Core::LinphoneCore::InviteWithParams(String^ destination, LinphoneCallParams^ params)
{
    API_LOCK;
    Linphone::Core::LinphoneCall^ lCall = nullptr;
    const char *cc = Linphone::Core::Utils::pstoccs(destination);
    ::LinphoneCall *call = linphone_core_invite_with_params(this->lc, cc, params->GetCallParamsPtr());
    call = linphone_call_ref(call);
    delete(cc);
    if (call != NULL) {
        Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *>(linphone_call_get_user_data(call));
        lCall = (proxy) ? proxy->Ref() : nullptr;
        if (lCall == nullptr)
            lCall = (Linphone::Core::LinphoneCall^)Linphone::Core::Utils::CreateLinphoneCall(call);
    }
    return lCall;
}

Linphone::Core::LinphoneCall^ Linphone::Core::LinphoneCore::InviteAddress(Linphone::Core::LinphoneAddress^ destination) 
{
	API_LOCK;
	Linphone::Core::LinphoneCall^ lCall = nullptr;
	::LinphoneCall *call = linphone_core_invite_address(this->lc, destination->GetAddressPtr());
	call = linphone_call_ref(call);
	if (call != NULL) {
		Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *>(linphone_call_get_user_data(call));
		lCall = (proxy) ? proxy->Ref() : nullptr;
		if (lCall == nullptr)
			lCall = (Linphone::Core::LinphoneCall^) Linphone::Core::Utils::CreateLinphoneCall(call);
	}
	return lCall;
}

Linphone::Core::LinphoneCall^ Linphone::Core::LinphoneCore::InviteAddressWithParams(Linphone::Core::LinphoneAddress^ destination, LinphoneCallParams^ params) 
{
	API_LOCK;
	Linphone::Core::LinphoneCall^ lCall = nullptr;
    ::LinphoneCall *call = linphone_core_invite_address_with_params(this->lc, destination->GetAddressPtr(), params->GetCallParamsPtr());
	call = linphone_call_ref(call);
	if (call != NULL) {
		Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *>(linphone_call_get_user_pointer(call));
		lCall = (proxy) ? proxy->Ref() : nullptr;
		if (lCall == nullptr)
			lCall = (Linphone::Core::LinphoneCall^) Linphone::Core::Utils::CreateLinphoneCall(call);
	}
	return lCall;
}

void Linphone::Core::LinphoneCore::TerminateCall(Linphone::Core::LinphoneCall^ call) 
{
	API_LOCK;
	if (call == nullptr) return;
	__try
	{
        linphone_call_terminate(call->GetCallPtr());
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		System::Console::WriteLine("Some Unmanage exception");
	}
}

void Linphone::Core::LinphoneCore::DeclineCall(Linphone::Core::LinphoneCall^ call, Linphone::Core::Reason reason)
{
	API_LOCK;	
	if (call == nullptr) return;
	__try
	{
        linphone_call_decline(call->GetCallPtr(), (LinphoneReason)reason);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		System::Console::WriteLine("Some Unmanage exception");
	}
}

Linphone::Core::LinphoneCall^ Linphone::Core::LinphoneCore::CurrentCall::get() 
{
	API_LOCK;
	Linphone::Core::LinphoneCall^ lCall = nullptr;
	::LinphoneCall *call = linphone_core_get_current_call(this->lc);
	if (call != nullptr) {
		Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *>(linphone_call_get_user_pointer(call));
		lCall = (proxy) ? proxy->Ref() : nullptr;
	}
	return lCall;
}

Boolean Linphone::Core::LinphoneCore::InCall::get() 
{
	API_LOCK;
	return (linphone_core_in_call(this->lc) == TRUE);
}

Boolean Linphone::Core::LinphoneCore::IncomingInvitePending::get()
{
	API_LOCK;
	return (linphone_core_is_incoming_invite_pending(this->lc) == TRUE);
}

void Linphone::Core::LinphoneCore::AcceptCall(Linphone::Core::LinphoneCall^ call) 
{
	API_LOCK;
	if (call == nullptr) return;
	__try
	{
		linphone_call_accept(call->GetCallPtr());
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		System::Console::WriteLine("Some Unmanage exception");
	}
}

int Linphone::Core::LinphoneCore::TransferCall(Linphone::Core::LinphoneCall^ call, String^ referTo)
{
	API_LOCK;
	const char *refer_To = Linphone::Core::Utils::pstoccs(referTo);
	int rusult = linphone_call_transfer(call->GetCallPtr(), refer_To);
	delete(refer_To);
	return rusult;
}

void Linphone::Core::LinphoneCore::AcceptCallWithParams(Linphone::Core::LinphoneCall^ call, Linphone::Core::LinphoneCallParams^ params) 
{
	API_LOCK;
    linphone_call_accept_with_params(call->GetCallPtr(), params->GetCallParamsPtr());
}

void Linphone::Core::LinphoneCore::AcceptCallUpdate(Linphone::Core::LinphoneCall^ call, Linphone::Core::LinphoneCallParams^ params) 
{
	API_LOCK;
	if (call == nullptr) return;
	__try
	{
        linphone_call_accept_update(call->GetCallPtr(), params->GetCallParamsPtr());
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		System::Console::WriteLine("Some Unmanage exception");
	}
}

void Linphone::Core::LinphoneCore::DeferCallUpdate(Linphone::Core::LinphoneCall^ call) 
{
	API_LOCK;
    linphone_call_defer_update(call->GetCallPtr());
}

void Linphone::Core::LinphoneCore::UpdateCall(Linphone::Core::LinphoneCall^ call, Linphone::Core::LinphoneCallParams^ params) 
{
	API_LOCK;
	if (params != nullptr) {
        linphone_call_update(call->GetCallPtr(), params->GetCallParamsPtr());
	} else {
        linphone_call_update(call->GetCallPtr(), nullptr);
	}
}

Linphone::Core::LinphoneCallParams^ Linphone::Core::LinphoneCore::CreateDefaultCallParameters() 
{
	API_LOCK;
	return (Linphone::Core::LinphoneCallParams^) Linphone::Core::Utils::CreateLinphoneCallParams(linphone_core_create_call_params(this->lc, 0));
}

void AddLogToVector(void* nLog, void* vector)
{
	::LinphoneCallLog *cl = (LinphoneCallLog*)nLog;
	Linphone::Core::RefToPtrProxy<IList<Object^>^> *list = reinterpret_cast< Linphone::Core::RefToPtrProxy<IList<Object^>^> *>(vector);
	IList<Object^>^ logs = (list) ? list->Ref() : nullptr;

	Linphone::Core::LinphoneCallLog^ log = (Linphone::Core::LinphoneCallLog^)Linphone::Core::Utils::CreateLinphoneCallLog(cl);
	logs->Add(log);
}

IList<Object^>^ Linphone::Core::LinphoneCore::CallLogs::get() 
{
	API_LOCK;
	IList<Object^>^ logs = gcnew Collections::Generic::List<Object^>();
	const MSList* logslist = linphone_core_get_call_logs(this->lc);
	RefToPtrProxy<IList<Object^>^> *logsptr = new RefToPtrProxy<IList<Object^>^>(logs);
    bctbx_list_for_each2(logslist, AddLogToVector, logsptr);
	return logs;
}

void Linphone::Core::LinphoneCore::ClearCallLogs() 
{
	API_LOCK;
	linphone_core_clear_call_logs(this->lc);
}

void Linphone::Core::LinphoneCore::RemoveCallLog(Linphone::Core::LinphoneCallLog^ log) 
{
	API_LOCK;
    linphone_core_remove_call_log(this->lc, log->GetCallLogPtr());
}

void Linphone::Core::LinphoneCore::NetworkReachable::set(Boolean isReachable) 
{
	API_LOCK;
	linphone_core_set_network_reachable(this->lc, isReachable);
}

Boolean Linphone::Core::LinphoneCore::NetworkReachable::get() 
{
	API_LOCK;
	return (linphone_core_is_network_reachable(this->lc) == TRUE);
}

void Linphone::Core::LinphoneCore::MicGainDb::set(float gain) 
{
	API_LOCK;
	linphone_core_set_mic_gain_db(this->lc, gain);
}

float Linphone::Core::LinphoneCore::MicGainDb::get()
{
	API_LOCK;
	return linphone_core_get_mic_gain_db(this->lc);
}

void Linphone::Core::LinphoneCore::PlaybackGainDb::set(float gain)
{
	API_LOCK;
	linphone_core_set_playback_gain_db(this->lc, gain);
}

float Linphone::Core::LinphoneCore::PlaybackGainDb::get()
{
	API_LOCK;
	return linphone_core_get_playback_gain_db(this->lc);
}

void Linphone::Core::LinphoneCore::PlayLevel::set(int level)
{
	API_LOCK;
	linphone_core_set_play_level(this->lc, level);
}

int Linphone::Core::LinphoneCore::PlayLevel::get()
{
	API_LOCK;
	return linphone_core_get_play_level(this->lc);
}

void Linphone::Core::LinphoneCore::MicMuted::set(Boolean isMuted) 
{
	API_LOCK;
    linphone_core_enable_mic(this->lc, !isMuted);
}

Boolean Linphone::Core::LinphoneCore::MicMuted::get() 
{
	API_LOCK;
	return (linphone_core_mic_enabled(this->lc) == FALSE);
}

void Linphone::Core::LinphoneCore::SendDTMF(char16 number) 
{
	API_LOCK;
	char conv[4];
	if (wctomb(conv, number) == 1) {
        linphone_call_send_dtmf(this->CurrentCall->GetCallPtr(), conv[0]);
	}
}

void Linphone::Core::LinphoneCore::PlayDTMF(char16 number, int duration) 
{
	API_LOCK;
	char conv[4];
	if (wctomb(conv, number) == 1) {
		linphone_core_play_dtmf(this->lc, conv[0], duration);
	}
}

void Linphone::Core::LinphoneCore::StopDTMF() 
{
	API_LOCK;
	linphone_core_stop_dtmf(this->lc);
}

Linphone::Core::PayloadType^ Linphone::Core::LinphoneCore::FindPayloadType(String^ mime, int clockRate, int channels) 
{
	API_LOCK;
	const char* type = Linphone::Core::Utils::pstoccs(mime);
	::LinphonePayloadType* pt = linphone_core_get_payload_type(this->lc, type, clockRate, channels);
	delete type;
	return gcnew Linphone::Core::PayloadType(pt);
}

Linphone::Core::PayloadType^ Linphone::Core::LinphoneCore::FindPayloadType(String^ mime, int clockRate) 
{
	API_LOCK;
	const char* type = Linphone::Core::Utils::pstoccs(mime);
	::LinphonePayloadType* pt = linphone_core_get_payload_type(this->lc, type, clockRate, LINPHONE_FIND_PAYLOAD_IGNORE_CHANNELS);
	delete type;
	return gcnew Linphone::Core::PayloadType(pt);
}

bool Linphone::Core::LinphoneCore::PayloadTypeEnabled(PayloadType^ pt)
{
	API_LOCK;
	::LinphonePayloadType *payload = pt->GetPayloadPtr();
	return (linphone_payload_type_enabled(payload) == TRUE);
}

void Linphone::Core::LinphoneCore::EnablePayloadType(PayloadType^ pt, Boolean enable) 
{
	API_LOCK;
    ::LinphonePayloadType *payload = pt->GetPayloadPtr();
    linphone_payload_type_enable(payload, enable);
}

static void AddCodecToVector(void *vCodec, void *vector)
{
	::LinphonePayloadType *pt = (LinphonePayloadType *)vCodec;
	Linphone::Core::RefToPtrProxy<IList<Object^>^> *list = reinterpret_cast< Linphone::Core::RefToPtrProxy<IList<Object^>^> *>(vector);
	IList<Object^>^ codecs = (list) ? list->Ref() : nullptr;

	Linphone::Core::PayloadType^ codec = (Linphone::Core::PayloadType^)Linphone::Core::Utils::CreatePayloadType(pt);
	codecs->Add(codec);
}

IList<Object^>^ Linphone::Core::LinphoneCore::AudioCodecs::get()
{
	API_LOCK;
	IList<Object^>^ codecs = gcnew Collections::Generic::List<Object^>();
	const bctbx_list_t *codecslist = linphone_core_get_audio_payload_types(this->lc);
	RefToPtrProxy<IList<Object^>^> *codecsPtr = new RefToPtrProxy<IList<Object^>^>(codecs);
    bctbx_list_for_each2(codecslist, AddCodecToVector, codecsPtr);
	return codecs;
}

void Linphone::Core::LinphoneCore::EchoCancellationEnabled::set(Boolean enable)
{
	API_LOCK;
	linphone_core_enable_echo_cancellation(this->lc, enable);
}

Boolean Linphone::Core::LinphoneCore::EchoCancellationEnabled::get()
{
	API_LOCK;
	return (linphone_core_echo_cancellation_enabled(this->lc) == TRUE);
}

static void EchoCalibrationCallback(LinphoneCore *lc, LinphoneEcCalibratorStatus status, int delay_ms, void *data)
{
	Linphone::Core::Utils::EchoCalibrationCallback(lc, status, delay_ms, data);
}

static void EchoCalibrationAudioInit(void *data)
{
    // TODO: Should implement
    return;
	//Linphone::Core::EchoCalibrationData *ecData = static_cast<Linphone::Core::EchoCalibrationData *>(data);
	//if (ecData != nullptr) {
	//	ecData->endpoint = AudioRoutingManager::GetDefault()->GetAudioEndpoint();
	//	// Need to create a dummy VoipPhoneCall to be able to capture audio!
	//	VoipCallCoordinator::GetDefault()->RequestNewOutgoingCall(
	//		"ECCalibrator",
	//		"ECCalibrator",
	//		"ECCalibrator",
	//		VoipCallMedia::Audio,
	//		&ecData->call);
	//	ecData->call->NotifyCallActive();
	//}
	//AudioRoutingManager::GetDefault()->SetAudioEndpoint(AudioRoutingEndpoint::Speakerphone);
}

static void EchoCalibrationAudioUninit(void *data)
{
    // TODO: Should implement
    return;
	//Linphone::Core::EchoCalibrationData *ecData = static_cast<Linphone::Core::EchoCalibrationData *>(data);
	//if (ecData != nullptr) {
	//	ecData->call->NotifyCallEnded();
	//	AudioRoutingManager::GetDefault()->SetAudioEndpoint(AudioRoutingEndpoint::Default);
	//}
}

void Linphone::Core::LinphoneCore::StartEchoCalibration() 
{
	API_LOCK;
    // TODO: Should implement
    return;
	//Linphone::Core::EchoCalibrationData *data = new Linphone::Core::EchoCalibrationData();
	//linphone_core_start_echo_calibration(this->lc, EchoCalibrationCallback, EchoCalibrationAudioInit, EchoCalibrationAudioUninit, data);
}

void Linphone::Core::LinphoneCore::EchoLimiterEnabled::set(Boolean enable)
{
	API_LOCK;
	linphone_core_enable_echo_limiter(this->lc, enable);
}

Boolean Linphone::Core::LinphoneCore::EchoLimiterEnabled::get()
{
	API_LOCK;
	return (linphone_core_echo_limiter_enabled(this->lc) == TRUE);
}

void Linphone::Core::LinphoneCore::SipTransports::set(Transports^ t)
{
	API_LOCK;
	::LinphoneSipTransports transports;
	memset(&transports, 0, sizeof(LinphoneSipTransports));
	transports.udp_port = t->UDP;
	transports.tcp_port = t->TCP;
	transports.tls_port = t->TLS;
	linphone_core_set_sip_transports(this->lc, &transports);
}

Linphone::Core::Transports^ Linphone::Core::LinphoneCore::SipTransports::get()
{
	API_LOCK;
	::LinphoneSipTransports transports;
	linphone_core_get_sip_transports(this->lc, &transports);
	return gcnew Linphone::Core::Transports(transports.udp_port, transports.tcp_port, transports.dtls_port, transports.tls_port);
}

void Linphone::Core::LinphoneCore::IPv6Enabled::set(Boolean enable)
{
	API_LOCK;
	linphone_core_enable_ipv6(this->lc, enable);
}

Boolean Linphone::Core::LinphoneCore::IPv6Enabled::get()
{
	API_LOCK;
	return (linphone_core_ipv6_enabled(this->lc) == TRUE);
}

void Linphone::Core::LinphoneCore::SetPresenceInfo(int minuteAway, String^ alternativeContact, OnlineStatus status) 
{
	API_LOCK;
	const char* ac = Linphone::Core::Utils::pstoccs(alternativeContact);
	linphone_core_set_presence_info(this->lc, minuteAway, ac, (LinphoneOnlineStatus) status);
	delete(ac);
}

void Linphone::Core::LinphoneCore::StunServer::set(String^ stun)
{
	API_LOCK;
	const char* stunserver = Linphone::Core::Utils::pstoccs(stun);
	linphone_core_set_stun_server(this->lc, stunserver);
	delete(stunserver);
}

String^ Linphone::Core::LinphoneCore::StunServer::get()
{
	API_LOCK;
	return Linphone::Core::Utils::cctops(linphone_core_get_stun_server(this->lc));
}

void Linphone::Core::LinphoneCore::FirewallPolicy::set(Linphone::Core::FirewallPolicy policy)
{
	API_LOCK;
	linphone_core_set_firewall_policy(this->lc, (LinphoneFirewallPolicy) policy);
}

Linphone::Core::FirewallPolicy Linphone::Core::LinphoneCore::FirewallPolicy::get()
{
	API_LOCK;
	return (Linphone::Core::FirewallPolicy) linphone_core_get_firewall_policy(this->lc);
}

void Linphone::Core::LinphoneCore::RootCA::set(String^ path)
{
	API_LOCK;
	const char *ccPath = Utils::pstoccs(path);
	linphone_core_set_root_ca(this->lc, ccPath);
	delete ccPath;
}

String^ Linphone::Core::LinphoneCore::RootCA::get()
{
	API_LOCK;
	return Utils::cctops(linphone_core_get_root_ca(this->lc));
}

int Linphone::Core::LinphoneCore::UploadBandwidth::get()
{
	API_LOCK;
	return linphone_core_get_upload_bandwidth(this->lc);
}

void Linphone::Core::LinphoneCore::UploadBandwidth::set(int value)
{
	API_LOCK;
	linphone_core_set_upload_bandwidth(this->lc, value);
}

int Linphone::Core::LinphoneCore::DownloadBandwidth::get()
{
	API_LOCK;
	return linphone_core_get_download_bandwidth(this->lc);
}

void Linphone::Core::LinphoneCore::DownloadBandwidth::set(int value)
{
	API_LOCK;
	linphone_core_set_download_bandwidth(this->lc, value);
}

void Linphone::Core::LinphoneCore::DownloadPTime::set(int ptime)
{
	API_LOCK;
	linphone_core_set_download_ptime(this->lc, ptime);
}

int Linphone::Core::LinphoneCore::DownloadPTime::get()
{
	API_LOCK;
	return linphone_core_get_download_ptime(this->lc);
}

void Linphone::Core::LinphoneCore::UploadPTime::set(int ptime)
{
	API_LOCK;
	linphone_core_set_upload_ptime(this->lc, ptime);
}

int Linphone::Core::LinphoneCore::UploadPTime::get()
{
	API_LOCK;
	return linphone_core_get_upload_ptime(this->lc);
}

void Linphone::Core::LinphoneCore::KeepAliveEnabled::set(Boolean enable)
{
	API_LOCK;
	linphone_core_enable_keep_alive(this->lc, enable);
}

Boolean Linphone::Core::LinphoneCore::KeepAliveEnabled::get()
{
	API_LOCK;
	return (linphone_core_keep_alive_enabled(this->lc) == TRUE);
}

void Linphone::Core::LinphoneCore::PlayFile::set(String^ path)
{
	API_LOCK;
	const char* file = Linphone::Core::Utils::pstoccs(path);
	linphone_core_set_play_file(this->lc, file);
	delete(file);
}

Boolean Linphone::Core::LinphoneCore::PauseCall(LinphoneCall^ call) 
{
	API_LOCK;
	return (linphone_call_pause(call->GetCallPtr()) == 0);
}

Boolean Linphone::Core::LinphoneCore::ResumeCall(LinphoneCall^ call) 
{
	API_LOCK;
	return (linphone_call_resume(call->GetCallPtr()) == 0);
}

Boolean Linphone::Core::LinphoneCore::PauseAllCalls() 
{
	API_LOCK;
	return (linphone_core_pause_all_calls(this->lc) == 0);
}

Boolean Linphone::Core::LinphoneCore::InConference::get()
{
	API_LOCK;
	return (linphone_core_is_in_conference(this->lc) == TRUE);
}

Boolean Linphone::Core::LinphoneCore::EnterConference() 
{
	API_LOCK;
	return (linphone_core_enter_conference(this->lc) == 0);
}

void Linphone::Core::LinphoneCore::LeaveConference() 
{
	API_LOCK;
	linphone_core_leave_conference(this->lc);
}

void Linphone::Core::LinphoneCore::AddToConference(LinphoneCall^ call) 
{
	API_LOCK;
	linphone_core_add_to_conference(this->lc, call->GetCallPtr());
}

void Linphone::Core::LinphoneCore::AddAllToConference() 
{
	API_LOCK;
	linphone_core_add_all_to_conference(this->lc);
}

void Linphone::Core::LinphoneCore::RemoveFromConference(LinphoneCall^ call) 
{
	API_LOCK;
	linphone_core_remove_from_conference(this->lc, call->GetCallPtr());
}

void Linphone::Core::LinphoneCore::TerminateConference() 
{
	API_LOCK;
	linphone_core_terminate_conference(this->lc);
}

int Linphone::Core::LinphoneCore::ConferenceSize::get()
{
	API_LOCK;
	return linphone_core_get_conference_size(this->lc);
}

void Linphone::Core::LinphoneCore::TerminateAllCalls() 
{
	API_LOCK;
	linphone_core_terminate_all_calls(this->lc);
}

static void AddCallToVector(void *vCall, void *vector)
{
	::LinphoneCall* c = (::LinphoneCall *)vCall;
	Linphone::Core::RefToPtrProxy<IList<Object^>^> *list = reinterpret_cast< Linphone::Core::RefToPtrProxy<IList<Object^>^> *>(vector);
	IList<Object^>^ calls = (list) ? list->Ref() : nullptr;

	Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *>(linphone_call_get_user_pointer(c));
	Linphone::Core::LinphoneCall^ call = (proxy) ? proxy->Ref() : nullptr;
	calls->Add(call);
}

IList<Object^>^ Linphone::Core::LinphoneCore::Calls::get()
{
	API_LOCK;
	List<Object^>^ calls = gcnew Collections::Generic::List<Object^>();
	const MSList *callsList = linphone_core_get_calls(this->lc);
	RefToPtrProxy<IList<Object^>^> *callsPtr = new RefToPtrProxy<IList<Object^>^>(calls);
    bctbx_list_for_each2(callsList, AddCallToVector, callsPtr);
	return calls;
}

int Linphone::Core::LinphoneCore::CallsNb::get()
{
	API_LOCK;
	return linphone_core_get_calls_nb(this->lc);
}

Linphone::Core::LinphoneCall^ Linphone::Core::LinphoneCore::FindCallFromUri(String^ uri) 
{
	API_LOCK;
	const char *curi = Utils::pstoccs(uri);
	::LinphoneCall *call = const_cast<::LinphoneCall *>(linphone_core_find_call_from_uri(this->lc, curi));
	Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *>(linphone_call_get_user_pointer(call));
	Linphone::Core::LinphoneCall^ lCall = (proxy) ? proxy->Ref() : nullptr;
	if (lCall == nullptr) {
		lCall = (Linphone::Core::LinphoneCall^)Linphone::Core::Utils::CreateLinphoneCall(call);
	}
	delete curi;
	return lCall;
}

int Linphone::Core::LinphoneCore::MaxCalls::get() 
{
	API_LOCK;
	return linphone_core_get_max_calls(this->lc);
}

void Linphone::Core::LinphoneCore::MaxCalls::set(int max)
{
	API_LOCK;
	linphone_core_set_max_calls(this->lc, max);
}

Boolean Linphone::Core::LinphoneCore::IsMyself(String^ uri) 
{
	API_LOCK;
	Boolean myself = false;
	LinphoneProxyConfig^ lpc = DefaultProxyConfig;
	if (lpc != nullptr) {
		myself = uri->Equals(lpc->GetIdentity());
	}
	return myself;
}

Boolean Linphone::Core::LinphoneCore::SoundResourcesLocked::get()
{
	API_LOCK;
	return (linphone_core_sound_resources_locked(this->lc) == TRUE);
}

Boolean Linphone::Core::LinphoneCore::IsMediaEncryptionSupported(Linphone::Core::MediaEncryption menc) 
{
	API_LOCK;
	return (linphone_core_media_encryption_supported(this->lc, (LinphoneMediaEncryption) menc) == TRUE);
}

void Linphone::Core::LinphoneCore::MediaEncryption::set(Linphone::Core::MediaEncryption menc)
{
	API_LOCK;
	linphone_core_set_media_encryption(this->lc, (LinphoneMediaEncryption) menc);
}

Linphone::Core::MediaEncryption Linphone::Core::LinphoneCore::MediaEncryption::get()
{
	API_LOCK;
	return (Linphone::Core::MediaEncryption) linphone_core_get_media_encryption(this->lc);
}

void Linphone::Core::LinphoneCore::MediaEncryptionMandatory::set(Boolean yesno)
{
	API_LOCK;
	linphone_core_set_media_encryption_mandatory(this->lc, yesno);
}

Boolean Linphone::Core::LinphoneCore::MediaEncryptionMandatory::get()
{
	API_LOCK;
	return (linphone_core_is_media_encryption_mandatory(this->lc) == TRUE);
}

Boolean Linphone::Core::LinphoneCore::TunnelAvailable::get()
{
	API_LOCK;
	//return (linphone_core_tunnel_available() == TRUE);
    return FALSE;
}

// TODO: Should implement
//Linphone::Core::Tunnel^ Linphone::Core::LinphoneCore::Tunnel::get()
//{
//	API_LOCK;
//    
//	Linphone::Core::Tunnel^ tunnel = nullptr;
//	LinphoneTunnel *lt = linphone_core_get_tunnel(this->lc);
//	if (lt != nullptr) {
//		tunnel = gcnew Linphone::Core::Tunnel(lt);
//	}
//	return tunnel;
//}

void Linphone::Core::LinphoneCore::SetUserAgent(String^ name, String^ version) 
{
	API_LOCK;
	const char* ua = Linphone::Core::Utils::pstoccs(name);
	const char* v = Linphone::Core::Utils::pstoccs(version);
	linphone_core_set_user_agent(this->lc, ua, v);
	delete(v);
	delete(ua);
}

int Linphone::Core::LinphoneCore::MissedCallsCount::get()
{
	API_LOCK;
	return linphone_core_get_missed_calls_count(this->lc);
}

void Linphone::Core::LinphoneCore::ResetMissedCallsCount() 
{
	API_LOCK;
	linphone_core_reset_missed_calls_count(this->lc);
}

void Linphone::Core::LinphoneCore::RefreshRegisters() 
{
	API_LOCK;
	linphone_core_refresh_registers(this->lc);
}

String^ Linphone::Core::LinphoneCore::Version::get()
{
	API_LOCK;
	return Linphone::Core::Utils::cctops(linphone_core_get_version());
}

void Linphone::Core::LinphoneCore::AudioPort::set(int port)
{
	API_LOCK;
	linphone_core_set_audio_port(this->lc, port);
}

int Linphone::Core::LinphoneCore::AudioPort::get()
{
	API_LOCK;
	return linphone_core_get_audio_port(this->lc);
}

void Linphone::Core::LinphoneCore::SetAudioPortRange(int minP, int maxP) 
{
	API_LOCK;
	linphone_core_set_audio_port_range(this->lc, minP, maxP);
}

void Linphone::Core::LinphoneCore::IncTimeout::set(int timeout)
{
	API_LOCK;
	linphone_core_set_inc_timeout(this->lc, timeout);
}

int Linphone::Core::LinphoneCore::IncTimeout::get()
{
	API_LOCK;
	return linphone_core_get_inc_timeout(this->lc);
}

void Linphone::Core::LinphoneCore::InCallTimeout::set(int timeout)
{
	API_LOCK;
	linphone_core_set_in_call_timeout(this->lc, timeout);
}

int Linphone::Core::LinphoneCore::InCallTimeout::get()
{
	API_LOCK;
	return linphone_core_get_in_call_timeout(this->lc);
}

void Linphone::Core::LinphoneCore::SetPrimaryContact(String^ displayName, String^ userName) 
{
	API_LOCK;
	const char* dn = Linphone::Core::Utils::pstoccs(displayName);
	const char* un = Linphone::Core::Utils::pstoccs(userName);
	::LinphoneAddress* addr = linphone_core_get_primary_contact_parsed(this->lc);
	if (addr != nullptr) {
		linphone_address_set_display_name(addr, dn);
		linphone_address_set_username(addr, un);
		char* contact = linphone_address_as_string(addr);
		linphone_core_set_primary_contact(this->lc, contact);
	}
	delete(un);
	delete(dn);
}

Boolean Linphone::Core::LinphoneCore::UseInfoForDtmf::get()
{
	API_LOCK;
	return (linphone_core_get_use_info_for_dtmf(this->lc) == TRUE);
}

Boolean Linphone::Core::LinphoneCore::UseRfc2833ForDtmf::get()
{
	API_LOCK;
	return (linphone_core_get_use_rfc2833_for_dtmf(this->lc) == TRUE);
}

void Linphone::Core::LinphoneCore::UseInfoForDtmf::set(Boolean use)
{
	API_LOCK;
	linphone_core_set_use_info_for_dtmf(this->lc, use);
}

void Linphone::Core::LinphoneCore::UseRfc2833ForDtmf::set(Boolean use)
{
	API_LOCK;
	linphone_core_set_use_rfc2833_for_dtmf(this->lc, use);
}

Linphone::Core::LpConfig^ Linphone::Core::LinphoneCore::Config::get()
{
	API_LOCK;
	::LpConfig *config = linphone_core_get_config(this->lc);
	return (Linphone::Core::LpConfig^)Linphone::Core::Utils::CreateLpConfig(config);
}

Boolean Linphone::Core::LinphoneCore::VideoSupported::get()
{
	API_LOCK;
	return (linphone_core_video_supported(this->lc) == TRUE);
}

Linphone::Core::VideoPolicy^ Linphone::Core::LinphoneCore::VideoPolicy::get()
{
	API_LOCK;
	const ::LinphoneVideoPolicy *lvp = linphone_core_get_video_policy(this->lc);
	return gcnew Linphone::Core::VideoPolicy((lvp->automatically_initiate == TRUE), (lvp->automatically_accept == TRUE));
}

void Linphone::Core::LinphoneCore::VideoPolicy::set(Linphone::Core::VideoPolicy^ policy)
{
	API_LOCK;
	::LinphoneVideoPolicy lvp;
	lvp.automatically_initiate = policy->AutomaticallyInitiate;
	lvp.automatically_accept = policy->AutomaticallyAccept;
	linphone_core_set_video_policy(this->lc, &lvp);
}

IList<Object^>^ Linphone::Core::LinphoneCore::SupportedVideoSizes::get()
{
	API_LOCK;
    List<Object^>^ sizes = gcnew Collections::Generic::List<Object^>();
	const MSVideoSizeDef *sizesList = linphone_core_get_supported_video_sizes(this->lc);
	while (sizesList->name != NULL) {
		String^ sizeName = Utils::cctops(sizesList->name);
		Linphone::Core::VideoSize^ size = gcnew Linphone::Core::VideoSize(sizesList->vsize.width, sizesList->vsize.height, sizeName);
		sizes->Add(size);
		sizesList++;
	}
	return sizes;
}

Linphone::Core::VideoSize^ Linphone::Core::LinphoneCore::PreferredVideoSize::get()
{
	API_LOCK;
	Linphone::Core::VideoSize^ size = nullptr;
	const MSVideoSizeDef *sizesList = linphone_core_get_supported_video_sizes(this->lc);
	MSVideoSize vsize = linphone_core_get_preferred_video_size(this->lc);
	while (sizesList->name != NULL) {
		if ((sizesList->vsize.width == vsize.width) && (sizesList->vsize.height == vsize.height)) {
			String^ sizeName = Utils::cctops(sizesList->name);
			size = gcnew Linphone::Core::VideoSize(vsize.width, vsize.height, sizeName);
			break;
		}
		sizesList++;
	}
	if (size == nullptr) {
		size = gcnew Linphone::Core::VideoSize(vsize.width, vsize.height);
	}
	return size;
}

String^ Linphone::Core::LinphoneCore::GetPreferredVideoSizeName()
{
	API_LOCK;
	char *cSizeName = linphone_core_get_preferred_video_size_name(this->lc);
	String^ sizeName = Utils::cctops(cSizeName);
	ms_free(cSizeName);
	return sizeName;
}

void Linphone::Core::LinphoneCore::PreferredVideoSize::set(Linphone::Core::VideoSize^ size)
{
	API_LOCK;
	if (size->Name != nullptr) {
		const char *ccname = Utils::pstoccs(size->Name);
		linphone_core_set_preferred_video_size_by_name(this->lc, ccname);
		delete ccname;
	} else {
		SetPreferredVideoSize(size->Width, size->Height);
	}
}

void Linphone::Core::LinphoneCore::SetPreferredVideoSize(int width, int height)
{
	API_LOCK;
    LinphoneFactory* factory = linphone_factory_get();
    LinphoneVideoDefinition* videoDefinition = linphone_factory_create_video_definition(factory, width, height);
    linphone_core_set_preferred_video_definition(this->lc, videoDefinition);
}

void Linphone::Core::LinphoneCore::SetPreferredVideoSizeByName(String^ sizeName)
{
	API_LOCK;
	linphone_core_set_preferred_video_size_by_name(this->lc, Utils::pstoccs(sizeName));
}

void Linphone::Core::LinphoneCore::ReloadVideoDevices()
{
	API_LOCK;
	linphone_core_reload_video_devices(this->lc);
}

IList<Object^>^ Linphone::Core::LinphoneCore::VideoDevices::get()
{
	API_LOCK;
	List<Object^>^ devices = gcnew Collections::Generic::List<Object^>();
	const char **lvds = linphone_core_get_video_devices(this->lc);
	while (*lvds != NULL) {
		String^ device = Utils::cctops(*lvds);
		devices->Add(device);
		lvds++;
	}
	return devices;
}

String^ Linphone::Core::LinphoneCore::VideoDevice::get()
{
	API_LOCK;
	String^ device = nullptr;
	const char *ccname = linphone_core_get_video_device(this->lc);
	if (ccname != NULL) {
		device = Utils::cctops(ccname);
	}
	return device;
}

void Linphone::Core::LinphoneCore::VideoDevice::set(String^ device)
{
	API_LOCK;
	const char *ccname = Utils::pstoccs(device);
	linphone_core_set_video_device(this->lc, ccname);
	delete ccname;
}

void Linphone::Core::LinphoneCore::ReloadSoundDevices()
{
	API_LOCK;
	linphone_core_reload_sound_devices(this->lc);
}

Boolean Linphone::Core::LinphoneCore::SoundDeviceCanCapture(System::String^ soundDeviceId)
{
	API_LOCK;
	const char *ccname = Utils::pstoccs(soundDeviceId);
	return (linphone_core_sound_device_can_capture(this->lc, ccname) == TRUE);
}

IList<Object^>^ Linphone::Core::LinphoneCore::SoundDevices::get()
{
	API_LOCK;
	List<Object^>^ devices = gcnew Collections::Generic::List<Object^>();
	const char **lvds = linphone_core_get_sound_devices(this->lc);
	while (*lvds != NULL) {
		String^ device = Utils::cctops(*lvds);
		devices->Add(device);
		lvds++;
	}
	return devices;
}

String^ Linphone::Core::LinphoneCore::SoundCaptureDevice::get()
{
	API_LOCK;
	String^ device = nullptr;
	const char *ccname = linphone_core_get_capture_device(this->lc);
	if (ccname != NULL) {
		device = Utils::cctops(ccname);
	}
	return device;
}

void Linphone::Core::LinphoneCore::SoundCaptureDevice::set(String^ soundDeviceId)
{
	API_LOCK;
	const char *ccname = Utils::pstoccs(soundDeviceId);
	linphone_core_set_capture_device(this->lc, ccname);
	delete ccname;
}

IList<Object^>^ Linphone::Core::LinphoneCore::VideoCodecs::get()
{
	API_LOCK;
	IList<Object^>^ codecs = gcnew Collections::Generic::List<Object^>();
	const bctbx_list_t *codecslist = linphone_core_get_video_payload_types(this->lc);
	RefToPtrProxy<IList<Object^>^> *codecsPtr = new RefToPtrProxy<IList<Object^>^>(codecs);
    bctbx_list_for_each2(codecslist, AddCodecToVector, codecsPtr);
	return codecs;
}

Boolean Linphone::Core::LinphoneCore::VideoEnabled::get()
{
	API_LOCK;
	return (linphone_core_video_enabled(this->lc) == TRUE);
}

Boolean Linphone::Core::LinphoneCore::VideoCaptureEnabled::get()
{
	API_LOCK;
	return (linphone_core_video_capture_enabled(this->lc) == TRUE);
}

Boolean Linphone::Core::LinphoneCore::VideoDisplayEnabled::get()
{
	API_LOCK;
	return (linphone_core_video_display_enabled(this->lc) == TRUE);
}

void Linphone::Core::LinphoneCore::EnableVideo(Boolean enableCapture, Boolean enableDisplay)
{
	API_LOCK;
    linphone_core_enable_video_capture(this->lc, enableCapture);
    linphone_core_enable_video_display(this->lc, enableDisplay);
}

void Linphone::Core::LinphoneCore::VideoCaptureEnabled::set(Boolean enable)
{
	API_LOCK;
	linphone_core_enable_video_capture(this->lc, enable);
}

void Linphone::Core::LinphoneCore::VideoDisplayEnabled::set(Boolean enable)
{
	API_LOCK;
	linphone_core_enable_video_display(this->lc, enable);
}

ulong Linphone::Core::LinphoneCore::NativeVideoWindowId::get()
{
	API_LOCK;
    return (ulong)linphone_core_get_native_video_window_id(this->lc);
}

void Linphone::Core::LinphoneCore::NativeVideoWindowId::set(ulong windowId)
{
    linphone_core_set_native_video_window_id(this->lc, (void*)windowId);
}

ulong Linphone::Core::LinphoneCore::NativePreviewVideoWindowId::get()
{
	API_LOCK;
	return (ulong)linphone_core_get_native_preview_window_id(this->lc);
}

void Linphone::Core::LinphoneCore::NativePreviewVideoWindowId::set(ulong windowId)
{
	linphone_core_set_native_preview_window_id(this->lc, (void*)windowId);
}

int Linphone::Core::LinphoneCore::CameraSensorRotation::get()
{
	API_LOCK;
	return linphone_core_get_camera_sensor_rotation(this->lc);
}

Boolean Linphone::Core::LinphoneCore::SelfViewEnabled::get()
{
	API_LOCK;
	return (linphone_core_self_view_enabled(this->lc) == TRUE);
}

void Linphone::Core::LinphoneCore::SelfViewEnabled::set(Boolean enable)
{
	API_LOCK;
	linphone_core_enable_self_view(this->lc, enable);
}

Boolean Linphone::Core::LinphoneCore::VideoPreviewEnabled::get()
{
	API_LOCK;
	return (linphone_core_video_preview_enabled(this->lc) == TRUE);
}

void Linphone::Core::LinphoneCore::VideoPreviewEnabled::set(Boolean enable)
{
	API_LOCK;
	__try
	{
		linphone_core_enable_video_preview(this->lc, enable);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		System::Console::WriteLine("Some Unmanage exception");
	}
}

Linphone::Core::LinphoneChatRoom^ Linphone::Core::LinphoneCore::GetChatRoom(Linphone::Core::LinphoneAddress^ address)
{
	API_LOCK;
	::LinphoneChatRoom * chatRoom = linphone_core_get_chat_room(this->lc, address->GetAddressPtr());
	Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatRoom^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatRoom^> *>(linphone_chat_room_get_user_data(chatRoom));
	Linphone::Core::LinphoneChatRoom^ lChatRoom = (proxy) ? proxy->Ref() : nullptr;
	if (lChatRoom == nullptr) {
		lChatRoom = (Linphone::Core::LinphoneChatRoom^) Linphone::Core::Utils::CreateLinphoneChatRoom(chatRoom);
	}
	return lChatRoom;
}

Linphone::Core::LinphoneChatRoom^ Linphone::Core::LinphoneCore::GetChatRoomFromUri(String^ to)
{
	API_LOCK;
	::LinphoneChatRoom * chatRoom = linphone_core_get_chat_room_from_uri(this->lc, Linphone::Core::Utils::pstoccs(to));
	Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatRoom^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatRoom^> *>(linphone_chat_room_get_user_data(chatRoom));
	Linphone::Core::LinphoneChatRoom^ lChatRoom = (proxy) ? proxy->Ref() : nullptr;
	if (lChatRoom == nullptr) {
		lChatRoom = (Linphone::Core::LinphoneChatRoom^) Linphone::Core::Utils::CreateLinphoneChatRoom(chatRoom);
	}
	return lChatRoom;
}

void Linphone::Core::LinphoneCore::LogCollectionUploadServerUrl::set(String^ url)
{
	API_LOCK;
	const char *curl = Linphone::Core::Utils::pstoccs(url);
	linphone_core_set_log_collection_upload_server_url(this->lc, curl);
	delete(curl);
}

void Linphone::Core::LinphoneCore::UploadLogCollection()
{
	API_LOCK;
	linphone_core_upload_log_collection(this->lc);
}

void Linphone::Core::LinphoneCore::DeviceRotation::set(int rotation)
{
	API_LOCK;
	linphone_core_set_device_rotation(this->lc, rotation);
}

int Linphone::Core::LinphoneCore::DeviceRotation::get()
{
	API_LOCK;
	return linphone_core_get_device_rotation(this->lc);
}

void Linphone::Core::LinphoneCore::NotifyMute(bool isMuted)
{
	API_LOCK;
    // TODO: Should implement
	//Globals::Instance->CallController->NotifyMute(isMuted);
	MicMuted = isMuted;
}

void Linphone::Core::LinphoneCore::ChatDatabasePath::set(String^ chatDatabasePath)
{
	API_LOCK;
	linphone_core_set_chat_database_path(this->lc, Linphone::Core::Utils::pstoccs(chatDatabasePath));
}

static void AddChatRoomListToVector(void *vRoom, void *vector)
{
	::LinphoneChatRoom *chatRoom = (LinphoneChatRoom*) vRoom;
	Linphone::Core::RefToPtrProxy<IList<Object^>^> *list = reinterpret_cast< Linphone::Core::RefToPtrProxy<IList<Object^>^> *>(vector);
	IList<Object^>^ rooms = (list) ? list->Ref() : nullptr;
	Linphone::Core::LinphoneChatRoom^ room = (Linphone::Core::LinphoneChatRoom^) Linphone::Core::Utils::CreateLinphoneChatRoom(chatRoom);
	rooms->Add(room);
}

IList<Object^>^ Linphone::Core::LinphoneCore::ChatRooms::get()
{
	API_LOCK;
	IList<Object^>^ rooms = gcnew Collections::Generic::List<Object^>();
	const bctbx_list_t* roomList = linphone_core_get_chat_rooms(this->lc);
	RefToPtrProxy<IList<Object^>^> *roomsPtr = new RefToPtrProxy<IList<Object^>^>(rooms);
    bctbx_list_for_each2(roomList, AddChatRoomListToVector, roomsPtr);
	return rooms;
}

Linphone::Core::ILinphoneCoreListener^ Linphone::Core::LinphoneCore::CoreListener::get()
{
	return this->listener;
}

void Linphone::Core::LinphoneCore::CoreListener::set(ILinphoneCoreListener^ listener)
{
	API_LOCK;
	this->listener = listener;
}

void call_state_changed(::LinphoneCore *lc, ::LinphoneCall *call, ::LinphoneCallState cstate, const char *msg) 
{
	Linphone::Core::LinphoneCallState state = (Linphone::Core::LinphoneCallState) cstate;
	Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *>(linphone_call_get_user_pointer(call));
	Linphone::Core::LinphoneCall^ lCall = (proxy) ? proxy->Ref() : nullptr;
	if (lCall == nullptr) {
		call = linphone_call_ref(call);
		lCall = (Linphone::Core::LinphoneCall^)Linphone::Core::Utils::CreateLinphoneCall(call);
	}
    
    // TODO: Must implement, important
	//Linphone::Core::CallController^ callController = Linphone::Core::Globals::Instance->CallController;
	//if (state == Linphone::Core::LinphoneCallState::IncomingReceived) {
	//	String^ name = lCall->RemoteAddress->DisplayName;
	//	if (name == nullptr || name->Length <= 0) 
	//	{
	//		name = lCall->RemoteAddress->UserName;
	//	}
	//	Windows::Phone::Networking::Voip::VoipPhoneCall^ platformCall = callController->OnIncomingCallReceived(lCall, name, lCall->RemoteAddress->AsStringUriOnly(), callController->IncomingCallViewDismissed);
	//	lcall->GetCallPtr()Context = platformCall;
	//} 
	//else if (state == Linphone::Core::LinphoneCallState::OutgoingProgress) {
	//	Windows::Phone::Networking::Voip::VoipPhoneCall^ platformCall = callController->NewOutgoingCall(lCall->RemoteAddress->AsStringUriOnly());
	//	lcall->GetCallPtr()Context = platformCall;
	//}
	//else if (state == Linphone::Core::LinphoneCallState::CallEnd || state == Linphone::Core::LinphoneCallState::Error) {
	//	Windows::Phone::Networking::Voip::VoipPhoneCall^ platformCall = (Windows::Phone::Networking::Voip::VoipPhoneCall^) lcall->GetCallPtr()Context;
	//	if (platformCall != nullptr)
	//		platformCall->NotifyCallEnded();

	//	if (callController->IncomingCallViewDismissed != nullptr) {
	//		// When we receive a call with PN, call the callback to kill the agent process in case the caller stops the call before user accepts/denies it
	//		callController->IncomingCallViewDismissed();
	//	}
	//}
	//else if (state == Linphone::Core::LinphoneCallState::Paused || state == Linphone::Core::LinphoneCallState::PausedByRemote) {
	//	Windows::Phone::Networking::Voip::VoipPhoneCall^ platformCall = (Windows::Phone::Networking::Voip::VoipPhoneCall^) lcall->GetCallPtr()Context;
	//	if (platformCall != nullptr)
	//		platformCall->NotifyCallHeld();
	//}
	//else if (state == Linphone::Core::LinphoneCallState::StreamsRunning) {
	//	Windows::Phone::Networking::Voip::VoipPhoneCall^ platformCall = (Windows::Phone::Networking::Voip::VoipPhoneCall^) lcall->GetCallPtr()Context;
	//	if (platformCall == nullptr) {
	//		// If CallContext is null here, it is because we have an incoming call using the custom incoming call view so create the VoipPhoneCall now
	//		platformCall = callController->NewIncomingCallForCustomIncomingCallView(lCall->RemoteAddress->DisplayName);
	//		lcall->GetCallPtr()Context = platformCall;
	//	}
	//	if (lCall->CameraEnabled) {
	//		platformcall->GetCallPtr()Media = VoipCallMedia::Audio | VoipCallMedia::Video;
	//	} else {
	//		platformcall->GetCallPtr()Media = VoipCallMedia::Audio;
	//	}
	//	platformCall->NotifyCallActive();
	//}
	
	Linphone::Core::ILinphoneCoreListener^ listener = Linphone::Core::Globals::Instance->LinphoneCore->CoreListener;
	if (listener != nullptr) {
		listener->CallState(lCall, state, Linphone::Core::Utils::cctops(msg));
	}
}

void registration_state_changed(::LinphoneCore *lc, ::LinphoneProxyConfig *cfg, ::LinphoneRegistrationState cstate, const char *msg)
{
	Linphone::Core::ILinphoneCoreListener^ listener = Linphone::Core::Globals::Instance->LinphoneCore->CoreListener;
	if (listener != nullptr) {
		Linphone::Core::RegistrationState state = (Linphone::Core::RegistrationState) cstate;
		Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneProxyConfig^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneProxyConfig^> *>(linphone_proxy_config_get_user_data(cfg));
		Linphone::Core::LinphoneProxyConfig^ config = (proxy) ? proxy->Ref() : nullptr;
		listener->RegistrationState(config, state, Linphone::Core::Utils::cctops(msg));
	}
}

void global_state_changed(::LinphoneCore *lc, ::LinphoneGlobalState gstate, const char *msg)
{
	Linphone::Core::ILinphoneCoreListener^ listener = Linphone::Core::Globals::Instance->LinphoneCore->CoreListener;
	if (listener != nullptr) {
		Linphone::Core::GlobalState state = (Linphone::Core::GlobalState) gstate;
		listener->GlobalState(state, Linphone::Core::Utils::cctops(msg));
	}
}

void authentication_requested(LinphoneCore *lc, LinphoneAuthInfo *auth_info, LinphoneAuthMethod method)
{
	Linphone::Core::ILinphoneCoreListener^ listener = Linphone::Core::Globals::Instance->LinphoneCore->CoreListener;
	if (listener != nullptr) {
        const char* realm = linphone_auth_info_get_realm(auth_info);
        const char* username = linphone_auth_info_get_username(auth_info);
        const char* domain = linphone_auth_info_get_domain(auth_info);
		listener->AuthInfoRequested(Linphone::Core::Utils::cctops(realm), Linphone::Core::Utils::cctops(username), Linphone::Core::Utils::cctops(domain));
	}
}

void dtmf_received(LinphoneCore *lc, LinphoneCall *call, int dtmf)
{
	Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *>(linphone_call_get_user_pointer(call));
	Linphone::Core::LinphoneCall^ lCall = (proxy) ? proxy->Ref() : nullptr;
	if (lCall == nullptr) {
		lCall = (Linphone::Core::LinphoneCall^)Linphone::Core::Utils::CreateLinphoneCall(call);
	}
	if (lCall != nullptr) {
		Linphone::Core::ILinphoneCoreListener^ listener = Linphone::Core::Globals::Instance->LinphoneCore->CoreListener;
		if (listener != nullptr) {
			char cdtmf = (char)dtmf;
			char16 wdtmf;
			mbtowc(&wdtmf, &cdtmf, 1);
			listener->DTMFReceived(lCall, wdtmf);
		}
	}
}

void call_encryption_changed(LinphoneCore *lc, LinphoneCall *call, bool_t on, const char *authentication_token)
{
	Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *>(linphone_call_get_user_pointer(call));
	Linphone::Core::LinphoneCall^ lCall = (proxy) ? proxy->Ref() : nullptr;
	if (lCall == nullptr) {
		lCall = (Linphone::Core::LinphoneCall^)Linphone::Core::Utils::CreateLinphoneCall(call);
	}
	if (lCall != nullptr) {
		Linphone::Core::ILinphoneCoreListener^ listener = Linphone::Core::Globals::Instance->LinphoneCore->CoreListener;
		if (listener != nullptr) {
			listener->CallEncryptionChanged(lCall, (on == TRUE), Linphone::Core::Utils::cctops(authentication_token));
		}
	}
}

void call_stats_updated(LinphoneCore *lc, LinphoneCall *call, const LinphoneCallStats *stats)
{
	Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCall^> *>(linphone_call_get_user_pointer(call));
	Linphone::Core::LinphoneCall^ lCall = (proxy) ? proxy->Ref() : nullptr;
	if (lCall == nullptr) {
		lCall = (Linphone::Core::LinphoneCall^)Linphone::Core::Utils::CreateLinphoneCall(call);
	}
	Linphone::Core::LinphoneCallStats^ lStats = (Linphone::Core::LinphoneCallStats^)Linphone::Core::Utils::CreateLinphoneCallStats((void *)stats);
	if ((lCall != nullptr) && (lStats != nullptr)) {
		Linphone::Core::ILinphoneCoreListener^ listener = Linphone::Core::Globals::Instance->LinphoneCore->CoreListener;
		if (listener != nullptr) {
			listener->CallStatsUpdated(lCall, lStats);
		}
	}
}

void message_received(LinphoneCore *lc, LinphoneChatRoom* chat_room, LinphoneChatMessage* message) 
{
	Linphone::Core::ILinphoneCoreListener^ listener = Linphone::Core::Globals::Instance->LinphoneCore->CoreListener;
	if (listener != nullptr)
	{
		Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatMessage^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatMessage^> *>(linphone_chat_message_get_user_data(message));
		Linphone::Core::LinphoneChatMessage^ lMessage = (proxy) ? proxy->Ref() : nullptr;
		if (lMessage == nullptr) {
			lMessage = (Linphone::Core::LinphoneChatMessage^)Linphone::Core::Utils::CreateLinphoneChatMessage(message);
		}

		listener->MessageReceived(lMessage);
	}
}

void composing_received(LinphoneCore *lc, LinphoneChatRoom *room) 
{
	Linphone::Core::ILinphoneCoreListener^ listener = Linphone::Core::Globals::Instance->LinphoneCore->CoreListener;
	if (listener != nullptr)
	{
		Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatRoom^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatRoom^> *>(linphone_chat_room_get_user_data(room));
		Linphone::Core::LinphoneChatRoom^ lRoom = (proxy) ? proxy->Ref() : nullptr;
		if (lRoom == nullptr) {
			lRoom = (Linphone::Core::LinphoneChatRoom^)Linphone::Core::Utils::CreateLinphoneChatRoom(room);
		}
		listener->ComposingReceived(lRoom);
	}
}

void file_transfer_progress_indication(LinphoneCore *lc, LinphoneChatMessage *message, const LinphoneContent *content, size_t offset, size_t total)
{
	Linphone::Core::ILinphoneCoreListener^ listener = Linphone::Core::Globals::Instance->LinphoneCore->CoreListener;
	if (listener != nullptr)
	{
		Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatMessage^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneChatMessage^> *>(linphone_chat_message_get_user_data(message));
		Linphone::Core::LinphoneChatMessage^ lMessage = (proxy) ? proxy->Ref() : nullptr;
		if (lMessage == nullptr) {
			lMessage = (Linphone::Core::LinphoneChatMessage^)Linphone::Core::Utils::CreateLinphoneChatMessage(message);
		}
		listener->FileTransferProgressIndication(lMessage, (int)offset, (int)total);
	}
}

void log_collection_upload_progress_indication(LinphoneCore *lc, size_t offset, size_t total)
{
	Linphone::Core::ILinphoneCoreListener^ listener = Linphone::Core::Globals::Instance->LinphoneCore->CoreListener;
	if (listener != nullptr)
	{
		listener->LogUploadProgressIndication((int)offset, (int)total);
	}
}

void log_collection_upload_state_changed(LinphoneCore *lc, ::LinphoneCoreLogCollectionUploadState state, const char *info)
{
	Linphone::Core::ILinphoneCoreListener^ listener = Linphone::Core::Globals::Instance->LinphoneCore->CoreListener;
	if (listener != nullptr)
	{
		listener->LogUploadStatusChanged((Linphone::Core::LinphoneCoreLogCollectionUploadState)state, info ? Linphone::Core::Utils::cctops(info) : nullptr);
	}
}

Linphone::Core::LinphoneCore::LinphoneCore(ILinphoneCoreListener^ coreListener) :
	lc(nullptr),
	listener(coreListener),
    config(nullptr)
{
}

Linphone::Core::LinphoneCore::LinphoneCore(ILinphoneCoreListener^ coreListener, LpConfig^ config) :
	lc(nullptr),
	listener(coreListener),
	config(config),
	isIterateEnabled(false)
{
}

void Linphone::Core::LinphoneCore::IterateEnabled::set(Boolean value)
{
	API_LOCK;

    //** WinPhone 8
	//if (isIterateEnabled && !value && IterateWorkItem)
	//{
	//	IterateWorkItem->Cancel();
	//	IterateWorkItem = nullptr;
	//}
	//else if (!isIterateEnabled && value) 
	//{
	//	IAsyncAction^ IterateWorkItem = ThreadPool::RunAsync(ref new WorkItemHandler([this](IAsyncAction^ action)
	//	{
	//		while (true) {
	//			GlobalApiLock::Instance()->Lock(__FUNCTION__);
	//			linphone_core_iterate(this->lc);
	//			GlobalApiLock::Instance()->Unlock(__FUNCTION__);
	//			ms_usleep(20000);
	//		}
	//	}), WorkItemPriority::Low);
	//}

    // TODO: Must review carefully, important
    if (isIterateEnabled && !value && IterateWorkItem)
    {
        //stopIterate = true;
        IterateWorkEvent->Set();
        //IterateWorkItem->Join(10000);
        if (IterateWorkItem->IsAlive)
        {
            IterateWorkItem->Abort();
        }
    	IterateWorkItem = nullptr;
    }
    else if (!isIterateEnabled && value) 
    {
        //stopIterate = false;
        IterateWorkEvent->Reset();
        ThreadStart^ threadStart = gcnew ThreadStart(this, &LinphoneCore::IterateThreadProc);
        IterateWorkItem = gcnew Thread(threadStart);
        IterateWorkItem->Priority = ThreadPriority::BelowNormal;
        IterateWorkItem->Start();
    }
	isIterateEnabled = value;

    API_UNLOCK;
}

void Linphone::Core::LinphoneCore::IterateThreadProc()
{
    //while (!stopIterate)
    while (true)
    {
        // Stop iterating
        if (this->IterateWorkEvent->WaitOne(100))
        {
            break;
        }

        try
        {
            API_LOCK;
            linphone_core_iterate(this->lc);
        }
        catch (Exception^ ex)
        {
            Console::WriteLine(ex->ToString());
        }
        finally
        {
            API_UNLOCK;
        }
    }
}

Boolean Linphone::Core::LinphoneCore::IterateEnabled::get()
{
	return isIterateEnabled;
}

void Linphone::Core::LinphoneCore::Init()
{
	LinphoneCoreVTable *vtable = (LinphoneCoreVTable*) malloc(sizeof(LinphoneCoreVTable));
	memset (vtable, 0, sizeof(LinphoneCoreVTable));
	vtable->global_state_changed = global_state_changed;
	vtable->registration_state_changed = registration_state_changed;
	vtable->call_state_changed = call_state_changed;
	vtable->authentication_requested = authentication_requested;
	vtable->dtmf_received = dtmf_received;
	vtable->call_encryption_changed = call_encryption_changed;
	vtable->call_stats_updated = call_stats_updated;
	vtable->message_received = message_received;
	vtable->is_composing_received = composing_received;
	vtable->file_transfer_progress_indication = file_transfer_progress_indication;
	vtable->log_collection_upload_progress_indication = log_collection_upload_progress_indication;
	vtable->log_collection_upload_state_changed = log_collection_upload_state_changed;

    if (config == nullptr)
    {
        this->lc = linphone_core_new(vtable, NULL, NULL, NULL);
    }
    else
    {
        this->lc = linphone_core_new_with_config(vtable, NULL, NULL);
    }	
	RefToPtrProxy<LinphoneCore^> *proxy = new RefToPtrProxy<LinphoneCore^>(this);
	linphone_core_set_user_data(this->lc, proxy);

	linphone_core_set_ring(this->lc, nullptr);

    // TODO: Should implement
	//RefToPtrProxy<Mediastreamer2::WP8Video::IVideoRenderer^> *renderer = new RefToPtrProxy<Mediastreamer2::WP8Video::IVideoRenderer^>(Globals::Instance->VideoRenderer);
	//linphone_core_set_native_video_window_id(this->lc, (unsigned long)renderer);
    linphone_core_set_native_video_window_id(this->lc, LINPHONE_VIDEO_DISPLAY_AUTO);
}

Linphone::Core::LinphoneCore::~LinphoneCore()
{
	API_LOCK;
	RefToPtrProxy<LinphoneCore^> *proxy = reinterpret_cast< RefToPtrProxy<LinphoneCore^> *>(linphone_core_get_user_data(this->lc));
	delete proxy;
}

int Linphone::Core::LinphoneCore::SoundPlayback::get()
{
	API_LOCK;
	return linphone_core_get_play_level(this->lc);
}

void Linphone::Core::LinphoneCore::SoundPlayback::set(int level)
{
	API_LOCK;
	linphone_core_set_play_level(this->lc, level);
}

String^ Linphone::Core::LinphoneCore::SoundPlaybackDevice::get()
{
	API_LOCK;
	String^ device = nullptr;
	const char *ccname = linphone_core_get_playback_device(this->lc);
	if (ccname != NULL) {
		device = Utils::cctops(ccname);
	}
	return device;
}

void Linphone::Core::LinphoneCore::SoundPlaybackDevice::set(String^ soundDeviceId)
{
	API_LOCK;
	const char *ccname = Utils::pstoccs(soundDeviceId);
	linphone_core_set_playback_device(this->lc, ccname);
	delete ccname;
}

::LinphoneCore* Linphone::Core::LinphoneCore::GetCorePtr()
{
    return lc;
}