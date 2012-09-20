
#pragma once

typedef int PROJECTION_TYPE;
#define PT_PARALLEL		0
#define PT_CENTRAL		1

typedef int RENDER_MODE;
#define RM_WIREFRAME	0x01
#define RM_SHADED		0x02
#define RM_SHADEDWF		0x03

typedef int VIEW_TYPE;
#define	VIEW_LEFT			1
#define	VIEW_RIGHT			2
#define	VIEW_FRONT			3
#define	VIEW_BACK			4
#define	VIEW_TOP			5
#define	VIEW_BOTTOM			6
#define	VIEW_PERSPECTIVE	7