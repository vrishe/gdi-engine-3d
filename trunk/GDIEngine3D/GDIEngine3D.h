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
	BOOL EngineObjectRelease(HGDIENGINE3DOBJ hGDIEngineObject);

// ============================================================================
// CRenderPool library interface declaration

GDIENGINE3D_USERAPI 
	HRENDERPOOL RenderPoolCreate();

GDIENGINE3D_USERAPI
	DWORD RenderPoolViewportAdd(HRENDERPOOL hRenderPool, HVIEWPORT hViewport, HCAMERA hCamera, HDC hDCScreen);

GDIENGINE3D_USERAPI
	BOOL RenderPoolViewportRemoveByIndex(HRENDERPOOL hRenderPool, UINT uViewportIndex);

GDIENGINE3D_USERAPI
	BOOL RenderPoolViewportRemoveByID(HRENDERPOOL hRenderPool, DWORD dwViewportID);

GDIENGINE3D_USERAPI
	BOOL RenderPoolViewportScreenSetByIndex(
		HRENDERPOOL hRenderPool, 
		UINT uViewportIndex, 
		HDC hDCNew,
		HDC &hDCOld
		);

GDIENGINE3D_USERAPI
	BOOL RenderPoolViewportScreenSetByID(
		HRENDERPOOL hRenderPool, 
		DWORD dwViewportID,
		HDC hDCNew,
		HDC &hDCOld
		);

GDIENGINE3D_USERAPI
	BOOL RenderPoolViewportGetCount(HRENDERPOOL hRenderPool, UINT &uVpCount);

GDIENGINE3D_USERAPI
	BOOL RenderPoolRenderWorld(HRENDERPOOL hRenderPool, DWORD &dwResult);

// ============================================================================
// CViewport library interface implementation

GDIENGINE3D_USERAPI 
	//HVIEWPORT ViewportCreate(LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode);
	HVIEWPORT ViewportCreate(RENDER_MODE rMode);

GDIENGINE3D_USERAPI
	BOOL ViewportSizeSet(HVIEWPORT hViewport, LONG uVpWidth, LONG uVpHeight);

GDIENGINE3D_USERAPI
	BOOL ViewportSizeGet(HVIEWPORT hViewport, LONG &uVpWidth, LONG &uVpHeight);

GDIENGINE3D_USERAPI
	BOOL ViewportRenderModeSet(HVIEWPORT hViewport, RENDER_MODE rMode);

GDIENGINE3D_USERAPI
	BOOL ViewportRenderModeGet(HVIEWPORT hViewport, RENDER_MODE &rMode);

GDIENGINE3D_USERAPI
	BOOL ViewportRender(HVIEWPORT hViewport, HSCENE hScene, HCAMERA hCam, HDC hDcScreen);

// ============================================================================
// CCamera library interface implementation

GDIENGINE3D_USERAPI 
	HCAMERA CameraCreate(PROJECTION_TYPE pt, FLOAT hFOV, FLOAT vFOV);

GDIENGINE3D_USERAPI 
	BOOL CameraTranslate(HCAMERA hCamera, FLOAT fX, FLOAT fY, FLOAT fZ);

GDIENGINE3D_USERAPI 
	BOOL CameraRotate(HCAMERA hCamera, FLOAT Roll, FLOAT Yaw, FLOAT Pitch);

GDIENGINE3D_USERAPI 
	BOOL CameraNameSet(HCAMERA hCamera, LPTSTR tcsName);

GDIENGINE3D_USERAPI 
	BOOL CameraNameGet(HCAMERA hCamera, LPTSTR tcsName, UINT &tcsCharCount);

GDIENGINE3D_USERAPI 
	BOOL CameraFocalDistanceSet(HCAMERA hCamera, FLOAT fd);

GDIENGINE3D_USERAPI 
	BOOL CameraFocalDistanceGet(HCAMERA hCamera, FLOAT &fd);

GDIENGINE3D_USERAPI 
	BOOL CameraNearClipSet(HCAMERA hCamera, FLOAT fd);

GDIENGINE3D_USERAPI 
	BOOL CameraNearClipGet(HCAMERA hCamera, FLOAT &fd);

GDIENGINE3D_USERAPI 
	BOOL CameraFarClipSet(HCAMERA hCamera, FLOAT fd);

GDIENGINE3D_USERAPI 
	BOOL CameraFarClipGet(HCAMERA hCamera, FLOAT &fd);

GDIENGINE3D_USERAPI 
	BOOL CameraHorizontalFOVSet(HCAMERA hCamera, FLOAT fHFov);

GDIENGINE3D_USERAPI 
	BOOL CameraHorizontalFOVGet(HCAMERA hCamera, FLOAT &fHFov);

GDIENGINE3D_USERAPI 
	BOOL CameraVerticalFOVSet(HCAMERA hCamera, FLOAT fVFov);

GDIENGINE3D_USERAPI 
	BOOL CameraVerticalFOVGet(HCAMERA hCamera, FLOAT &fVFov);

// ============================================================================
// CScene library interface implementation

GDIENGINE3D_USERAPI
	HSCENE SceneCreate(); 

GDIENGINE3D_USERAPI
	BOOL SceneClear(HSCENE hScene);

GDIENGINE3D_USERAPI
	BOOL SceneAmbientColorSet(HSCENE hScene, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL SceneAmbientColorGet(HSCENE hScene, COLORREF &color);

GDIENGINE3D_USERAPI
	BOOL SceneObjectRemove(HSCENE hScene, SCENE_OBJECT scObject);

GDIENGINE3D_USERAPI 
	BOOL SceneObjectTranslate(HSCENE hScene, SCENE_OBJECT scObject, FLOAT fX, FLOAT fY, FLOAT fZ);

GDIENGINE3D_USERAPI 
	BOOL SceneObjectRotate(HSCENE hScene, SCENE_OBJECT scObject, FLOAT Roll, FLOAT Yaw, FLOAT Pitch);

GDIENGINE3D_USERAPI
	BOOL SceneObjectNameSet(HSCENE hScene, SCENE_OBJECT scObject, LPTSTR tcsName);

GDIENGINE3D_USERAPI
	BOOL SceneObjectNameGet(HSCENE hScene, SCENE_OBJECT scObject, LPTSTR tcsName, UINT &tcsCharCount);

GDIENGINE3D_USERAPI
	BOOL SceneObjectColorSet(HSCENE hScene, SCENE_OBJECT scObject, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL SceneObjectColorGet(HSCENE hScene, SCENE_OBJECT scObject, COLORREF &color);

GDIENGINE3D_USERAPI
	BOOL SceneMeshSelfIlluminationSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT self_illumination);

GDIENGINE3D_USERAPI
	BOOL SceneMeshSelfIlluminationGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &self_illumination);

GDIENGINE3D_USERAPI
	SCENE_OBJECT SceneSphereCreate(HSCENE hScene, FLOAT radius, FLOAT crop, FLOAT sliceFrom, FLOAT sliceTo, UINT precision, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL SceneSphereRadiusSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT radius);

GDIENGINE3D_USERAPI
	BOOL SceneSphereRadiusGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &radius);

GDIENGINE3D_USERAPI
	BOOL SceneSpherePrecisionSet(HSCENE hScene, SCENE_OBJECT scObject, UINT precision);

GDIENGINE3D_USERAPI
	BOOL SceneSpherePrecisionGet(HSCENE hScene, SCENE_OBJECT scObject, UINT &precision);

GDIENGINE3D_USERAPI
	BOOL SceneSphereCropSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT crop);

GDIENGINE3D_USERAPI
	BOOL SceneSphereCropGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &crop);

GDIENGINE3D_USERAPI
	BOOL SceneSphereSliceFromSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT from);

GDIENGINE3D_USERAPI
	BOOL SceneSphereSliceFromGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &from);

GDIENGINE3D_USERAPI
	BOOL SceneSphereSliceToSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT to);

GDIENGINE3D_USERAPI
	BOOL SceneSphereSliceToGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &to);

GDIENGINE3D_USERAPI
	SCENE_OBJECT ScenePyramidCreate(HSCENE hScene, FLOAT height, FLOAT baseLength, FLOAT baseWidth, FLOAT topLength, FLOAT topWidth, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL ScenePyramidHeightSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT height);

GDIENGINE3D_USERAPI
	BOOL ScenePyramidHeightGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &height);

GDIENGINE3D_USERAPI
	BOOL ScenePyramidBaseLengthSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT baseLength);

GDIENGINE3D_USERAPI
	BOOL ScenePyramidBaseLengthGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &baseLength);

GDIENGINE3D_USERAPI
	BOOL ScenePyramidBaseWidthSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT baseWidth);

GDIENGINE3D_USERAPI
	BOOL ScenePyramidBaseWidthGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &baseWidth);

GDIENGINE3D_USERAPI
	BOOL ScenePyramidTopLengthSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT topLength);

GDIENGINE3D_USERAPI
	BOOL ScenePyramidTopLengthGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &topLength);

GDIENGINE3D_USERAPI
	BOOL ScenePyramidTopWidthSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT topWidth);

GDIENGINE3D_USERAPI
	BOOL ScenePyramidTopWidthGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &topWidth);

GDIENGINE3D_USERAPI
	SCENE_OBJECT SceneConeCreate(HSCENE hScene, FLOAT height, FLOAT baseRadius, FLOAT topRadius, UINT precision, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL SceneConeBaseRadiusSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT baseRadius);

GDIENGINE3D_USERAPI
	BOOL SceneConeBaseRadiusGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &baseRadius);

GDIENGINE3D_USERAPI
	BOOL SceneConeTopRadiusSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT topRadius);

GDIENGINE3D_USERAPI
	BOOL SceneConeTopRadiusGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &topRadius);

GDIENGINE3D_USERAPI
	BOOL SceneConeHeightSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT height);

GDIENGINE3D_USERAPI
	BOOL SceneConeHeightGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &height);

GDIENGINE3D_USERAPI
	BOOL SceneConePrecisionSet(HSCENE hScene, SCENE_OBJECT scObject, UINT precision);

GDIENGINE3D_USERAPI
	BOOL SceneConePrecisionGet(HSCENE hScene, SCENE_OBJECT scObject, UINT &precision);

GDIENGINE3D_USERAPI
	SCENE_OBJECT SceneOmniLightCreate(HSCENE hScene, FLOAT Power, COLORREF color);

GDIENGINE3D_USERAPI
	BOOL SceneLightPowerSet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT power);

GDIENGINE3D_USERAPI
	BOOL SceneLightPowerGet(HSCENE hScene, SCENE_OBJECT scObject, FLOAT &power);

#ifdef __cplusplus
}
#endif

