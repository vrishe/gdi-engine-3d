
#pragma once

#include "Basis.h"

#define DARK_SIDE 0.1

// ============================================================================
// CLight class

class CLight : public CObject, public IColorable {
private:
	float		power;

	static void InitDefaultValues(CLight *obj);

public:
	CLight();
	CLight(COLORREF c, float p);

	virtual float getPower();
	void setPower(float);
};
typedef CLight DIFLIGHT3D, *LPDIFLIGHT3D;

#include "Light.inl"