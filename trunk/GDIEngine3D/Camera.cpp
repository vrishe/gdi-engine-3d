
#include "stdafx.h"
#include "Camera.h"

// ============================================================================
// _clsCamera partial implementation:

_clsCamera::_clsCamera() : OBJECT3D(CLS_CAMERA) 
{ 
	InitDefaultValues(this);
}

_clsCamera::_clsCamera(PROJECTION_TYPE projType, float horizFov, float vertFov) 
	: OBJECT3D(CLS_CAMERA) 
{ 
	InitDefaultValues(this);
	projectionType	= projType;
	hFOV			= horizFov;
	vFOV			= vertFov;
}

void _clsCamera::GetViewMatrix(MATRIX3D &mOut)
{
	mOut._11 = rWd.x;
	mOut._12 = uWd.x;
	mOut._13 = fWd.x;
	mOut._14 = .0f;

	mOut._21 = rWd.y;
	mOut._22 = uWd.y;
	mOut._23 = fWd.y;
	mOut._24 = .0f;

	mOut._31 = rWd.z;
	mOut._32 = uWd.z;
	mOut._33 = fWd.z;
	mOut._34 = .0f;

	mOut._41 = -Vector3DMultS(rWd, pos);
	mOut._42 = -Vector3DMultS(uWd, pos);
	mOut._43 = -Vector3DMultS(fWd, pos);
	mOut._44 = 1.0f;
}

void _clsCamera::GetProjectionMatrix(MATRIX3D &mOut)
{
	mOut.SetIdentity();

	mOut._11 = 1 / tan(hFOV / 2.0f);
	mOut._22 = 1 / tan(vFOV / 2.0f);
	mOut._33 = farClip / (farClip - nearClip);
	mOut._43 = mOut._33 * (-nearClip);
	mOut._34 = fDist;
	if ( projectionType == PT_CENTRAL ) 
	{
		mOut._11 *= mOut._34;
		mOut._22 *= mOut._34;
		mOut._44 = -mOut._44;
	}
}