
#include "stdafx.h"
#include "Basis.h"

// ============================================================================
// _tagEdge3D partial implementation:

//_tagEdge3D::_tagEdge3D() : first(-1), second(-1) { }
//_tagEdge3D::_tagEdge3D(size_t a, size_t b) : first(a), second(b) { }

// ============================================================================
// _tagNormalPolygon implementation:

VECTOR3D _tagNormalPolygon::Normal(size_t startVert) {
	VECTOR3D v1, v2, ans;
	switch (startVert) {
	case 1:
		v1 = VECTOR3D(second.x - first.x,
					second.y - first.y,
					second.z - first.z);
		v2 = VECTOR3D(third.x - first.x,
					third.y - first.y,
					third.z - first.z);
		break;
	case 2:
		v1 = VECTOR3D(first.x - second.x,
					first.y - second.y,
					first.z - second.z);
		v2 = VECTOR3D(third.x - second.x,
					third.y - second.y,
					third.z - second.z);
		break;
	case 3:
		v1 = VECTOR3D(first.x - third.x,
					first.y - third.y,
					first.z - third.z);
		v2 = VECTOR3D(second.x - third.x,
					second.y - third.y,
					second.z - third.z);
	}
	Vector3DMultV(v1, v2, ans);
	return ans;
}

// ============================================================================
// _tagPoly3D partial implementation:

_tagPoly3D::_tagPoly3D() 
	: first(UINT_MAX), second(UINT_MAX), third(UINT_MAX) { }

_tagPoly3D::_tagPoly3D(size_t a, size_t b, size_t c) 
	: first(a), second(b), third(c) { }

VECTOR3D _tagPoly3D::Normal(LPCVECTOR3D vs, size_t startVert) const
{
	VECTOR3D v1, v2, ans;
	switch (startVert) {
	case 1:
		v1 = VECTOR3D(vs[second].x - vs[first].x,
					vs[second].y - vs[first].y,
					vs[second].z - vs[first].z);
		v2 = VECTOR3D(vs[third].x - vs[first].x,
					vs[third].y - vs[first].y,
					vs[third].z - vs[first].z);
		break;
	case 2:
		v1 = VECTOR3D(vs[first].x - vs[second].x,
					vs[first].y - vs[second].y,
					vs[first].z - vs[second].z);
		v2 = VECTOR3D(vs[third].x - vs[second].x,
					vs[third].y - vs[second].y,
					vs[third].z - vs[second].z);
		break;
	case 3:
		v1 = VECTOR3D(vs[first].x - vs[third].x,
					vs[first].y - vs[third].y,
					vs[first].z - vs[third].z);
		v2 = VECTOR3D(vs[second].x - vs[third].x,
					vs[second].y - vs[third].y,
					vs[second].z - vs[third].z);
	}
	Vector3DMultV(v1, v2, ans);
	Vector3DNormalize(ans, ans);
	return ans;
}

// ===========================================================================
// _clsObject partial implementation:

_clsObject::_clsObject(CLASS_ID clsID) 
	: ClassID(clsID)
{
	InitDefaultValues(this);

	pos	= VECTOR3D(.0f, .0f, .0f);
}

void _clsObject::RotatePitch(float angle) 
{
	MATRIX4 M;

	Matrix3DRotateAxis(rWd, angle, M);
	Matrix3DTransformNormal(M, fWd, fWd);

	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, rWd, uWd);
	Vector3DNormalize(uWd, uWd);
}

void _clsObject::RotateYaw(float angle) 
{
	MATRIX4 M;

	Matrix3DRotateAxis(uWd, angle, M);
	Matrix3DTransformNormal(M, fWd, fWd);

	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(uWd, fWd, rWd);
	Vector3DNormalize(rWd, rWd);
}

void _clsObject::RotateRoll(float angle) 
{
	MATRIX4 M;

	Matrix3DRotateAxis(fWd, angle, M);
	Matrix3DTransformNormal(M, rWd, rWd);

	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, rWd, uWd);
	Vector3DNormalize(uWd, uWd);
}

void _clsObject::LocalRotate(float roll, float yaw, float pitch, bool reset_rotation)
{
	MATRIX4 M;

	if (reset_rotation)
	{
		fWd	= VECTOR3D(1.0f, .0f, .0f);
		rWd	= VECTOR3D(.0f, .0f, 1.0f);
		uWd	= VECTOR3D(.0f, 1.0f, .0f);
	}

	Matrix3DRotateAxis(uWd, yaw, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, uWd, rWd);
	Vector3DNormalize(rWd, rWd);

	Matrix3DRotateAxis(rWd, pitch, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(rWd, fWd, uWd);
	Vector3DNormalize(uWd, uWd);

	Matrix3DRotateAxis(fWd, roll, M);
	Matrix3DTransformNormal(M, rWd, rWd);
	Vector3DNormalize(rWd, rWd);
	Vector3DMultV(rWd, fWd, uWd);
	Vector3DNormalize(uWd, uWd);
}

void _clsObject::LookAt(const VECTOR3D &lookAt, const LPVECTOR3D upOrient)
{
	VECTOR3D lookDir	= lookAt - pos;
	VECTOR3D vUp		= abs(uWd.z) < .0 ? uWd : VECTOR3D(0, 0, 1) * uWd.z;

	if ( Vector3DLength(lookDir) > .0 )
	{
		if (upOrient != NULL && Vector3DLength(*upOrient) > .0) Vector3DNormalize(*upOrient, vUp);

		Vector3DNormalize(lookDir, fWd);
		Vector3DMultV(vUp, fWd, rWd);
		Vector3DNormalize(rWd, rWd);

		if ( Vector3DLength(rWd) < .0 ) 
			rWd = VECTOR3D(0, 1, 0) * (FLOAT)((uWd.z >= 0) - (uWd.z < 0));

		Vector3DMultV(fWd, rWd, uWd);
	}
}

void _clsObject::ArbitraryOrbitMoveAround(
	_clsObject *obj, 
	const VECTOR3D &vAxis, 
	const VECTOR3D &vPivot, 
	float units
) {
	if (Vector3DLength(vAxis) > .0)
	{
		MATRIX4 M(true);
		float angle = units / obj->Distance(vPivot);

		Matrix3DRotateAxis(vAxis, angle, M);
		obj->pos -= vPivot;
		Matrix3DTransformCoord(M, obj->pos, obj->pos);
		obj->pos += vPivot;
	}
}

