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
typedef UINT_PTR SCENE_OBJECT;

DECLARE_HANDLE(HVIEWPORT);
DECLARE_HANDLE(HRENDERPOOL);

DECLARE_HANDLE(HCAMERA);
DECLARE_HANDLE(HOBJECT3D);


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
		HVIEWPORT	hViewport,
		HCAMERA		hCamera, 
		HDC			hDCScreen
		);

GDIENGINE3D_USERAPI
	BOOL WINAPI RenderPoolViewportRemoveByIndex(HRENDERPOOL hRenderPool, UINT_PTR uViewportIndex);

GDIENGINE3D_USERAPI
	BOOL WINAPI RenderPoolViewportRemoveByID(HRENDERPOOL hRenderPool, DWORD dwViewportID);

GDIENGINE3D_USERAPI
	HDC WINAPI RenderPoolViewportScreenSetByIndex(
		HRENDERPOOL hRenderPool, 
		UINT_PTR uViewportIndex, 
		HDC hDCScreen
		);

GDIENGINE3D_USERAPI
	HDC WINAPI RenderPoolViewportScreenSetByID(
		HRENDERPOOL hRenderPool, 
		DWORD dwViewportID, 
		HDC hDCScreen
		);

GDIENGINE3D_USERAPI
	UINT_PTR WINAPI RenderPoolViewportGetCount(HRENDERPOOL hRenderPool);


// ============================================================================
// CViewport library interface implementation

GDIENGINE3D_USERAPI 
	HVIEWPORT WINAPI ViewportCreate(LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode);

GDIENGINE3D_USERAPI
	BOOL WINAPI ViewportSizeSet(HVIEWPORT hViewport, LONG uVpWidth, LONG uVpHeight);

GDIENGINE3D_USERAPI
	BOOL WINAPI ViewportRenderModeBySet(HVIEWPORT hViewport, RENDER_MODE rMode);


// ============================================================================
// CCamera library interface implementation

GDIENGINE3D_USERAPI 
	HCAMERA WINAPI CameraCreate(LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode);

GDIENGINE3D_USERAPI 
	BOOL WINAPI CameraTranslate(HCAMERA hCamera, FLOAT fX, FLOAT fY, FLOAT fZ);

GDIENGINE3D_USERAPI 
	BOOL WINAPI CameraRotate(HCAMERA hCamera, FLOAT fXRoll, FLOAT fYYaw, FLOAT fZPitch);

// ============================================================================
// CScene library interface implementation

GDIENGINE3D_USERAPI
	HSCENE WINAPI SceneCreate(); 

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneClear(HSCENE hScene);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneSetAmbientColor(HSCENE hScene, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneGetAmbientColor(HSCENE hScene, COLORREF &color);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneObjectRemove(HSCENE hScene, SCENE_OBJECT scObject);

GDIENGINE3D_USERAPI
	SCENE_OBJECT WINAPI SceneSphereCreate(HSCENE hScene, FLOAT Radius, UINT Precision);



#ifdef __cplusplus
}
#endif

