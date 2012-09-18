// GDIEngine3D.h: Contains declarations that describe the interface functions 
//				  and necessary data structures of GDIEngine3D.dll

#pragma once

#include <Windows.h>

#ifdef GDIENGINE3D_EXPORTS
#define GDIENGINE3D_USERAPI __declspec(dllexport)
#else
#define GDIENGINE3D_USERAPI __declspec(dllimport)
#endif

typedef HANDLE HRENDERPOOL3D;
typedef HANDLE HSCENE3D;
typedef HANDLE HOBJECT3D;

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// CRenderPool library interface declaration

GDIENGINE3D_USERAPI 
	HRENDERPOOL3D WINAPI CreateRenderPool3D();

GDIENGINE3D_USERAPI 
	BOOL WINAPI ReleaseRenderPool3D(HRENDERPOOL3D hRp3D);

GDIENGINE3D_USERAPI
	BOOL WINAPI RenderPoolAssignScene3D(HRENDERPOOL3D hRp3D, HSCENE3D hScn3D);

GDIENGINE3D_USERAPI
	BOOL WINAPI RenderPoolRemoveViewport3D(HRENDERPOOL3D hRp3D, HSCENE3D hScn3D);

#ifdef __cplusplus
}
#endif

