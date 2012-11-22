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
	BOOL WINAPI CameraRotate(HCAMERA hCamera, FLOAT Roll, FLOAT Yaw, FLOAT Pitch);

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
	BOOL WINAPI SceneObjectRotate(HSCENE hScene, SCENE_OBJECT scObject, FLOAT Roll, FLOAT Yaw, FLOAT Pitch);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneObjectNameSet(HSCENE hScene, SCENE_OBJECT scObject, LPTSTR tcsName);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneObjectNameGet(HSCENE hScene, SCENE_OBJECT scObject, LPTSTR tcsName, UINT &tcsCharCount);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneObjectColorSet(HSCENE hScene, SCENE_OBJECT scObject, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneObjectColorGet(HSCENE hScene, SCENE_OBJECT scObject, COLORREF &color);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneMeshSelfIlluminationSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT self_illumination);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneMeshSelfIlluminationGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &self_illumination);

GDIENGINE3D_USERAPI
	SCENE_OBJECT WINAPI SceneSphereCreate(HSCENE hScene, FLOAT radius, FLOAT crop, FLOAT sliceFrom, FLOAT sliceTo, UINT precision, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneSphereRadiusSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT radius);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneSphereRadiusGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &radius);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneSpherePrecisionSet(HSCENE hScene, SCENE_OBJECT scObject, UINT precision);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneSpherePrecisionGet(HSCENE hScene, SCENE_OBJECT scObject, UINT &precision);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneSphereCropSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT crop);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneSphereCropGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &crop);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneSphereSliceFromSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT from);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneSphereSliceFromGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &from);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneSphereSliceToSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT to);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneSphereSliceToGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &to);

GDIENGINE3D_USERAPI
	SCENE_OBJECT WINAPI ScenePyramidCreate(HSCENE hScene, FLOAT height, FLOAT baseLength, FLOAT baseWidth, FLOAT topLength, FLOAT topWidth, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL WINAPI ScenePyramidHeightSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT height);

GDIENGINE3D_USERAPI
	BOOL WINAPI ScenePyramidHeightGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &height);

GDIENGINE3D_USERAPI
	BOOL WINAPI ScenePyramidBaseLengthSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT baseLength);

GDIENGINE3D_USERAPI
	BOOL WINAPI ScenePyramidBaseLengthGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &baseLength);

GDIENGINE3D_USERAPI
	BOOL WINAPI ScenePyramidBaseWidthSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT baseWidth);

GDIENGINE3D_USERAPI
	BOOL WINAPI ScenePyramidBaseWidthGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &baseWidth);

GDIENGINE3D_USERAPI
	BOOL WINAPI ScenePyramidTopLengthSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT topLength);

GDIENGINE3D_USERAPI
	BOOL WINAPI ScenePyramidTopLengthGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &topLength);

GDIENGINE3D_USERAPI
	BOOL WINAPI ScenePyramidTopWidthSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT topWidth);

GDIENGINE3D_USERAPI
	BOOL WINAPI ScenePyramidTopWidthGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &topWidth);

GDIENGINE3D_USERAPI
	SCENE_OBJECT WINAPI SceneOmniLightCreate(HSCENE hScene, FLOAT Power, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneLightPowerSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT power);

GDIENGINE3D_USERAPI
	BOOL WINAPI SceneLightPowerGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &power);

#ifdef __cplusplus
}
#endif
