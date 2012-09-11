
#include "stdafx.h"
#include "Light.h"

// ============================================================================
// CLight partial implementation:

CLight::CLight() { InitDefaultValues(this); }

CLight::CLight(COLORREF c, float p) 
	: CObject(CLS_LIGHT) 
{
	setPower(p);
	setColor(c);
}