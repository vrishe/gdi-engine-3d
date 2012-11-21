
#pragma once

#include "Basis.h"

#define DARK_SIDE 0.1

// ============================================================================
// _clsOmniLight class

class IAffectPolygonColor
{
public:
	virtual COLORREF AffectPolygonColor(const POLY3D &poly_lighted, const LPVECTOR3D vs, COLORREF initial_color) const = 0;
};

typedef class _clsOmniLight : public OBJECT3D, public IColorable, public IAffectPolygonColor {
private:
	float power;

	static void InitDefaultValues(_clsOmniLight *obj);

public:
	_clsOmniLight();
	_clsOmniLight(COLORREF c, float p);

	virtual float getPower();
	void setPower(float);

	COLORREF AffectPolygonColor(const POLY3D &poly_lighted, const LPVECTOR3D vs, COLORREF initial_color) const;
} OMNILIGHT3D, *LPOMNILIGHT3D;

#include "Light.inl"