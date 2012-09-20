
#pragma once

#include "Basis.h"

// ============================================================================
// CMesh class represents some body model inside of a 3D scene
// Obviously it consist of: vertices, edges and polygons

typedef size_t MESH_ID;
#define MSH_UNDEFINED	0x00

class CMesh : public CObject, public IColorable {
private:
	MESH_ID			_mTypeID;

protected:
	VERT_LIST		vertices;	// list of vertexes
	VERT_LIST		cache;		// list of transformed vertices
	EDGE_LIST		edges;		// list of edges
	POLY_LIST		polygons;	// list of polygons

	COLORREF		color;

	void flushVertices();
	size_t findVertex(const VECTOR3D &v);	// returns a vertex position
	size_t findEdge(const EDGE3D &e);
	size_t findPolygon(const POLY3D &p);	// returns a Polygon_ position

public:
	CMesh(MESH_ID meshTypeId);
	CMesh(COLORREF c, MESH_ID meshTypeId);
	CMesh(const VECTOR3D &pt,
		float p, 
		float y, 
		float r, 
		MESH_ID meshTypeId
		);
	CMesh(const VECTOR3D &pt,
		float p, 
		float y, 
		float r, 
		COLORREF c,
		MESH_ID meshTypeId
		);
	CMesh(float	pX, 
		float pY, 
		float pZ, 
		float p, 
		float y, 
		float r, 
		MESH_ID meshTypeId
		);
	CMesh(float	pX, 
		float pY, 
		float pZ, 
		float p, 
		float y, 
		float r, 
		COLORREF c,
		MESH_ID meshTypeId
		);

	// getters
	MESH_ID			getMeshID();
	size_t			getVerticesCount();
	size_t			getEdgesCount();
	size_t			getPolygonsCount();
	LPVECTOR3D		getVerticesRaw();
	LPEDGE3D		getEdgesRaw();
	LPPOLY3D		getPolygonsRaw();
	VERT_LIST		getVertices();
	EDGE_LIST		getEdges();
	POLY_LIST		getPolygons();

	POLY3D			getPolygon(int);

	void			getBuffersRaw(LPVECTOR3D *vs, LPEDGE3D *es, LPPOLY3D *ps);
	void			getBuffers(LPVERT_LIST vs, LPEDGE_LIST es, LPPOLY_LIST ps);
	void			getVerticesTransformed(LPVECTOR3D v);

	void			setMeshID(MESH_ID);

	void			addVertices(const LPVECTOR3D vs, UINT vsCount);
	bool			delVertices();
	
	void			addEdges(const LPEDGE3D es, UINT esCount);
	bool			delEdges(EDGE3D);

	void			addPolygons(const LPPOLY3D ps, UINT psCount);
	bool			delPolygon(POLY3D);

	void			Transform();
};
typedef CMesh MESH3D, *LPMESH3D;

#include "Mesh.inl"