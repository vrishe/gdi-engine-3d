// IUnknown.h: Contains declaration of an IUnknown interface

#pragma once

class IUnknown
{
protected:
	IUnknown() { };

public:
	virtual ~IUnknown() { };
};
typedef IUnknown *LPUNKNOWN;

bool operator==(const IUnknown &a, const IUnknown &b);
bool operator!=(const IUnknown &a, const IUnknown &b);

#include "IUnknown.inl"