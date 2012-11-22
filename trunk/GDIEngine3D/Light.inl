#pragma once

// ============================================================================
// _clsOmniLight partial implementation:

inline void _clsOmniLight::InitDefaultValues(_clsOmniLight *obj)
{
	obj->setPower(.2f);
	obj->setColor(RGB(255, 255, 255));
}

inline float _clsOmniLight::getPower()			{ return power; }
inline void _clsOmniLight::setPower(float p)	{ power = p; }