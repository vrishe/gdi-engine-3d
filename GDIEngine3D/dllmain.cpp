// dllmain.cpp: implements an entry point of GDIEngine3D.dll

#include "stdafx.h"
#include "thread_safety.h"

void accessor_cleanup(LPUNKNOWN lpObject);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		return thread_safety::InitializeHandles();
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		// Cleanup here
		thread_safety::ForeachObjectRegistered(accessor_cleanup);
		thread_safety::ReleaseHandles();
		break;
	}
	return TRUE;
}

