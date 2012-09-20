
#pragma once

#include "Basis.h"

#define DARK_SIDE 0.1

// ============================================================================
// _clsOmniLight class

typedef class _clsOmniLight : public OBJECT3D, public IColorable {
private:
	float		power;

	static void InitDefaultValues(_clsOmniLight *obj);

public:
	_clsOmniLight();
	_clsOmniLight(COLORREF c, float p);

	virtual float getPower();
	void setPower(float);
} OMNILIGHT3D, *LPOMNILIGHT3D;

#include "Light.inl"