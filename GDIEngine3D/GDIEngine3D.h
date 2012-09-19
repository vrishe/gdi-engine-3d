// GDIEngine3D.h: Contains declarations that describe the interface functions 
//				  and necessary data structures of GDIEngine3D.dll

#pragma once

#include <Windows.h>

#ifdef GDIENGINE3D_EXPORTS
#define GDIENGINE3D_USERAPI __declspec(dllexport)

#include "SharedDef.h"
#else
#define GDIENGINE3D_USERAPI __declspec(dllimport)

#include <SharedDef.h>
#endif

typedef HANDLE HGDIENGINE3DOBJ;

DECLARE_HANDLE(HSCENE);
DECLARE_HANDLE(HCAMERA);
DECLARE_HANDLE(HOBJECT3D);
DECLARE_HANDLE(HVIEWPORT);
DECLARE_HANDLE(HRENDERPOOL);

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Common object interface declaration

GDIENGINE3D_USERAPI 
	BOOL WINAPI EngineObjectRelease(HGDIENGINE3DOBJ hGDIEngineObject);

// ============================================================================
// CRenderPool library interface declaration

GDIENGINE3D_USERAPI 
	HRENDERPOOL WINAPI RenderPoolCreate();

GDIENGINE3D_USERAPI
	DWORD WINAPI RenderPoolViewportAdd(
		HRENDERPOOL hRenderPool,
		HCAMERA		hCamera, 
		HDC			hDCScreen,
		UINT		uVpWidth,
		UINT		uVpHeight,
		RENDER_MODE rMode
		);

GDIENGINE3D_USERAPI
	BOOL WINAPI RenderPoolViewportRemoveByIndex(HRENDERPOOL hRenderPool, UINT_PTR uViewportIndex);

GDIENGINE3D_USERAPI
	BOOL WINAPI RenderPoolViewportRemoveByID(HRENDERPOOL hRenderPool, DWORD wdViewportID);

GDIENGINE3D_USERAPI
	UINT_PTR WINAPI RenderPoolViewportGetCount(HRENDERPOOL hRenderPool);

// ============================================================================
// CScene library interface implementation

GDIENGINE3D_USERAPI
	HSCENE WINAPI SceneCreate(); 

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneClear(HSCENE hScene);

// TODO: Explode thi method for a plenty of concretized i.e. SceneGetLightsCount and so on
//GDIENGINE3D_USERAPI
//	UINT_PTR WINAPI SceneGetObjectClassCount(HSCENE hScene, CLASS_ID clsID);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneSetAmbientColor(HSCENE hScene, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneGetAmbientColor(HSCENE hScene, COLORREF &color);

// Do not touch this, please
//GDIENGINE3D_USERAPI
//	BOOL WINAPI SceneAddObject(HSCENE hScene, HOBJECT3D hObj3D);
//
//GDIENGINE3D_USERAPI
//	BOOL WINAPI SceneDeleteObject(HSCENE hScene, HOBJECT3D hObj3D);
//
//GDIENGINE3D_USERAPI
//	BOOL WINAPI SceneGetFirstObject(HSCENE hScene, CLASS_ID clsID);

#ifdef __cplusplus
}
#endif

