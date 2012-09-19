#pragma once

// ============================================================================
// CViewport partial implementation:

inline RENDER_MODE CViewport::getRenderMode() const { return rMode; }

inline UINT CViewport::getWidth() const { return GetDeviceCaps(hDCOutput, HORZRES); }
inline UINT CViewport::getHeight() const { return GetDeviceCaps(hDCOutput, VERTRES); }
inline VOID CViewport::getSize(SIZE &vpSize) const
{
	vpSize.cx = getWidth();
	vpSize.cy = getHeight();
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

inline size_t ÑRenderPool::getViewportCount() { return tdlViewports.size(); }

inline size_t ÑRenderPool::getViewportIndex(DWORD dwVpID)
{
	for(size_t i = 0, max = tdlViewports.size(); i < max; i++)
		if ( GetThreadId(tdlViewports[i]->hThread) == dwVpID ) return i;

	return SIZE_MAX;
}

inline DWORD ÑRenderPool::getViewportID(size_t uVpIndex)
{
	if (uVpIndex >= tdlViewports.size()) return ((DWORD)0U);

	return GetThreadId(tdlViewports[uVpIndex]->hThread);
}

inline size_t ÑRenderPool::getActiveViewportIndex()
{
	for(size_t i = 0, max = tdlViewports.size(); i < max; i++)
		if ( tdlViewports[i]->bIsActive ) return i;

	return SIZE_MAX;
}

inline DWORD ÑRenderPool::getActiveViewportID()
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

