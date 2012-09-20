#pragma once

// ============================================================================
// _clsMesh partial implementation:

inline void _clsMesh::getVerticesTransformed(LPVECTOR3D v)
{
	CopyMemory(v, cache.data(), cache.size() * sizeof(VECTOR3D));
}

inline MESH_ID		_clsMesh::getMeshID()				{ return _mTypeID; }
inline size_t		_clsMesh::getVerticesCount()		{ return vertices.size(); }
inline size_t		_clsMesh::getEdgesCount()			{ return edges.size(); }
inline size_t		_clsMesh::getPolygonsCount()		{ return polygons.size(); }
inline LPVECTOR3D	_clsMesh::getVerticesRaw()			{ return vertices.data(); }
inline LPEDGE3D		_clsMesh::getEdgesRaw()				{ return edges.data(); }
inline LPPOLY3D		_clsMesh::getPolygonsRaw()			{ return polygons.data(); }
inline VERT_LIST	_clsMesh::getVertices()				{ return vertices; }
inline EDGE_LIST	_clsMesh::getEdges()				{ return edges; }
inline POLY_LIST	_clsMesh::getPolygons()				{ return polygons; }
inline POLY3D		_clsMesh::getPolygon(int i)			{ return polygons[i]; }

inline void			_clsMesh::setMeshID(MESH_ID mTypeID)	{ _mTypeID = mTypeID; }