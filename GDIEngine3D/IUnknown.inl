// IUnknown.inl: Contains implementation for objects decalred

#pragma once

inline bool operator==(const IUnknown &a, const IUnknown &b)
{
	return typeid(a) == typeid(b);
}

inline bool operator!=(const IUnknown &a, const IUnknown &b)
{
	return !(a == b);
}

