#pragma once

// ============================================================================
// Partial implementation of _tagEdge3D struct:

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
// Partial implementation of _tagPoly3D struct:

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

// ============================================================================
// Implementation of _tagColor3D struct:
inline _tagColor3D::_tagColor3D(unsigned char r, unsigned char g, unsigned char b) { Red = r; Green = g; Blue = b; }
inline bool _tagColor3D::operator==(const float &b) const { return ( Red == b && Green == b && Blue == b); }
inline bool _tagColor3D::operator!=(const float &b) const { return ( Red != b || Green != b || Blue != b); }

// ============================================================================
// Partial implementation of CObject class:

inline void CObject::InitDefaultValues(CObject *obj)
{
	obj->Name		= new TCHAR[MAX_OBJECT_NAME_LEN];
	obj->Name[0]	= '\0';

	obj->fWd		= VECTOR3D(1.0f, .0f, .0f);
	obj->rWd		= VECTOR3D(.0f, 1.0f, .0f);
	obj->uWd		= VECTOR3D(.0f, .0f, 1.0f);
	obj->world		= VECTOR3D();

	obj->worldScale	= VECTOR3D(1.0f, 1.0f, 1.0f);
	obj->localScale	= obj->worldScale;
}

inline CLASS_ID CObject::clsID() { return ClassID; }
inline size_t CObject::objID()	{ return ID; }

inline VECTOR3D CObject::getPosition() { return pos; }

inline void CObject::MoveAlong(float units)			{ pos += fWd * units; }
inline void CObject::MoveHAcross(float units)		{ pos += rWd * units; }
inline void CObject::MoveVAcross(float units)		{ pos += uWd * units; }
inline void CObject::Move(const VECTOR3D &tV)
{
	pos += fWd * tV.x;
	pos += rWd * tV.y;
	pos += uWd * tV.z;
}
inline void CObject::Move(float tX, float tY, float tZ)
{
	pos += fWd * tX;
	pos += rWd * tY;
	pos += uWd * tZ;
}
inline void CObject::Translate(const VECTOR3D &tV) { pos = tV; }
inline void CObject::Translate(float tX, float tY, float tZ) 
{ 
	pos = VECTOR3D(tX, tY, tZ); 
}

inline void CObject::ScaleAlong(float factor)	{ localScale.x *= factor; }
inline void CObject::ScaleHAcross(float factor)	{ localScale.y *= factor; }
inline void CObject::ScaleVAcross(float factor)	{ localScale.z *= factor; }
inline void CObject::LocalScale(float fX, float fY, float fZ)
{
	localScale.x *= fX;
	localScale.y *= fY;
	localScale.z *= fZ;
}
inline void CObject::Scale(float fX, float fY, float fZ)
{
	worldScale.x *= fX;
	worldScale.y *= fY;
	worldScale.z *= fZ;
}

inline float CObject::Distance(const VECTOR3D &vDst)
{
	return Vector3DLength(pos - vDst);
}

inline float CObject::Distance(const CObject &objDst)
{
	return Distance(objDst.pos);
}

inline void CObject::LookAt(const CObject &objToLookAt) 
{ 
	LookAt(objToLookAt.pos, (LPVECTOR3D)&objToLookAt.uWd); 
}

inline void CObject::LookAt(float lX, float lY, float lZ) 
{ 
	LookAt(VECTOR3D(lX, lY, lZ), NULL); 
}

inline void CObject::HorizOrbitMoveAround(const VECTOR3D &vPivot, float units)
{
	ArbitraryOrbitMoveAround(this, this->uWd, vPivot, units);
}

inline void CObject::VertOrbitMoveAround(const VECTOR3D &vPivot, float units)
{
	ArbitraryOrbitMoveAround(this, this->rWd, vPivot, units);
}

inline void CObject::MoveTo(const VECTOR3D &vPivot, float units)
{
	VECTOR3D vN;
	Vector3DNormalize(vPivot, vN);
	pos += vN * units;
}

inline void CObject::MoveTo(const CObject &objPivot, float units)
{
	MoveTo(objPivot.pos, units);
}

inline void CObject::MoveTo(float pX, float pY, float pZ, float units)
{
	MoveTo(VECTOR3D(pX, pY, pZ), units);
}

inline void CObject::GetMoveMatrix(MATRIX3D &mOut) 
{
	mOut._41 = pos.x;
	mOut._42 = pos.y;
	mOut._43 = pos.z;
}

inline void CObject::GetLocalScaleMatrix(MATRIX3D &mOut)
{
	mOut._11 = localScale.x;
	mOut._22 = localScale.y;
	mOut._33 = localScale.z;
}

inline void CObject::GetScaleMatrix(MATRIX3D &mOut) 
{
	mOut._11 = worldScale.x;
	mOut._22 = worldScale.y;
	mOut._33 = worldScale.z;
}

inline void CObject::GetRotationMatrix(MATRIX3D &mOut) 
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

inline VECTOR3D CObject::getForwardLookDirection() { return fWd; }
inline VECTOR3D CObject::getRightLookDirection() { return rWd; }
inline VECTOR3D CObject::getUpLookDirection() { return uWd; }
inline VECTOR3D CObject::getRotation() { return world; }
inline VECTOR3D CObject::getScale() { return worldScale; }

inline void CObject::setForwardLookDirection(const VECTOR3D &v) { fWd = v; }
inline void CObject::setRightLookDirection(const VECTOR3D &v) { rWd = v; }
inline void CObject::setUpLookDirection(const VECTOR3D &v) { uWd = v; }

// ============================================================================
// Partial implementation of CMesh class:

inline void CMesh::getVerticesTransformed(LPVECTOR3D v)
{
	CopyMemory(v, cache.data(), cache.size() * sizeof(VECTOR3D));
}

inline size_t		CMesh::getMeshID()					{ return _mTypeID; }
inline size_t		CMesh::getVerticesCount()			{ return vertices.size(); }
inline size_t		CMesh::getEdgesCount()				{ return edges.size(); }
inline size_t		CMesh::getPolygonsCount()			{ return polygons.size(); }
inline LPVECTOR3D	CMesh::getVerticesRaw()				{ return vertices.data(); }
inline LPEDGE3D		CMesh::getEdgesRaw()				{ return edges.data(); }
inline LPPOLY3D		CMesh::getPolygonsRaw()				{ return polygons.data(); }
inline VERT_LIST	CMesh::getVertices()				{ return vertices; }
inline EDGE_LIST	CMesh::getEdges()					{ return edges; }
inline POLY_LIST	CMesh::getPolygons()				{ return polygons; }
inline POLY3D		CMesh::getPolygon(int i)			{ return polygons[i]; }

inline void			CMesh::setMeshID(size_t mTypeID)	{ _mTypeID = mTypeID; }