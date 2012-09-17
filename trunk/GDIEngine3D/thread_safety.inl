// thread_safety.inl: contains inline implementation of several thread-safety providing tools' methods

#pragma once

// ============================================================================
// CMutualAccessible partial implementation

inline LPUNKNOWN CMutualAccessible::Lock()
{
	WaitForSingleObject(hMutex, INFINITE);
	return lpObj;
}

inline BOOL CMutualAccessible::Unlock()
{
	return ReleaseMutex(hMutex);
}

inline const type_info& CMutualAccessible::getObjectTypeInfo() const
{
	return typeid(*lpObj);
}