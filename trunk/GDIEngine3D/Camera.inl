#pragma once

// ============================================================================
// CCamera partial implementation:

inline PROJECTION_TYPE	CCamera::getProjectionType()	{ return projectionType; }
inline float			CCamera::getHFov()				{ return hFOV; }
inline float			CCamera::getVFov()				{ return vFOV; }
inline float			CCamera::getFDist()				{ return fDist; }
inline float			CCamera::getNearCP()			{ return nearClip; }
inline float			CCamera::getFarCP()				{ return farClip; }

inline void CCamera::Zoom(float aspect)
{
	float rel = vFOV / hFOV;
	setHFov(hFOV + aspect);
	vFOV = hFOV * rel;
}

inline void CCamera::setProjectionType(PROJECTION_TYPE projType) 
{ 
	projectionType = projType; 
}

inline void CCamera::setHFov(float fieldOfViewHoriz) 
{ 
	if (fieldOfViewHoriz > EPS 
		&& (FLOAT)M_PI - fieldOfViewHoriz > EPS
		) hFOV = fieldOfViewHoriz;
}

inline void CCamera::setVFov(float fieldOfViewVert) 
{ 
	if (fieldOfViewVert > EPS 
		&& (FLOAT)M_PI - fieldOfViewVert > EPS
		) vFOV = fieldOfViewVert;
}

inline void CCamera::setFDist(float dist)
{
	if (dist - nearClip > EPS
		&& farClip - dist > EPS
		) fDist = dist;
}

inline void CCamera::setNearCP(float nearCP) 
{ 
	if (nearCP >= EPS 
		&& farClip - nearClip > EPS
		) nearClip = nearCP; 
}

inline void CCamera::setFarCP(float farCP) 
{ 
	if ( farCP - nearClip > EPS ) farClip = farCP; 
}