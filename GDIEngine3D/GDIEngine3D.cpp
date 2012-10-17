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

BOOL WINAPI RenderPoolViewportRemoveByIndex(HRENDERPOOL hRenderPool, UINT uViewportIndex)
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

BOOL WINAPI RenderPoolViewportScreenSetByIndex(
	HRENDERPOOL hRenderPool, 
	UINT uViewportIndex, 
	HDC hDcNew,
	HDC &hDcOld
) {
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
		bResult = ((LPRENDER_POOL)Master)->setViewportScreen(uViewportIndex, hDcNew, hDcOld);

		thread_safety::UnlockObjectRegistered((size_t)hRenderPool, Master);
	}

	return bResult;
}

BOOL WINAPI RenderPoolViewportScreenSetByID(
	HRENDERPOOL hRenderPool, 
	DWORD dwViewportID, 
	HDC hDcNew,
	HDC &hDcOld
) {
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
		bResult = ((LPRENDER_POOL)Master)->setViewportScreen(dwViewportID, hDcNew, hDcOld);

		thread_safety::UnlockObjectRegistered((size_t)hRenderPool, Master);
	}

	return bResult;
}

BOOL WINAPI RenderPoolViewportGetCount(HRENDERPOOL hRenderPool, UINT &uVpCount)
{
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
		uVpCount = ((LPRENDER_POOL)Master)->getViewportCount();

		thread_safety::UnlockObjectRegistered((size_t)hRenderPool, Master);
	}

	return isValid;
}

BOOL WINAPI RenderPoolRenderWorld(HRENDERPOOL hRenderPool, DWORD &dwResult)
{
	thread_safety::LockModule(INFINITE);

	LPUNKNOWN Master = NULL;

	BOOL isValid = thread_safety::IsObjectRegistered((size_t)hRenderPool, typeid(RENDER_POOL));

	thread_safety::UnlockModule();

	if (isValid) dwResult = ((LPRENDER_POOL)Master)->RenderWorld();

	return isValid;
}
// ============================================================================
// CViewport library interface implementation

//HVIEWPORT WINAPI ViewportCreate(LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode)
//{
//	return (HVIEWPORT)CreateObject(new VIEWPORT(uVpWidth, uVpHeight, rMode));
//}

HVIEWPORT WINAPI ViewportCreate(RENDER_MODE rMode)
{
	return (HVIEWPORT)CreateObject(new VIEWPORT(rMode));
}

BOOL WINAPI ViewportSizeSet(HVIEWPORT hViewport, LONG uVpWidth, LONG uVpHeight)
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
		bResult = ((LPVIEWPORT)Master)->setSize(uVpWidth, uVpHeight);

		thread_safety::UnlockObjectRegistered((size_t)hViewport, Master);
	}

	return bResult;
}

BOOL WINAPI ViewportSizeGet(HVIEWPORT hViewport, LONG &uVpWidth, LONG &uVpHeight)
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
		((LPVIEWPORT)Master)->getSize(uVpWidth, uVpHeight);

		thread_safety::UnlockObjectRegistered((size_t)hViewport, Master);
	}

	return bResult;
}

BOOL WINAPI ViewportRenderModeSet(HVIEWPORT hViewport, RENDER_MODE rMode)
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

BOOL WINAPI ViewportRender(HVIEWPORT hViewport, HSCENE hScene, HCAMERA hCam, HDC hDcScreen)
{
	BOOL bResult = FALSE;

	thread_safety::LockModule(INFINITE);

	LPUNKNOWN	Master = NULL,
				Slave1 = NULL,
				Slave2 = NULL;

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hViewport, typeid(VIEWPORT))
		&& thread_safety::IsObjectRegistered((size_t)hScene, typeid(SCENE3D))
		&& thread_safety::IsObjectRegistered((size_t)hCam, typeid(CAMERA3D))
	) {
		thread_safety::LockObjectRegistered((size_t)hViewport, Master);
		thread_safety::LockObjectRegistered((size_t)hScene, Slave1);
		thread_safety::LockObjectRegistered((size_t)hCam, Slave2);
	}

	thread_safety::UnlockModule();

	if (isValid)
	{
		bResult = ((LPVIEWPORT)Master)->Render(((LPSCENE3D)Slave1), ((LPCAMERA3D)Slave2), hDcScreen);

		thread_safety::UnlockObjectRegistered((size_t)hViewport, Master);
		thread_safety::UnlockObjectRegistered((size_t)hScene, Slave1);
		thread_safety::UnlockObjectRegistered((size_t)hCam, Slave2);
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
		((LPCAMERA3D)Master)->Translate(fX, fY, fZ);

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Master);
	}

	return isValid;
}

BOOL WINAPI CameraRotate(HCAMERA hCamera, FLOAT fXRoll, FLOAT fYYaw, FLOAT fZPitch)
{
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
		((LPCAMERA3D)Master)->Rotate(fXRoll, fYYaw, fZPitch);

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Master);
	}

	return isValid;
}

BOOL WINAPI CameraNameSet(HCAMERA hCamera, LPTSTR tcsName)
{
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
		((LPCAMERA3D)Master)->setName(tstring(tcsName != NULL ? tcsName : _T("")));

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Master);
	}

	return isValid;
}

BOOL WINAPI CameraNameGet(HCAMERA hCamera, LPTSTR tcsName, UINT &tcsCharCount)
{
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
		tcsCharCount = tcsName == NULL ? 
			((LPCAMERA3D)Master)->getName().length() : 
		((LPCAMERA3D)Master)->getName().copy(tcsName, tcsCharCount, 0);


		thread_safety::UnlockObjectRegistered((size_t)hCamera, Master);
	}

	return isValid;
}

BOOL WINAPI CameraFocalDistanceSet(HCAMERA hCamera, FLOAT fFd)
{
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
		((LPCAMERA3D)Master)->setFDist(fFd);

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Master);
	}

	return isValid;
}

BOOL WINAPI CameraFocalDistanceGet(HCAMERA hCamera, FLOAT &fd)
{
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
		fd = ((LPCAMERA3D)Master)->getFDist();

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Master);
	}

	return isValid;
}

BOOL WINAPI CameraHorizontalFOVSet(HCAMERA hCamera, FLOAT fHFov)
{
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
		((LPCAMERA3D)Master)->setHFov(fHFov);

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Master);
	}

	return isValid;
}

BOOL WINAPI CameraHorizontalFOVGet(HCAMERA hCamera, FLOAT &fHFov)
{
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
		fHFov = ((LPCAMERA3D)Master)->getHFov();

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Master);
	}

	return isValid;
}

BOOL WINAPI CameraVerticalFOVSet(HCAMERA hCamera, FLOAT fVFov)
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
		((LPCAMERA3D)Master)->setVFov(fVFov);

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Master);

		bResult = isValid;
	}

	return bResult;
}

BOOL WINAPI CameraVerticalFOVGet(HCAMERA hCamera, FLOAT &fVFov)
{
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
		fVFov = ((LPCAMERA3D)Master)->getVFov();

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Master);
	}

	return isValid;
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

		OBJECTS_LIST garbage;
		((LPSCENE3D)Master)->Clear(garbage);
		__foreach(OBJECTS_LIST::iterator, entry, garbage) delete *entry;

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

BOOL WINAPI SceneAmbientColorSet(HSCENE hScene, COLORREF color)
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

BOOL WINAPI SceneAmbientColorGet(HSCENE hScene, COLORREF &color)
{
	LPUNKNOWN Master = NULL;

	thread_safety::LockModule(INFINITE);

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hScene, typeid(SCENE3D)))
		thread_safety::LockObjectRegistered((size_t)hScene, Master);

	thread_safety::UnlockModule();

	if (isValid)
	{
		color = ((LPSCENE3D)Master)->getAmbientColor();

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return isValid;
}

BOOL WINAPI SceneObjectRemove(HSCENE hScene, SCENE_OBJECT scObject)
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
		CLASS_ID	clsID		= _EXTRACT_OBJECT_CLSID(scObject);
		size_t		uObjIndex	= _EXTRACT_OBJECT_INDEX(scObject);

		LPOBJECT3D victim = ((LPSCENE3D)Master)->DeleteObject(clsID, uObjIndex);
		if (bResult = victim != NULL) delete victim;

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

BOOL WINAPI SceneObjectTranslate(HSCENE hScene, SCENE_OBJECT scObject, FLOAT fX, FLOAT fY, FLOAT fZ)
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
		CLASS_ID	clsID		= _EXTRACT_OBJECT_CLSID(scObject);
		size_t		uObjIndex	= _EXTRACT_OBJECT_INDEX(scObject);

		LPOBJECT3D victim = ((LPSCENE3D)Master)->getObject(clsID, uObjIndex);
		if (bResult = victim != NULL)
		{
			victim->Translate(fX, fY, fZ);
		}		

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

BOOL WINAPI SceneObjectRotate(HSCENE hScene, SCENE_OBJECT scObject, FLOAT fXRoll, FLOAT fYYaw, FLOAT fZPitch)
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
		CLASS_ID	clsID		= _EXTRACT_OBJECT_CLSID(scObject);
		size_t		uObjIndex	= _EXTRACT_OBJECT_INDEX(scObject);

		LPOBJECT3D victim = ((LPSCENE3D)Master)->getObject(clsID, uObjIndex);
		if (bResult = victim != NULL)
		{
			victim->Translate(fXRoll, fYYaw, fZPitch);
		}		

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

BOOL WINAPI SceneObjectNameSet(HSCENE hScene, SCENE_OBJECT scObject, LPTSTR tcsName)
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
		LPOBJECT3D victim = ((LPSCENE3D)Master)->getObject(
			_EXTRACT_OBJECT_CLSID(scObject), 
			_EXTRACT_OBJECT_INDEX(scObject)
			);

		if (bResult = victim != NULL)
		{
			victim->setName(tstring(tcsName != NULL ? tcsName : _T("")));
		}

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

BOOL WINAPI SceneObjectNameGet(HSCENE hScene, SCENE_OBJECT scObject, LPTSTR tcsName, UINT &tcsCharCount)
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
		LPOBJECT3D victim = ((LPSCENE3D)Master)->getObject(
			_EXTRACT_OBJECT_CLSID(scObject), 
			_EXTRACT_OBJECT_INDEX(scObject)
			);

		if ( bResult = victim != NULL )
		{
			tcsCharCount = tcsName == NULL ?
				victim->getName().length() :
			victim->getName().copy(tcsName, tcsCharCount, 0);
		}

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

BOOL WINAPI SceneObjectColorSet(HSCENE hScene, SCENE_OBJECT scObject, COLORREF color)
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
		LPOBJECT3D victim = ((LPSCENE3D)Master)->getObject(
			_EXTRACT_OBJECT_CLSID(scObject), 
			_EXTRACT_OBJECT_INDEX(scObject)
			);

		if (bResult = victim != NULL)
		{
			((IColorable*)victim)->setColor(color);
		}

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

BOOL WINAPI SceneObjectColorGet(HSCENE hScene, SCENE_OBJECT scObject, COLORREF &color)
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
		LPOBJECT3D victim = ((LPSCENE3D)Master)->getObject(
			_EXTRACT_OBJECT_CLSID(scObject), 
			_EXTRACT_OBJECT_INDEX(scObject)
			);

		if ( bResult = victim != NULL )
		{
			color = ((IColorable*)victim)->getColor();
		}

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

SCENE_OBJECT SceneObjectAdd(HSCENE hScene, LPOBJECT3D lpObject)
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
		if (((LPSCENE3D)Master)->AddObject(lpObject))
		{
			scoResult = _MAKE_SCENE_OBJECT(
				lpObject->getClassID(), 
				((LPSCENE3D)Master)->findObjectIndex(lpObject)
				);
		}

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return scoResult;
}

SCENE_OBJECT WINAPI SceneOmniLightCreate(HSCENE hScene, FLOAT Power, COLORREF Color)
{
	return SceneObjectAdd(hScene, new OMNILIGHT3D(Color, Power));
}

SCENE_OBJECT WINAPI SceneSphereCreate(HSCENE hScene, FLOAT Radius, UINT Precision, COLORREF Color)
{
	return SceneObjectAdd(hScene, new SPHERE3D(Radius, 0, 0, float(2 * M_PI), Precision, Color));
}

// TODO: implement CScene findObjectIndex, getFirstObject, getObject methods
