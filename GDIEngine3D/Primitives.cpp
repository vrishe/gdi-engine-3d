#pragma once

#include "stdafx.h"
#include "Primitives.h"


// ============================================================================
// _clsPyramid class implementation

void _clsPyramid::Triangulate() {
	vertices.clear();
	polygons.clear();

	// setting base vertices
	vertices.push_back(VECTOR3D(-bL/2, -bW/2, 0));	// 0
	vertices.push_back(VECTOR3D(-bL/2,  bW/2, 0));	// 1
	vertices.push_back(VECTOR3D( bL/2,  bW/2, 0));	// 2
	vertices.push_back(VECTOR3D( bL/2, -bW/2, 0));	// 3
	// setting top vertices
	vertices.push_back(VECTOR3D(-tL/2 + shift, -tW/2, h));	// 4
	vertices.push_back(VECTOR3D(-tL/2 + shift,  tW/2, h));	// 5
	vertices.push_back(VECTOR3D( tL/2 + shift,  tW/2, h));	// 6
	vertices.push_back(VECTOR3D( tL/2 + shift, -tW/2, h));	// 7

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
	polygons.shrink_to_fit();
}

_clsPyramid::_clsPyramid(COLORREF c) 
	: MESH3D(MSH_PRIMITIVE, c), h(0), bL(0), bW(0), tL(0), tW(0), shift(0) { }
_clsPyramid::_clsPyramid(
		float		height, 
		float		bLength, 
		float		bWidth, 
		float		tLength, 
		float		tWidth,
		COLORREF	c,
		float		sh
) : MESH3D(MSH_PRIMITIVE, c)
{ 
	setHeight(height);
	setBLength(bLength);
	setBWidth(bWidth);
	setTLength(tLength);
	setTWidth(tWidth);
	shift = sh;
	Triangulate();
}

float _clsPyramid::getHeight()	{ return h;  }
float _clsPyramid::getBLength() { return bL; }
float _clsPyramid::getBWidth()	{ return bW; }
float _clsPyramid::getTLength() { return tL; }
float _clsPyramid::getTWidth()	{ return tW; }

void _clsPyramid::setHeight(float height)	{ h  = max(height, .0F);  }
void _clsPyramid::setBLength(float bLength)	{ bL = max(bLength, .0F); }
void _clsPyramid::setBWidth(float bWidth)	{ bW = max(bWidth, .0F);  }
void _clsPyramid::setTLength(float tLength)	{ tL = max(tLength, .0F); }
void _clsPyramid::setTWidth(float tWidth)	{ tW = max(tWidth, .0F);  }


// ============================================================================
// _clsCone class implementation

void _clsCone::Triangulate() {
	vertices.clear();
	polygons.clear();

	if (bR > .0F || tR > .0F)
	{
		vertices.push_back(VECTOR3D(.0F, .0F, .0F));
		vertices.push_back(VECTOR3D(.0F, .0F,   h));

		float step = 2 * (FLOAT)M_PI / precision;
		for (unsigned int i = 0; i < precision; ++i)
		{
			float angle		= step * i;
			float cos_angle = cos(angle);
			float sin_angle = sin(angle);

			if (bR > .0F) vertices.push_back(VECTOR3D(cos_angle * bR, sin_angle * bR, .0F));
			if (tR > .0F) vertices.push_back(VECTOR3D(cos_angle * tR, sin_angle * tR,   h));
		}
	}

	if (vertices.size() == 2 * (precision + 1))
	{
		for (size_t i = 2, max = vertices.size(); i < max; i += 2)
		{
			size_t next_index = i < (max - 3) ? i : 0;

			polygons.push_back(POLY3D(i, 0, next_index + 2));
			polygons.push_back(POLY3D(i + 1, i, next_index + 3));
			polygons.push_back(POLY3D(next_index + 2, next_index + 3, i));
			polygons.push_back(POLY3D(1, i + 1, next_index + 3));
		}
	}
	else if (vertices.size() == precision + 2)
	{
		for (size_t i = 2, max = vertices.size(); i < max; ++i)
		{
			size_t next_index = i < (max - 1) ? i + 1 : 2;

			polygons.push_back(POLY3D(i, 0, next_index));
			polygons.push_back(POLY3D(1, i, next_index));
		}
	}

	flushVertices();
	vertices.shrink_to_fit();
	polygons.shrink_to_fit();
}

_clsCone::_clsCone(COLORREF c) 
	: MESH3D(MSH_PRIMITIVE, c), h(0), bR(0), tR(0), precision(16U) { }
_clsCone::_clsCone(
		float height, 
		float bRadius, 
		float tRadius, 
		unsigned int prec,
		COLORREF	c
) : MESH3D(MSH_PRIMITIVE, c)
{ 
	setHeight(height);
	setBRadius(bRadius);
	setTRadius(tRadius);
	setPrecision(prec);
	Triangulate();
}

float _clsCone::getHeight()		{ return h; }
float _clsCone::getBRadius()	{ return bR; }
float _clsCone::getTRadius()	{ return tR; }
unsigned int _clsCone::getPrecision() { return precision; }

void _clsCone::setHeight(float height)			{ h = max(height, .0F); } 
void _clsCone::setBRadius(float bottomRadius)	{ bR = max(bottomRadius, .0F); }
void _clsCone::setTRadius(float topRadius)		{ tR = max(topRadius, .0F); }
void _clsCone::setPrecision(unsigned int prec)	{ precision = max(prec, 0); }


// ============================================================================
// _clsSphere class implementation

void _clsSphere::Triangulate()
{
	if ( cropMult < 1.0f )
	{
		vertices.clear();
		polygons.clear();

		bool isSliced = angleFrom != .0f || angleTo != .0f;

		float cropZ	 = radius * (2.0F * cropMult - 1.0F),
			  vAngle = asin(cropZ / radius),
			  vDelta = ((float)M_PI_2 - vAngle) / ((float)(precision / 2)),	
			  hDelta = abs((isSliced ? (angleTo - angleFrom) : ((float)((2.0F * M_PI) / (double)precision))));

		size_t perCircle = isSliced ? precision + 2 : precision;
		
		if (abs(cropZ) < radius) vertices.push_back(VECTOR3D(.0f, .0f, cropZ));
		for (size_t v = 0, max_v = precision / 2 + 1; v < max_v; ++v)
		{
			float  cropX  = radius * cos(vAngle);
			size_t vCount = (v != 0 && v != max_v - 1) ? perCircle : 1;

			LPVECTOR3D new_vertices = new VECTOR3D[vCount];

			float  hAngle = angleFrom;
			size_t max_h    = isSliced ? vCount - 1 : vCount;
			for( size_t h = 0; h < max_h; ++h ) 
			{
				new_vertices[h].x = cropX * cos(hAngle);
				new_vertices[h].y = cropX * sin(hAngle);
				new_vertices[h].z = cropZ;

				hAngle += hDelta;
			}
			if ( isSliced ) new_vertices[max_h].z = cropZ;
			vertices.insert(vertices.end(), new_vertices, new_vertices + vCount);
			
			delete[] new_vertices;

			cropZ = radius * sin(vAngle += vDelta); 
		}
		if (vertices.empty()) return;

		for ( size_t i = 1, max_i = perCircle + 1; i < max_i; ++i )
		{
			polygons.push_back(POLY3D(0, (i < max_i - 1 ? i + 1 : 1), i));
		}
		for ( size_t i = 1, max_i = vertices.size() - (perCircle + 1); i < max_i; i += perCircle )
		{
			for ( size_t j = i, max_j = i + perCircle; j < max_j; ++j )
			{
				size_t forward  = j < max_j - 1 ? j + 1 : max_j - perCircle,
					   upward   = j + perCircle, 
					   diagonal = forward + perCircle;

				polygons.push_back(POLY3D(j, forward, upward));
				polygons.push_back(POLY3D(forward, diagonal, upward));
			}

		}
		for ( size_t max_i = vertices.size() - 1, start_i = max_i - perCircle, i = start_i; i < max_i; ++i )
		{
			polygons.push_back(POLY3D((i < max_i - 1 ? i + 1 : start_i), max_i, i));
		}

		flushVertices();
		vertices.shrink_to_fit();
		polygons.shrink_to_fit();
	}
}

_clsSphere::_clsSphere(float Radius, float Crop, float From, float To, unsigned int	Precision, COLORREF	c)
	: MESH3D(MSH_PRIMITIVE, c) 
{
	setRadius(Radius);
	setCrop(Crop);
	setPrecision(Precision);

	setSliceFrom(From);
	setSliceTo(To);

	Triangulate();
}

void _clsSphere::setRadius(float Radius)		 { radius    = max(Radius, .0F); }
void _clsSphere::setCrop(float Crop)			 { cropMult  = max(min(Crop, 1.0F), .0F); }
void _clsSphere::setPrecision(unsigned int Prec) { precision = max(Prec, 4); }
void _clsSphere::setSliceFrom(float from)		 { angleFrom = from - (float)(floor((double)from / (2 * M_PI)) * 2 * M_PI); }
void _clsSphere::setSliceTo(float to)			 { angleTo   = to - (float)(floor((double)to / (2 * M_PI)) * 2 * M_PI); }

float _clsSphere::getRadius()			{ return radius; }
float _clsSphere::getCrop()				{ return cropMult; }
unsigned int _clsSphere::getPrecision() { return precision; }
float _clsSphere::getSliceFrom()		{ return angleFrom; }
float _clsSphere::getSliceTo()			{ return angleTo; }

// ============================================================================
// _clsPipe class implementation

//_clsPipe::_clsPipe(COLORREF c) 
//	: MESH3D(MSH_PRIMITIVE, c), h(0), bR(0), bRh(0), tR(0), tRh(0), precision(0) { }
//
//_clsPipe::_clsPipe(
//		float height, 
//		float bRadius, 
//		float bHoleRadius,
//		float tRadius,
//		float tHoleRadius,
//		int	  prec,
//		COLORREF	c
//) : MESH3D(MSH_PRIMITIVE, c)
//{ 
//	h			= height;
//	bR			= bRadius;
//	bRh			= bHoleRadius;
//	tR			= tRadius;
//	tRh			= tHoleRadius;
//	precision	= prec;
//	Triangulate();
//}
//
//void _clsPipe::Triangulate() {
//	vertices.clear();
//	edges.clear();
//	polygons.clear();
//
//	int step = 360 / precision;
//		// setting vertices
//	for (int i = 0; i < precision; i++) { // setting outter circle
//		switch (step*i) {
//		case 0:
//			vertices.push_back(VECTOR3D(bR, 0, 0));
//			vertices.push_back(VECTOR3D(tR, 0, h));
//			break;
//		case 90:
//			vertices.push_back(VECTOR3D(0, bR, 0));
//			vertices.push_back(VECTOR3D(0, tR, h));
//			break;
//		case 180:
//			vertices.push_back(VECTOR3D(-bR, 0, 0));
//			vertices.push_back(VECTOR3D(-tR, 0, h));
//			break;
//		case 270:
//			vertices.push_back(VECTOR3D(0, -bR, 0));
//			vertices.push_back(VECTOR3D(0, -tR, h));
//			break;
//		default:
//			float k = tan((float)((step*i) * M_PI / 180)), x;
//			if (step*i < 90 || step*i > 270)
// 				x = sqrt(bR*bR / (1 + k*k));
//			else
//				x = -sqrt(bR*bR / (1 + k*k));
//			float y = k * x;
//			vertices.push_back(VECTOR3D(x, y, 0));
//
//			if (step*i < 90 || step*i > 270)
// 				x = sqrt(tR*tR / (1 + k*k));
//			else
//				x = -sqrt(tR*tR / (1 + k*k));
//			y = k * x;
//			vertices.push_back(VECTOR3D(x, y, h));
//		}
//	}
//	for (int i = 0; i < precision; i++) { // setting inner circle
//		switch (step*i) {
//		case 0:
//			vertices.push_back(VECTOR3D(bRh, 0, 0));
//			vertices.push_back(VECTOR3D(tRh, 0, h));
//			break;
//		case 90:
//			vertices.push_back(VECTOR3D(0, bRh, 0));
//			vertices.push_back(VECTOR3D(0, tRh, h));
//			break;
//		case 180:
//			vertices.push_back(VECTOR3D(-bRh, 0, 0));
//			vertices.push_back(VECTOR3D(-tRh, 0, h));
//			break;
//		case 270:
//			vertices.push_back(VECTOR3D(0, -bRh, 0));
//			vertices.push_back(VECTOR3D(0, -tRh, h));
//			break;
//		default:
//			float k = tan((float)((step*i) * M_PI / 180)), x;
//			if (step*i < 90 || step*i > 270)
// 				x = sqrt(bRh*bRh / (1 + k*k));
//			else
//				x = -sqrt(bRh*bRh / (1 + k*k));
//			float y = k * x;
//			vertices.push_back(VECTOR3D(x, y, 0));
//			if (step*i < 90 || step*i > 270)
// 				x = sqrt(tRh*tRh / (1 + k*k));
//			else
//				x = -sqrt(tRh*tRh / (1 + k*k));
//			y = k * x;
//			vertices.push_back(VECTOR3D(x, y, h));
//		}
//	}
//
//		// setting base edges
//	for (int i = 0; i < precision - 1; i++) {
//		edges.push_back(EDGE3D(i*2, precision*2 + i*2));
//		edges.push_back(EDGE3D(i*2, precision*2 + i*2 + 2));
//		edges.push_back(EDGE3D(i*2, i*2 + 2));
//		edges.push_back(EDGE3D(precision*2 + i*2, precision*2 + i*2 + 2));	
//	}
//	edges.push_back(EDGE3D(precision*2 - 2, precision*4 - 2));
//	edges.push_back(EDGE3D(precision*2 - 2, precision*2));
//	edges.push_back(EDGE3D(precision*2 - 2, 0));
//	edges.push_back(EDGE3D(precision*4 - 2, precision*2));
//		// setting top edges
//	for (int i = 0; i < precision - 1; i++) {
//		edges.push_back(EDGE3D(i*2 + 1, precision*2 + i*2 + 1));
//		edges.push_back(EDGE3D(i*2 + 1, precision*2 + i*2 + 3));
//		edges.push_back(EDGE3D(i*2 + 1, i*2 + 3));
//		edges.push_back(EDGE3D(precision*2 + i*2 + 1, precision*2 + i*2 + 3));
//	}
//	edges.push_back(EDGE3D(precision*2 - 1, precision*4 - 1));
//	edges.push_back(EDGE3D(precision*2 - 1, precision*2 + 1));
//	edges.push_back(EDGE3D(precision*2 - 1, 1));
//	edges.push_back(EDGE3D(precision*4 - 1, precision*2 + 1));
//		// setting side edges
//	for (int i = 0; i < precision - 1; i++) { // outter circle
//		edges.push_back(EDGE3D(i*2, i*2 + 1));
//		edges.push_back(EDGE3D(i*2 + 1, i*2 + 2));
//	}
//	edges.push_back(EDGE3D(precision*2 - 2, precision*2 - 1));
//	edges.push_back(EDGE3D(precision*2 - 1, 0));
//	for (int i = 0; i < precision - 1; i++) { // inner circle
//		edges.push_back(EDGE3D(precision*2 + i*2, precision*2 + i*2 + 1));
//		edges.push_back(EDGE3D(precision*2 + i*2 + 1, precision*2 + i*2 + 2));
//	}
//	edges.push_back(EDGE3D(precision*4 - 2, precision*4 - 1));
//	edges.push_back(EDGE3D(precision*4 - 1, precision*2));
//
//		// setting base polygons
//	for (int i = 0, j = 0; i < precision - 1; i++, j += 2) {
//		polygons.push_back(POLY3D(i*2, precision*2 + i*2, precision*2 + i*2 + 2));
//		polygons.push_back(POLY3D(precision*2 + i*2 + 2, i*2 + 2, i*2));
//	}
//	polygons.push_back(POLY3D(precision*2 - 2, precision*4 - 2, precision*2));
//	polygons.push_back(POLY3D(precision*2, 0, precision*2 - 2));
//		// setting top polygons
//	for (int i = 0, j = precision*2; i < precision - 1; i++, j += 2) {
//		polygons.push_back(POLY3D(precision*2 + i*2 + 1, i*2 + 1, precision*2 + i*2 + 3));
//		polygons.push_back(POLY3D(i*2 + 3, precision*2 + i*2 + 3, i*2 + 1));
//	}
//	polygons.push_back(POLY3D(precision*4 - 1, precision*2 - 1, precision*2 + 1));
//	polygons.push_back(POLY3D(1, precision*2 + 1, precision*2 - 1));
//		// setting side polygons
//	for (int i = 0, j = precision*4; i < precision - 1; i++, j+= 2) { // outter circle
//		polygons.push_back(POLY3D(i*2 + 1, i*2, i*2 + 2));
//		polygons.push_back(POLY3D(i*2 + 3, i*2 + 1, i*2 + 2));
//	}
//	polygons.push_back(POLY3D(precision*2 - 1, precision*2 - 2, 0));
//	polygons.push_back(POLY3D(1, precision*2 - 1, 0));
//	for (int i = 0, j = precision*6; i < precision - 1; i++, j += 2) { // inner circle
//		polygons.push_back(POLY3D(precision*2 + i*2, precision*2 + i*2 + 1, precision*2 + i*2 + 2));
//		polygons.push_back(POLY3D(precision*2 + i*2 + 1, precision*2 + i*2 + 3, precision*2 + i*2 + 2));
//	}
//	polygons.push_back(POLY3D(precision*4 - 2, precision*4 - 1, precision*2));
//	polygons.push_back(POLY3D(precision*4 - 1, precision*2 + 1, precision*2));
//
//	flushVertices();
//	vertices.shrink_to_fit();
//	edges.shrink_to_fit();
//	polygons.shrink_to_fit();
//}
//
//float _clsPipe::getHeight()			{ return h; }
//float _clsPipe::getBRadius()		{ return bR; }
//float _clsPipe::getTRadius()		{ return tR; }
//float _clsPipe::getBHoleRadius()	{ return bRh; }
//float _clsPipe::getTHoleRadius()	{ return tRh; }
//int   _clsPipe::getPrecision()		{ return precision; }
//
//
//void _clsPipe::setHeight(float n)		{ h = n; } 
//void _clsPipe::setBRadius(float n)		{ bR = n; }
//void _clsPipe::setTRadius(float n)		{ tR = n; }
//void _clsPipe::setBHoleRadius(float n)	{ bRh = n; }
//void _clsPipe::setTHoleRadius(float n)	{ tRh = n; }
//void _clsPipe::setPrecission(int n)		{ precision = n; }