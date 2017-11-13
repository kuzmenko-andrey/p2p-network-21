#pragma once
#include"stdafx.h"

namespace  Client
{
	class MyCRITICAL
	{
		CRITICAL_SECTION cs = { 0 };
	public:
		MyCRITICAL(void){
			InitializeCriticalSection(&this->cs);
		};

		void EnterCritical(void) throw(){
			EnterCriticalSection(&this->cs);
		}

		BOOL TryCritical(void) throw() {
			return	TryEnterCriticalSection(&this->cs);
		}

		void LeaveCritical(void) throw(){
			LeaveCriticalSection(&this->cs);
		}

		~MyCRITICAL(){
			DeleteCriticalSection(&this->cs);
		}
		MyCRITICAL(const MyCRITICAL&) = delete;
		MyCRITICAL& operator=(const MyCRITICAL&) = delete;
	};
}