
#include "stdafx.h"
#include "Mesh.h"

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