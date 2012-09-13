// thread_safety.h: Contains some useful tools that provide thread-safe interoperability

#pragma once

typedef struct _tagObjectDescriptor {
private:
	LPVOID _lpObjPtr;
	HANDLE _hObjMutex;
	DWORD  _threadOwnerID;

public:
	_tagObjectDescriptor(LPVOID lpObj, DWORD dwThreadOwnderID);

	LPVOID Lock(); // Returns object address only when mutex is released and not abandoned, otherwise - NULL
	BOOL Unlock(); // Releases the mutex object

	BOOL Equals(const _tagObjectDescriptor &obj);

	LPVOID operator*();

	bool operator==(const _tagObjectDescriptor &obj); // Compares whole descriptor object; Equals(const _tagObjectDescriptor &obj) must be wrapped here
} OBJECT_DESCRIPTOR, *LPOBJECT_DESCRIPTOR;

typedef std::list<LPOBJECT_DESCRIPTOR> OBJECT_DESCRIPTION_LIST;

#include "thread_safety.inl"