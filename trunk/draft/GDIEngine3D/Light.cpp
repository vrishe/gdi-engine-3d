
#include "stdafx.h"
#include "Light.h"

// ============================================================================
// _clsOmniLight partial implementation:

_clsOmniLight::_clsOmniLight() { InitDefaultValues(this); }

_clsOmniLight::_clsOmniLight(COLORREF c, float p) : OBJECT3D(CLS_LIGHT) 
{
	setPower(p);
	setColor(c);
}