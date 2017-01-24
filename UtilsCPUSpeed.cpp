// ---------------------------------------------------------------------------

#pragma hdrstop

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#include "UtilsCPUSpeed.h"

SIZE_T _GetCPUSpeed() {
	const DelayTime = 500;

	DWORD TimerHi, TimerLo;

	int PriorityClass = GetPriorityClass(GetCurrentProcess());
	int Priority      = GetThreadPriority(GetCurrentThread());
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

	Sleep(10);
	asm {
		dw 310Fh
		mov TimerLo, eax
		mov TimerHi, edx
	}

	Sleep(DelayTime);

	asm {
		dw 310Fh
		sub eax, TimerLo
		sbb edx, TimerHi
		mov TimerLo, eax
		mov TimerHi, edx
	}

	SetThreadPriority(GetCurrentThread(), Priority);
	SetPriorityClass(GetCurrentProcess(), PriorityClass);

	return 1000.0 * TimerLo / DelayTime;
}
