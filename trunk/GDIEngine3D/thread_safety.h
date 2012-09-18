// thread_safety.h: Contains some useful tools that provide thread-safe interoperability

#pragma once

#include "IUnknown.h"

// ============================================================================
// CMutualAccessible template declaration

namespace thread_safety
{
	BOOL	InitializeHandles();
	BOOL	ReleaseHandles();

	DWORD	LockModule(DWORD dwMilliseconds);
	BOOL	UnlockModule();

	size_t		RegisterObject(LPUNKNOWN lpObj);
	LPUNKNOWN	DeregisterObject(size_t uIndex);
	BOOL		IsObjectRegistered(size_t uIndex, const type_info &typeID);

	BOOL	LockObjectRegistered(size_t uIndex, LPUNKNOWN &lpObject);
	BOOL	UnlockObjectRegistered(size_t uIndex, LPUNKNOWN &lpObject);

	VOID ForeachObjectRegistered(VOID (*_foreach_func)(LPUNKNOWN));
}