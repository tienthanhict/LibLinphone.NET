/*
Utils.cpp
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

//#include <Stringapiset.h>
#include <vcclr.h>
#include <time.h>

#include "Utils.h"
#include "LinphoneCall.h"
#include "LinphoneAddress.h"
#include "LinphoneAuthInfo.h"
#include "LinphoneProxyConfig.h"
#include "LinphoneCallLog.h"
#include "LinphoneCallParams.h"
#include "LinphoneCallStats.h"
#include "LinphoneChatMessage.h"
#include "LinphoneChatRoom.h"
#include "LpConfig.h"
#include "PayloadType.h"
#include "Transports.h"
#include "VideoPolicy.h"
#include "VideoSize.h"

using namespace System;
using namespace System::Reflection;

std::wstring Linphone::Core::Utils::UTF8ToUTF16(const char *utf8)
{
	if ((utf8 == nullptr) || (*utf8 == '\0'))
		return std::wstring();

	int utf8len = static_cast<int>(strlen(utf8));

	// Get the size to alloc for utf-16 string
	int utf16len = MultiByteToWideChar(CP_UTF8, 0, utf8, utf8len, nullptr, 0);
	if (utf16len == 0) {
		DWORD error = GetLastError();
		ms_error("Invalid UTF-8 character, can't convert to UTF-16: %d", error);
		return std::wstring();
	}

	// Do the conversion
	std::wstring utf16;
	utf16.resize(utf16len);
	if (MultiByteToWideChar(CP_UTF8, 0, utf8, utf8len, &utf16[0], utf16.length()) == 0) {
		DWORD error = GetLastError();
		ms_error("Error during string conversion from UTF-8 to UTF-16: %d", error);
		return std::wstring();
	}
	return utf16;
}

std::string Linphone::Core::Utils::UTF16ToUTF8(const wchar_t *utf16)
{
	if ((utf16 == nullptr) || (*utf16 == L'\0'))
		return std::string();

	// Get the size to alloc for utf-8 string
	int utf16len = static_cast<int>(wcslen(utf16));
	int utf8len = WideCharToMultiByte(CP_UTF8, 0, utf16, utf16len, NULL, 0, NULL, NULL);
	if (utf8len == 0) {
		DWORD error = GetLastError();
		ms_error("Invalid UTF-16 character, can't convert to UTF-8: %d", error);
		return std::string();
	}

	// Do the conversion
	std::string utf8;
	utf8.resize(utf8len);
	if (WideCharToMultiByte(CP_UTF8, 0, utf16, utf16len, &utf8[0], utf8.length(), NULL, NULL) == 0) {
		DWORD error = GetLastError();
		ms_error("Error during string conversion from UTF-16 to UTF-8: %d", error);
		return std::string();
	}
	return utf8;
}

const char* Linphone::Core::Utils::pstoccs(System::String^ ps)
{
	if (ps == nullptr || ps->Length == 0)
		return NULL;

    pin_ptr<const wchar_t> data = PtrToStringChars(ps);

	std::string s = Linphone::Core::Utils::UTF16ToUTF8((const wchar_t *)data);
	char* cc = (char*) malloc(s.length()+1);
	memcpy(cc, s.c_str(), s.length());
	cc[s.length()] = '\0';
	return cc;
}

System::String^ Linphone::Core::Utils::cctops(const char* cc)
{
	if (cc == NULL)
		return nullptr;

	std::wstring wid_str = Linphone::Core::Utils::UTF8ToUTF16(cc);
	const wchar_t* w_char = wid_str.c_str();
	return gcnew System::String(w_char);
}

void Linphone::Core::Utils::LinphoneCoreSetLogHandler(void* logfunc)
{
	API_LOCK;
	linphone_core_set_log_handler(static_cast<OrtpLogFunc>(logfunc));
}

void Linphone::Core::Utils::LinphoneCoreEnableLogCollection(bool enable)
{
	API_LOCK;
	linphone_core_enable_log_collection(enable ? LinphoneLogCollectionEnabledWithoutPreviousLogHandler : LinphoneLogCollectionDisabled);
    IO::FileInfo^ fileInfo = gcnew IO::FileInfo(Assembly::GetEntryAssembly()->Location);
    linphone_core_set_log_collection_path(pstoccs(fileInfo->Directory->FullName));
}

void Linphone::Core::Utils::LinphoneCoreSetLogLevel(int loglevel)
{
	API_LOCK;
	linphone_core_set_log_level(static_cast<OrtpLogLevel>(loglevel));
}

System::Object^ Linphone::Core::Utils::CreateLpConfig(void* config)
{
	return gcnew Linphone::Core::LpConfig((::LpConfig *)config);
}

System::Object^ Linphone::Core::Utils::CreateLpConfig(System::String^ configPath, System::String^ factoryConfigPath)
{
	return gcnew Linphone::Core::LpConfig(configPath, factoryConfigPath);
}

System::Object^ Linphone::Core::Utils::CreatePayloadType(void* pt)
{
	return gcnew Linphone::Core::PayloadType((::PayloadType *)pt);
}

System::Object^ Linphone::Core::Utils::CreateLinphoneCall(void* call)
{
	return gcnew Linphone::Core::LinphoneCall((::LinphoneCall *)call);
}

System::Object^ Linphone::Core::Utils::CreateLinphoneAddress(void* address)
{
	return gcnew Linphone::Core::LinphoneAddress((::LinphoneAddress *)address);
}

System::Object^ Linphone::Core::Utils::CreateLinphoneAddressFromUri(const char* uri)
{
	LinphoneAddress^ addr = gcnew Linphone::Core::LinphoneAddress(uri);
	if (addr->GetAddressPtr() != nullptr)
		return addr;
	return nullptr;
}

System::Object^ Linphone::Core::Utils::CreateLinphoneAuthInfo(void* auth_info)
{
	return gcnew Linphone::Core::LinphoneAuthInfo((::LinphoneAuthInfo *)auth_info);
}

System::Object^ Linphone::Core::Utils::CreateLinphoneAuthInfo(System::String^ username, System::String^ userid, System::String^ password, System::String^ ha1, System::String^ realm, System::String^ domain)
{
	return gcnew Linphone::Core::LinphoneAuthInfo(username, userid, password, ha1, realm, domain);
}

System::Object^ Linphone::Core::Utils::CreateLinphoneProxyConfig(void* proxy_config)
{
	return gcnew Linphone::Core::LinphoneProxyConfig((::LinphoneProxyConfig *)proxy_config);
}

System::Object^ Linphone::Core::Utils::CreateLinphoneCallLog(void* callLog)
{
	return gcnew Linphone::Core::LinphoneCallLog((::LinphoneCallLog *)callLog);
}

System::Object^ Linphone::Core::Utils::CreateLinphoneCallParams(void* callParams)
{
	return gcnew Linphone::Core::LinphoneCallParams((::LinphoneCallParams *)callParams);
}

System::Object^ Linphone::Core::Utils::CreateLinphoneCallStats(void* call, int mediaType)
{
	return gcnew Linphone::Core::LinphoneCallStats((::LinphoneCall *)call, (Linphone::Core::MediaType)mediaType);
}

System::Object^ Linphone::Core::Utils::CreateLinphoneCallStats(void *callStats)
{
	return gcnew Linphone::Core::LinphoneCallStats((::LinphoneCallStats *)callStats);
}

System::Object^ Linphone::Core::Utils::CreateTransports()
{
	return gcnew Linphone::Core::Transports();
}

System::Object^ Linphone::Core::Utils::CreateTransports(int udp_port, int tcp_port, int tls_port)
{
	return gcnew Linphone::Core::Transports(udp_port, tcp_port, tls_port);
}

System::Object^ Linphone::Core::Utils::CreateTransports(System::Object^ t)
{
	return gcnew Linphone::Core::Transports(dynamic_cast<Linphone::Core::Transports^>(t));
}

System::Object^ Linphone::Core::Utils::CreateVideoPolicy()
{
	return gcnew Linphone::Core::VideoPolicy();
}

System::Object^ Linphone::Core::Utils::CreateVideoPolicy(System::Boolean automaticallyInitiate, System::Boolean automaticallyAccept)
{
	return gcnew Linphone::Core::VideoPolicy(automaticallyInitiate, automaticallyAccept);
}

System::Object^ Linphone::Core::Utils::CreateVideoSize(int width, int height)
{
	return gcnew Linphone::Core::VideoSize(width, height);
}

System::Object^ Linphone::Core::Utils::CreateVideoSize(int width, int height, System::String^ name)
{
	return gcnew Linphone::Core::VideoSize(width, height, name);
}

void Linphone::Core::Utils::EchoCalibrationCallback(void *lc, int status, int delay_ms, void *data)
{
	API_LOCK;
    // Must implement
    throw gcnew NotImplementedException();
	//EchoCalibrationData *ecData = static_cast<EchoCalibrationData *>(data);
	//if (ecData != nullptr) {
	//	delete ecData;
	//}
	//Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCore^> *proxy = reinterpret_cast< Linphone::Core::RefToPtrProxy<Linphone::Core::LinphoneCore^> *>(linphone_core_get_user_data(static_cast<::LinphoneCore *>(lc)));
	//Linphone::Core::LinphoneCore^ lCore = (proxy) ? proxy->Ref() : nullptr;
	//Linphone::Core::EcCalibratorStatus ecStatus = (Linphone::Core::EcCalibratorStatus) status;
	//lCore->listener->EcCalibrationStatus(ecStatus, delay_ms);
}

 System::Object^ Linphone::Core::Utils::CreateLinphoneChatMessage(void* message)
 {
	 return gcnew Linphone::Core::LinphoneChatMessage((::LinphoneChatMessage *)message);
 }

 System::Object^ Linphone::Core::Utils::CreateLinphoneChatRoom(void* room)
 {
	 return gcnew Linphone::Core::LinphoneChatRoom((::LinphoneChatRoom *)room);
 }
