#pragma once

// ============================================================================
// _clsCamera partial implementation:

inline void _clsCamera::InitDefaultValues(_clsCamera *cam)
{
	cam->projectionType	= PT_PARALLEL;
	cam->hFOV			= (float)M_PI_2;
	cam->vFOV			= (float)M_PI_2;
	cam->nearClip		= 1.0F;
	cam->farClip		= 1000.0F;
	cam->fDist			= 499.5F;
}

inline PROJECTION_TYPE	_clsCamera::getProjectionType()	{ return projectionType; }
inline float			_clsCamera::getHFov()			{ return hFOV; }
inline float			_clsCamera::getVFov()			{ return vFOV; }
inline float			_clsCamera::getFDist()			{ return fDist; }
inline float			_clsCamera::getNearCP()			{ return nearClip; }
inline float			_clsCamera::getFarCP()			{ return farClip; }

inline void _clsCamera::Zoom(float aspect)
{
	float rel = vFOV / hFOV;
	setHFov(hFOV + aspect);
	vFOV = hFOV * rel;
}

inline void _clsCamera::setProjectionType(PROJECTION_TYPE projType) 
{ 
	projectionType = projType; 
}

inline void _clsCamera::setHFov(float fieldOfViewHoriz) 
{ 
	if (fieldOfViewHoriz > .0 && ((FLOAT)M_PI - fieldOfViewHoriz) > .0) hFOV = fieldOfViewHoriz;
}

inline void _clsCamera::setVFov(float fieldOfViewVert) 
{ 
	if (fieldOfViewVert > .0 && ((FLOAT)M_PI - fieldOfViewVert) > .0) vFOV = fieldOfViewVert;
}

inline void _clsCamera::setFDist(float dist)
{
	if (dist - nearClip > .0
		&& farClip - dist > .0
		) fDist = dist;
}

inline void _clsCamera::setNearCP(float nearCP) 
{ 
	if (nearCP >= .0 
		&& farClip - nearClip > .0
		) nearClip = nearCP; 
}

inline void _clsCamera::setFarCP(float farCP) 
{ 
	if ( farCP - nearClip > .0 ) farClip = farCP; 
}