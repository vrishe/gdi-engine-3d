
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

COLORREF _clsOmniLight::AffectPolygonColor(const POLY3D &poly_lighted, LPCVECTOR3D vs, COLORREF object_color) const
{
	VECTOR3D light_ray;
	Vector3DNormalize(poly_lighted.CoordinateMassCenter(vs) - pos, light_ray);

	FLOAT common_ratio = max(Vector3DMultS(poly_lighted.Normal(vs, 2), light_ray), .0F) * max(power, .0F);

	return RGB(
			min((UINT)(RED(color)   * (FLOAT)RED(object_color)   / 255.0F * common_ratio), 255U),
			min((UINT)(GREEN(color) * (FLOAT)GREEN(object_color) / 255.0F * common_ratio), 255U),
			min((UINT)(BLUE(color)  * (FLOAT)BLUE(object_color)  / 255.0F * common_ratio), 255U)
	);
}