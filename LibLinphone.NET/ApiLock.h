
#pragma once

//#define API_LOCK_DEBUG
//#define API_LOCK_ENABLED

#ifdef API_LOCK_ENABLED
#define API_LOCK System::Threading::Monitor::Enter(System::Reflection::MethodBase::GetCurrentMethod())
#define API_UNLOCK System::Threading::Monitor::Exit(System::Reflection::MethodBase::GetCurrentMethod())
#else
#define API_LOCK ;
#define API_UNLOCK ;
#endif
