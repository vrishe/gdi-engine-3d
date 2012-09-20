#pragma once

#include "stdafx.h"
#include "Primitives.h"


// ============================================================================
// _clsPyramid class implementation

void _clsPyramid::Triangulate() {
	vertices.clear();
	edges.clear();
	polygons.clear();

		// setting base vertices
	vertices.push_back(VECTOR3D(-bL/2,-bW/2, 0));	// 0
	vertices.push_back(VECTOR3D(-bL/2,bW/2, 0));	// 1
	vertices.push_back(VECTOR3D(bL/2,bW/2, 0));		// 2
	vertices.push_back(VECTOR3D(bL/2,-bW/2, 0));	// 3
		// setting top vertices
	vertices.push_back(VECTOR3D(-tL/2 + shift,-tW/2, h));	// 4
	vertices.push_back(VECTOR3D(-tL/2 + shift,tW/2, h));	// 5
	vertices.push_back(VECTOR3D(tL/2 + shift,tW/2, h));		// 6
	vertices.push_back(VECTOR3D(tL/2 + shift,-tW/2, h));	// 7

		// setting edges
	// setting base edges
	edges.push_back(EDGE3D(0,1));	
	edges.push_back(EDGE3D(1,2));
	edges.push_back(EDGE3D(2,3));
	edges.push_back(EDGE3D(3,0));
	edges.push_back(EDGE3D(0,2));
		// setting top edges
	edges.push_back(EDGE3D(4,5));	
	edges.push_back(EDGE3D(5,6));
	edges.push_back(EDGE3D(6,7));
	edges.push_back(EDGE3D(7,4));
	edges.push_back(EDGE3D(4,6));	
		// setting side edges
	edges.push_back(EDGE3D(0,4));
	edges.push_back(EDGE3D(0,5));
	edges.push_back(EDGE3D(1,5));
	edges.push_back(EDGE3D(1,6));
	edges.push_back(EDGE3D(2,6));	
	edges.push_back(EDGE3D(2,7));	
	edges.push_back(EDGE3D(3,7));	
	edges.push_back(EDGE3D(3,4));

		// setting base polygons
	polygons.push_back(POLY3D(0,1,2)); // 0
	polygons.push_back(POLY3D(0,2,3)); // 1
		// setting top polygons
	polygons.push_back(POLY3D(5,4,6)); // 2
	polygons.push_back(POLY3D(6,4,7)); // 3
		// setting side polygons
	polygons.push_back(POLY3D(4,0,3));	// 4
	polygons.push_back(POLY3D(7,4,3));	// 5
	polygons.push_back(POLY3D(7,3,2));	// 6
	polygons.push_back(POLY3D(6,7,2));	// 7
	polygons.push_back(POLY3D(6,2,1));	// 8
	polygons.push_back(POLY3D(5,6,1));	// 9
	polygons.push_back(POLY3D(5,1,0));	// 10
	polygons.push_back(POLY3D(4,5,0));	// 11

	flushVertices();
	vertices.shrink_to_fit();
	edges.shrink_to_fit();
	polygons.shrink_to_fit();
}

_clsPyramid::_clsPyramid(COLORREF c) 
	: MESH3D(c, MSH_PRIMITIVE), h(0), bL(0), bW(0), tL(0), tW(0), shift(0) { }

_clsPyramid::_clsPyramid(
		float		height, 
		float		bLength, 
		float		bWidth, 
		float		tLength, 
		float		tWidth,
		COLORREF	c,
		float		sh
) : MESH3D(c, MSH_PRIMITIVE)
{ 
	h  = height;
	bL = bLength;
	bW = bWidth;
	tL = tLength;
	tW = tWidth;
	shift = sh;
	Triangulate();
}

float _clsPyramid::getHeight()	{ return h; }
float _clsPyramid::getBLength() { return bL; }
float _clsPyramid::getBWidth()	{ return bW; }
float _clsPyramid::getTLength() {return tL; }
float _clsPyramid::getTWidth()	{return tW; }

void _clsPyramid::setHeight(float n)	{ h = n; }
void _clsPyramid::setBLength(float n)	{ bL = n; }
void _clsPyramid::setBWidth(float n)	{ bW = n; }
void _clsPyramid::setTLength(float n)	{ tL = n; }
void _clsPyramid::setTWidth(float n)	{tW = n; }


// ============================================================================
// _clsCone class implementation

void _clsCone::Triangulate() {
	vertices.clear();
	edges.clear();
	polygons.clear();

	int step = 360 / precision;
		// setting vertices
	vertices.push_back(VECTOR3D(0, 0, 0));
	vertices.push_back(VECTOR3D(0, 0, h));
	for (int i = 0; i < precision; i++) {
		switch (step*i) {
		case 0:
			vertices.push_back(VECTOR3D(bR, 0, 0));
			vertices.push_back(VECTOR3D(tR, 0, h));
			break;
		case 90:
			vertices.push_back(VECTOR3D(0, bR, 0));
			vertices.push_back(VECTOR3D(0, tR, h));
			break;
		case 180:
			vertices.push_back(VECTOR3D(-bR, 0, 0));
			vertices.push_back(VECTOR3D(-tR, 0, h));
			break;
		case 270:
			vertices.push_back(VECTOR3D(0, -bR, 0));
			vertices.push_back(VECTOR3D(0, -tR, h));
			break;
		default:
			float k = (float)tan((step*i) * 3.1415926535 / 180), x;
			if (step*i < 90 || step*i > 270)
 				x = sqrt(bR*bR / (1 + k*k));
			else
				x = -sqrt(bR*bR / (1 + k*k));
			float y = k * x;
			vertices.push_back(VECTOR3D(x, y, 0));
			if (step*i < 90 || step*i > 270)
 				x = sqrt(tR*tR / (1 + k*k));
			else
				x = -sqrt(tR*tR / (1 + k*k));
			y = k * x;
			vertices.push_back(VECTOR3D(x, y, h));
		}
	}

		// setting base edges
	for (int i = 1; i < precision; i++) {
		edges.push_back(EDGE3D(0, i*2));
		edges.push_back(EDGE3D(i*2, i*2 + 2));
	}
	edges.push_back(EDGE3D(0, precision*2));
	edges.push_back(EDGE3D(precision*2, 2));
		// setting top edges
	for (int i = 1; i < precision; i++) {
		edges.push_back(EDGE3D(1, i*2 + 1));
		edges.push_back(EDGE3D(i*2 + 1, i*2 + 3));
	}
	edges.push_back(EDGE3D(1, precision*2 + 1));
	edges.push_back(EDGE3D(precision*2 + 1, 3));
		// setting side edges
	for (int i = 1; i < precision; i++) {
		edges.push_back(EDGE3D(i*2, i*2 + 1));
		edges.push_back(EDGE3D(i*2 + 1, i*2 + 2));
	}
	edges.push_back(EDGE3D(precision*2, precision*2 + 1));
	edges.push_back(EDGE3D(precision*2 + 1, 2));

		// setting base polygons
	for (int i = 1; i < precision; i++)
		polygons.push_back(POLY3D(i*2, 0, i*2 + 2));
	polygons.push_back(POLY3D(precision*2, 0, 2));
		// setting top polygons
	for (int i = 1; i < precision; i++)
		polygons.push_back(POLY3D(1, i*2 + 1, i*2 + 3));
	polygons.push_back(POLY3D(1, precision*2 + 1, 3));
		// setting side polygons
	for (int i = 1; i < precision; i++) {
		polygons.push_back(POLY3D(i*2 + 1, i*2, i*2 + 2));
		polygons.push_back(POLY3D(i*2 + 1, i*2 + 2, i*2 + 3));
	}
	polygons.push_back(POLY3D(precision*2 + 1, precision*2, 2));
	polygons.push_back(POLY3D(precision*2 + 1, 2, 3));

	flushVertices();
	vertices.shrink_to_fit();
	edges.shrink_to_fit();
	polygons.shrink_to_fit();
}

_clsCone::_clsCone(COLORREF c) 
	: MESH3D(c, MSH_PRIMITIVE), h(0), bR(0), tR(0), precision(24) { }

_clsCone::_clsCone(
		float		height, 
		float		bRadius, 
		float		tRadius, 
		int			prec,
		COLORREF	c
) : MESH3D(c, MSH_PRIMITIVE)
{ 
	h			= height;
	bR			= bRadius;
	tR			= tRadius;
	precision	= prec;
	Triangulate();
}

float _clsCone::getHeight()		{ return h; }
float _clsCone::getBRadius()	{ return bR; }
float _clsCone::getTRadius()	{ return tR; }
int	  _clsCone::getPrecision()	{ return precision; }

void _clsCone::setHeight(float n)	{ h = n; } 
void _clsCone::setBRadius(float n)	{ bR = n; }
void _clsCone::setTRadius(float n)	{ tR = n; }
void _clsCone::setPrecission(int n)	{ precision = n; }


// ============================================================================
// _clsPipe class implementation

_clsPipe::_clsPipe(COLORREF c) 
	: MESH3D(c, MSH_PRIMITIVE), h(0), bR(0), bRh(0), tR(0), tRh(0), precision(0) { }

_clsPipe::_clsPipe(
		float height, 
		float bRadius, 
		float bHoleRadius,
		float tRadius,
		float tHoleRadius,
		int	  prec,
		COLORREF	c
) : MESH3D(c, MSH_PRIMITIVE)
{ 
	h			= height;
	bR			= bRadius;
	bRh			= bHoleRadius;
	tR			= tRadius;
	tRh			= tHoleRadius;
	precision	= prec;
	Triangulate();
}

void _clsPipe::Triangulate() {
	vertices.clear();
	edges.clear();
	polygons.clear();

	int step = 360 / precision;
		// setting vertices
	for (int i = 0; i < precision; i++) { // setting outter circle
		switch (step*i) {
		case 0:
			vertices.push_back(VECTOR3D(bR, 0, 0));
			vertices.push_back(VECTOR3D(tR, 0, h));
			break;
		case 90:
			vertices.push_back(VECTOR3D(0, bR, 0));
			vertices.push_back(VECTOR3D(0, tR, h));
			break;
		case 180:
			vertices.push_back(VECTOR3D(-bR, 0, 0));
			vertices.push_back(VECTOR3D(-tR, 0, h));
			break;
		case 270:
			vertices.push_back(VECTOR3D(0, -bR, 0));
			vertices.push_back(VECTOR3D(0, -tR, h));
			break;
		default:
			float k = tan((float)((step*i) * M_PI / 180)), x;
			if (step*i < 90 || step*i > 270)
 				x = sqrt(bR*bR / (1 + k*k));
			else
				x = -sqrt(bR*bR / (1 + k*k));
			float y = k * x;
			vertices.push_back(VECTOR3D(x, y, 0));

			if (step*i < 90 || step*i > 270)
 				x = sqrt(tR*tR / (1 + k*k));
			else
				x = -sqrt(tR*tR / (1 + k*k));
			y = k * x;
			vertices.push_back(VECTOR3D(x, y, h));
		}
	}
	for (int i = 0; i < precision; i++) { // setting inner circle
		switch (step*i) {
		case 0:
			vertices.push_back(VECTOR3D(bRh, 0, 0));
			vertices.push_back(VECTOR3D(tRh, 0, h));
			break;
		case 90:
			vertices.push_back(VECTOR3D(0, bRh, 0));
			vertices.push_back(VECTOR3D(0, tRh, h));
			break;
		case 180:
			vertices.push_back(VECTOR3D(-bRh, 0, 0));
			vertices.push_back(VECTOR3D(-tRh, 0, h));
			break;
		case 270:
			vertices.push_back(VECTOR3D(0, -bRh, 0));
			vertices.push_back(VECTOR3D(0, -tRh, h));
			break;
		default:
			float k = tan((float)((step*i) * M_PI / 180)), x;
			if (step*i < 90 || step*i > 270)
 				x = sqrt(bRh*bRh / (1 + k*k));
			else
				x = -sqrt(bRh*bRh / (1 + k*k));
			float y = k * x;
			vertices.push_back(VECTOR3D(x, y, 0));
			if (step*i < 90 || step*i > 270)
 				x = sqrt(tRh*tRh / (1 + k*k));
			else
				x = -sqrt(tRh*tRh / (1 + k*k));
			y = k * x;
			vertices.push_back(VECTOR3D(x, y, h));
		}
	}

		// setting base edges
	for (int i = 0; i < precision - 1; i++) {
		edges.push_back(EDGE3D(i*2, precision*2 + i*2));
		edges.push_back(EDGE3D(i*2, precision*2 + i*2 + 2));
		edges.push_back(EDGE3D(i*2, i*2 + 2));
		edges.push_back(EDGE3D(precision*2 + i*2, precision*2 + i*2 + 2));	
	}
	edges.push_back(EDGE3D(precision*2 - 2, precision*4 - 2));
	edges.push_back(EDGE3D(precision*2 - 2, precision*2));
	edges.push_back(EDGE3D(precision*2 - 2, 0));
	edges.push_back(EDGE3D(precision*4 - 2, precision*2));
		// setting top edges
	for (int i = 0; i < precision - 1; i++) {
		edges.push_back(EDGE3D(i*2 + 1, precision*2 + i*2 + 1));
		edges.push_back(EDGE3D(i*2 + 1, precision*2 + i*2 + 3));
		edges.push_back(EDGE3D(i*2 + 1, i*2 + 3));
		edges.push_back(EDGE3D(precision*2 + i*2 + 1, precision*2 + i*2 + 3));
	}
	edges.push_back(EDGE3D(precision*2 - 1, precision*4 - 1));
	edges.push_back(EDGE3D(precision*2 - 1, precision*2 + 1));
	edges.push_back(EDGE3D(precision*2 - 1, 1));
	edges.push_back(EDGE3D(precision*4 - 1, precision*2 + 1));
		// setting side edges
	for (int i = 0; i < precision - 1; i++) { // outter circle
		edges.push_back(EDGE3D(i*2, i*2 + 1));
		edges.push_back(EDGE3D(i*2 + 1, i*2 + 2));
	}
	edges.push_back(EDGE3D(precision*2 - 2, precision*2 - 1));
	edges.push_back(EDGE3D(precision*2 - 1, 0));
	for (int i = 0; i < precision - 1; i++) { // inner circle
		edges.push_back(EDGE3D(precision*2 + i*2, precision*2 + i*2 + 1));
		edges.push_back(EDGE3D(precision*2 + i*2 + 1, precision*2 + i*2 + 2));
	}
	edges.push_back(EDGE3D(precision*4 - 2, precision*4 - 1));
	edges.push_back(EDGE3D(precision*4 - 1, precision*2));

		// setting base polygons
	for (int i = 0, j = 0; i < precision - 1; i++, j += 2) {
		polygons.push_back(POLY3D(i*2, precision*2 + i*2, precision*2 + i*2 + 2));
		polygons.push_back(POLY3D(precision*2 + i*2 + 2, i*2 + 2, i*2));
	}
	polygons.push_back(POLY3D(precision*2 - 2, precision*4 - 2, precision*2));
	polygons.push_back(POLY3D(precision*2, 0, precision*2 - 2));
		// setting top polygons
	for (int i = 0, j = precision*2; i < precision - 1; i++, j += 2) {
		polygons.push_back(POLY3D(precision*2 + i*2 + 1, i*2 + 1, precision*2 + i*2 + 3));
		polygons.push_back(POLY3D(i*2 + 3, precision*2 + i*2 + 3, i*2 + 1));
	}
	polygons.push_back(POLY3D(precision*4 - 1, precision*2 - 1, precision*2 + 1));
	polygons.push_back(POLY3D(1, precision*2 + 1, precision*2 - 1));
		// setting side polygons
	for (int i = 0, j = precision*4; i < precision - 1; i++, j+= 2) { // outter circle
		polygons.push_back(POLY3D(i*2 + 1, i*2, i*2 + 2));
		polygons.push_back(POLY3D(i*2 + 3, i*2 + 1, i*2 + 2));
	}
	polygons.push_back(POLY3D(precision*2 - 1, precision*2 - 2, 0));
	polygons.push_back(POLY3D(1, precision*2 - 1, 0));
	for (int i = 0, j = precision*6; i < precision - 1; i++, j += 2) { // inner circle
		polygons.push_back(POLY3D(precision*2 + i*2, precision*2 + i*2 + 1, precision*2 + i*2 + 2));
		polygons.push_back(POLY3D(precision*2 + i*2 + 1, precision*2 + i*2 + 3, precision*2 + i*2 + 2));
	}
	polygons.push_back(POLY3D(precision*4 - 2, precision*4 - 1, precision*2));
	polygons.push_back(POLY3D(precision*4 - 1, precision*2 + 1, precision*2));

	flushVertices();
	vertices.shrink_to_fit();
	edges.shrink_to_fit();
	polygons.shrink_to_fit();
}

float _clsPipe::getHeight()			{ return h; }
float _clsPipe::getBRadius()		{ return bR; }
float _clsPipe::getTRadius()		{ return tR; }
float _clsPipe::getBHoleRadius()	{ return bRh; }
float _clsPipe::getTHoleRadius()	{ return tRh; }
int   _clsPipe::getPrecision()		{ return precision; }


void _clsPipe::setHeight(float n)		{ h = n; } 
void _clsPipe::setBRadius(float n)		{ bR = n; }
void _clsPipe::setTRadius(float n)		{ tR = n; }
void _clsPipe::setBHoleRadius(float n)	{ bRh = n; }
void _clsPipe::setTHoleRadius(float n)	{ tRh = n; }
void _clsPipe::setPrecission(int n)		{ precision = n; }


// ============================================================================
// _clsSphere class implementation

_clsSphere::_clsSphere(
		float			Radius,
		float			Crop,
		float			From,
		float			To,
		unsigned int	Prec,
		COLORREF		c
) : MESH3D(c, MSH_PRIMITIVE) {
	radius		= abs(Radius);
	precision	= Prec;
	cropMult	= Crop;
	angleFrom	= From;
	angleTo		= To;
	if ( cropMult < EPS	)			cropMult = .0f;
	if ( cropMult - 1.0f > EPS )	cropMult = 1.0f;

	Triangulate();
}

void _clsSphere::Triangulate()
{
	if ( cropMult < 1.0f )
	{
		vertices.clear();
		edges.clear();
		polygons.clear();

		float	cropZ	= radius *  (2 * cropMult - 1),
				vAngle  = asin(cropZ / radius),
				vDelta	= ((float)M_PI_2 - vAngle) / (float)(precision / 2),	
				hDelta	= abs((angleTo - angleFrom) / precision),
				radius2p = radius * radius;

		BOOL isSliced	= cos(angleFrom) != 1.0f || cos(angleTo) != 1.0f;
		UINT perCircle	= isSliced ? precision + 2 : precision;
		if ( abs(cropZ) < radius - EPS ) 
			vertices.push_back(VECTOR3D(.0f, .0f, cropZ));

		while ( (float)M_PI_2 - vAngle > -EPS )
		{
			float cropX	= radius * cos(vAngle);
			UINT vCount	= cropX > EPS ? perCircle : 1;

			LPVECTOR3D v	= new VECTOR3D[vCount];

			float hAngle = angleFrom;
			UINT max = isSliced ? vCount - 1 : vCount;
			for( UINT	i	= 0; i < max; i++ ) 
			{
				v[i].x = cropX * cos(hAngle);
				v[i].y = cropX * sin(hAngle);
				v[i].z = cropZ;

				hAngle += hDelta;
			}
			if ( isSliced ) v[max].z = cropZ;
			vertices.insert(vertices.end(), v, v + vCount);
			
			delete[] v;

			vAngle += vDelta;
			cropZ = radius * sin(vAngle); 
		}

		UINT	i	= 1,
				max = vertices.size();
		while ( i < max )
		{
			for ( UINT j = i, m = i + perCircle; j < m; j++ )
			{
				UINT current	= j >= max ? max - 1 : j,
					 upward		= ((INT)(current - perCircle)) <= 0 
											? 0 : (current - perCircle),
					 forward	= current >= m - 1 ? i : current + 1,
					 diagonal	= upward == 0 ? 0 : upward + 1;

				if ( forward == max ) 
				{
					upward		+= j - i;
					diagonal	= upward + 1;
					if ( diagonal == i ) diagonal = i - perCircle;
					polygons.push_back(POLY3D(diagonal, current, upward));
				}
				else
				{
					edges.push_back(EDGE3D(current, forward));
					if ( diagonal != 0 )
					{
						if ( diagonal == i ) diagonal = i - perCircle;
						edges.push_back(EDGE3D(current, diagonal));
						polygons.push_back(POLY3D(current, diagonal, forward));
						polygons.push_back(POLY3D(upward, diagonal, current));
					}
					else
					{
						polygons.push_back(POLY3D(upward, forward, current));
					}
				}
				edges.push_back(EDGE3D(current, upward));
			}
			i += perCircle;
		}

		flushVertices();
		vertices.shrink_to_fit();
		edges.shrink_to_fit();
		polygons.shrink_to_fit();
	}
}