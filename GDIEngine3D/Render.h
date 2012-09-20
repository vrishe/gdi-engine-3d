
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
// CViewport provides an interface for picture plane that recieves 2D projection 
// of a 3D space through camera's eye.

class CViewport : public IUnknown {
private:
	HDC				hDCOutput;
	HBITMAP			hBmpOutput;
	HBITMAP			hBmpDefault;

	RENDER_MODE		rMode;

	static void InitValues(CViewport *obj, LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode);

public:
	CViewport();
	CViewport(LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode);
	CViewport(const SIZE &szVp, RENDER_MODE rMode);
	~CViewport();
	
	RENDER_MODE	getRenderMode()								const;
	LONG		getWidth()									const;
	LONG		getHeight()									const;
	VOID		getSize(LONG &uVpWidth, LONG &uVpHeight)	const;
	VOID		getSize(SIZE &szVp)							const;

	VOID		setRenderMode(RENDER_MODE renderMode);
	VOID		setWidth(LONG uVpWidth);
	VOID		setHeight(LONG uVpHeight);
	VOID		setSize(LONG uVpWidth, LONG uVpHeight);
	VOID		setSize(const SIZE &szVp);

	BOOL Render(LPSCENE3D lpScene, LPCAMERA3D lpCamera, HDC hDCScreen) const;
};
typedef CViewport VIEWPORT, *LPVIEWPORT;

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

class CRenderPool : public IUnknown {
private:
	THREAD_DATA_LIST	tdlViewports;
	EVENT_LIST			evlStates;
	EVENT				evTrigger;

	LPSCENE3D			lpScene;

	static DWORD WINAPI Render(LPVOID renderInfo);

public:
	CRenderPool();
	CRenderPool(LPSCENE3D lpScene);
	~CRenderPool();

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

	HDC setViewportScreen(size_t uVpIndex, HDC hDCScreen);
	HDC setViewportScreen(DWORD dwVpID, HDC hDCScreen);

	DWORD RenderWorld() const;
};
typedef CRenderPool RENDER_POOL, *LPRENDER_POOL;

#include "Render.inl"