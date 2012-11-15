// Basis.h: Contains declaration of basic data types such as: edge, polygon and object from the 
//			geometric point of view. Also here is declared mesh and scene data types.

#pragma once
// TODO: Fix constant object method accessibility
#define BLACK 0

// Macro-definitions for simple single-colored component extraction from COLORREF bit field 
#define RED(c)		(BYTE)(c)
#define GREEN(c)	(BYTE)(c>>8)
#define BLUE(c)		(BYTE)(c>>16)

using namespace std;

// ============================================================================
// Supporting structures: Edge, Polygon (/w normal vector) and Color
// Also here is declared IColorable interface that allows you to define 
// a type for an object that has it's own color.

typedef vector<VECTOR3D> VERT_LIST, *LPVERT_LIST;
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
typedef vector<EDGE3D> EDGE_LIST, *LPEDGE_LIST;

typedef struct _tagNormalPolygon {
	VECTOR3D first;
	VECTOR3D second;
	VECTOR3D third;

	DWORD colorRef;

	VECTOR3D Normal(size_t startVert);
} DIRECTPOLY3D, *LPDIRECTPOLY3D;

typedef struct _tagPoly3D {
	size_t first;
	size_t second;
	size_t third;

	_tagPoly3D();
	_tagPoly3D(size_t a, size_t b, size_t c);
	VECTOR3D Normal(const LPVERT_LIST vs, size_t startVert)	const;
	VECTOR3D Normal(const LPVECTOR3D vs, size_t startVert)	const;
	bool CoordinateMassCenter(const LPVERT_LIST, VECTOR3D &out)		const;
	bool CoordinateMassCenter(const LPVECTOR3D , VECTOR3D &out)		const;

	_tagPoly3D& operator+= (const UINT& p);
	bool operator==(const _tagPoly3D &b) const;
	bool operator!=(const _tagPoly3D &b) const;
	bool isContainingEdge (const EDGE3D &e);
	bool isContainingVertex (size_t vi);
} POLY3D, *LPPOLY3D;
typedef vector<POLY3D> POLY_LIST, *LPPOLY_LIST;

class IColorable
{
protected:
	COLORREF color;

	inline IColorable()				{ setColor(RGB(255, 255, 255)); }
	inline IColorable(COLORREF c)	{ setColor(c); }

public:
	inline void setColor(COLORREF c)	{ color = c; }
	inline COLORREF getColor()			{ return color; }
};

typedef std::basic_string<TCHAR> tstring;

#include "IUnknown.h"

// ============================================================================
// Object class provides important model positioning data such as:
// it's Decart space coordinates and it's LCS position relative to WCS 

#define MAX_OBJECT_NAME_LEN	256

enum CLASS_ID {
	CLS_UNKNOWN,
	CLS_OBJECT ,
	CLS_MESH   ,
	CLS_CAMERA ,
	CLS_LIGHT
};

enum CONSTRAINTS {
	CNST_OBJECT_FREE	,
	CNST_OBJECT_XY_PLANE,	
	CNST_OBJECT_ZX_PLANE,
	CNST_OBJECT_ZY_PLANE
};

typedef class _clsObject : public IUnknown {
private: 	
	CLASS_ID ClassID;

	static void InitDefaultValues(_clsObject *obj);

protected:
	tstring	Name;

	// Position
	VECTOR3D pos;

	// Orientation orts
	VECTOR3D fWd;		// Forward
	VECTOR3D rWd;		// Rightward
	VECTOR3D uWd;		// Upward
	VECTOR3D rot;		// World orts-in-one

	// Scaling
	VECTOR3D	worldScale;
	VECTOR3D	localScale;

public:
	_clsObject(CLASS_ID getClassID = CLS_OBJECT);
	virtual ~_clsObject();

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
	void LocalRotate(float roll, float yaw, float pitch, bool reset_rotation);

	float Distance(const VECTOR3D &vDst);
	float Distance(const _clsObject &objDst);

	// Take a look at point or object
	void LookAt(const VECTOR3D &lookAt, const LPVECTOR3D upOrient);
	void LookAt(const _clsObject &objToLookAt);
	void LookAt(float lX, float lY, float lZ);

	static void ArbitraryOrbitMoveAround(
		_clsObject *obj, 
		const VECTOR3D &vAxis, 
		const VECTOR3D &vPivot, 
		float units
		);

	void HorizOrbitMoveAround(const VECTOR3D &vPivot, float units);
	void HorizOrbitMoveAround(const _clsObject &objPivot, float units);
	void HorizOrbitMoveAround(float pX, float pY, float pZ, float units);

	void VertOrbitMoveAround(const VECTOR3D &vPivot, float units);
	void VertOrbitMoveAround(const _clsObject &objPivot, float units);
	void VertOrbitMoveAround(float pX, float pY, float pZ, float units);

	void MoveTo(const VECTOR3D &vPivot, float units);
	void MoveTo(const _clsObject &objPivot, float units);
	void MoveTo(float pX, float pY, float pZ, float units);

	void GetMoveMatrix(MATRIX4 &mOut);
	void GetLocalScaleMatrix(MATRIX4 &mOut);
	void GetScaleMatrix(MATRIX4 &mOut);
	void GetRotationMatrix(MATRIX4 &mOut);

	CLASS_ID getClassID()	const;

	VECTOR3D getForwardLookDirection();
	VECTOR3D getRightLookDirection();
	VECTOR3D getUpLookDirection();	
	VECTOR3D getPosition();
	VECTOR3D getRotation();
	VECTOR3D getScale();

	void setForwardLookDirection(const VECTOR3D &);
	void setRightLookDirection(const VECTOR3D &);
	void setUpLookDirection(const VECTOR3D &);

	tstring getName() const;
	void setName(const tstring &objName);
} OBJECT3D, *LPOBJECT3D;
typedef std::vector<LPOBJECT3D> OBJECTS_LIST;
typedef std::map<CLASS_ID, OBJECTS_LIST>  CONTENT;
typedef std::pair<CLASS_ID, OBJECTS_LIST> CONTENT_CLASS;

#include "Basis.inl"

