
#include "stdafx.h"
#include "Basis.h"

// ============================================================================
// Partial implementation of _tagEdge3D struct:

_tagEdge3D::_tagEdge3D() : first(-1), second(-1) { }
_tagEdge3D::_tagEdge3D(size_t a, size_t b) : first(a), second(b) { }

// ============================================================================
// Implementation of _tagNormalPolygon struct:

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
// Partial implementation of _tagPoly3D struct:

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
// Partial implementation of CObject class:
size_t CObject::Counter = 1;

CObject::CObject(CLASS_ID clsID) 
	: ClassID(clsID), ID(Counter++)
{
	InitDefaultValues(this);

	pos	= VECTOR3D(.0f, .0f, .0f);
}

CObject::CObject(
			const VECTOR3D &pt, 
			float p, 
			float y, 
			float r, 
			CLASS_ID clsID) 
	: ClassID(clsID), ID(Counter++)
{
	InitDefaultValues(this);

	pos	= pt;

	RotatePitch(p);
	RotateYaw(y);
	RotateRoll(r);
}

CObject::CObject(
		float pX, 
		float pY, 
		float pZ, 
		float p,
		float y,
		float r, 
		CLASS_ID clsID)
	: ClassID(clsID), ID(Counter++)
{
	InitDefaultValues(this);

	pos	= VECTOR3D(pX, pY, pZ);

	RotatePitch(p);
	RotateYaw(y);
	RotateRoll(r);
}

CObject::~CObject() 
{
	Counter--;
	if ( Name != NULL ) { delete[] Name; Name = NULL; }
}

void CObject::RotatePitch(float angle) 
{
	MATRIX3D M;

	Matrix3DRotateAxis(rWd, angle, M);
	Matrix3DTransformNormal(M, fWd, fWd);

	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, rWd, uWd);
	Vector3DNormalize(uWd, uWd);
}

void CObject::RotateYaw(float angle) 
{
	MATRIX3D M;

	Matrix3DRotateAxis(uWd, angle, M);
	Matrix3DTransformNormal(M, fWd, fWd);

	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(uWd, fWd, rWd);
	Vector3DNormalize(rWd, rWd);
}

void CObject::RotateRoll(float angle) 
{
	MATRIX3D M;

	Matrix3DRotateAxis(fWd, angle, M);
	Matrix3DTransformNormal(M, rWd, rWd);

	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, rWd, uWd);
	Vector3DNormalize(uWd, uWd);
}

void CObject::LocalRotate(float roll, float yaw, float pitch)
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

void CObject::Rotate(float y, float z, float x)
{
	MATRIX3D M;

	world.x = x;
	world.y = y;
	world.z = z;

	Matrix3DRotateAxis(VECTOR3D(.0f, 1.0f, .0f), world.y, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Matrix3DTransformNormal(M, rWd, rWd);
	Matrix3DTransformNormal(M, uWd, uWd);
	Matrix3DTransformCoord(M, pos, pos);

	Matrix3DRotateAxis(VECTOR3D(.0f, .0f, 1.0f), world.z, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Matrix3DTransformNormal(M, rWd, rWd);
	Matrix3DTransformNormal(M, uWd, uWd);
	Matrix3DTransformCoord(M, pos, pos);

	Matrix3DRotateAxis(VECTOR3D(1.0f, .0f, .0f), world.x, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Matrix3DTransformNormal(M, rWd, rWd);
	Matrix3DTransformNormal(M, uWd, uWd);
	Matrix3DTransformCoord(M, pos, pos);

	Vector3DNormalize(fWd, fWd);
	Vector3DNormalize(rWd, rWd);
	Vector3DNormalize(uWd, uWd);
}

void CObject::LookAt(const VECTOR3D &lookAt, const LPVECTOR3D upOrient)
{
	VECTOR3D lookDir	= lookAt - pos;
	VECTOR3D vUp		= abs(uWd.z) < EPS ? uWd : VECTOR3D(0, 0, 1) * uWd.z;

	if ( Vector3DLength(lookDir) > EPS )
	{
		if (upOrient != NULL && Vector3DLength(*upOrient) > EPS) Vector3DNormalize(*upOrient, vUp);

		Vector3DNormalize(lookDir, fWd);
		Vector3DMultV(vUp, fWd, rWd);
		Vector3DNormalize(rWd, rWd);

		if ( Vector3DLength(rWd) < EPS ) 
			rWd = VECTOR3D(0, 1, 0) * (FLOAT)((uWd.z >= 0) - (uWd.z < 0));

		Vector3DMultV(fWd, rWd, uWd);
	}
}

void CObject::ArbitraryOrbitMoveAround(
	CObject *obj, 
	const VECTOR3D &vAxis, 
	const VECTOR3D &vPivot, 
	float units
) {
	if (Vector3DLength(vAxis) > EPS)
	{
		MATRIX3D M(true);
		float angle = units / obj->Distance(vPivot);

		Matrix3DRotateAxis(vAxis, angle, M);
		obj->pos -= vPivot;
		Matrix3DTransformCoord(M, obj->pos, obj->pos);
		obj->pos += vPivot;
	}
}

void CObject::getName(LPTSTR objName, size_t bufSize) 
{ 
	size_t bSize = bufSize >= MAX_OBJECT_NAME_LEN 
		? MAX_OBJECT_NAME_LEN : bufSize;

	bSize--;
	_tcsncpy_s(objName, bufSize, Name, bSize);
}
void CObject::setName(LPTSTR objName, size_t srcSize)
{
	size_t bSize = srcSize >= MAX_OBJECT_NAME_LEN 
		? MAX_OBJECT_NAME_LEN : srcSize;

	bSize--;
	_tcsncpy_s(Name, MAX_OBJECT_NAME_LEN, objName, bSize);
}

// ============================================================================
// Partial implementation of CMesh class:

CMesh::CMesh(size_t meshTypeId) 
	: CObject(CLS_MESH), IColorable() { setMeshID(meshTypeId); }

CMesh::CMesh(COLORREF c, size_t meshTypeId) 
	: CObject(CLS_MESH), IColorable(c) { setMeshID(meshTypeId); }

CMesh::CMesh(const VECTOR3D &pt,
		float p, 
		float y, 
		float r,
		size_t meshTypeId
		) : CObject(pt, p, y, r, CLS_MESH), IColorable() { }

CMesh::CMesh(const VECTOR3D &pt,
		float p, 
		float y, 
		float r, 
		COLORREF c,
		size_t meshTypeId
		) : CObject(pt, p, y, r, CLS_MESH), IColorable(c) { }

CMesh::CMesh(float pX, 
		float pY, 
		float pZ, 
		float p, 
		float y, 
		float r, 
		size_t meshTypeId
		) : CObject(pX, pY, pZ, p, y, r, CLS_MESH), IColorable() { }

CMesh::CMesh(float pX, 
		float pY, 
		float pZ, 
		float p, 
		float y, 
		float r, 
		COLORREF c,
		size_t meshTypeId
		) : CObject(pX, pY, pZ, p, y, r, CLS_MESH), IColorable(c) { }

void CMesh::flushVertices()
{
	cache.clear();
	cache.insert(cache.begin(), vertices.begin(), vertices.end());
	cache.shrink_to_fit();
}

size_t CMesh::findVertex(const VECTOR3D &v) 
{
	size_t vCount = vertices.size();

	for (size_t i = 0; i < vCount; i++)
		if (vertices[i] == v)
			return i;
	return vCount;
}

size_t CMesh::findEdge(const EDGE3D &e) 
{
	size_t eCount = edges.size();

	for (size_t i = 0; i < eCount; i++)
		if (edges[i] == e)
			return i;
	return eCount;
}

size_t CMesh::findPolygon(const POLY3D &p) 
{
	size_t pCount = polygons.size();

	for (size_t i = 0; i < pCount; i++)
		if (polygons[i] == p)
			return i;
	return pCount;
}

void CMesh::getBuffersRaw(LPVECTOR3D *vs, LPEDGE3D *es, LPPOLY3D *ps) 
{
	if ( vs != NULL ) *vs = vertices.data();
	if ( es != NULL ) *es = edges.data();
	if ( ps != NULL ) *ps = polygons.data();
}

void CMesh::getBuffers(LPVERT_LIST vs, LPEDGE_LIST es, LPPOLY_LIST ps) 
{
	if ( vs != NULL ) *vs = vertices;
	if ( es != NULL ) *es = edges;
	if ( ps != NULL ) *ps = polygons;
}

void CMesh::Transform()
{
	LPVECTOR3D	v;
	MATRIX3D	mTransScalePos(true),
				mLocalScale(true),
				mLocalRot(true);
	size_t vertCount = getVerticesCount();
	flushVertices();
	
	GetLocalScaleMatrix(mLocalScale);
	GetRotationMatrix(mLocalRot);
	GetMoveMatrix(mTransScalePos);
	GetScaleMatrix(mTransScalePos);
	
	v = cache.data();
	for ( size_t i = 0; i < vertCount; i++ )
	{
		Matrix3DTransformNormal(
				mLocalScale,
				*(v + i),
				*(v + i)
			);		
		Matrix3DTransformNormal(
				mLocalRot,
				*(v + i),
				*(v + i)
			);
		Matrix3DTransformCoord(
				mTransScalePos,
				*(v + i),
				*(v + i)
			);
	}
}


// ============================================================================
// Implementation of CScene class:

bool CScene::findObjectIndex(const LPOBJECT3D lpObject, size_t *objIndex)
{
	bool bResult = lpObject != NULL;
	if ( !bResult ) return bResult;
	CONTENT::iterator finder = objects.find(lpObject->clsID());
	bResult = finder != objects.end();
	size_t objCount;
 	if ( bResult ) 
	{
		objCount	= finder->second.size();
		bResult		= false;
		for ( 
			size_t i = 0;
			i < objCount;
			i++ 
		) {
			if ( finder->second[i] == lpObject ) 
			{
				if (objIndex != NULL)  *objIndex = i;
				i		= objCount;
				bResult = true;
			}
		}
	}
	return bResult;
}

bool CScene::findObjectIndex(size_t objID, CLASS_ID *objClsID, size_t *objIndex)
{
	CONTENT::iterator finder = objects.begin();
	size_t	i,
			listCount;

	while ( finder != objects.end() )
	{
		listCount = finder->second.size();
		i = 0;
		while ( i < listCount ) 
		{
			if ( finder->second[i]->objID() == objID ) 
			{
				if ( objClsID != NULL ) *objClsID	= finder->first;
				if ( objIndex != NULL )	*objIndex	= i;
				return true;
			}
			i++;
		}
		finder++;
	}
	return false;
}

CScene::CScene() 
{ 
	CONTENT_CLASS clsObjLst;

	clsObjLst.first = CLS_CAMERA;
	objects.insert(clsObjLst);
	clsObjLst.first = CLS_MESH;
	objects.insert(clsObjLst);
	clsObjLst.first = CLS_LIGHT;
	objects.insert(clsObjLst);

	ambientColor = 0;
}
CScene::~CScene() { }

bool CScene::AddObject(const LPOBJECT3D lpObject)
{
	CONTENT::iterator finder = objects.find(lpObject->clsID());
	bool bResult 
		= lpObject != NULL
		&& finder != objects.end();
 	if ( bResult ) finder->second.push_back(lpObject);
	return bResult;
}

bool CScene::DeleteObject(CLASS_ID clsID, size_t objIndex)
{
	CONTENT::iterator finder = objects.find(clsID);
	bool bResult 
		= finder != objects.end()
		&& objIndex < finder->second.size();
	if ( bResult ) 
		finder->second.erase(finder->second.begin() + objIndex);
	return bResult;
}

bool CScene::DeleteObject(size_t objID)
{
	CONTENT::iterator finder;
	CLASS_ID objClsID;
	size_t objIndex;
	bool bResult 
		= findObjectIndex(objID, &objClsID, &objIndex);
	if ( bResult ) 
	{
		finder = objects.find(objClsID);
		bResult = finder != objects.end();
		if ( bResult ) 
			finder->second.erase(finder->second.begin() + objIndex);
	}
	return bResult;
}

bool CScene::DeleteObject(const LPOBJECT3D lpObject)
{
	size_t objIndex;
	return findObjectIndex(lpObject, &objIndex)
			&& DeleteObject(lpObject->clsID(), objIndex);	
}

void CScene::Clear() {
	objects.clear();
	
	CONTENT_CLASS clsObjLst;

	clsObjLst.first = CLS_CAMERA;
	objects.insert(clsObjLst);
	clsObjLst.first = CLS_MESH;
	objects.insert(clsObjLst);
	clsObjLst.first = CLS_LIGHT;
	objects.insert(clsObjLst);

	ambientColor = 0;
}

bool CScene::getFirstObject(CLASS_ID *objID)
{
	CONTENT::iterator i = objects.begin();
	while ( i != objects.end() )
	{
		if ( i->second.size() > 0 ) 
		{
			if (objID != NULL) *objID = i->first;
			return true;
		}
		i++;
	}
	return false;
}

LPOBJECT3D CScene::getFirstObject()
{
	CLASS_ID idFound;
	if ( getFirstObject(&idFound) )
		return objects.at(idFound).at(0);
	return NULL;
}

LPOBJECT3D CScene::getObject(CLASS_ID objID, size_t objIndex)
{
	CONTENT::iterator finder = objects.find(objID);
	LPOBJECT3D found = NULL;
	if ( finder != objects.end() && objIndex < finder->second.size() ) 
		found = finder->second[objIndex];
	return found;
}

LPOBJECT3D CScene::getObject(size_t objID)
{
	CLASS_ID clsID; 
	UINT_PTR objIndex;
	LPOBJECT3D found = findObjectIndex(objID, &clsID, &objIndex) ?
		objects[clsID][objIndex] : NULL;
	return found;
}

size_t CScene::getPolygonsCount() {
	UINT_PTR count = 0, N = objects[CLS_MESH].size();
	for (UINT_PTR i = 0; i < N; i++) {
		LPMESH3D temp = (LPMESH3D)objects[CLS_MESH][i];
		count += temp->getPolygonsCount();
	}
	return count;
}

size_t CScene::getVerticesCount() {
	UINT_PTR count = 0, N = objects[CLS_MESH].size();
	for (UINT_PTR i = 0; i < N; i++){
		LPMESH3D temp = (LPMESH3D)objects[CLS_MESH][i];
		count += temp->getVerticesCount();
	}
	return count;
}

COLORREF CScene::getAmbientColor() { return ambientColor; }

VOID CScene::setAmbientColor(COLORREF color) { ambientColor = color; }

size_t CScene::getObjectClassCount(CLASS_ID clsID)
{
	return objects[clsID].size();
}

