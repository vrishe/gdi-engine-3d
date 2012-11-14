
#pragma once

#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Scene.h"
#include "SharedDef.h"

// first - index of a polygon in mesh, second - index of a mesh in scene
typedef vector <pair<DIRECTPOLY3D, UINT>> SCENEPOLY;
typedef vector <VECTOR3D> SCENEVERT;

// ============================================================================
// _clsViewport provides an interface for picture plane that recieves 2D projection 
// of a 3D space through camera's eye.

typedef class _clsViewport : public IUnknown {
private:
	HDC				hDCOutput;
	HBITMAP			hBmpOutput;
	HBITMAP			hBmpOriginal;

	RENDER_MODE		rMode;

	//static void InitValues(_clsViewport *obj, LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode);
	static void InitValues(_clsViewport *obj, RENDER_MODE rMode);

public:
	_clsViewport();
	_clsViewport(RENDER_MODE rMode);
	//_clsViewport(LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode);
	//_clsViewport(const SIZE &szVp, RENDER_MODE rMode);
	~_clsViewport();
	
	RENDER_MODE	getRenderMode()								const;
	VOID		getSize(SIZE &szVp)							const;
	VOID		getSize(LONG &uVpWidth, LONG &uVpHeight)	const;
	LONG		getWidth()									const;
	LONG		getHeight()									const;

	VOID		setRenderMode(RENDER_MODE renderMode);
	BOOL		setWidth(LONG uVpWidth);
	BOOL		setHeight(LONG uVpHeight);
	BOOL		setSize(LONG uVpWidth, LONG uVpHeight);
	BOOL		setSize(const SIZE &szVp);

	BOOL Render(LPSCENE3D lpScene, LPCAMERA3D lpCamera, HDC hDCScreen) const;
} VIEWPORT, *LPVIEWPORT;

VOID SetViewportDefaultView(LPVIEWPORT vp, VIEW_TYPE vt);

// ============================================================================
// _clsRenderPool provides centralized viewport-n-rendering management. That means 
// it controls multithreaded rendering process which is implemented in
// barrier computtation manner

// TODO: Fetch these constants to an interface-styled customisation (lowest priority)
#define FRAME_STROKE_COLORREF	RGB(250, 170, 65)
#define FRAME_STROKE_WIDTH		3
#define FRAME_FONT_COLOR		RGB(50, 50, 50)
#define FRAME_FONT_SIZE			14
#define FRAME_FONT_FAMILY		_T("Arial narrow")

#define SHUTDOWN_ON_DEMAND			0
#define SHUTDOWN_ON_TERMINATE		-1

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

	LPSCENE3D		*lppScene;
	LPCAMERA3D		lpCamera;
	HDC				hDCScreen;

	THREAD			hThread;
	THREAD_CONTROLS tcEvents;
} THREAD_DATA, *LPTHREAD_DATA;

typedef vector<LPTHREAD_DATA>	THREAD_DATA_LIST;
typedef vector<EVENT>			EVENT_LIST;

typedef class _clsRenderPool : public IUnknown {
private:
	THREAD_DATA_LIST	tdlViewports;
	EVENT_LIST			evlStates;
	EVENT				evTrigger;

	LPSCENE3D			lpScene;

	static DWORD WINAPI Render(LPVOID renderInfo);

public:
	_clsRenderPool();
	_clsRenderPool(LPSCENE3D lpScene);
	~_clsRenderPool();

	DWORD addViewport(
		LPVIEWPORT	lpViewport,
		LPCAMERA3D	lpCamera,
		HDC			hDCScreen
		);
	BOOL delViewport(DWORD dwVpID);
	BOOL delViewport(size_t uVpIndex);

	LPSCENE3D getRenderScene() const;
	
	LPVIEWPORT getViewport(DWORD dwVpID)	const;
	LPVIEWPORT getViewport(size_t uVpIndex)	const;

	size_t getViewportCount() const;

	size_t getViewportIndex(DWORD dwVpID) const;
	DWORD  getViewportID(size_t uVpIndex) const;

	size_t getActiveViewportIndex() const;
	DWORD  getActiveViewportID()	const;

	VOID setRenderScene(LPSCENE3D lpScene);

	VOID setActiveViewport(size_t uVpIndex);
	VOID setActiveViewport(DWORD dwVpID);

	BOOL setViewportScreen(size_t uVpIndex, HDC hDcNew, HDC &hDcOld);
	BOOL setViewportScreen(DWORD dwVpID, HDC hDcNew, HDC &hDcOld);

	DWORD RenderWorld() const;
} RENDER_POOL, *LPRENDER_POOL;

#include "Render.inl"