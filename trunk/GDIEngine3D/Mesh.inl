#pragma once

// ============================================================================
// _clsMesh partial implementation:

inline void _clsMesh::flushVertices()
{
	cache = vertices;
	cache.shrink_to_fit();
}

inline size_t _clsMesh::findVertex(const VECTOR3D &v) 
{
	for (size_t i = 0, max = vertices.size(); i < max; i++)
	{
		if (vertices[i] == v) return i;
	}

	return SIZE_MAX;
}

inline size_t _clsMesh::findEdge(const EDGE3D &e) 
{
	for (size_t i = 0, max = edges.size(); i < max; i++)
	{
		if (edges[i] == e) return i;
	}

	return SIZE_MAX;
}

inline size_t _clsMesh::findPolygon(const POLY3D &p) 
{
	for (size_t i = 0, max = polygons.size(); i < max; i++)
	{
		if (polygons[i] == p) return i;
	}

	return SIZE_MAX;
}

inline MESH_ID		_clsMesh::getMeshID()				{ return _mTypeID; }
inline size_t		_clsMesh::getVerticesCount()		{ return vertices.size(); }
inline size_t		_clsMesh::getEdgesCount()			{ return edges.size(); }
inline size_t		_clsMesh::getPolygonsCount()		{ return polygons.size(); }

inline void _clsMesh::setSelfIllumination(float self_illumination)
{ 
	self_illumination_coeff = self_illumination;
}
inline float _clsMesh::getSelfIllumination() const { return self_illumination_coeff; }

inline void _clsMesh::getVertexData(VERT_LIST &vertices)
{
	vertices = this->vertices;
}
inline void _clsMesh::getVertexDataRaw(LPVECTOR3D &lpVertexArray, size_t &uVertexCount)
{
	lpVertexArray	= vertices.begin()._Ptr;
	uVertexCount	= vertices.size();
}
inline void _clsMesh::getVertexCacheData(VERT_LIST &vertices)
{
	vertices = this->cache;
}
inline void _clsMesh::getVertexCacheDataRaw(LPVECTOR3D &lpVertexArray, size_t &uVertexCount)
{
	lpVertexArray	= cache.begin()._Ptr;
	uVertexCount	= cache.size();
}

inline void _clsMesh::getEdgeData(EDGE_LIST &edges)
{
	edges = this->edges;
}
inline void _clsMesh::getEdgeDataRaw(LPEDGE3D &lpEdgeArray, size_t &uEdgeCount)
{
	lpEdgeArray	= edges.begin()._Ptr;
	uEdgeCount	= edges.size();
}

inline void _clsMesh::getPolygonData(POLY_LIST &polygons)
{
	polygons = this->polygons;
}
inline void _clsMesh::getPolygonDataRaw(LPPOLY3D &lpPolygonArray, size_t &uPolygonCount)
{
	lpPolygonArray	= polygons.begin()._Ptr;
	uPolygonCount	= polygons.size();
}

inline void	_clsMesh::setMeshID(MESH_ID mTypeID) { _mTypeID = mTypeID; }

inline void _clsMesh::ClearHullData() 
{
	vertices.clear();
	edges.clear();
	polygons.clear();
}