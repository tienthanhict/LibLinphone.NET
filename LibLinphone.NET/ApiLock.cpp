///*
//ApiLock.cpp
//Copyright (C) 2015  Belledonne Communications, Grenoble, France
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//*/
//
//#include "ApiLock.h"
//
//namespace Linphone
//{
//    namespace Core
//    {
//		GlobalApiLock *GlobalApiLock::instance = nullptr;
//		std::mutex GlobalApiLock::instance_mutex;
//
//		GlobalApiLock::GlobalApiLock() : count(0), pool(nullptr)
//		{}
//
//		GlobalApiLock::~GlobalApiLock()
//		{}
//
//		GlobalApiLock * GlobalApiLock::Instance()
//		{
//			if (instance == nullptr) {
//				instance_mutex.lock();
//				if (instance == nullptr) {
//					instance = new GlobalApiLock();
//				}
//				instance_mutex.unlock();
//			}
//			return instance;
//		}
//
//		void GlobalApiLock::Lock(std::string func)
//		{
//			mutex.lock();
//		}
//
//		void GlobalApiLock::Unlock(std::string func)
//		{
//			mutex.unlock();
//		}
//
//		ApiLock::ApiLock(std::string func) : func(func)
//		{
//			GlobalApiLock::Instance()->Lock(func);
//		}
//
//		ApiLock::~ApiLock()
//		{
//			GlobalApiLock::Instance()->Unlock(func);
//		}
//    }
//}
