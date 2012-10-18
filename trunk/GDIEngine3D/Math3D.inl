#pragma once

#include <memory.h>
#include <cmath>

// ============================================================================
// Implementation of _tagVector3D struct:
inline _tagVector3D::_tagVector3D() 
{ 
	x = 0;
	y = 0;
	z = 0;
}

inline _tagVector3D::_tagVector3D(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}

inline _tagVector3D _tagVector3D::operator+ (const _tagVector3D& u) const
{
	return _tagVector3D(x+u.x, y+u.y, z+u.z);
}

inline _tagVector3D _tagVector3D::operator- (const _tagVector3D& u) const
{
	return _tagVector3D(x-u.x, y-u.y, z-u.z);
}

inline _tagVector3D _tagVector3D::operator* (float k) const
{
	return _tagVector3D(x*k, y*k, z*k);
}

inline _tagVector3D _tagVector3D::operator/ (float k) const
{
	return _tagVector3D(x/k, y/k, z/k);
}

inline _tagVector3D& _tagVector3D::operator+= (const _tagVector3D& u)
{
	x += u.x;
	y += u.y;
	z += u.z;
	return *this;
}

inline _tagVector3D& _tagVector3D::operator-= (const _tagVector3D& u)
{
	x -= u.x;
	y -= u.y;
	z -= u.z;
	return *this;
}

inline _tagVector3D& _tagVector3D::operator*= (float k)
{
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

inline _tagVector3D& _tagVector3D::operator/= (float k)
{
	x /= k;
	y /= k;
	z /= k;
	return *this;
}

inline _tagVector3D _tagVector3D::operator+ () const
{
	return _tagVector3D(
		x > 0 ? x : -x,
		y > 0 ? y : -y,
		z > 0 ? z : -z
		);
}

inline _tagVector3D _tagVector3D::operator- () const
{
	return _tagVector3D(
		x < 0 ? x : -x,
		y < 0 ? y : -y,
		z < 0 ? z : -z
		);
}

inline bool _tagVector3D::operator== (const _tagVector3D& u) const
{
	return abs(x - u.x) < EPS
		&& abs(y - u.y) < EPS
		&& abs(z - u.z) < EPS;
}

inline bool _tagVector3D::operator!= (const _tagVector3D& u) const {	return !operator==(u); }

float Vector3DLength(const VECTOR3D &src) {
	return sqrt(
		src.x*src.x 
		+ src.y*src.y 
		+ src.z*src.z
		);
}
inline void Vector3DNormalize(const VECTOR3D &src, VECTOR3D &rslt) {
	float vLen = Vector3DLength(src);
	if ( vLen > EPS )
		rslt = src / vLen;
}
inline void Vector3DMultV(const VECTOR3D &in1, const VECTOR3D &in2, VECTOR3D &out)
{
	out = VECTOR3D(
		in1.y * in2.z - in1.z * in2.y,
		in1.z * in2.x - in1.x * in2.z,
		in1.x * in2.y - in1.y * in2.x
		);
}
inline float Vector3DMultS(const VECTOR3D &in1, const VECTOR3D &in2)
{
	return in1.x * in2.x 
		+ in1.y * in2.y 
		+ in1.z * in2.z;
}

// ============================================================================
// Implementation of _tagMatrix3D struct:

inline _tagMatrix3D::_tagMatrix3D(bool bSetIdentity) 
{ 
	bSetIdentity ? SetIdentity() : Fill(0); 
}

inline _tagMatrix3D::_tagMatrix3D(int filler) { Fill(filler); }

inline _tagMatrix3D::_tagMatrix3D(
	        float a11, float a12, float a13, float a14,
            float a21, float a22, float a23, float a24,
            float a31, float a32, float a33, float a34,
            float a41, float a42, float a43, float a44
) {	memcpy_s(m, sizeof(m), &a11, sizeof(m)); }

inline void _tagMatrix3D::Fill(int filler) { memset(m, filler, sizeof(m)); }

inline void _tagMatrix3D::SetIdentity() 
{ 
	Fill(0);
	for (
		size_t  i	= 0,
				len = sizeof(m) / sizeof(float),
				inc = ((len & 1) | len>>2) + 1;
		i < len;
		i += inc
	) { *((float*)m + i) = 1.0; }
}

inline void Matrix3DRotateAxis(const VECTOR3D &axis, float rads, MATRIX3D &rslt)
{
	float	sinTheta	= sin(rads),
			cosTheta	= cos(rads),
			_1mCosTheta	= 1 - cosTheta;

	rslt._11 = _1mCosTheta * axis.x * axis.x + cosTheta;
	rslt._12 = _1mCosTheta * axis.x * axis.y - axis.z * sinTheta;
	rslt._13 = _1mCosTheta * axis.x * axis.z + axis.y * sinTheta;
	rslt._14 = .0f;

	rslt._21 = _1mCosTheta * axis.x * axis.y + axis.z * sinTheta;;
	rslt._22 = _1mCosTheta * axis.y * axis.y + cosTheta;
	rslt._23 = _1mCosTheta * axis.y * axis.z - axis.x * sinTheta;
	rslt._24 = .0f;

	rslt._31 = _1mCosTheta * axis.x * axis.z - axis.y * sinTheta;
	rslt._32 = _1mCosTheta * axis.y * axis.z + axis.x * sinTheta;
	rslt._33 = _1mCosTheta * axis.z * axis.z + cosTheta;
	rslt._34 = .0f;

	rslt._41 = .0f;
	rslt._42 = .0f;
	rslt._43 = .0f;
	rslt._44 =	1.0f; 
}
inline void Matrix3DTransformCoord(const MATRIX3D &T, const VECTOR3D &in, VECTOR3D &out) 
{
	out = VECTOR3D(
			T._11 * in.x 
			+ T._21 * in.y 
			+ T._31 * in.z
			+ T._41,

			T._12 * in.x 
			+ T._22 * in.y 
			+ T._32 * in.z
			+ T._42,

			T._13 * in.x 
			+ T._23 * in.y 
			+ T._33 * in.z
			+ T._43
		);
}
inline void Matrix3DTransformNormal(const MATRIX3D &T, const VECTOR3D &in, VECTOR3D &out) 
{
	out = VECTOR3D(
			T._11 * in.x 
			+ T._21 * in.y 
			+ T._31 * in.z,

			T._12 * in.x 
			+ T._22 * in.y 
			+ T._32 * in.z,

			T._13 * in.x 
			+ T._23 * in.y 
			+ T._33 * in.z
		);
}