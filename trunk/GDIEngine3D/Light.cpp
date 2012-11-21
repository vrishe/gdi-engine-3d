
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

COLORREF _clsOmniLight::AffectPolygonColor(const POLY3D &poly_lighted, const LPVECTOR3D vs, COLORREF object_color) const
{
	VECTOR3D light_ray;
	Vector3DNormalize(poly_lighted.CoordinateMassCenter(vs) - pos, light_ray);

	FLOAT ratio = max(Vector3DMultS(poly_lighted.Normal(vs, 2), light_ray), .0) * power;
	return RGB(
			RED(color)   * RED(object_color)   / 255U * ratio,
			GREEN(color) * GREEN(object_color) / 255U * ratio,
			BLUE(color)  * BLUE(object_color)  / 255U * ratio
	);
}