// IUnknown.inl: Contains implementation for objects decalred

#pragma once

inline LPUNKNOWN IUnknown::getByID(size_t objID)
{
	__foreach(GLOBAL_REGISTRY::iterator, entry, _registry)
	{
		if ((*entry)->getID() == objID) return *entry;
	}

	return NULL;
}

inline IUnknown::IUnknown()
{
	// TODO: Make binary search here
	size_t prediction = 1;
	__foreach(GLOBAL_REGISTRY::iterator, entry, _registry)
	{
		if ((*entry)->_id != prediction++) break;		
	}
	_id = prediction;
	_registry.push_back(this);
}

inline IUnknown::~IUnknown()
{
	// TODO: And here
	__foreach(GLOBAL_REGISTRY::iterator, entry, _registry)
	{
		if ((*entry) == this) 
		{
			_registry.erase(entry);
			break;
		}
	}
}

inline size_t IUnknown::getID() const { return _id; }

inline bool operator==(const IUnknown &a, const IUnknown &b)
{
	return typeid(a) == typeid(b);
}

inline bool operator!=(const IUnknown &a, const IUnknown &b)
{
	return !(a == b);
}

