#pragma once

// ============================================================================
// _clsViewport partial implementation:

inline RENDER_MODE _clsViewport::getRenderMode() const { return rMode; }
inline VOID _clsViewport::getSize(SIZE &szVp) const
{
	BITMAP info;
	if (GetObject(hBmpOutput, sizeof(BITMAP), &info) == sizeof(BITMAP))
	{
		szVp.cx = info.bmWidth;
		szVp.cy = info.bmHeight;
	}
}
inline VOID _clsViewport::getSize(LONG &uVpWidth, LONG &uVpHeight) const
{
	SIZE szVp = { 0, 0 };
	getSize(szVp);

	uVpWidth	= szVp.cx;
	uVpHeight	= szVp.cy;
}
inline LONG _clsViewport::getWidth() const 
{ 
	SIZE szVp = { 0, 0 };
	getSize(szVp);
	return szVp.cx; 
}
inline LONG _clsViewport::getHeight() const 
{ 
	SIZE szVp = { 0, 0 };
	getSize(szVp);
	return szVp.cy; 
}

inline VOID _clsViewport::setRenderMode(RENDER_MODE renderMode) { rMode = renderMode; }
inline BOOL _clsViewport::setWidth(LONG vpWidth)
{
	return setSize(vpWidth, getHeight());
}
inline BOOL _clsViewport::setHeight(LONG vpHeight)
{
	return setSize(getWidth(), vpHeight);
}
inline BOOL _clsViewport::setSize(const SIZE &szVp)
{
	return setSize(szVp.cx, szVp.cy);
}


// ============================================================================
// _clsRenderPool class partial implementation:

inline BOOL _clsRenderPool::delViewport(DWORD dwVpID)
{
	return delViewport(getViewportIndex(dwVpID));
}

inline LPSCENE3D _clsRenderPool::getRenderScene() const { return lpScene; }

inline LPVIEWPORT _clsRenderPool::getViewport(DWORD dwVpID) const
{
	size_t uVpIndex;
	if ((uVpIndex = getViewportIndex(dwVpID)) != SIZE_MAX)
		return tdlViewports[uVpIndex]->lpViewport;

	return NULL;
}

inline LPVIEWPORT _clsRenderPool::getViewport(size_t uVpIndex) const
{
	if (uVpIndex < tdlViewports.size())
		return tdlViewports[uVpIndex]->lpViewport;

	return NULL;
}

inline size_t _clsRenderPool::getViewportCount() const { return tdlViewports.size(); }

inline size_t _clsRenderPool::getViewportIndex(DWORD dwVpID) const
{
	for(size_t i = 0, max = tdlViewports.size(); i < max; i++)
		if ( GetThreadId(tdlViewports[i]->hThread) == dwVpID ) return i;

	return SIZE_MAX;
}

inline DWORD _clsRenderPool::getViewportID(size_t uVpIndex) const
{
	if (uVpIndex < tdlViewports.size())
		return GetThreadId(tdlViewports[uVpIndex]->hThread);

	return ((DWORD)0U);
}

inline size_t _clsRenderPool::getActiveViewportIndex() const
{
	for(size_t i = 0, max = tdlViewports.size(); i < max; i++)
		if ( tdlViewports[i]->bIsActive ) return i;

	return SIZE_MAX;
}

inline DWORD _clsRenderPool::getActiveViewportID() const
{
	return getViewportID(getActiveViewportIndex());
}

inline VOID _clsRenderPool::setRenderScene(LPSCENE3D lpScene)
{
	this->lpScene = lpScene;
}

inline VOID _clsRenderPool::setActiveViewport(size_t uVpIndex)
{
	__foreach(THREAD_DATA_LIST::const_iterator, entry, tdlViewports)
	{
		(*entry)->bIsActive = FALSE;
	}

	if (uVpIndex < tdlViewports.size()) tdlViewports[uVpIndex]->bIsActive = TRUE;
}

inline VOID _clsRenderPool::setActiveViewport(DWORD dwVpID)
{
	setActiveViewport(getViewportIndex(dwVpID));
}

inline BOOL _clsRenderPool::setViewportScreen(DWORD dwVpID, HDC hDcNew, HDC &hDcOld)
{
	return setViewportScreen(getViewportIndex(dwVpID), hDcNew, hDcOld);
}

inline DWORD _clsRenderPool::RenderWorld() const 
{
	SetEvent(evTrigger);
	return WaitForMultipleObjects(DWORD(evlStates.size()), evlStates.data(), TRUE, THREAD_WAIT_TIMEOUT);
}

