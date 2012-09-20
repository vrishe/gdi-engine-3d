
#include "stdafx.h"
#include "Mesh.h"

// ============================================================================
// Partial implementation of _clsMesh class:

_clsMesh::_clsMesh(MESH_ID meshTypeId) 
	: OBJECT3D(CLS_MESH), IColorable() { setMeshID(meshTypeId); }

_clsMesh::_clsMesh(COLORREF c, MESH_ID meshTypeId) 
	: OBJECT3D(CLS_MESH), IColorable(c) { setMeshID(meshTypeId); }

_clsMesh::_clsMesh(const VECTOR3D &pt,
		float p, 
		float y, 
		float r,
		MESH_ID meshTypeId
		) : OBJECT3D(pt, p, y, r, CLS_MESH), IColorable() { }

_clsMesh::_clsMesh(const VECTOR3D &pt,
		float p, 
		float y, 
		float r, 
		COLORREF c,
		MESH_ID meshTypeId
		) : OBJECT3D(pt, p, y, r, CLS_MESH), IColorable(c) { }

_clsMesh::_clsMesh(float pX, 
		float pY, 
		float pZ, 
		float p, 
		float y, 
		float r, 
		MESH_ID meshTypeId
		) : OBJECT3D(pX, pY, pZ, p, y, r, CLS_MESH), IColorable() { }

_clsMesh::_clsMesh(float pX, 
		float pY, 
		float pZ, 
		float p, 
		float y, 
		float r, 
		COLORREF c,
		MESH_ID meshTypeId
		) : OBJECT3D(pX, pY, pZ, p, y, r, CLS_MESH), IColorable(c) { }

void _clsMesh::flushVertices()
{
	cache.clear();
	cache.insert(cache.begin(), vertices.begin(), vertices.end());
	cache.shrink_to_fit();
}

size_t _clsMesh::findVertex(const VECTOR3D &v) 
{
	for (size_t i = 0, max = vertices.size(); i < max; i++)
	{
		if (vertices[i] == v) return i;
	}

	return SIZE_MAX;
}

size_t _clsMesh::findEdge(const EDGE3D &e) 
{
	for (size_t i = 0, max = edges.size(); i < max; i++)
	{
		if (edges[i] == e) return i;
	}

	return SIZE_MAX;
}

size_t _clsMesh::findPolygon(const POLY3D &p) 
{
	for (size_t i = 0, max = polygons.size(); i < max; i++)
	{
		if (polygons[i] == p) return i;
	}

	return SIZE_MAX;
}

void _clsMesh::getBuffersRaw(LPVECTOR3D *vs, LPEDGE3D *es, LPPOLY3D *ps) 
{
	if ( vs != NULL ) *vs = vertices.data();
	if ( es != NULL ) *es = edges.data();
	if ( ps != NULL ) *ps = polygons.data();
}

void _clsMesh::getBuffers(LPVERT_LIST vs, LPEDGE_LIST es, LPPOLY_LIST ps) 
{
	if ( vs != NULL ) *vs = vertices;
	if ( es != NULL ) *es = edges;
	if ( ps != NULL ) *ps = polygons;
}

void _clsMesh::Transform()
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