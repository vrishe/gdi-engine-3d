
#pragma once

#ifndef EPS
#define EPS .0001f
#endif // EPS

typedef struct _tagVector
{
	float x;
	float y;
	float z;
} VECTOR, *LPVECTOR;

typedef struct _tagVector3D : public _tagVector {
public:
	_tagVector3D();
	_tagVector3D(float fX, float fY, float fZ);

	_tagVector3D	operator+ (const _tagVector3D& u) const;
	_tagVector3D	operator- (const _tagVector3D& u) const;
	_tagVector3D	operator* (float k) const;
	_tagVector3D	operator/ (float k) const;
	
	_tagVector3D&	operator+= (const _tagVector3D& u);
	_tagVector3D&	operator-= (const _tagVector3D& u);
	_tagVector3D&	operator*= (float k);
	_tagVector3D&	operator/= (float k);

	_tagVector3D	operator+ () const;
	_tagVector3D	operator- () const;

	bool		operator== (const _tagVector3D& u) const;
	bool		operator!= (const _tagVector3D& u) const;
} VECTOR3D, *LPVECTOR3D;

extern inline float Vector3DLength(const VECTOR3D &src);
extern inline void Vector3DNormalize(const VECTOR3D &in, VECTOR3D &out);
extern inline void Vector3DMultV(const VECTOR3D &in1, const VECTOR3D &in2, VECTOR3D &out);
extern inline float Vector3DMultS(const VECTOR3D &in1, const VECTOR3D &in2);

#define _sign(x) ((x >= 0) - (x < 0))

typedef struct _tagMatrix {
    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
} MATRIX4, *LPMATRIX4;

typedef struct _tagMatrix3D : public _tagMatrix {
public:
	_tagMatrix3D(bool bSetIdentity = false);
	_tagMatrix3D(int filler);
	_tagMatrix3D(
            float a11, float a12, float a13, float a14,
            float a21, float a22, float a23, float a24,
            float a31, float a32, float a33, float a34,
            float a41, float a42, float a43, float a44
		);

	void Fill(int filler);
	void SetIdentity();

} MATRIX3D, *LPMATRIX3D;

extern inline void Matrix3DRotateAxis(const VECTOR3D &axis, float angle, MATRIX3D &rslt);
extern inline void Matrix3DTransformCoord(const MATRIX3D &T, const VECTOR3D &in, VECTOR3D &out);
extern inline void Matrix3DTransformNormal(const MATRIX3D &T, const VECTOR3D &in,	VECTOR3D &out);

#include "Math3D.inl"