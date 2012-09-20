#pragma once

// ============================================================================
// CViewport partial implementation:

inline void CViewport::InitValues(CViewport *obj, LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode)
{
	obj->setSize(uVpWidth, uVpHeight);	
	obj->rMode = rMode;
}

inline RENDER_MODE CViewport::getRenderMode() const { return rMode; }
inline LONG CViewport::getWidth() const { return GetDeviceCaps(hDCOutput, HORZRES); }
inline LONG CViewport::getHeight() const { return GetDeviceCaps(hDCOutput, VERTRES); }
inline VOID CViewport::getSize(LONG &uVpWidth, LONG &uVpHeight) const
{
	uVpWidth	= getWidth();
	uVpHeight	= getHeight();
}
inline VOID CViewport::getSize(SIZE &szVp) const
{
	szVp.cx = getWidth();
	szVp.cy = getHeight();
}

inline VOID CViewport::setRenderMode(RENDER_MODE renderMode) { rMode = renderMode; }
inline VOID CViewport::setWidth(LONG vpWidth)
{
	setSize(vpWidth, getHeight());
}
inline VOID CViewport::setHeight(LONG vpHeight)
{
	setSize(getWidth(), vpHeight);
}
inline VOID CViewport::setSize(const SIZE &szVp)
{
	setSize(szVp.cx, szVp.cy);
}


// ============================================================================
// �RenderPool class partial implementation:

inline BOOL �RenderPool::delViewport(DWORD dwVpID)
{
	return delViewport(getViewportIndex(dwVpID));
}

inline LPVIEWPORT �RenderPool::getViewport(DWORD dwVpID) const
{
	size_t uVpIndex;
	if ((uVpIndex = getViewportIndex(dwVpID)) != SIZE_MAX)
		return tdlViewports[uVpIndex]->lpViewport;

	return NULL;
}

inline LPVIEWPORT �RenderPool::getViewport(size_t uVpIndex) const
{
	if (uVpIndex < tdlViewports.size())
		return tdlViewports[uVpIndex]->lpViewport;

	return NULL;
}

inline size_t �RenderPool::getViewportCount() const { return tdlViewports.size(); }

inline size_t �RenderPool::getViewportIndex(DWORD dwVpID) const
{
	for(size_t i = 0, max = tdlViewports.size(); i < max; i++)
		if ( GetThreadId(tdlViewports[i]->hThread) == dwVpID ) return i;

	return SIZE_MAX;
}

inline DWORD �RenderPool::getViewportID(size_t uVpIndex) const
{
	if (uVpIndex < tdlViewports.size())
		return GetThreadId(tdlViewports[uVpIndex]->hThread);

	return ((DWORD)0U);
}

inline size_t �RenderPool::getActiveViewportIndex() const
{
	for(size_t i = 0, max = tdlViewports.size(); i < max; i++)
		if ( tdlViewports[i]->bIsActive ) return i;

	return SIZE_MAX;
}

inline DWORD �RenderPool::getActiveViewportID() const
{
	return getViewportID(getActiveViewportIndex());
}

inline VOID �RenderPool::setActiveViewport(size_t uVpIndex)
{
	__foreach(THREAD_DATA_LIST::const_iterator, entry, tdlViewports)
	{
		(*entry)->bIsActive = FALSE;
	}

	if (uVpIndex < tdlViewports.size()) tdlViewports[uVpIndex]->bIsActive = TRUE;
}

inline VOID �RenderPool::setActiveViewport(DWORD dwVpID)
{
	setActiveViewport(getViewportIndex(dwVpID));
}

inline HDC �RenderPool::setViewportScreen(DWORD dwVpID, HDC hDCScreen)
{
	return setViewportScreen(getViewportIndex(dwVpID), hDCScreen);
}

