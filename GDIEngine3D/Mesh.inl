#pragma once

// ============================================================================
// CMesh partial implementation:

inline void CMesh::getVerticesTransformed(LPVECTOR3D v)
{
	CopyMemory(v, cache.data(), cache.size() * sizeof(VECTOR3D));
}

inline MESH_ID		CMesh::getMeshID()					{ return _mTypeID; }
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

inline void			CMesh::setMeshID(MESH_ID mTypeID)	{ _mTypeID = mTypeID; }