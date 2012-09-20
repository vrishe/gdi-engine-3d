#pragma once

// ============================================================================
// _clsViewport partial implementation:

inline void _clsViewport::InitValues(_clsViewport *obj, LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode)
{
	obj->setSize(uVpWidth, uVpHeight);	
	obj->rMode = rMode;
}

inline RENDER_MODE _clsViewport::getRenderMode()	const { return rMode; }
inline LONG _clsViewport::getWidth()				const { return GetDeviceCaps(hDCOutput, HORZRES); }
inline LONG _clsViewport::getHeight()				const { return GetDeviceCaps(hDCOutput, VERTRES); }
inline VOID _clsViewport::getSize(LONG &uVpWidth, LONG &uVpHeight) const
{
	uVpWidth	= getWidth();
	uVpHeight	= getHeight();
}
inline VOID _clsViewport::getSize(SIZE &szVp) const
{
	szVp.cx = getWidth();
	szVp.cy = getHeight();
}

inline VOID _clsViewport::setRenderMode(RENDER_MODE renderMode) { rMode = renderMode; }
inline VOID _clsViewport::setWidth(LONG vpWidth)
{
	setSize(vpWidth, getHeight());
}
inline VOID _clsViewport::setHeight(LONG vpHeight)
{
	setSize(getWidth(), vpHeight);
}
inline VOID _clsViewport::setSize(const SIZE &szVp)
{
	setSize(szVp.cx, szVp.cy);
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

inline HDC _clsRenderPool::setViewportScreen(DWORD dwVpID, HDC hDCScreen)
{
	return setViewportScreen(getViewportIndex(dwVpID), hDCScreen);
}

inline DWORD _clsRenderPool::RenderWorld() const 
{
	SetEvent(evTrigger);
	return WaitForMultipleObjects(DWORD(evlStates.size()), evlStates.data(), TRUE, THREAD_WAIT_TIMEOUT);
}

