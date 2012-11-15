
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

VECTOR3D _tagPoly3D::Normal(const LPVECTOR3D vs, size_t startVert) const
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
	return ans;
}
VECTOR3D _tagPoly3D::Normal(const LPVERT_LIST vs, size_t startVert) const
{
	return Normal(vs->data(), startVert);
}

#define A(va, vb) (b.y * b.z - b.y * a.z + a.y * a.z - a.y * b.z)
#define B(va, vb) (b.x * a.z + a.z * b.z - a.x * a.z - b.z * b.z)
#define C(va, vb) (b.x * b.y - b.x * a.y - a.x * b.y + a.x * a.y)
#define D(va, vb) (2 * a.x * b.y * a.z - a.x * b.y * b.z - a.x * a.y * a.z + b.x * a.y * b.z - b.z * b.y * a.z)
bool _tagPoly3D::CoordinateMassCenter(const LPVECTOR3D vs, VECTOR3D &out) const
{
	VECTOR3D a(vs[first]);
	VECTOR3D b(vs[third].x - vs[second].x, vs[third].y - vs[second].y, vs[third].z - vs[second].z);
	float A1 = A(a,b), B1 = B(a,b), C1 = C(a,b), D1 = D(a,b);

	a = vs[second];
	b = VECTOR3D(vs[third].x - vs[first].x, vs[third].y - vs[first].y, vs[third].z - vs[first].z);
	float A2 = A(a,b), B2 = B(a,b), C2 = C(a,b), D2 = D(a,b);

	a = vs[third];
	b = VECTOR3D(vs[second].x - vs[first].x, vs[second].y - vs[first].y, vs[second].z - vs[first].z);
	float A3 = A(a,b), B3 = B(a,b), C3 = C(a,b), D3 = D(a,b);

	float det_main = MATRIX3(A1, B1, C1, A2, B2, C2, A3, B3, C3).Determinant();
	float det_x	   = MATRIX3(D1, B1, C1, D2, B2, C2, D3, B3, C3).Determinant();
	float det_y	   = MATRIX3(A1, D1, C1, A2, D2, C2, A3, D3, C3).Determinant();
	float det_z	   = MATRIX3(A1, B1, D1, A2, B2, D2, A3, B3, D3).Determinant();

	if (det_main == .0F && (det_x != .0F || det_y != .0F || det_x != .0F)) return false;

	if (!(det_main == .0F && det_x == .0F && det_y == .0F && det_x == .0F)) 
	{
		out = VECTOR3D(det_main / det_x, det_main / det_y, det_main / det_z);
	}
	else
	{
		out = vs[first];
	}

	return true;
}
bool _tagPoly3D::CoordinateMassCenter(const LPVERT_LIST vs, VECTOR3D &out) const
{
	return CoordinateMassCenter(vs->data(), out);
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

	Matrix3DRotateAxis(rWd, pitch, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(rWd, fWd, uWd);
	Vector3DNormalize(uWd, uWd);

	Matrix3DRotateAxis(uWd, yaw, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, uWd, rWd);
	Vector3DNormalize(rWd, rWd);

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

