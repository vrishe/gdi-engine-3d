
#include "stdafx.h"
#include "Basis.h"

// ============================================================================
// _tagEdge3D partial implementation:

_tagEdge3D::_tagEdge3D() : first(-1), second(-1) { }
_tagEdge3D::_tagEdge3D(size_t a, size_t b) : first(a), second(b) { }

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

VECTOR3D _tagPoly3D::Normal(const LPVERT_LIST vs, size_t startVert) {
	VECTOR3D v1, v2, ans;
	switch (startVert) {
	case 1:
		v1 = VECTOR3D(vs->at(second).x - vs->at(first).x,
					vs->at(second).y - vs->at(first).y,
					vs->at(second).z - vs->at(first).z);
		v2 = VECTOR3D(vs->at(third).x - vs->at(first).x,
					vs->at(third).y - vs->at(first).y,
					vs->at(third).z - vs->at(first).z);
		break;
	case 2:
		v1 = VECTOR3D(vs->at(first).x - vs->at(second).x,
					vs->at(first).y - vs->at(second).y,
					vs->at(first).z - vs->at(second).z);
		v2 = VECTOR3D(vs->at(third).x - vs->at(second).x,
					vs->at(third).y - vs->at(second).y,
					vs->at(third).z - vs->at(second).z);
		break;
	case 3:
		v1 = VECTOR3D(vs->at(first).x - vs->at(third).x,
					vs->at(first).y - vs->at(third).y,
					vs->at(first).z - vs->at(third).z);
		v2 = VECTOR3D(vs->at(second).x - vs->at(third).x,
					vs->at(second).y - vs->at(third).y,
					vs->at(second).z - vs->at(third).z);
	}
	Vector3DMultV(v1, v2, ans);
	return ans;
}

VECTOR3D _tagPoly3D::Normal(const LPVECTOR3D vs, size_t startVert) {
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
	MATRIX3D M;

	Matrix3DRotateAxis(rWd, angle, M);
	Matrix3DTransformNormal(M, fWd, fWd);

	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, rWd, uWd);
	Vector3DNormalize(uWd, uWd);
}

void _clsObject::RotateYaw(float angle) 
{
	MATRIX3D M;

	Matrix3DRotateAxis(uWd, angle, M);
	Matrix3DTransformNormal(M, fWd, fWd);

	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(uWd, fWd, rWd);
	Vector3DNormalize(rWd, rWd);
}

void _clsObject::RotateRoll(float angle) 
{
	MATRIX3D M;

	Matrix3DRotateAxis(fWd, angle, M);
	Matrix3DTransformNormal(M, rWd, rWd);

	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, rWd, uWd);
	Vector3DNormalize(uWd, uWd);
}

void _clsObject::LocalRotate(float roll, float yaw, float pitch)
{
	MATRIX3D M;

	Matrix3DRotateAxis(rWd, pitch, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, rWd, uWd);
	Vector3DNormalize(uWd, uWd);

	Matrix3DRotateAxis(uWd, yaw, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(uWd, fWd, rWd);
	Vector3DNormalize(rWd, rWd);

	Matrix3DRotateAxis(fWd, roll, M);
	Matrix3DTransformNormal(M, rWd, rWd);
	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, rWd, uWd);
	Vector3DNormalize(uWd, uWd);
}

void _clsObject::Rotate(float x, float y, float z)
{
	MATRIX3D M;

	rot.x = x;
	rot.y = y;
	rot.z = z;

	fWd = VECTOR3D(1.0F, .0F, .0F);
	rWd = VECTOR3D(.0F, 1.0F, .0F);
	uWd = VECTOR3D(.0F, .0F, 1.0F);

	Matrix3DRotateAxis(VECTOR3D(1.0f, .0f, .0f), rot.x, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Matrix3DTransformNormal(M, rWd, rWd);
	Matrix3DTransformNormal(M, uWd, uWd);
	Matrix3DTransformCoord(M, pos, pos);

	Matrix3DRotateAxis(VECTOR3D(.0f, 1.0f, .0f), rot.y, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Matrix3DTransformNormal(M, rWd, rWd);
	Matrix3DTransformNormal(M, uWd, uWd);
	Matrix3DTransformCoord(M, pos, pos);

	Matrix3DRotateAxis(VECTOR3D(.0f, .0f, 1.0f), rot.z, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Matrix3DTransformNormal(M, rWd, rWd);
	Matrix3DTransformNormal(M, uWd, uWd);
	Matrix3DTransformCoord(M, pos, pos);

	Vector3DNormalize(fWd, fWd);
	Vector3DNormalize(rWd, rWd);
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
		MATRIX3D M(true);
		float angle = units / obj->Distance(vPivot);

		Matrix3DRotateAxis(vAxis, angle, M);
		obj->pos -= vPivot;
		Matrix3DTransformCoord(M, obj->pos, obj->pos);
		obj->pos += vPivot;
	}
}

