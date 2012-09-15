// thread_safety.inl: contains inline implementation of several thread-safety providing tools' methods

#pragma once

// ============================================================================
// IMutualAccessible partial implementation

inline LPUNKNOWN IMutualAccessible::Lock()
{
	WaitForSingleObject(hMutex, INFINITE);
	return lpObj;
}

inline BOOL IMutualAccessible::Unlock()
{
	return ReleaseMutex(hMutex);
}
