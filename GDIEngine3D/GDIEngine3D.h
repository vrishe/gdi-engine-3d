// GDIEngine3D.h: Contains declarations that describe the interface functions 
//				  and necessary data structures of GDIEngine3D.dll

#pragma once

#include <Windows.h>

#ifdef GDIENGINE3D_EXPORTS
#define GDIENGINE3D_USERAPI __declspec(dllexport)
#else
#define GDIENGINE3D_USERAPI __declspec(dllimport)
#endif

typedef HANDLE HGDIENGINE3DOBJ;

typedef HGDIENGINE3DOBJ HRENDERPOOL;
typedef HGDIENGINE3DOBJ HSCENE;
typedef HGDIENGINE3DOBJ HOBJECT3D;

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
	BOOL WINAPI RenderPoolRemoveViewport(HRENDERPOOL hRenderPool, HSCENE hScn3D);

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

