#pragma once

#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Scene.h"

// Custom error codes
#define E_POOL_NOT_INITIALIZED		0x10000000
#define E_EVENT_CREATION_FAILED		0x10000005
#define E_THREAD_CREATION_FAILED	0x10000006

#define E_EVENT_NOT_EXIST			0x1000000A
#define E_VIEWPORT_NOT_EXIST		0x1000000B
#define E_NO_VIEWPORTS_EXIST		0x1000000C
#define E_THREAD_WAIT_TIMEOUT		0x1000000D
#define E_MAX_COUNT_REACHED			0x1000000E

#define SHUTDOWN_ON_DEMAND			0

typedef HANDLE	THREAD, *LPTHREAD, 
				EVENT, *LPEVENT;

// Necessary Viewport class definitions
enum RENDER_MODE {
	RM_WIREFRAME	= 0x01,
	RM_SHADED		= 0x02,
	RM_SHADEDWF		= 0x03
};

#define MAX_VIEWPORT_COUNT	6
#define CLOCKS_PER_FRAME	50
#define THREAD_WAIT_TIMEOUT	5000

typedef struct tagTHREAD_CONTROLS {
	EVENT doRender;
	EVENT shutDown;
	EVENT jobDone;
} THREAD_CONTROLS, *LPTHREAD_CONTROLS;

// first - index of a polygon in mesh, second - index of a mesh in scene
typedef vector <pair<DIRECTPOLY3D, UINT>> SCENEPOLY;
typedef vector <VECTOR3D> SCENEVERT;

#define DEFAULT_CAMERA_ID	UINT_MAX

// ============================================================================
// CViewport provides an interface for picture plane that recieves 2D projection 
// of a 3D space through camera's eye.

class CViewport {
private:
	static CAMERA3D	camDefault;

	HDC				hDCOutput;
	HBITMAP			hBmpOutput;
	HBITMAP			hBmpDefault;

	LPSCENE3D		Scene;
	LPCAMERA3D		camOutput;

	RENDER_MODE		rMode;

public:
	CViewport();
	virtual ~CViewport();
	
	LPSCENE3D		getScene()				const;	
	UINT_PTR		getCameraObjectID()		const;
	LPCAMERA3D	getCamera()					const;
	RENDER_MODE		getRenderMode()			const;
	UINT			getWidth()				const;
	UINT			getHeight()				const;
	VOID			getSize(SIZE &vpSize)	const;

	BOOL			setScene(LPSCENE3D lpSceneHost);
	BOOL			setCameraByObjectID(UINT uCameraObjectID);
	BOOL			setCamera(const LPCAMERA3D cam);
	VOID			setRenderMode(RENDER_MODE renderMode);
	VOID			setWidth(UINT vpWidth);
	VOID			setHeight(UINT vpHeight);
	VOID			setSize(const SIZE &vpSize);

	BOOL			Render(HDC hDCScreen);
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

#define NO_ACTIVE_VIEWPORT UINT_MAX

typedef struct tagTHREAD_DATA {
	HDC				hDCScreen;

	LPVIEWPORT		Viewport;
	BOOL			isActive;

	THREAD			Thread;
	THREAD_CONTROLS threadControls;
} THREAD_DATA, *LPTHREAD_DATA;

typedef vector<LPTHREAD_DATA> LPVIEWPORTS_LIST;
typedef vector<EVENT> LPEVENTS_LIST;
class ÑRenderPool {
private:
	LPSCENE3D			Scene;
	LPVIEWPORTS_LIST	Viewports;

	LPEVENTS_LIST		vpStates;
	EVENT				renderEvent;

	static DWORD WINAPI Render(LPVOID renderInfo);
	UINT				findViewport(DWORD vpID);
public:
	ÑRenderPool();
	~ÑRenderPool();

	BOOL		assignScene(LPSCENE3D lpScene);
	DWORD		addViewport(HDC hDCScreen, UINT vpWidth, UINT vpHeight, VIEW_TYPE vpVType, RENDER_MODE vpRMode);
	BOOL		delViewport(UINT vpIndex);

	LPVIEWPORT	getViewport(UINT vpIndex);
	LPVIEWPORT	getActiveViewport();
	UINT_PTR	getActiveViewportIndex();
	UINT_PTR	getViewportCount();
	
	VOID		setActiveViewport(UINT vpIndex);

	DWORD		RenderWorld();
};
typedef ÑRenderPool RENDER_POOL, *LPRENDER_POOL;