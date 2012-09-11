#pragma once

using namespace std;

// ============================================================================
// Supporting structures

typedef struct _tagEdge3D {
	size_t first;
	size_t second;

	_tagEdge3D();
	_tagEdge3D(size_t nFirst, size_t nSecond);

	_tagEdge3D& operator+= (const UINT& p);
	bool operator==(const _tagEdge3D &b) const;
	bool operator!=(const _tagEdge3D &b) const;
	bool isContianingVertex(size_t vi) const;
} EDGE3D, *LPEDGE3D;

typedef struct _tagNormalPolygon {
	VECTOR3D first;
	VECTOR3D second;
	VECTOR3D third;

	DWORD colorRef;

	VECTOR3D Normal(size_t startVert);
} DIRECTPOLY3D, *LPDIRECTPOLY3D;

typedef vector<VECTOR3D> VERT_LIST, *LPVERT_LIST;

typedef struct _tagPoly3D {
	size_t first;
	size_t second;
	size_t third;

	_tagPoly3D();
	_tagPoly3D(size_t a, size_t b, size_t c);
	VECTOR3D Normal(const LPVERT_LIST, size_t startVert);
	VECTOR3D Normal(const LPVECTOR3D, size_t startVert);

	_tagPoly3D& operator+= (const UINT& p);
	bool operator==(const _tagPoly3D &b) const;
	bool operator!=(const _tagPoly3D &b) const;
	bool isContainingEdge (const EDGE3D &e);
	bool isContainingVertex (size_t vi);
} POLY3D, *LPPOLY3D;

typedef struct _tagColor3D
{
	unsigned char Red;
	unsigned char Green;
	unsigned char Blue;
	_tagColor3D(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0);

	bool operator==(const float &b) const;
	bool operator!=(const float &b) const;
} COLOR3D, *LPCOLOR3D;

class IColorable
{
protected:
	COLORREF color;

	inline IColorable() { setColor(RGB(255, 255, 255)); }
	inline IColorable(COLORREF c) { setColor(c); }

public:
	inline void setColor(COLORREF c) { color = c; }
	inline COLORREF getColor() { return color; }
};

#define BLACK 0

// This is for simple colorref color extraction 
#define RED(c)		(BYTE)(c)
#define GREEN(c)	(BYTE)(c>>8)
#define BLUE(c)		(BYTE)(c>>16)

// ============================================================================
// Object class that represents current object's in-space position

#define MAX_OBJECT_NAME_LEN	256
// #define OBJECTS_NUM			4

enum CLASS_ID {
	CLS_OBJECT,
	CLS_MESH,
	CLS_CAMERA,
	CLS_LIGHT
};

enum CONSTRAINTS {
	C_FREE	= 0,
	C_XY	= 1,	
	C_ZX	= 2,
	C_ZY	= 3
};

class CObject {
private: 	
	CLASS_ID		ClassID;
	size_t			ID;

	static size_t	Counter;

	static void InitDefaultValues(CObject *obj);

protected:
	LPTSTR			Name;

	// Position
	VECTOR3D		pos;
	// Orientation
	VECTOR3D		fWd;
	VECTOR3D		rWd;
	VECTOR3D		uWd;
	VECTOR3D		world;

	// Scaling
	VECTOR3D		worldScale;
	VECTOR3D		localScale;

	CONSTRAINTS		moveConst;

	void PitchYawRoll();

public:
	CObject(CLASS_ID clsID = CLS_OBJECT);
	CObject(const VECTOR3D	&pt, 
		float		p, 
		float		y, 
		float		r, 
		CLASS_ID	clsID = CLS_OBJECT
	);
	CObject(float pX, 
		float pY, 
		float pZ, 
		float p,
		float y,
		float r, 
		CLASS_ID clsID = CLS_OBJECT
	);
	virtual ~CObject();
	CLASS_ID clsID();
	size_t	 objID();

	// Positioning
	void MoveAlong(float units);		// Along local-x
	void MoveHAcross(float units);		// Along local-y
	void MoveVAcross(float units);		// Along local-z
	void Move(const VECTOR3D &tV);
	void Move(float tX, float tY, float tZ);

	void Translate(const VECTOR3D &tV);
	void Translate(float tX, float tY, float tZ);	 

	// Scaling
	void ScaleAlong(float factor);
	void ScaleHAcross(float factor);
	void ScaleVAcross(float factor);
	void LocalScale(float fx, float fy, float fz);
	void Scale(float fx, float fy, float fz);
	
	// Orientation
	void RotatePitch(float angle);
	void RotateYaw(float angle);
	void RotateRoll(float angle);
	void LocalRotate(float roll, float yaw, float pitch);
	void Rotate(float y, float z, float x);

	float Distance(const VECTOR3D &vDst);
	float Distance(const CObject &objDst);

	// Take a look at point or object
	void LookAt(const VECTOR3D &lookAt, const LPVECTOR3D upOrient);
	void LookAt(const CObject &objToLookAt);
	void LookAt(float lX, float lY, float lZ);

	static void ArbitraryOrbitMoveAround(
		CObject *obj, 
		const VECTOR3D &vAxis, 
		const VECTOR3D &vPivot, 
		float units
		);

	void HorizOrbitMoveAround(const VECTOR3D &vPivot, float units);
	void HorizOrbitMoveAround(const CObject &objPivot, float units);
	void HorizOrbitMoveAround(float pX, float pY, float pZ, float units);

	void VertOrbitMoveAround(const VECTOR3D &vPivot, float units);
	void VertOrbitMoveAround(const CObject &objPivot, float units);
	void VertOrbitMoveAround(float pX, float pY, float pZ, float units);

	void MoveTo(const VECTOR3D &vPivot, float units);
	void MoveTo(const CObject &objPivot, float units);
	void MoveTo(float pX, float pY, float pZ, float units);

	void GetMoveMatrix(MATRIX3D &mOut);
	void GetLocalScaleMatrix(MATRIX3D &mOut);
	void GetScaleMatrix(MATRIX3D &mOut);
	void GetRotationMatrix(MATRIX3D &mOut);

	VECTOR3D getForwardLookDirection();
	VECTOR3D getRightLookDirection();
	VECTOR3D getUpLookDirection();	
	VECTOR3D getPosition();
	VECTOR3D getRotation();
	VECTOR3D getScale();

	void setForwardLookDirection(const VECTOR3D &);
	void setRightLookDirection(const VECTOR3D &);
	void setUpLookDirection(const VECTOR3D &);

	void getName(LPTSTR objName, size_t bufSize);
	void setName(LPTSTR objName, size_t srcSize);
};
typedef CObject OBJECT3D, *LPOBJECT3D;

// ============================================================================
// Class CMesh

typedef vector<EDGE3D> EDGE_LIST, *LPEDGE_LIST;
typedef vector<POLY3D> POLY_LIST, *LPPOLY_LIST;

class CMesh : public CObject, public IColorable {
private:
	size_t			_mTypeID;

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
	CMesh(size_t meshTypeId);
	CMesh(COLORREF c, size_t meshTypeId);
	CMesh(const VECTOR3D &pt,
		float p, 
		float y, 
		float r, 
		size_t meshTypeId
		);
	CMesh(const VECTOR3D &pt,
		float p, 
		float y, 
		float r, 
		COLORREF c,
		size_t meshTypeId
		);
	CMesh(float	pX, 
		float pY, 
		float pZ, 
		float p, 
		float y, 
		float r, 
		size_t meshTypeId
		);
	CMesh(float	pX, 
		float pY, 
		float pZ, 
		float p, 
		float y, 
		float r, 
		COLORREF c,
		size_t meshTypeId
		);

	// getters
	size_t			getMeshID();
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

	void			setMeshID(size_t);

	void			addVertices(const LPVECTOR3D vs, UINT vsCount);
	bool			delVertices();
	
	void			addEdges(const LPEDGE3D es, UINT esCount);
	bool			delEdges(EDGE3D);

	void			addPolygons(const LPPOLY3D ps, UINT psCount);
	bool			delPolygon(POLY3D);

	void			Transform();
};
typedef CMesh MESH3D, *LPMESH3D;

// ============================================================================
// World class which represents real world enumerations and sets the WCS

typedef vector<LPOBJECT3D> OBJECTS_LIST;
typedef map<CLASS_ID, OBJECTS_LIST> CONTENT;
typedef pair<CLASS_ID, OBJECTS_LIST> CONTENT_CLASS;

class CScene {
private:
	CONTENT		objects;
	COLORREF	ambientColor;

public:
	CScene();
	~CScene();

	bool AddObject(const LPOBJECT3D lpObject);
	bool DeleteObject(CLASS_ID clsID, size_t objIndex); 
	bool DeleteObject(size_t objID);
	bool DeleteObject(const LPOBJECT3D lpObject);
	void Clear();

	bool findObjectIndex(const LPOBJECT3D lpObject, size_t *objIndex = NULL);
	bool findObjectIndex(
				size_t objID, 
				CLASS_ID *objClsID	= NULL, 
				size_t *objIndex	= NULL
			);

	bool		getFirstObject(CLASS_ID *objID);
	LPOBJECT3D	getFirstObject();
	LPOBJECT3D	getObject(CLASS_ID objID, size_t objIndex);
	LPOBJECT3D	getObject(size_t objID);
	size_t		getPolygonsCount();
	size_t		getVerticesCount();
	COLORREF	getAmbientColor();

	VOID		setAmbientColor(COLORREF color);

	size_t getObjectClassCount(CLASS_ID clsID);
};
typedef CScene SCENE3D, *LPSCENE3D;

#include "Basis.inl"

