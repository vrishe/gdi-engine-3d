// thread_safety.cpp: contains implementation of several thread-safety providing tools

#include "stdafx.h"
#include "thread_safety.h"

// ============================================================================
// CMutualAccessible partial implementation

CMutualAccessible::CMutualAccessible(LPUNKNOWN lpUnknwnObj) : lpObj(lpUnknwnObj)
{
	if (lpObj == NULL) throw std::invalid_argument(
		std::string(typeid(*this).name()).append("::CMutualAccessible: lpUnknwnObj is NULL.").c_str()
		);

	hMutex = CreateMutex(NULL, FALSE, NULL);
	if (hMutex == NULL) throw std::bad_exception(
		std::string(typeid(*this).name()).append("::CMutualAccessible: CreateMutex failed.").c_str()
		);
}

CMutualAccessible::~CMutualAccessible()
{
	Lock();
	CloseHandle(hMutex);
}

// ============================================================================
// thread_safety namespace methods implementation

#if (defined _DEBUG)
#define _INITIALIZATION_CHECK \
	if ( hModuleMutex != NULL ) throw std::bad_exception("thread_safety is not initialized properly!")
#else
#define _INITIALIZATION_CHECK
#endif

namespace thread_safety
{
	namespace 
	{
		HANDLE			hModuleMutex		= NULL; 
		ACCESSORS_LIST	alModuleObjects;
	}

	BOOL InitializeHandles()
	{
		return (hModuleMutex = CreateMutex(NULL, FALSE, _T("GDIEngine3D_Library"))) != NULL;
	}

	BOOL ReleaseHandles()
	{
		_INITIALIZATION_CHECK;

		return CloseHandle(hModuleMutex) != 0;
	}

	DWORD thread_safety::LockModule(DWORD dwMilliseconds)
	{
		_INITIALIZATION_CHECK;

		return WaitForSingleObject(hModuleMutex, dwMilliseconds);
	}

	BOOL thread_safety::UnlockModule()
	{
		_INITIALIZATION_CHECK;

		return ReleaseMutex(hModuleMutex);
	}
	
	BOOL AccessorExists(LPMUTUAL_ACCESSIBLE obj)
	{
		_INITIALIZATION_CHECK;

		return std::find(alModuleObjects.begin(), alModuleObjects.end(), obj) != alModuleObjects.end();
	}

	BOOL AddAccessor(LPMUTUAL_ACCESSIBLE obj)
	{
		_INITIALIZATION_CHECK;

		if (obj == NULL || AccessorExists(obj)) return FALSE;
		
		alModuleObjects.push_back(obj);
		return TRUE;
	}

	BOOL RemoveAccessor(LPMUTUAL_ACCESSIBLE obj)
	{
		_INITIALIZATION_CHECK;

		size_t libObjectsCount = alModuleObjects.size();
		alModuleObjects.remove(obj);

		return alModuleObjects.size() != libObjectsCount;					
	}

	VOID ForeachAccessor(VOID (*_foreach_func)(LPMUTUAL_ACCESSIBLE))
	{
		_INITIALIZATION_CHECK;

		if (_foreach_func == NULL) return;

		for ( thread_safety::ACCESSORS_LIST::iterator 
				cur = thread_safety::alModuleObjects.begin(), 
				end = thread_safety::alModuleObjects.end(); 
			  cur != end;
			  cur++
			)
		{
			_foreach_func(*cur);
		}
	}
}






