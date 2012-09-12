// GDIEngine3D.h: Contains declarations that describe the interface functions 
//				  and necessary data structures of GDIEngine3D.dll

#pragma once

#include <Windows.h>

#ifdef GDIENGINE3D_EXPORTS
#define GDIENGINE3D_USERAPI __declspec(dllexport)
#else
#define GDIENGINE3D_USERAPI __declspec(dllimport)
#endif

#include "Render.h"

typedef HANDLE HRENDERPOOL3D;

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// CRenderPool library interface declaration

GDIENGINE3D_USERAPI 
	HRENDERPOOL3D WINAPI CreateRenderPool3D();

GDIENGINE3D_USERAPI 
	BOOL WINAPI ReleaseRenderPool3D(HRENDERPOOL3D hRp3D);

#ifdef __cplusplus
}
#endif

