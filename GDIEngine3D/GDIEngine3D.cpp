// GDIEngine3D.cpp: Contains an implementation of the interface of GDIEngine3D.dll

#include "stdafx.h"
#include "GDIEngine3D.h"

#include "thread_safety.h"
#include "Render.h"

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

//
//UINT_PTR WINAPI SceneGetObjectClassCount(HSCENE hScene, CLASS_ID clsID) 
//{
//	BOOL isValid;
//
//	LPUNKNOWN Master = NULL;
//
//	thread_safety::LockModule(INFINITE);
//
//	if (isValid = thread_safety::IsObjectRegistered((size_t)hScene, typeid(SCENE3D)))
//		thread_safety::LockObjectRegistered((size_t)hScene, Master);
//
//	thread_safety::UnlockModule();
//
//	UINT_PTR uResult = UINT_MAX;
//	if (isValid)
//	{
//		((LPSCENE3D)Master)->getObjectClassCount(clsID);
//
//		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
//	}
//
//	return uResult;
//}

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

//BOOL WINAPI SceneAddObject(HSCENE hScene, HOBJECT3D hObj3D)
//{
//	BOOL bResult = FALSE;
//
//	LPUNKNOWN Scene			= NULL;
//	LPUNKNOWN Object		= NULL;
//
//	thread_safety::LockModule(INFINITE);
//	if ( bResult = _IS_HANDLE_VALID(SCENE3D, hScene) 
//		&& _IS_HANDLE_VALID(OBJECT3D, hObj3D) )
//	{
//		Scene	= ((LPMUTUAL_ACCESSIBLE)hScene)->Lock();
//		Object	= ((LPMUTUAL_ACCESSIBLE)hObj3D)->Lock();
//	}
//	thread_safety::UnlockModule();
//
//	if (bResult)
//	{
//		bResult = ((LPSCENE3D)Scene)->AddObject((LPOBJECT3D)Object);
//		((LPMUTUAL_ACCESSIBLE)hObj3D)->Unlock();
//		((LPMUTUAL_ACCESSIBLE)hScene)->Unlock();
//	}
//
//	return bResult;
//}
//
//BOOL WINAPI SceneDeleteObject(HSCENE hScene, HOBJECT3D hObj3D)
//{
//	BOOL bResult = FALSE;
//
//	LPUNKNOWN Scene			= NULL;
//	LPUNKNOWN Object		= NULL;
//
//	thread_safety::LockModule(INFINITE);
//	if ( bResult = _IS_HANDLE_VALID(SCENE3D, hScene) 
//		&& _IS_HANDLE_VALID(OBJECT3D, hObj3D) )
//	{
//		Scene	= ((LPMUTUAL_ACCESSIBLE)hScene)->Lock();
//		Object	= ((LPMUTUAL_ACCESSIBLE)hObj3D)->Lock();
//	}
//	thread_safety::UnlockModule();
//
//	if (bResult)
//	{
//		// TODO: Implement another DeleteObject methods if necessary
//		bResult = ((LPSCENE3D)Scene)->DeleteObject(((LPOBJECT3D)Object)->objID);
//		((LPMUTUAL_ACCESSIBLE)hObj3D)->Unlock();
//		((LPMUTUAL_ACCESSIBLE)hScene)->Unlock();
//	}
//
//	return bResult;
//}
//
//BOOL WINAPI SceneGetFirstObject(HSCENE hScene, CLASS_ID clsID)
//{
//	BOOL bResult;
//
//	LPUNKNOWN Master = NULL;
//
//	thread_safety::LockModule(INFINITE);
//
//	if (bResult = thread_safety::IsObjectRegistered((size_t)hScene, typeid(SCENE3D)))
//		thread_safety::LockObjectRegistered((size_t)hScene, Master);
//
//	thread_safety::UnlockModule();
//
//	if (bResult)
//	{
//		((LPSCENE3D)Master)->getFirstObject(clsID);
//
//		thread_safety::UnlockObjectRegistered((size_t)hScene, Master);
//	}
//
//	return bResult;
//}

// TODO: implement CScene findObjectIndex, getFirstObject, getObject methods
