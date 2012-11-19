#pragma once

// ============================================================================
// _clsCamera partial implementation:

inline void _clsCamera::InitDefaultValues(_clsCamera *cam)
{
	cam->projectionType	= PT_PARALLEL;
	cam->hFOV			= (float)M_PI_2;
	cam->vFOV			= (float)M_PI_2;
	cam->fDist			= 1.0F;
	cam->nearClip		= 1.0F;
	cam->farClip		= 1000.0F;
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
	switch(projectionType)
	{
	case PT_CENTRAL:  hFOV = vFOV = static_cast<float>(M_PI / 2.0);		break;
	case PT_PARALLEL: hFOV = vFOV = numeric_limits<float>::infinity();	break;
	}
}

inline void _clsCamera::setHFov(float fieldOfViewHoriz) 
{
	if (projectionType == PT_PARALLEL) return;
	if (fieldOfViewHoriz > .0 && ((FLOAT)M_PI - fieldOfViewHoriz) > .0) hFOV = fieldOfViewHoriz;
}

inline void _clsCamera::setVFov(float fieldOfViewVert) 
{
	if (projectionType == PT_PARALLEL) return;
	if (fieldOfViewVert > .0 && ((FLOAT)M_PI - fieldOfViewVert) > .0) vFOV = fieldOfViewVert;
}

inline void _clsCamera::setFDist(float dist)
{
	if (dist < 1.0F)		return;
	if (dist > nearClip)	return;

	fDist = dist;
}

inline void _clsCamera::setNearCP(float nearCP) 
{
	if (farClip - nearCP < 1.0F) return;
	if (nearCP < fDist)
	{
		setFDist(nearCP);
		nearCP = fDist;
	}
	nearClip = nearCP;
}

inline void _clsCamera::setFarCP(float farCP) 
{ 
	if ( farCP < nearClip )
	{
		float difference = farClip - nearClip;
		setNearCP(farCP - difference);
		farCP = nearClip + difference;
	}
	farClip = farCP;
}