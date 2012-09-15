// thread_safety.cpp: contains implementation of several thread-safety providing tools

#include "stdafx.h"
#include "thread_safety.h"

// ============================================================================
// IMutualAccessible partial implementation

IMutualAccessible::IMutualAccessible(LPUNKNOWN lpUnknwnObj) : lpObj(lpUnknwnObj)
{
	if (lpObj == NULL) throw std::invalid_argument(
		std::string(typeid(*this).name()).append("::IMutualAccessible: lpUnknwnObj is NULL.").c_str()
		);

	hMutex = CreateMutex(NULL, FALSE, NULL);
	if (hMutex == NULL) throw std::bad_exception(
		std::string(typeid(*this).name()).append("::IMutualAccessible: CreateMutex failed.").c_str()
		);
}

IMutualAccessible::~IMutualAccessible()
{
	Lock();
	CloseHandle(hMutex);
}

// ============================================================================
// thread_safety namespace methods implementation

#if (defined _DEBUG)
#define _INITIALIZATION_CHECK \
	if ( hLibMutex != NULL ) throw std::bad_exception("thread_safety is not initialized properly!")
#else
#define _INITIALIZATION_CHECK
#endif

#define _OBJECT_EXISTS(obj_name) \
	std::find(alLibObjects.begin(), alLibObjects.end(), obj_name) != alLibObjects.end()

#define _LOCK_LIBRARY	WaitForSingleObject(hLibMutex, INFINITE)
#define _UNLOCK_LIBRARY ReleaseMutex(hLibMutex); 

namespace thread_safety
{
	namespace 
	{
		HANDLE			hLibMutex		= NULL; 
		ACCESSORS_LIST	alLibObjects;
	}

	bool InitializeHandles()
	{
		return (hLibMutex = CreateMutex(NULL, FALSE, _T("GDIEngine3D_Library"))) != NULL;
	}

	bool ReleaseHandles()
	{
		_INITIALIZATION_CHECK;

		_LOCK_LIBRARY;
		return CloseHandle(hLibMutex) != 0;
	}

	bool AccessorExists(LPMUTUAL_ACCESSIBLE obj)
	{
		_INITIALIZATION_CHECK;

		_LOCK_LIBRARY;
		bool bResult = _OBJECT_EXISTS(obj);
		_UNLOCK_LIBRARY;

		return bResult;
	}

	bool AddAccessor(LPMUTUAL_ACCESSIBLE obj)
	{
		_INITIALIZATION_CHECK;

		bool bResult;
		_LOCK_LIBRARY;
		if (bResult = (obj != NULL && !(_OBJECT_EXISTS(obj)))) alLibObjects.push_back(obj);
		_UNLOCK_LIBRARY;
		
		return bResult;		
	}

	bool RemoveAccessor(LPMUTUAL_ACCESSIBLE obj)
	{
		_INITIALIZATION_CHECK;

		bool bResult;
		_LOCK_LIBRARY;
		size_t libObjectsCount = alLibObjects.size();
		alLibObjects.remove(obj);
		bResult = alLibObjects.size() != libObjectsCount;		
		_UNLOCK_LIBRARY;
		
		return bResult;				
	}

	void ForeachAccessor(void (*_foreach_func)(LPMUTUAL_ACCESSIBLE))
	{
		_INITIALIZATION_CHECK;

		if (_foreach_func == NULL) return;

		_LOCK_LIBRARY;
		for ( thread_safety::ACCESSORS_LIST::iterator 
				cur = thread_safety::alLibObjects.begin(), 
				end = thread_safety::alLibObjects.end(); 
			  cur != end;
			  cur++
			)
		{
			_foreach_func(*cur);
		}
		_UNLOCK_LIBRARY;
	}
}






