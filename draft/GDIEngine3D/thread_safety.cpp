// thread_safety.cpp: contains implementation of several thread-safety providing tools

#include "stdafx.h"
#include "thread_safety.h"

// ============================================================================
// thread_safety namespace methods implementation

#if (defined _DEBUG)
#define _INITIALIZATION_CHECK \
	if ( hModuleMutex == NULL ) throw std::bad_exception("thread_safety is not initialized properly!")
#else
#define _INITIALIZATION_CHECK
#endif

#define _CHECK_ITERATOR(iterator) \
	iterator != alModuleObjects.end() && iterator != alModuleObjects.begin()

namespace thread_safety
{
	namespace 
	{
		typedef struct _tagMutualAccessor
		{
			HANDLE		_hMutex;
			LPUNKNOWN	_lpObject;

			inline _tagMutualAccessor()
			{
				_hMutex		= NULL;
				_lpObject	= NULL;
			}
		} MUTUAL_ACCESSOR, &REFMUTUAL_ACCESSOR;

		typedef std::map<size_t, MUTUAL_ACCESSOR> OBJECT_REGISTRY;

		HANDLE			hModuleMutex		= NULL; 
		OBJECT_REGISTRY	alModuleObjects;
	}


	BOOL InitializeHandles()
	{
		if (hModuleMutex == NULL)
		{
			alModuleObjects.insert(OBJECT_REGISTRY::value_type(0, MUTUAL_ACCESSOR()));
			return (hModuleMutex = CreateMutex(NULL, FALSE, _T("GDIEngine3D_Library"))) != NULL;
		}

		return TRUE;
	}

	BOOL ReleaseHandles()
	{
		_INITIALIZATION_CHECK;

		LockModule(INFINITE);
		return CloseHandle(hModuleMutex);
	}

	DWORD LockModule(DWORD dwMilliseconds)
	{
		_INITIALIZATION_CHECK;

		return WaitForSingleObject(hModuleMutex, dwMilliseconds);
	}

	BOOL UnlockModule()
	{
		_INITIALIZATION_CHECK;

		return ReleaseMutex(hModuleMutex);
	}

	size_t RegisterObject(LPUNKNOWN lpObj)
	{
		_INITIALIZATION_CHECK;

		if (lpObj != NULL)
		{
			//size_t uObjKey = alModuleObjects.size();

			//if (alModuleObjects.find(uObjKey) != alModuleObjects.end())
			//{
			//	uObjKey = 0;

			//	__foreach(OBJECT_REGISTRY::iterator, entry, alModuleObjects)
			//	{
			//		OBJECT_REGISTRY::iterator entry_next = entry;
			//		if (++entry_next == _end || (entry_next->first - entry->first) > 1) 
			//		{
			//			uObjKey = entry->first + 1;
			//			break;
			//		}

			//		//if (++uObjKey != entry->first) break;
			//	}
			//}

			//if (uObjKey != SIZE_MAX) 
			//{
				OBJECT_REGISTRY::_Pairib pbInsertResult 
					= alModuleObjects.insert(OBJECT_REGISTRY::value_type(lpObj->getID(), MUTUAL_ACCESSOR()));
				
				if (pbInsertResult.second)
				{
					pbInsertResult.first->second._lpObject	= lpObj;
					pbInsertResult.first->second._hMutex	= CreateMutex(NULL, FALSE, NULL);
					
					if (pbInsertResult.first->second._hMutex == NULL)
					{
						alModuleObjects.erase(pbInsertResult.first);
					}
					else
					{
						return pbInsertResult.first->first;
					}
				}
			//}
		}
		else
		{
			SetLastError(ERROR_BAD_ARGUMENTS);
		}

		return SIZE_MAX;
	}

	LPUNKNOWN DeregisterObject(size_t uIndex)
	{
		OBJECT_REGISTRY::iterator finder = alModuleObjects.find(uIndex);

		if (_CHECK_ITERATOR(finder))
		{
			WaitForSingleObject(finder->second._hMutex, INFINITE);
			if (CloseHandle(finder->second._hMutex))
			{
				LPUNKNOWN lpObject = finder->second._lpObject;
				alModuleObjects.erase(finder);
				return lpObject;
			}
		}

		return NULL;
	}

	BOOL IsObjectRegistered(size_t uIndex, const type_info &typeID)
	{
		OBJECT_REGISTRY::iterator finder = alModuleObjects.find(uIndex);
		return _CHECK_ITERATOR(finder) && typeid(*(finder->second._lpObject)) == typeID;
	}

	BOOL LockObjectRegistered(size_t uIndex, LPUNKNOWN &lpObject)
	{
		OBJECT_REGISTRY::iterator finder = alModuleObjects.find(uIndex);

		if (_CHECK_ITERATOR(finder))
		{
			WaitForSingleObject(finder->second._hMutex, INFINITE);
			lpObject = finder->second._lpObject;

			return TRUE;
		}

		return FALSE;
	}

	BOOL UnlockObjectRegistered(size_t uIndex, LPUNKNOWN &lpObject)
	{
		OBJECT_REGISTRY::iterator finder = alModuleObjects.find(uIndex);

		if (_CHECK_ITERATOR(finder) && finder->second._lpObject == lpObject)
		{
			lpObject = NULL;

			return ReleaseMutex(finder->second._hMutex);
		}

		return FALSE;
	}

	VOID ForeachObjectRegistered(VOID (*_foreach_func)(LPUNKNOWN))
	{
		_INITIALIZATION_CHECK;

		if (_foreach_func != NULL)
		{
			__foreach(thread_safety::OBJECT_REGISTRY::iterator, entry, alModuleObjects)
			{
				_foreach_func(entry->second._lpObject);
			}
		}
	}
}






