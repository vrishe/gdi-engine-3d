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

DECLARE_HANDLE(HSCENE);
typedef UINT_PTR SCENE_OBJECT;

DECLARE_HANDLE(HVIEWPORT);
DECLARE_HANDLE(HRENDERPOOL);

DECLARE_HANDLE(HCAMERA);
DECLARE_HANDLE(HOBJECT3D);

#include "SharedDef.h"

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
	DWORD WINAPI RenderPoolViewportAdd(HRENDERPOOL hRenderPool, HVIEWPORT hViewport, HCAMERA hCamera, HDC hDCScreen);

GDIENGINE3D_USERAPI
	BOOL WINAPI RenderPoolViewportRemoveByIndex(HRENDERPOOL hRenderPool, UINT uViewportIndex);

GDIENGINE3D_USERAPI
	BOOL WINAPI RenderPoolViewportRemoveByID(HRENDERPOOL hRenderPool, DWORD dwViewportID);

GDIENGINE3D_USERAPI
	BOOL WINAPI RenderPoolViewportScreenSetByIndex(
		HRENDERPOOL hRenderPool, 
		UINT uViewportIndex, 
		HDC hDCNew,
		HDC &hDCOld
		);

GDIENGINE3D_USERAPI
	BOOL WINAPI RenderPoolViewportScreenSetByID(
		HRENDERPOOL hRenderPool, 
		DWORD dwViewportID,
		HDC hDCNew,
		HDC &hDCOld
		);

GDIENGINE3D_USERAPI
	BOOL WINAPI RenderPoolViewportGetCount(HRENDERPOOL hRenderPool, UINT &uVpCount);

GDIENGINE3D_USERAPI
	BOOL WINAPI RenderPoolRenderWorld(HRENDERPOOL hRenderPool, DWORD &dwResult);

// ============================================================================
// CViewport library interface implementation

GDIENGINE3D_USERAPI 
	//HVIEWPORT WINAPI ViewportCreate(LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode);
	HVIEWPORT WINAPI ViewportCreate(RENDER_MODE rMode);

GDIENGINE3D_USERAPI
	BOOL WINAPI ViewportSizeSet(HVIEWPORT hViewport, LONG uVpWidth, LONG uVpHeight);

GDIENGINE3D_USERAPI
	BOOL WINAPI ViewportSizeGet(HVIEWPORT hViewport, LONG &uVpWidth, LONG &uVpHeight);

GDIENGINE3D_USERAPI
	BOOL WINAPI ViewportRenderModeSet(HVIEWPORT hViewport, RENDER_MODE rMode);

GDIENGINE3D_USERAPI
	BOOL WINAPI ViewportRenderModeGet(HVIEWPORT hViewport, RENDER_MODE &rMode);

GDIENGINE3D_USERAPI
	BOOL WINAPI ViewportRender(HVIEWPORT hViewport, HSCENE hScene, HCAMERA hCam, HDC hDcScreen);

// ============================================================================
// CCamera library interface implementation

GDIENGINE3D_USERAPI 
	HCAMERA WINAPI CameraCreate(PROJECTION_TYPE pt, FLOAT hFOV, FLOAT vFOV);

GDIENGINE3D_USERAPI 
	BOOL WINAPI CameraTranslate(HCAMERA hCamera, FLOAT fX, FLOAT fY, FLOAT fZ);

GDIENGINE3D_USERAPI 
	BOOL WINAPI CameraRotate(HCAMERA hCamera, FLOAT fXRoll, FLOAT fYYaw, FLOAT fZPitch);

GDIENGINE3D_USERAPI 
	BOOL WINAPI CameraNameSet(HCAMERA hCamera, LPTSTR tcsName);

GDIENGINE3D_USERAPI 
	BOOL WINAPI CameraNameGet(HCAMERA hCamera, LPTSTR tcsName, UINT &tcsCharCount);

GDIENGINE3D_USERAPI 
	BOOL WINAPI CameraFocalDistanceSet(HCAMERA hCamera, FLOAT fd);

GDIENGINE3D_USERAPI 
	BOOL WINAPI CameraFocalDistanceGet(HCAMERA hCamera, FLOAT &fd);

GDIENGINE3D_USERAPI 
	BOOL WINAPI CameraHorizontalFOVSet(HCAMERA hCamera, FLOAT fHFov);

GDIENGINE3D_USERAPI 
	BOOL WINAPI CameraHorizontalFOVGet(HCAMERA hCamera, FLOAT &fHFov);

GDIENGINE3D_USERAPI 
	BOOL WINAPI CameraVerticalFOVSet(HCAMERA hCamera, FLOAT fVFov);

GDIENGINE3D_USERAPI 
	BOOL WINAPI CameraVerticalFOVGet(HCAMERA hCamera, FLOAT &fVFov);

// ============================================================================
// CScene library interface implementation

GDIENGINE3D_USERAPI
	HSCENE WINAPI SceneCreate(); 

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneClear(HSCENE hScene);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneAmbientColorSet(HSCENE hScene, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneAmbientColorGet(HSCENE hScene, COLORREF &color);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneObjectRemove(HSCENE hScene, SCENE_OBJECT scObject);

GDIENGINE3D_USERAPI 
	BOOL WINAPI SceneObjectTranslate(HSCENE hScene, SCENE_OBJECT scObject, FLOAT fX, FLOAT fY, FLOAT fZ);

GDIENGINE3D_USERAPI 
	BOOL WINAPI SceneObjectRotate(HSCENE hScene, SCENE_OBJECT scObject, FLOAT fXRoll, FLOAT fYYaw, FLOAT fZPitch);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneObjectNameSet(HSCENE hScene, SCENE_OBJECT scObject, LPTSTR tcsName);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneObjectNameGet(HSCENE hScene, SCENE_OBJECT scObject, LPTSTR tcsName, UINT &tcsCharCount);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneObjectColorSet(HSCENE hScene, SCENE_OBJECT scObject, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneObjectColorGet(HSCENE hScene, SCENE_OBJECT scObject, COLORREF &color);

GDIENGINE3D_USERAPI
	SCENE_OBJECT WINAPI SceneSphereCreate(HSCENE hScene, FLOAT Radius, UINT Precision, COLORREF Color);

GDIENGINE3D_USERAPI
	SCENE_OBJECT WINAPI SceneOmniLightCreate(HSCENE hScene, FLOAT Power, COLORREF Color);

#ifdef __cplusplus
}
#endif

