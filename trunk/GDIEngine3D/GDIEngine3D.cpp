// GDIEngine3D.cpp: Contains an implementation of the interface of GDIEngine3D.dll

#include "stdafx.h"
#include "GDIEngine3D.h"

// ============================================================================
// Internal mechanism, that allows to control library interface workflow safety
extern HANDLE				hProcessHeap;
extern std::deque<LPVOID>	utilizedMemory;

BOOL ObjectIsUsed(HANDLE obj, std::deque<LPVOID>::iterator &Where)
{
	for (std::deque<LPVOID>::iterator cur = utilizedMemory.begin(), end = utilizedMemory.end(); cur != end; cur++)
	{
		if (*cur == obj) 
		{
			Where = cur;
			return TRUE;
		}
	}

	return FALSE;
}

// ============================================================================
// CRenderPool library interface implementation

HRENDERPOOL3D WINAPI CreateRenderPool3D()
{
	LPRENDER_POOL obj = (LPRENDER_POOL)HeapAlloc(hProcessHeap, HEAP_ZERO_MEMORY, sizeof(RENDER_POOL));
	
	if (obj != NULL)
	{
		utilizedMemory.push_back(obj);
		
		*obj = RENDER_POOL();
		return (HRENDERPOOL3D)obj;
	}

	return NULL;
}

BOOL WINAPI ReleaseRenderPool3D(HRENDERPOOL3D hRp3D)
{
	std::deque<LPVOID>::iterator finder;
	if (hRp3D != NULL && ObjectIsUsed(hRp3D, finder)) 
	{
		utilizedMemory.erase(finder);
		((LPRENDER_POOL)hRp3D)->~ÑRenderPool();
		return HeapFree(hProcessHeap, 0, hRp3D);
	}

	return FALSE;
}

