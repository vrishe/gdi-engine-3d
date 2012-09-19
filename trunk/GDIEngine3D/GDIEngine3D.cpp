// GDIEngine3D.cpp: Contains an implementation of the interface of GDIEngine3D.dll

#include "stdafx.h"

#include "Render.h"
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
	HCAMERA		hCamera, 
	HDC			hDCScreen,
	UINT		uVpWidth,
	UINT		uVpHeight,
	RENDER_MODE rMode
) {
	DWORD dwResult = ((DWORD)0U);

	thread_safety::LockModule(INFINITE);

	LPUNKNOWN Master	= NULL,
			  Slave		= NULL;

	BOOL isValid;
	if (isValid = thread_safety::IsObjectRegistered((size_t)hRenderPool, typeid(RENDER_POOL))
		&& thread_safety::IsObjectRegistered((size_t)hCamera, typeid(CAMERA3D)))
	{
		thread_safety::LockObjectRegistered((size_t)hRenderPool, Master);
		thread_safety::LockObjectRegistered((size_t)hCamera, Slave);
	}

	thread_safety::UnlockModule();

	if (isValid)
	{
		dwResult = ((LPRENDER_POOL)Master)->
			addViewport((LPCAMERA3D)Slave, hDCScreen, uVpWidth, uVpHeight, rMode);

		thread_safety::UnlockObjectRegistered((size_t)hCamera, Slave);
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

BOOL WINAPI RenderPoolViewportRemoveByID(HRENDERPOOL hRenderPool, DWORD wdViewportID)
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
		bResult = ((LPRENDER_POOL)Master)->delViewport(wdViewportID);

		thread_safety::UnlockObjectRegistered((size_t)hRenderPool, Master);
	}

	return bResult;
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
// CScene library interface implementation

HSCENE WINAPI SceneCreate() 
{
	return (HSCENE)CreateObject(new SCENE3D());
}

BOOL WINAPI SceneClear(HSCENE hScene)
{
	BOOL bResult;

	LPUNKNOWN Master = NULL;

	thread_safety::LockModule(INFINITE);

	if (bResult = thread_safety::IsObjectRegistered((size_t)hScene, typeid(SCENE3D)))
		thread_safety::LockObjectRegistered((size_t)hScene, Master);

	thread_safety::UnlockModule();

	if (bResult)
	{
		((LPSCENE3D)Master)->Clear();

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

BOOL WINAPI SceneSetAmbientColor(HSCENE hScene, COLORREF color)
{
	BOOL bResult;

	LPUNKNOWN Master = NULL;

	thread_safety::LockModule(INFINITE);

	if (bResult = thread_safety::IsObjectRegistered((size_t)hScene, typeid(SCENE3D)))
		thread_safety::LockObjectRegistered((size_t)hScene, Master);

	thread_safety::UnlockModule();

	if (bResult)
	{
		((LPSCENE3D)Master)->setAmbientColor(color);

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

BOOL WINAPI SceneGetAmbientColor(HSCENE hScene, COLORREF &color)
{
	BOOL bResult;

	LPUNKNOWN Master = NULL;

	thread_safety::LockModule(INFINITE);

	if (bResult = thread_safety::IsObjectRegistered((size_t)hScene, typeid(SCENE3D)))
		thread_safety::LockObjectRegistered((size_t)hScene, Master);

	thread_safety::UnlockModule();

	if (bResult)
	{
		color = ((LPSCENE3D)Master)->getAmbientColor();

		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
	}

	return bResult;
}

// TODO: implement CScene findObjectIndex, getFirstObject, getObject methods
