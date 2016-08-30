/*
LinphoneCoreFactory.cpp
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

#include "LinphoneAuthInfo.h"
#include "LinphoneCoreFactory.h"
#include "LinphoneCore.h"
#include "ILinphoneCoreListener.h"
#include "LinphoneAddress.h"
#include "LpConfig.h"
#include "Transports.h"
#include "VideoPolicy.h"
#include "VideoSize.h"

using namespace System;
using namespace Linphone::Core;

#define MAX_TRACE_SIZE		2048
#define MAX_SUITE_NAME_SIZE	128

void LinphoneCoreFactory::CreateLinphoneCore(Linphone::Core::ILinphoneCoreListener^ listener)
{
	CreateLinphoneCore(listener, nullptr);
}

void LinphoneCoreFactory::CreateLinphoneCore(Linphone::Core::ILinphoneCoreListener^ listener, Linphone::Core::LpConfig^ config)
{
	if (this->linphoneCore == nullptr) {
		API_LOCK;
		if (this->linphoneCore == nullptr) {
			Utils::LinphoneCoreEnableLogCollection(true);
			this->linphoneCore = gcnew Linphone::Core::LinphoneCore(listener, config);
			this->linphoneCore->Init();
		}
	}
}

Linphone::Core::LpConfig^ LinphoneCoreFactory::CreateLpConfig(String^ configPath, String^ factoryConfigPath)
{
	API_LOCK;
	return dynamic_cast<Linphone::Core::LpConfig^>(Utils::CreateLpConfig(configPath, factoryConfigPath));
}

Linphone::Core::LinphoneAuthInfo^ LinphoneCoreFactory::CreateAuthInfo(String^ username, String^ userid, String^ password, String^ ha1, String^ realm, String^ domain)
{
	API_LOCK;
	return dynamic_cast<Linphone::Core::LinphoneAuthInfo^>(Utils::CreateLinphoneAuthInfo(username, userid, password, ha1, realm, domain));
}

Linphone::Core::LinphoneAddress^ LinphoneCoreFactory::CreateLinphoneAddress(String^ username, String^ domain, String^ displayName)
{
	API_LOCK;
	Linphone::Core::LinphoneAddress^ address = CreateLinphoneAddress("sip:user@domain.com");
	address->UserName = username;
	address->Domain = domain;
	address->DisplayName = displayName;
	return address;
}

Linphone::Core::LinphoneAddress^ LinphoneCoreFactory::CreateLinphoneAddress(String^ uri)
{
	API_LOCK;
	return dynamic_cast<Linphone::Core::LinphoneAddress^>(Utils::CreateLinphoneAddressFromUri(Utils::pstoccs(uri)));
}

Linphone::Core::Transports^ LinphoneCoreFactory::CreateTransports()
{
	API_LOCK;
	return dynamic_cast<Linphone::Core::Transports^>(Utils::CreateTransports());
}

Linphone::Core::Transports^ LinphoneCoreFactory::CreateTransports(int udp_port, int tcp_port, int tls_port)
{
	API_LOCK;
	return dynamic_cast<Linphone::Core::Transports^>(Utils::CreateTransports(udp_port, tcp_port, tls_port));
}

Linphone::Core::Transports^ LinphoneCoreFactory::CreateTransports(Linphone::Core::Transports^ t)
{
	API_LOCK;
	return dynamic_cast<Linphone::Core::Transports^>(Utils::CreateTransports(t));
}

Linphone::Core::VideoPolicy^ LinphoneCoreFactory::CreateVideoPolicy()
{
	API_LOCK;
	return dynamic_cast<Linphone::Core::VideoPolicy^>(Utils::CreateVideoPolicy());
}

Linphone::Core::VideoPolicy^ LinphoneCoreFactory::CreateVideoPolicy(Boolean automaticallyInitiate, Boolean automaticallyAccept)
{
	API_LOCK;
	return dynamic_cast<Linphone::Core::VideoPolicy^>(Utils::CreateVideoPolicy(automaticallyInitiate, automaticallyAccept));
}

Linphone::Core::VideoSize^ LinphoneCoreFactory::CreateVideoSize(int width, int height)
{
	API_LOCK;
	return dynamic_cast<Linphone::Core::VideoSize^>(Utils::CreateVideoSize(width, height));
}

Linphone::Core::VideoSize^ LinphoneCoreFactory::CreateVideoSize(int width, int height, String^ name)
{
	API_LOCK;
	return dynamic_cast<Linphone::Core::VideoSize^>(Utils::CreateVideoSize(width, height, name));
}

void LinphoneCoreFactory::SetLogLevel(OutputTraceLevel logLevel)
{
	API_LOCK;
	Linphone::Core::LinphoneCore::LogLevel = logLevel;
}

void LinphoneCoreFactory::ResetLogCollection()
{
	API_LOCK;
	Linphone::Core::LinphoneCore::ResetLogCollection();
}

Linphone::Core::LinphoneCore^ LinphoneCoreFactory::LinphoneCore::get()
{
	return this->linphoneCore;
}

void LinphoneCoreFactory::Destroy()
{
	API_LOCK;
	this->linphoneCore->Destroy();
	delete this->linphoneCore;
	this->linphoneCore = nullptr;
}

LinphoneCoreFactory::LinphoneCoreFactory() :
	linphoneCore(nullptr)
{
}

LinphoneCoreFactory::~LinphoneCoreFactory()
{
}