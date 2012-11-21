
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

COLORREF _clsOmniLight::PolygonLightedColor(const POLY3D &poly_lighted, const LPVECTOR3D vs, COLORREF initial_color) const
{
	VECTOR3D light_ray(poly_lighted.CoordinateMassCenter(vs));

	Vector3DNormalize(light_ray - pos, light_ray);
	FLOAT ratio = Vector3DMultS(poly_lighted.Normal(vs, 2), light_ray);
	if (ratio < .0F)
	{
		ratio = power - ratio;
	}
	else if (ratio == .0)
	{
		ratio = max(power / 3.3f, (FLOAT)DARK_SIDE);
	}
	else
	{
		ratio = (FLOAT)DARK_SIDE;
	}

	UINT red	= (UINT)(min((RED(initial_color) + RED(color))/2,     255) * ratio);
	UINT green	= (UINT)(min((GREEN(initial_color) + GREEN(color))/2, 255) * ratio);
	UINT blue	= (UINT)(min((BLUE(initial_color) + BLUE(color))/2,   255) * ratio);

	return initial_color == BLACK 
	  ? RGB(red > 255	? 255 : red, green > 255 ? 255 : green, blue > 255 ? 255 : blue)
	  : RGB((BYTE)min((red + RED(initial_color)), 255), 
			(BYTE)min((green + GREEN(initial_color)), 255),	
			(BYTE)min((blue + BLUE(initial_color)), 255)
		);
}