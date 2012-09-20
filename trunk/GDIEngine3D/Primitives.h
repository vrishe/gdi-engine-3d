#pragma once

#include "Mesh.h"

#define MSH_PYRAMID		MSH_UNDEFINED + 1
#define MSH_CONE		MSH_UNDEFINED + 2
#define MSH_EXCONE		MSH_UNDEFINED + 3
#define MSH_HOLE		MSH_UNDEFINED + 4
#define MSH_SPHERE		MSH_UNDEFINED + 5


// ============================================================================
// CPyramid class declaration

class CPyramid : public CMesh {
protected:
	float h;		// height
	float bL;		// base length
	float bW;		// base width
	float tL;		// top length
	float tW;		// top width
	float shift;    // get your head out by OX

public:
	CPyramid(COLORREF c);
	CPyramid(
		float height, 
		float bLength, 
		float bWidth, 
		float tLength, 
		float tWidth,
		COLORREF	c,
		float sh = 0
	);

	// functionality
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
};
typedef CPyramid PYRAMID3D, *LPPYRAMID3D;


// ============================================================================
// CCone class declaration

class CCone : public CMesh {
protected:
	float h;			// cone height
	float bR;			// base radius
	float tR;			// top radius
	int precision;	// precision of circle approximation in polygons

public:
	CCone(COLORREF c);
	CCone(
		float		height, 
		float		bRadius, 
		float		tRadius, 
		int			prec,
		COLORREF	c
	);


	// functionality
	virtual void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getHeight();
	float getBRadius();
	float getTRadius();
	int getPrecision();

	// setters
	void setHeight(float); 
	void setBRadius(float);
	void setTRadius(float);
	void setPrecission(int);
};
typedef CCone CONE3D, *LPCONE3D;


// ============================================================================
// CExCone class declaration

class CExCone : public CCone {
	/// конус, отсеченный плоскостью, паралельной OXZ и пересекающей OY в т. (0, secant, 0)
	/// должно выполнятся неравенство -min(bR,tR) <= secant <= max(bR, tR),
	/// иначе фигура неопределена
	float secant;

public:
	CExCone(COLORREF c);
	CExCone(
		float height, 
		float bRadius, 
		float tRadius,
		float s,
		int	  prec,
		COLORREF	c
	);

	// functionality
	virtual void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getSecant();

	// setters
	void setSecant(float); 
};
typedef CExCone EXCONE3D, *LPEXCONE3D;


// ============================================================================
// CHole class declaration

class CHole : public CMesh {
protected:
	float h;			// cone height
	float bR;			// base radius
	float bRh;			// base hole radius
	float tR;			// top radius
	float tRh;			// top hole radius
	int precision;	// precision of circle approximation in polygons

public:
	CHole(COLORREF c);
	CHole(
		float height, 
		float bRadius, 
		float bHoleRadius,
		float tRadius,
		float tHoleRadius,
		int	  prec,
		COLORREF	c
	);

	// functionality
	void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getHeight();
	float getBRadius();
	float getTRadius();
	float getBHoleRadius();
	float getTHoleRadius();
	int getPrecision();

	// setters
	void setHeight(float); 
	void setBRadius(float);
	void setTRadius(float);
	void setBHoleRadius(float);
	void setTHoleRadius(float);
	void setPrecission(int);
};
typedef CHole HOLE3D, *LPHOLE3D;


// ============================================================================
// CSphere class declaration

class CSphere : public CMesh {
protected:
	float			radius;
	float			cropMult;
	float			angleFrom;
	float			angleTo;
	unsigned int	precision;

	void			Triangulate();
public:
	CSphere(COLORREF c);
	CSphere(
		float			Radius,
		float			Crop,
		float			From,
		float			To,
		unsigned int	Prec,
		COLORREF		c
	);

	void	setRadius(float Radius);
	void	setCrop(float Plane);
	void	setPrecision(int Prec);

	float	getRadius();
	float	getCrop();
	int		getPrecision();
};
typedef CSphere SPHERE3D, *LPSPHERE3D;