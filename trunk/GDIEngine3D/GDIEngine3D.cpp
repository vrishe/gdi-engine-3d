// GDIEngine3D.cpp: Contains an implementation of the interface of GDIEngine3D.dll

#include "stdafx.h"
#include "GDIEngine3D.h"

#include "thread_safety.h"
#include "Render.h"

#define _IS_HANDLE_VALID(type_name, handle_name)											\
	handle_name != NULL																		\
	&& thread_safety::AccessorExists((LPMUTUAL_ACCESSIBLE)handle_name)						\
	&& ((LPMUTUAL_ACCESSIBLE)handle_name)->getObjectTypeInfo() == typeid(type_name)


void accessor_cleanup(LPMUTUAL_ACCESSIBLE lpAcObj) { delete lpAcObj; }

// ============================================================================
// CRenderPool library interface implementation

HRENDERPOOL3D WINAPI CreateRenderPool3D()
{
	// Step 1: Object Initialization
	LPMUTUAL_ACCESSIBLE newborn = new MUTUAL_ACCESSIBLE(new RENDER_POOL());
	// Step 2: Library lock	
	thread_safety::LockModule(INFINITE);
	// Step 3: Write object ptr to Library
	thread_safety::AddAccessor(newborn);
	// Step 4: Unlock library
	thread_safety::UnlockModule();
	// Step 5: return object handle
	return (HRENDERPOOL3D)newborn;
}

BOOL WINAPI ReleaseRenderPool3D(HRENDERPOOL3D hRp3D)
{
	LPMUTUAL_ACCESSIBLE victim = NULL;

	// Step 1: Lock library
	thread_safety::LockModule(INFINITE);
	// Step 2: Check handle
	if ( _IS_HANDLE_VALID(RENDER_POOL, hRp3D) )
	{
		// Step 3: If VALID - erase handle from the list
		victim = (LPMUTUAL_ACCESSIBLE)hRp3D;
		thread_safety::RemoveAccessor(victim);
		// Step 4: if VALID - lock and delete victim object
		delete victim->Lock();
		delete victim;
	}
	// Step 6: Unlock Library
	thread_safety::UnlockModule();
	// Step 7: return operation result
	return victim != NULL;
}

BOOL WINAPI RenderPoolAssignScene3D(HRENDERPOOL3D hRp3D, HSCENE3D hSc3D)
{
	BOOL bResult = FALSE;

	LPUNKNOWN RenderPool	= NULL;
	LPUNKNOWN Scene			= NULL;

	// Step 1: Lock library
	thread_safety::LockModule(INFINITE);
	// Step 2: Check handle(s)
	if ( bResult = _IS_HANDLE_VALID(RENDER_POOL, hRp3D) 
		&& _IS_HANDLE_VALID(SCENE3D, hSc3D) )
	{
		// Step 3: Lock objects(s)
		RenderPool	= ((LPMUTUAL_ACCESSIBLE)hRp3D)->Lock();
		Scene		= ((LPMUTUAL_ACCESSIBLE)hSc3D)->Lock();
	}
	// Step 4: Unlock library 
	thread_safety::UnlockModule();

	if (bResult)
	{
		// Step 5: Make necessary adjustments
		((LPRENDER_POOL)RenderPool)->assignScene((LPSCENE3D)Scene);
		// Step 6: Unlock object(s)
		((LPMUTUAL_ACCESSIBLE)hRp3D)->Unlock();
		((LPMUTUAL_ACCESSIBLE)hSc3D)->Unlock();
	}
	// Step 7: Return operatiob result
	return bResult;
}

// TODO: Implement the most useful interface functions, such as: CreateLight, CreateCamera, RenderWorld(CRenderPool based) and so on 

// ============================================================================
// CScene library interface implementation

/* Constructor () */
HSCENE3D WINAPI CreateScene3D() 
{
	LPMUTUAL_ACCESSIBLE newborn = new MUTUAL_ACCESSIBLE(new SCENE3D());
	thread_safety::LockModule(INFINITE);
	thread_safety::AddAccessor(newborn);
	thread_safety::UnlockModule();
	return (HSCENE3D)newborn;
}

/* Destructor */
BOOL WINAPI ReleaseScene3D(HSCENE3D hSc3D)
{
	LPMUTUAL_ACCESSIBLE victim = NULL;

	thread_safety::LockModule(INFINITE);
	if ( _IS_HANDLE_VALID(RENDER_POOL, hSc3D) )
	{
		victim = (LPMUTUAL_ACCESSIBLE)hSc3D;
		thread_safety::RemoveAccessor(victim);
		delete victim->Lock();
		delete victim;
	}
	thread_safety::UnlockModule();
	return victim != NULL;
}

/* AddObject (const LPOBJECT3D lpObject) */
BOOL WINAPI SceneAddObject3D(HSCENE3D hSc3D, HOBJECT3D hObj3D)
{
	BOOL bResult = FALSE;

	LPUNKNOWN Scene			= NULL;
	LPUNKNOWN Object		= NULL;

	thread_safety::LockModule(INFINITE);
	if ( bResult = _IS_HANDLE_VALID(SCENE3D, hSc3D) 
		&& _IS_HANDLE_VALID(OBJECT3D, hObj3D) )
	{
		Scene	= ((LPMUTUAL_ACCESSIBLE)hSc3D)->Lock();
		Object	= ((LPMUTUAL_ACCESSIBLE)hObj3D)->Lock();
	}
	thread_safety::UnlockModule();

	if (bResult)
	{
		bResult = ((LPSCENE3D)Scene)->AddObject((LPOBJECT3D)Object);
		((LPMUTUAL_ACCESSIBLE)hObj3D)->Unlock();
		((LPMUTUAL_ACCESSIBLE)hSc3D)->Unlock();
	}

	return bResult;
}

/* DeleteObject (size_t objID) */
BOOL WINAPI SceneDeleteObject3D(HSCENE3D hSc3D, HOBJECT3D hObj3D)
{
	BOOL bResult = FALSE;

	LPUNKNOWN Scene			= NULL;
	LPUNKNOWN Object		= NULL;

	thread_safety::LockModule(INFINITE);
	if ( bResult = _IS_HANDLE_VALID(SCENE3D, hSc3D) 
		&& _IS_HANDLE_VALID(OBJECT3D, hObj3D) )
	{
		Scene	= ((LPMUTUAL_ACCESSIBLE)hSc3D)->Lock();
		Object	= ((LPMUTUAL_ACCESSIBLE)hObj3D)->Lock();
	}
	thread_safety::UnlockModule();

	if (bResult)
	{
		// TODO: Implement another DeleteObject methods if necessary
		bResult = ((LPSCENE3D)Scene)->DeleteObject(((LPOBJECT3D)Object)->objID);
		((LPMUTUAL_ACCESSIBLE)hObj3D)->Unlock();
		((LPMUTUAL_ACCESSIBLE)hSc3D)->Unlock();
	}

	return bResult;
}

/* Clear () */
BOOL WINAPI SceneClear3D(HSCENE3D hSc3D)
{
	BOOL bResult = FALSE;

	LPUNKNOWN Scene	= NULL;

	thread_safety::LockModule(INFINITE);
	if ( bResult = _IS_HANDLE_VALID(RENDER_POOL, hSc3D) )
	{
		Scene	= ((LPMUTUAL_ACCESSIBLE)hSc3D)->Lock();
	}
	thread_safety::UnlockModule();

	if (bResult)
	{
		((LPSCENE3D)Scene)->Clear();
		((LPMUTUAL_ACCESSIBLE)hSc3D)->Unlock();
	}

	return bResult;
}

/* getObjectClassCount (CLASS_ID clsID) */
UINT_PTR WINAPI SceneGetObjectClassCount3D(HSCENE3D hSc3D, CLASS_ID clsID) 
{
	BOOL isValid = FALSE;
	UINT_PTR uResult = UINT_MAX;

	LPUNKNOWN Scene			= NULL;

	thread_safety::LockModule(INFINITE);
	if ( isValid = _IS_HANDLE_VALID(SCENE3D, hSc3D) )
	{
		Scene	= ((LPMUTUAL_ACCESSIBLE)hSc3D)->Lock();
	}
	thread_safety::UnlockModule();

	if (isValid)
	{
		uResult = ((LPSCENE3D)Scene)->getObjectClassCount(clsID);
		((LPMUTUAL_ACCESSIBLE)hSc3D)->Unlock();
	}

	return uResult;
}

/* getFirstObject (CLASS_ID *objID) */
BOOL WINAPI SceneGetFirstObject3D(HSCENE3D hSc3D, CLASS_ID clsID)
{
	BOOL bResult = FALSE;

	LPUNKNOWN Scene			= NULL;

	thread_safety::LockModule(INFINITE);
	if ( bResult = _IS_HANDLE_VALID(SCENE3D, hSc3D) )
	{
		Scene	= ((LPMUTUAL_ACCESSIBLE)hSc3D)->Lock();
	}
	thread_safety::UnlockModule();

	if (bResult)
	{
		bResult = ((LPSCENE3D)Scene)->getFirstObject(&clsID);
		((LPMUTUAL_ACCESSIBLE)hSc3D)->Unlock();
	}

	return bResult;
}

/* getAmbientColor () */
COLORREF WINAPI SceneGetAmbientColor3D(HSCENE3D hSc3D)
{
	BOOL isValid = FALSE;
	COLORREF crResult = ULONG_MAX;

	LPUNKNOWN Scene	= NULL;

	thread_safety::LockModule(INFINITE);
	if ( isValid = _IS_HANDLE_VALID(RENDER_POOL, hSc3D) )
	{
		Scene	= ((LPMUTUAL_ACCESSIBLE)hSc3D)->Lock();
	}
	thread_safety::UnlockModule();

	if (isValid)
	{
		crResult = ((LPSCENE3D)Scene)->getAmbientColor();
		((LPMUTUAL_ACCESSIBLE)hSc3D)->Unlock();
	}

	return crResult;
}

/* setAmbientColor (COLORREF color) */
BOOL WINAPI SceneSetAmbientColor3D(HSCENE3D hSc3D, COLORREF color)
{
	BOOL bResult = FALSE;

	LPUNKNOWN Scene	= NULL;

	thread_safety::LockModule(INFINITE);
	if ( bResult = _IS_HANDLE_VALID(RENDER_POOL, hSc3D) )
	{
		Scene	= ((LPMUTUAL_ACCESSIBLE)hSc3D)->Lock();
	}
	thread_safety::UnlockModule();

	if (bResult)
	{
		((LPSCENE3D)Scene)->setAmbientColor(color);
		((LPMUTUAL_ACCESSIBLE)hSc3D)->Unlock();
	}

	return bResult;
}

// TODO: implement CScene findObjectIndex, getFirstObject, getObject methods
