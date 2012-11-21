#pragma once

// ============================================================================
// _tagEdge3D partial implementation:

inline _tagEdge3D& _tagEdge3D::operator+= (const UINT& p) 
{
	first += p;	second += p;
	return *this;
}

inline bool _tagEdge3D::operator== (const _tagEdge3D &b) const 
{
	return (first == b.first && second == b.second)	|| (first == b.second && second == b.first);
}

inline bool _tagEdge3D::operator!= (const _tagEdge3D &b) const { return !operator==(b); }
inline bool _tagEdge3D::isContianingVertex(size_t vi) const { return first == vi || second == vi; }

// ============================================================================
// _tagPoly3D partial implementation:

inline VECTOR3D _tagPoly3D::Normal(const LPVERT_LIST vs, size_t startVert) const
{
	return Normal(vs->data(), startVert);
}

inline VECTOR3D _tagPoly3D::CoordinateMassCenter(const LPVECTOR3D vs) const
{
	return (vs[first] + vs[second] + vs[third]) / 3.0f;
}

inline _tagPoly3D& _tagPoly3D::operator+= (const UINT& p) 
{
	first += p; second += p; third += p;
	return *this;
}

inline bool _tagPoly3D::operator== (const _tagPoly3D &b) const
{
	return first == b.first && second == b.second && third == b.third;
}

inline bool _tagPoly3D::operator!= (const _tagPoly3D &b) const { return !operator==(b); }

inline bool _tagPoly3D::isContainingVertex(size_t vi) 
{ 
	return first	== vi || second	== vi || third	== vi; 
}

inline bool _tagPoly3D::isContainingEdge(const EDGE3D &e)
{
	return isContainingVertex(e.first) && isContainingVertex(e.second);
}

// ===========================================================================
// _clsObject partial implementation:

inline void _clsObject::InitDefaultValues(_clsObject *obj)
{
	obj->fWd		= VECTOR3D(1.0f, .0f, .0f);
	obj->rWd		= VECTOR3D(.0f, .0f, 1.0f);
	obj->uWd		= VECTOR3D(.0f, 1.0f, .0f);
	obj->rot		= VECTOR3D();

	obj->worldScale	= VECTOR3D(1.0f, 1.0f, 1.0f);
	obj->localScale	= obj->worldScale;
}

inline _clsObject::~_clsObject() { }

inline VECTOR3D _clsObject::getPosition() { return pos; }

inline void _clsObject::MoveAlong(float units)			{ pos += fWd * units; }
inline void _clsObject::MoveHAcross(float units)		{ pos += rWd * units; }
inline void _clsObject::MoveVAcross(float units)		{ pos += uWd * units; }
inline void _clsObject::Move(const VECTOR3D &tV)
{
	pos += fWd * tV.x;
	pos += rWd * tV.y;
	pos += uWd * tV.z;
}
inline void _clsObject::Move(float tX, float tY, float tZ)
{
	pos += fWd * tX;
	pos += rWd * tY;
	pos += uWd * tZ;
}
inline void _clsObject::Translate(const VECTOR3D &tV) { pos = tV; }
inline void _clsObject::Translate(float tX, float tY, float tZ) 
{ 
	pos = VECTOR3D(tX, tY, tZ); 
}

inline void _clsObject::ScaleAlong(float factor)	{ localScale.x *= factor; }
inline void _clsObject::ScaleHAcross(float factor)	{ localScale.y *= factor; }
inline void _clsObject::ScaleVAcross(float factor)	{ localScale.z *= factor; }
inline void _clsObject::LocalScale(float fX, float fY, float fZ)
{
	localScale.x *= fX;
	localScale.y *= fY;
	localScale.z *= fZ;
}
inline void _clsObject::Scale(float fX, float fY, float fZ)
{
	worldScale.x *= fX;
	worldScale.y *= fY;
	worldScale.z *= fZ;
}

inline float _clsObject::Distance(const VECTOR3D &vDst)
{
	return Vector3DLength(pos - vDst);
}

inline float _clsObject::Distance(const _clsObject &objDst)
{
	return Distance(objDst.pos);
}

inline void _clsObject::LookAt(const _clsObject &objToLookAt) 
{ 
	LookAt(objToLookAt.pos, (LPVECTOR3D)&objToLookAt.uWd); 
}

inline void _clsObject::LookAt(float lX, float lY, float lZ) 
{ 
	LookAt(VECTOR3D(lX, lY, lZ), NULL); 
}

inline void _clsObject::HorizOrbitMoveAround(const VECTOR3D &vPivot, float units)
{
	ArbitraryOrbitMoveAround(this, this->uWd, vPivot, units);
}

inline void _clsObject::VertOrbitMoveAround(const VECTOR3D &vPivot, float units)
{
	ArbitraryOrbitMoveAround(this, this->rWd, vPivot, units);
}

inline void _clsObject::MoveTo(const VECTOR3D &vPivot, float units)
{
	VECTOR3D vN;
	Vector3DNormalize(vPivot, vN);
	pos += vN * units;
}

inline void _clsObject::MoveTo(const _clsObject &objPivot, float units)
{
	MoveTo(objPivot.pos, units);
}

inline void _clsObject::MoveTo(float pX, float pY, float pZ, float units)
{
	MoveTo(VECTOR3D(pX, pY, pZ), units);
}

inline void _clsObject::GetMoveMatrix(MATRIX4 &mOut) 
{
	mOut._41 = pos.x;
	mOut._42 = pos.y;
	mOut._43 = pos.z;
}

inline void _clsObject::GetLocalScaleMatrix(MATRIX4 &mOut)
{
	mOut._11 = localScale.x;
	mOut._22 = localScale.y;
	mOut._33 = localScale.z;
}

inline void _clsObject::GetScaleMatrix(MATRIX4 &mOut) 
{
	mOut._11 = worldScale.x;
	mOut._22 = worldScale.y;
	mOut._33 = worldScale.z;
}

inline void _clsObject::GetRotationMatrix(MATRIX4 &mOut) 
{
	mOut._11 = rWd.x;
	mOut._21 = fWd.x;
	mOut._31 = uWd.x;
	mOut._41 = .0f;

	mOut._12 = rWd.y;
	mOut._22 = fWd.y;
	mOut._32 = uWd.y;
	mOut._42 = .0f;

	mOut._13 = rWd.z;
	mOut._23 = fWd.z;
	mOut._33 = uWd.z;
	mOut._43 = .0f;

	mOut._14 = 0.f;
	mOut._24 = 0.f;
	mOut._34 = 0.f;
	mOut._44 = 1.0f;
}

inline CLASS_ID _clsObject::getClassID() const	{ return ClassID; }

inline VECTOR3D _clsObject::getForwardLookDirection()	{ return fWd; }
inline VECTOR3D _clsObject::getRightLookDirection()		{ return rWd; }
inline VECTOR3D _clsObject::getUpLookDirection()		{ return uWd; }
inline VECTOR3D _clsObject::getRotation()				{ return rot; }
inline VECTOR3D _clsObject::getScale()					{ return worldScale; }

inline void _clsObject::setForwardLookDirection(const VECTOR3D &v) { fWd = v; }
inline void _clsObject::setRightLookDirection(const VECTOR3D &v)	{ rWd = v; }
inline void _clsObject::setUpLookDirection(const VECTOR3D &v)		{ uWd = v; }

inline tstring	_clsObject::getName() const					{ return Name; }
inline void		_clsObject::setName(const tstring &objName)	{	Name = objName; }