// thread_safety.h: Contains some useful tools that provide thread-safe interoperability

#pragma once

#include "IUnknown.h"

// ============================================================================
// CMutualAccessible template declaration

class CMutualAccessible
{
private:
	HANDLE		hMutex;
	LPUNKNOWN	lpObj;

public:
	CMutualAccessible(LPUNKNOWN lpUnknwnObj);
	virtual ~CMutualAccessible();

	LPUNKNOWN	Lock();
	BOOL		Unlock();

	const type_info& getObjectTypeInfo() const;
};
typedef CMutualAccessible MUTUAL_ACCESSIBLE, *LPMUTUAL_ACCESSIBLE;

namespace thread_safety
{
	typedef std::list<LPMUTUAL_ACCESSIBLE> ACCESSORS_LIST;
	
	BOOL InitializeHandles();
	BOOL ReleaseHandles();

	DWORD LockModule(DWORD dwMilliseconds);
	BOOL UnlockModule();

	BOOL AccessorExists(LPMUTUAL_ACCESSIBLE obj);
	BOOL AddAccessor(LPMUTUAL_ACCESSIBLE obj);
	BOOL RemoveAccessor(LPMUTUAL_ACCESSIBLE obj);

	VOID ForeachAccessor(VOID (*_foreach_func)(LPMUTUAL_ACCESSIBLE));
}

#include "thread_safety.inl"