#pragma once

#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Scene.h"

// Necessary Viewport class definitions
enum RENDER_MODE {
	RM_WIREFRAME	= 0x01,
	RM_SHADED		= 0x02,
	RM_SHADEDWF		= 0x03
};

// first - index of a polygon in mesh, second - index of a mesh in scene
typedef vector <pair<DIRECTPOLY3D, UINT>> SCENEPOLY;
typedef vector <VECTOR3D> SCENEVERT;

#define DEFAULT_CAMERA_ID	UINT_MAX

// ============================================================================
// CViewport provides an interface for picture plane that recieves 2D projection 
// of a 3D space through camera's eye.

class CViewport : public IUnknown {
private:
	HDC				hDCOutput;
	HBITMAP			hBmpOutput;
	HBITMAP			hBmpDefault;

	RENDER_MODE		rMode;

public:
	CViewport();
	~CViewport();
	
	RENDER_MODE	getRenderMode()			const;
	UINT		getWidth()				const;
	UINT		getHeight()				const;
	VOID		getSize(SIZE &vpSize)	const;

	VOID		setRenderMode(RENDER_MODE renderMode);
	VOID		setWidth(UINT vpWidth);
	VOID		setHeight(UINT vpHeight);
	VOID		setSize(const SIZE &vpSize);

	BOOL Render(LPSCENE3D lpScene, LPCAMERA3D lpCamera, HDC hDCScreen) const;
};
typedef CViewport VIEWPORT, *LPVIEWPORT;

#define VIEW_DISTANCE_DEFAULT	250
enum VIEW_TYPE {
		VIEW_LEFT			= 1,
		VIEW_RIGHT			= 2,
		VIEW_FRONT			= 3,
		VIEW_BACK			= 4,
		VIEW_TOP			= 5,
		VIEW_BOTTOM			= 6,
		VIEW_PERSPECTIVE	= 7
};

VOID SetViewportDefaultView(LPVIEWPORT vp, VIEW_TYPE vt);

// ============================================================================
// CRenderPool provides centralized viewport-n-rendering management. That means 
// it controls multithreaded rendering process which is implemented in
// barrier computtation manner

// TODO: Fetch these constants to an interface-styled customisation (lowest priority)
#define FRAME_STROKE_COLORREF	RGB(250, 170, 65)
#define FRAME_STROKE_WIDTH		3
#define FRAME_FONT_COLOR		RGB(50, 50, 50)
#define FRAME_FONT_SIZE			14
#define FRAME_FONT_FAMILY		_T("Arial narrow")

#define SHUTDOWN_ON_DEMAND			0

#define MAX_VIEWPORT_COUNT	6
#define CLOCKS_PER_FRAME	50
#define THREAD_WAIT_TIMEOUT	5000
#define NO_ACTIVE_VIEWPORT UINT_MAX

typedef HANDLE	THREAD, *LPTHREAD, 
				EVENT,	*LPEVENT;

typedef struct tagTHREAD_CONTROLS {
	EVENT doRender;
	EVENT shutDown;
	EVENT jobDone;
} THREAD_CONTROLS, *LPTHREAD_CONTROLS;

typedef struct tagTHREAD_DATA {
	LPVIEWPORT		lpViewport;
	BOOL			bIsActive;

	LPSCENE3D		lpScene;
	LPCAMERA3D		lpCamera;
	HDC				hDCScreen;

	THREAD			hThread;
	THREAD_CONTROLS tcEvents;
} THREAD_DATA, *LPTHREAD_DATA;

typedef vector<LPTHREAD_DATA>	THREAD_DATA_LIST;
typedef vector<EVENT>			EVENT_LIST;

class ÑRenderPool : public IUnknown {
private:
	THREAD_DATA_LIST	tdlViewports;
	EVENT_LIST			evlStates;
	EVENT				evTrigger;

	static DWORD WINAPI Render(LPVOID renderInfo);

public:
	ÑRenderPool();
	~ÑRenderPool();

	DWORD addViewport(
		LPCAMERA3D lpCamera,
		HDC hDCScreen, 
		UINT vpWidth, 
		UINT vpHeight, 
		RENDER_MODE vpRMode
		);
	BOOL delViewport(size_t uVpIndex);

	size_t getViewportCount();

	size_t getViewportIndex(DWORD dwVpID);
	DWORD  getViewportID(size_t uVpIndex);

	size_t getActiveViewportIndex();
	DWORD  getActiveViewportID();

	VOID setActiveViewport(size_t uVpIndex);
	VOID setActiveViewport(DWORD dwVpID);

	DWORD RenderWorld(LPSCENE3D lpScene);
};
typedef ÑRenderPool RENDER_POOL, *LPRENDER_POOL;

#include "Render.inl"