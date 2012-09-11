#pragma once

// ============================================================================
// CLight partial implementation:

inline void CLight::InitDefaultValues(CLight *obj)
{
	obj->setPower(.2f);
	obj->setColor(RGB(255, 255, 255));
}

inline float CLight::getPower()		{ return power; }
inline void CLight::setPower(float p)	{ power = (p < 0 ? 0 : p); }