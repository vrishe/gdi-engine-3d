// GDIEngine3D.cpp: Contains an implementation of the interface of GDIEngine3D.dll

#include "stdafx.h"

#include "Render.h"
#include "Primitives.h"

#include "thread_safety.h"

#include "GDIEngine3D.h"


void accessor_cleanup(LPUNKNOWN lpObject) { delete lpObject; }

inline size_t CreateObject(LPUNKNOWN lpObject)
{
	thread_safety::LockModule(INFINITE);
	size_t objectID = thread_safety::RegisterObject(lpObject);
	thread_safety::UnlockModule();

	return objectID;
}

inline LPUNKNOWN ReleaseObject(size_t uObjectIndex)
{
	thread_safety::LockModule(INFINITE);
	LPUNKNOWN underlier = thread_safety::DeregisterObject(uObjectIndex);
	thread_safety::UnlockModule();

	return underlier;
}

// ============================================================================
// Common object interface implementation

BOOL WINAPI EngineObjectRelease(HGDIENGINE3DOBJ hGDIEngineObject)
{
	LPUNKNOWN lpObject;

	delete (lpObject = ReleaseObject((size_t)hGDIEngineObject));

	return lpObject != NULL;
}


// ============================================================================
// CRenderPool library interface implementation

HRENDERPOOL WINAPI RenderPoolCreate()
{
	return (HRENDERPOOL)CreateObject(new RENDER_POOL());
}

DWORD WINAPI RenderPoolViewportAdd(
	HRENDERPOOL hRenderPool,
	HVIEWPORT	hViewport,
	HCAMERA		hCamera, 
	HDC			hDCScreen
) {
	DWORD dwResult = ((DWORD)0U);

	thread_safety::LockModule(INFINITE);

	LPUNKNOWN Master	= NULL,
			  Slave1	= NULL,
			  Slave2	= NULL;

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hRenderPool, typeid(RENDER_POOL))
		&& thread_safety::IsObjectRegistered((size_t)hViewport, typeid(VIEWPORT))
		&& thread_safety::IsObjectRegistered((size_t)hCamera, typeid(CAMERA3D)))
	{
		thread_safety::LockObjectRegistered((size_t)hRenderPool, Master);
		thread_safety::LockObjectRegistered((size_t)hViewport, Slave1);
		thread_safety::LockObjectRegistered((size_t)hCamera, Slave2);
	}

	thread_safety::UnlockModule();

	if (isValid)
	{
		dwResult = ((LPRENDER_POOL)Master)->
			addViewport((LPVIEWPORT)Slave1, (LPCAMERA3D)Slave2, hDCScreen);

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Slave2);
		thread_safety::UnlockObjectRegistered((size_t)hViewport, Slave1);
		thread_safety::UnlockObjectRegistered((size_t)hRenderPool, Master);
	}

	return dwResult;
}

BOOL WINAPI RenderPoolViewportRemoveByIndex(HRENDERPOOL hRenderPool, UINT_PTR uViewportIndex)
{
	BOOL bResult = FALSE;

	thread_safety::LockModule(INFINITE);

	LPUNKNOWN Master = NULL;

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hRenderPool, typeid(RENDER_POOL)))
	{
		thread_safety::LockObjectRegistered((size_t)hRenderPool, Master);
	}

	thread_safety::UnlockModule();

	if (isValid)
	{
		bResult = ((LPRENDER_POOL)Master)->delViewport(uViewportIndex);

		thread_safety::UnlockObjectRegistered((size_t)hRenderPool, Master);
	}

	return bResult;
}

BOOL WINAPI RenderPoolViewportRemoveByID(HRENDERPOOL hRenderPool, DWORD dwViewportID)
{
	BOOL bResult = FALSE;

	thread_safety::LockModule(INFINITE);

	LPUNKNOWN Master = NULL;

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hRenderPool, typeid(RENDER_POOL)))
	{
		thread_safety::LockObjectRegistered((size_t)hRenderPool, Master);
	}

	thread_safety::UnlockModule();

	if (isValid)
	{
		bResult = ((LPRENDER_POOL)Master)->delViewport(dwViewportID);

		thread_safety::UnlockObjectRegistered((size_t)hRenderPool, Master);
	}

	return bResult;
}

HDC WINAPI RenderPoolViewportScreenSetByIndex(
	HRENDERPOOL hRenderPool, 
	UINT_PTR uViewportIndex, 
	HDC hDCScreen
) {
	HDC hDCResult = NULL;

	thread_safety::LockModule(INFINITE);

	LPUNKNOWN Master = NULL;

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hRenderPool, typeid(RENDER_POOL)))
	{
		thread_safety::LockObjectRegistered((size_t)hRenderPool, Master);
	}

	thread_safety::UnlockModule();

	if (isValid)
	{
		hDCResult = ((LPRENDER_POOL)Master)->setViewportScreen(uViewportIndex, hDCScreen);

		thread_safety::UnlockObjectRegistered((size_t)hRenderPool, Master);
	}

	return hDCResult;
}

HDC WINAPI RenderPoolViewportScreenSetByID(
	HRENDERPOOL hRenderPool, 
	DWORD dwViewportID, 
	HDC hDCScreen
) {
	HDC hDCResult = NULL;

	thread_safety::LockModule(INFINITE);

	LPUNKNOWN Master = NULL;

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hRenderPool, typeid(RENDER_POOL)))
	{
		thread_safety::LockObjectRegistered((size_t)hRenderPool, Master);
	}

	thread_safety::UnlockModule();

	if (isValid)
	{
		hDCResult = ((LPRENDER_POOL)Master)->setViewportScreen(dwViewportID, hDCScreen);

		thread_safety::UnlockObjectRegistered((size_t)hRenderPool, Master);
	}

	return hDCResult;
}

UINT_PTR WINAPI RenderPoolViewportGetCount(HRENDERPOOL hRenderPool)
{
	UINT_PTR uResult = SIZE_MAX;

	thread_safety::LockModule(INFINITE);

	LPUNKNOWN Master = NULL;

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hRenderPool, typeid(RENDER_POOL)))
	{
		thread_safety::LockObjectRegistered((size_t)hRenderPool, Master);
	}

	thread_safety::UnlockModule();

	if (isValid)
	{
		uResult = ((LPRENDER_POOL)Master)->getViewportCount();

		thread_safety::UnlockObjectRegistered((size_t)hRenderPool, Master);
	}

	return uResult;
}


// ============================================================================
// CViewport library interface implementation

HVIEWPORT WINAPI ViewportCreate(LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode)
{
	return (HVIEWPORT)CreateObject(new VIEWPORT(uVpWidth, uVpHeight, rMode));
}

BOOL WINAPI ViewportSizeSet(HVIEWPORT hViewport, UINT uVpWidth, UINT uVpHeight)
{
	BOOL bResult = FALSE;

	thread_safety::LockModule(INFINITE);

	LPUNKNOWN Master = NULL;

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hViewport, typeid(VIEWPORT)))
	{
		thread_safety::LockObjectRegistered((size_t)hViewport, Master);
	}

	thread_safety::UnlockModule();

	if (isValid)
	{
		bResult = isValid;
		((LPVIEWPORT)Master)->setSize(uVpWidth, uVpHeight);

		thread_safety::UnlockObjectRegistered((size_t)hViewport, Master);
	}

	return bResult;
}

BOOL WINAPI ViewportRenderModeBySet(HVIEWPORT hViewport, RENDER_MODE rMode)
{
	BOOL bResult = FALSE;

	thread_safety::LockModule(INFINITE);

	LPUNKNOWN Master = NULL;

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hViewport, typeid(VIEWPORT)))
	{
		thread_safety::LockObjectRegistered((size_t)hViewport, Master);
	}

	thread_safety::UnlockModule();

	if (isValid)
	{
		bResult = isValid;
		((LPVIEWPORT)Master)->setRenderMode(rMode);

		thread_safety::UnlockObjectRegistered((size_t)hViewport, Master);
	}

	return bResult;
}


// ============================================================================
// CCamera library interface implementation

HCAMERA WINAPI CameraCreate(PROJECTION_TYPE pt, FLOAT hFOV, FLOAT vFOV)
{
	return (HCAMERA)CreateObject(new CAMERA3D(pt, hFOV, vFOV));
}

BOOL WINAPI CameraTranslate(HCAMERA hCamera, FLOAT fX, FLOAT fY, FLOAT fZ)
{
	BOOL bResult = FALSE;

	thread_safety::LockModule(INFINITE);

	LPUNKNOWN Master = NULL;

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hCamera, typeid(CAMERA3D)))
	{
		thread_safety::LockObjectRegistered((size_t)hCamera, Master);
	}

	thread_safety::UnlockModule();

	if (isValid)
	{
		bResult = isValid;
		((LPCAMERA3D)Master)->Translate(fX, fY, fZ);

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Master);
	}

	return bResult;
}

BOOL WINAPI CameraRotate(HCAMERA hCamera, FLOAT fXRoll, FLOAT fYYaw, FLOAT fZPitch)
{
	BOOL bResult = FALSE;

	thread_safety::LockModule(INFINITE);

	LPUNKNOWN Master = NULL;

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hCamera, typeid(CAMERA3D)))
	{
		thread_safety::LockObjectRegistered((size_t)hCamera, Master);
	}

	thread_safety::UnlockModule();

	if (isValid)
	{
		bResult = isValid;
		((LPCAMERA3D)Master)->Rotate(fXRoll, fYYaw, fZPitch);

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Master);
	}

	return bResult;
}

// ============================================================================
// CScene library interface implementation

#define CLSID_BITCOUNT sizeof(SCENE_OBJECT)
#define INDEX_BITCOUNT (7 * sizeof(SCENE_OBJECT))

#define _MAX_OBJECT_CLSID (SIZE_MAX << INDEX_BITCOUNT)
#define _MAX_OBJECT_INDEX (SIZE_MAX >> CLSID_BITCOUNT)

#define _MAKE_SCENE_OBJECT(clsID, objIndex)	\
	( (((SCENE_OBJECT)clsID) << INDEX_BITCOUNT) | (((SCENE_OBJECT)objIndex) & _MAX_OBJECT_INDEX) )

#define _EXTRACT_OBJECT_CLSID(scene_object)	(CLASS_ID)(((SCENE_OBJECT)scene_object) >> INDEX_BITCOUNT)
#define _EXTRACT_OBJECT_INDEX(scene_object) (size_t)(((SCENE_OBJECT)scene_object) & _MAX_OBJECT_INDEX)


HSCENE WINAPI SceneCreate() 
{
	return (HSCENE)CreateObject(new SCENE3D());
}

BOOL WINAPI SceneClear(HSCENE hScene)
{
	BOOL bResult = FALSE;

	LPUNKNOWN Master = NULL;

	thread_safety::LockModule(INFINITE);

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hScene, typeid(SCENE3D)))
		thread_safety::LockObjectRegistered((size_t)hScene, Master);

	thread_safety::UnlockModule();

	if (isValid)
	{
		bResult = isValid;
		((LPSCENE3D)Master)->Clear();

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

BOOL WINAPI SceneSetAmbientColor(HSCENE hScene, COLORREF color)
{
	BOOL bResult = FALSE;

	LPUNKNOWN Master = NULL;

	thread_safety::LockModule(INFINITE);

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hScene, typeid(SCENE3D)))
		thread_safety::LockObjectRegistered((size_t)hScene, Master);

	thread_safety::UnlockModule();

	if (isValid)
	{
		bResult = isValid;
		((LPSCENE3D)Master)->setAmbientColor(color);

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

BOOL WINAPI SceneGetAmbientColor(HSCENE hScene, COLORREF &color)
{
	BOOL bResult = FALSE;

	LPUNKNOWN Master = NULL;

	thread_safety::LockModule(INFINITE);

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hScene, typeid(SCENE3D)))
		thread_safety::LockObjectRegistered((size_t)hScene, Master);

	thread_safety::UnlockModule();

	if (isValid)
	{
		bResult = isValid;
		color = ((LPSCENE3D)Master)->getAmbientColor();

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

BOOL WINAPI SceneObjectRemove(HSCENE hScene, SCENE_OBJECT scObject)
{
	BOOL bResult;

	LPUNKNOWN Master = NULL;

	thread_safety::LockModule(INFINITE);

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hScene, typeid(SCENE3D)))
		thread_safety::LockObjectRegistered((size_t)hScene, Master);

	thread_safety::UnlockModule();

	if (isValid)
	{
		CLASS_ID	clsID		= _EXTRACT_OBJECT_CLSID(scObject);
		size_t		uObjIndex	= _EXTRACT_OBJECT_INDEX(scObject);

		bResult = ((LPSCENE3D)Master)->DeleteObject(clsID, uObjIndex);

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

SCENE_OBJECT WINAPI SceneSphereCreate(HSCENE hScene, FLOAT Radius, UINT Precision)
{
	SCENE_OBJECT scoResult;

	LPUNKNOWN Master = NULL;

	thread_safety::LockModule(INFINITE);

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hScene, typeid(SCENE3D)))
		thread_safety::LockObjectRegistered((size_t)hScene, Master);

	thread_safety::UnlockModule();

	if (isValid)
	{
		LPSPHERE3D lpSphere = new SPHERE3D(Radius, 0, 0, float(2 * M_PI), Precision, RGB(255, 255, 255));
		if (((LPSCENE3D)Master)->AddObject(lpSphere))
		{
			size_t uSphereIndex = ((LPSCENE3D)Master)->findObjectIndex(lpSphere);
			scoResult = _MAKE_SCENE_OBJECT(lpSphere->getClassID(), uSphereIndex);
		}

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return scoResult;
}

// TODO: implement CScene findObjectIndex, getFirstObject, getObject methods
