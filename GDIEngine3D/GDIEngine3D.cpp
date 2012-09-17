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

