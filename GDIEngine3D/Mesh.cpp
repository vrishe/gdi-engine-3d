
#include "stdafx.h"
#include "Mesh.h"

// ============================================================================
// Partial implementation of CMesh class:

CMesh::CMesh(MESH_ID meshTypeId) 
	: CObject(CLS_MESH), IColorable() { setMeshID(meshTypeId); }

CMesh::CMesh(COLORREF c, MESH_ID meshTypeId) 
	: CObject(CLS_MESH), IColorable(c) { setMeshID(meshTypeId); }

CMesh::CMesh(const VECTOR3D &pt,
		float p, 
		float y, 
		float r,
		MESH_ID meshTypeId
		) : CObject(pt, p, y, r, CLS_MESH), IColorable() { }

CMesh::CMesh(const VECTOR3D &pt,
		float p, 
		float y, 
		float r, 
		COLORREF c,
		MESH_ID meshTypeId
		) : CObject(pt, p, y, r, CLS_MESH), IColorable(c) { }

CMesh::CMesh(float pX, 
		float pY, 
		float pZ, 
		float p, 
		float y, 
		float r, 
		MESH_ID meshTypeId
		) : CObject(pX, pY, pZ, p, y, r, CLS_MESH), IColorable() { }

CMesh::CMesh(float pX, 
		float pY, 
		float pZ, 
		float p, 
		float y, 
		float r, 
		COLORREF c,
		MESH_ID meshTypeId
		) : CObject(pX, pY, pZ, p, y, r, CLS_MESH), IColorable(c) { }

void CMesh::flushVertices()
{
	cache.clear();
	cache.insert(cache.begin(), vertices.begin(), vertices.end());
	cache.shrink_to_fit();
}

size_t CMesh::findVertex(const VECTOR3D &v) 
{
	for (size_t i = 0, max = vertices.size(); i < max; i++)
	{
		if (vertices[i] == v) return i;
	}

	return SIZE_MAX;
}

size_t CMesh::findEdge(const EDGE3D &e) 
{
	for (size_t i = 0, max = edges.size(); i < max; i++)
	{
		if (edges[i] == e) return i;
	}

	return SIZE_MAX;
}

size_t CMesh::findPolygon(const POLY3D &p) 
{
	for (size_t i = 0, max = polygons.size(); i < max; i++)
	{
		if (polygons[i] == p) return i;
	}

	return SIZE_MAX;
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
	
	flushVertices();
	
	GetLocalScaleMatrix(mLocalScale);
	GetRotationMatrix(mLocalRot);
	GetMoveMatrix(mTransScalePos);
	GetScaleMatrix(mTransScalePos);
	
	v = cache.data();
	for (size_t i = 0, max = getVerticesCount(); i < max; i++)
	{
		Matrix3DTransformNormal(mLocalScale, *(v + i), *(v + i));		
		Matrix3DTransformNormal(mLocalRot, *(v + i), *(v + i));		
		Matrix3DTransformCoord(mTransScalePos, *(v + i), *(v + i));
	}
}