
#pragma once

#include "Basis.h"

// ============================================================================
// _clsMesh class represents some body model inside of a 3D scene
// Obviously it consist of: vertices, edges and polygons

typedef size_t MESH_ID;
#define MSH_PRIMITIVE	0x00

typedef class _clsMesh : public OBJECT3D, public IColorable {
private:
	MESH_ID			_mTypeID;

protected:
	VERT_LIST		vertices;	// list of vertexes
	VERT_LIST		cache;		// list of transformed vertices
	EDGE_LIST		edges;		// list of edges
	POLY_LIST		polygons;	// list of polygons

	COLORREF		color;
	float			self_illumination_coeff;

	void flushVertices();
	size_t findVertex(const VECTOR3D &v);	// returns a vertex position
	size_t findEdge(const EDGE3D &e);
	size_t findPolygon(const POLY3D &p);	// returns a Polygon_ position

public:
	_clsMesh(MESH_ID meshTypeId);
	_clsMesh(MESH_ID meshTypeId, COLORREF c);

	MESH_ID	getMeshID();
	size_t	getVerticesCount();
	size_t	getEdgesCount();
	size_t	getPolygonsCount();

	void	setSelfIllumination(float self_illumination);
	float	getSelfIllumination() const;

	void	getVertexData(VERT_LIST &vertices);
	void	getVertexDataRaw(LPVECTOR3D &lpVertexArray, size_t &uVertexCount);
	void	getVertexCacheData(VERT_LIST &vertices);
	void	getVertexCacheDataRaw(LPVECTOR3D &lpVertexArray, size_t &uVertexCount);

	void	getEdgeData(EDGE_LIST &edges);
	void	getEdgeDataRaw(LPEDGE3D &lpEdgeArray, size_t &uEdgeCount);

	void	getPolygonData(POLY_LIST &polygons);
	void	getPolygonDataRaw(LPPOLY3D &lpPolyArray, size_t &uPolyCount);

	void	setMeshID(MESH_ID);

	void	ClearHullData();
	void	Transform();
} MESH3D, *LPMESH3D;

#include "Mesh.inl"