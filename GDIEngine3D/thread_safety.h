// thread_safety.h: Contains some useful tools that provide thread-safe interoperability

#pragma once

#include "IUnknown.h"

// ============================================================================
// IMutualAccessible template declaration

class IMutualAccessible
{
private:
	HANDLE		hMutex;
	LPUNKNOWN	lpObj;

protected:
	IMutualAccessible(LPUNKNOWN lpUnknwnObj);

public:
	virtual ~IMutualAccessible();

	LPUNKNOWN	Lock();
	BOOL		Unlock();
};
typedef IMutualAccessible *LPMUTUAL_ACCESSIBLE;

namespace thread_safety
{
	typedef std::list<LPMUTUAL_ACCESSIBLE> ACCESSORS_LIST;
	
	bool InitializeHandles();
	bool ReleaseHandles();

	bool AccessorExists(LPMUTUAL_ACCESSIBLE obj);
	bool AddAccessor(LPMUTUAL_ACCESSIBLE obj);
	bool RemoveAccessor(LPMUTUAL_ACCESSIBLE obj);

	void ForeachAccessor(void (*_foreach_func)(LPMUTUAL_ACCESSIBLE));
}

#include "thread_safety.inl"