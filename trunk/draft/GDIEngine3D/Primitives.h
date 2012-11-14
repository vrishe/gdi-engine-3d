#pragma once

#include "Mesh.h"

// ============================================================================
// _clsPyramid class declaration

class ITriangulable
{
public:
	virtual void Triangulate() = 0;
};

typedef class _clsPyramid : public MESH3D, public ITriangulable {
protected:
	float h;		// height
	float bL;		// base length
	float bW;		// base width
	float tL;		// top length
	float tW;		// top width
	float shift;    // get your head out by OX

public:
	_clsPyramid(COLORREF c);
	_clsPyramid(
		float height, 
		float bLength, 
		float bWidth, 
		float tLength, 
		float tWidth,
		COLORREF	c,
		float sh = 0
	);

	virtual void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getHeight();
	float getBLength();
	float getBWidth();
	float getTLength();
	float getTWidth();

	// setters
	void setHeight(float n);
	void setBLength(float n);
	void setBWidth(float n);
	void setTLength(float n);
	void setTWidth(float n);
} PYRAMID3D, *LPPYRAMID3D;


// ============================================================================
// _clsCone class declaration

typedef class _clsCone : public MESH3D, public ITriangulable {
protected:
	float h;			// cone height
	float bR;			// base radius
	float tR;			// top radius
	int precision;	// precision of circle approximation in polygons

public:
	_clsCone(COLORREF c);
	_clsCone(
		float		height, 
		float		bRadius, 
		float		tRadius, 
		int			prec,
		COLORREF	c
	);

	virtual void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getHeight();
	float getBRadius();
	float getTRadius();
	int   getPrecision();

	// setters
	void setHeight(float); 
	void setBRadius(float);
	void setTRadius(float);
	void setPrecission(int);
} CONE3D, *LPCONE3D;


// ============================================================================
// _clsPipe class declaration

typedef class _clsPipe : public MESH3D, public ITriangulable {
protected:
	float h;			// cone height
	float bR;			// base radius
	float bRh;			// base hole radius
	float tR;			// top radius
	float tRh;			// top hole radius
	int precision;	// precision of circle approximation in polygons

	

public:
	_clsPipe(COLORREF c);
	_clsPipe(
		float height, 
		float bRadius, 
		float bHoleRadius,
		float tRadius,
		float tHoleRadius,
		int	  prec,
		COLORREF	c
	);

	virtual void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getHeight();
	float getBRadius();
	float getTRadius();
	float getBHoleRadius();
	float getTHoleRadius();
	int   getPrecision();

	// setters
	void setHeight(float); 
	void setBRadius(float);
	void setTRadius(float);
	void setBHoleRadius(float);
	void setTHoleRadius(float);
	void setPrecission(int);
} PIPE3D, *LPPIPE3D;


// ============================================================================
// _clsSphere class declaration

typedef class _clsSphere : public MESH3D, public ITriangulable {
protected:
	float radius;
	float cropMult;
	float angleFrom;
	float angleTo;
	unsigned int precision;

public:
	_clsSphere(COLORREF c);
	_clsSphere(float Radius, float Crop, float From, float To, unsigned int Precision, COLORREF c);

	virtual void Triangulate();

	void setRadius(float Radius);
	void setCrop(float Crop);
	void setPrecision(unsigned int Precision);
	void setSliceFrom(float from);
	void setSliceTo(float to);

	float getRadius();
	float getCrop();
	unsigned int getPrecision();
	float getSliceFrom();
	float getSliceTo();
} SPHERE3D, *LPSPHERE3D;