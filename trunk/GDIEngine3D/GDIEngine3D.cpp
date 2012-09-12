// GDIEngine3D.cpp: Contains an implementation of the interface of GDIEngine3D.dll

#include "stdafx.h"
#include "GDIEngine3D.h"

#include "Render.h"

// ============================================================================
// Internal mechanism, that allows to control library interface workflow safety
extern HANDLE				hProcessHeap;
extern std::list<LPVOID>	utilizedMemory;

BOOL ObjectIsUsed(HANDLE obj, std::list<LPVOID>::iterator &Where)
{
	for (std::list<LPVOID>::iterator cur = utilizedMemory.begin(), end = utilizedMemory.end(); cur != end; cur++)
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
		
		return (HRENDERPOOL3D)new(obj) RENDER_POOL; // This line depends of arguments, so it's not intended to be placed inside of the template creation func.
	}

	return NULL;
}

BOOL WINAPI ReleaseRenderPool3D(HRENDERPOOL3D hRp3D)
{
	// Whole function is intended to be a template release func.
	std::list<LPVOID>::iterator finder;
	if (hRp3D != NULL && ObjectIsUsed(hRp3D, finder)) 
	{
		utilizedMemory.erase(finder);
		((LPRENDER_POOL)hRp3D)->~ÑRenderPool();
		return HeapFree(hProcessHeap, 0, hRp3D);
	}

	return FALSE;
}

// TODO: Implement the most useful interface functions, such as: CreateLight, CreateCamera, RenderWorld(CRenderPool based) and so on 

