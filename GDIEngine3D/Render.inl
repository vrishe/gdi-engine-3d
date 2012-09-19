#pragma once

// ============================================================================
// CViewport partial implementation:

inline void CViewport::InitValues(CViewport *obj, const SIZE &szVp, RENDER_MODE rMode)
{
	obj->setSize(szVp);	
	obj->rMode = rMode;
}

inline RENDER_MODE CViewport::getRenderMode() const { return rMode; }

inline UINT CViewport::getWidth() const { return GetDeviceCaps(hDCOutput, HORZRES); }
inline UINT CViewport::getHeight() const { return GetDeviceCaps(hDCOutput, VERTRES); }
inline VOID CViewport::getSize(SIZE &szVp) const
{
	szVp.cx = getWidth();
	szVp.cy = getHeight();
}

inline VOID CViewport::setRenderMode(RENDER_MODE renderMode) { rMode = renderMode; }

inline VOID CViewport::setWidth(UINT vpWidth)
{
	SIZE szVp = { vpWidth, getHeight() };
	setSize(szVp);
}
inline VOID CViewport::setHeight(UINT vpHeight)
{
	SIZE szVp = { getWidth(), vpHeight };
	setSize(szVp);
}


// ============================================================================
// ÑRenderPool class partial implementation:

inline BOOL ÑRenderPool::delViewport(DWORD dwVpID)
{
	return delViewport(getViewportIndex(dwVpID));
}

inline LPVIEWPORT ÑRenderPool::getViewport(DWORD dwVpID) const
{
	size_t uVpIndex;
	if ((uVpIndex = getViewportIndex(dwVpID)) != SIZE_MAX)
		return tdlViewports[uVpIndex]->lpViewport;

	return NULL;
}

inline LPVIEWPORT  ÑRenderPool::getViewport(size_t uVpIndex) const
{
	if (uVpIndex < tdlViewports.size())
		return tdlViewports[uVpIndex]->lpViewport;

	return NULL;
}

inline size_t ÑRenderPool::getViewportCount() const { return tdlViewports.size(); }

inline size_t ÑRenderPool::getViewportIndex(DWORD dwVpID) const
{
	for(size_t i = 0, max = tdlViewports.size(); i < max; i++)
		if ( GetThreadId(tdlViewports[i]->hThread) == dwVpID ) return i;

	return SIZE_MAX;
}

inline DWORD ÑRenderPool::getViewportID(size_t uVpIndex) const
{
	if (uVpIndex < tdlViewports.size())
		return GetThreadId(tdlViewports[uVpIndex]->hThread);

	return ((DWORD)0U);
}

inline size_t ÑRenderPool::getActiveViewportIndex() const
{
	for(size_t i = 0, max = tdlViewports.size(); i < max; i++)
		if ( tdlViewports[i]->bIsActive ) return i;

	return SIZE_MAX;
}

inline DWORD ÑRenderPool::getActiveViewportID() const
{
	return getViewportID(getActiveViewportIndex());
}

inline VOID ÑRenderPool::setActiveViewport(size_t uVpIndex)
{
	__foreach(THREAD_DATA_LIST::const_iterator, entry, tdlViewports)
	{
		(*entry)->bIsActive = FALSE;
	}

	if (uVpIndex < tdlViewports.size()) tdlViewports[uVpIndex]->bIsActive = TRUE;
}

inline VOID ÑRenderPool::setActiveViewport(DWORD dwVpID)
{
	setActiveViewport(getViewportIndex(dwVpID));
}

