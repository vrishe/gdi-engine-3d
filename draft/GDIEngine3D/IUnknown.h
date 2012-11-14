// IUnknown.h: Contains declaration of an IUnknown interface

#pragma once

typedef class IUnknown *LPUNKNOWN;
typedef std::vector<LPUNKNOWN> GLOBAL_REGISTRY;

class IUnknown
{
private:
	static GLOBAL_REGISTRY  _registry;
	size_t					_id;

protected:
	IUnknown();

public:
	static LPUNKNOWN getByID(size_t objID);

	virtual ~IUnknown();

	size_t getID() const;
};


bool operator==(const IUnknown &a, const IUnknown &b);
bool operator!=(const IUnknown &a, const IUnknown &b);

#include "IUnknown.inl"