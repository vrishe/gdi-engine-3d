// dllmain.cpp: implements an entry point of GDIEngine3D.dll

#include "stdafx.h"
#include "thread_safety.h"

extern HANDLE					hProcessHeap	= NULL;
extern OBJECT_DESCRIPTION_LIST	utilizedMemory	= OBJECT_DESCRIPTION_LIST();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hProcessHeap = GetProcessHeap();
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		if (hProcessHeap != NULL)
		{
			for (std::list<LPVOID>::iterator cur = utilizedMemory.begin(), end = utilizedMemory.end(); cur != end; cur++)
			{
				HeapFree(hProcessHeap, 0, *cur);
			}
		}
		break;
	}
	return TRUE;
}

