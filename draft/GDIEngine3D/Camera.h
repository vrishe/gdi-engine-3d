
#pragma once

#include "Basis.h"
#include "SharedDef.h"

// ============================================================================
// _clsCamera class

typedef class _clsCamera : public OBJECT3D {
private:
	static void InitDefaultValues(_clsCamera *cam);

protected:
	PROJECTION_TYPE projectionType;

	float			hFOV;		// FOV is an angle measured in radians
	float			vFOV;
	float			fDist;
	float			nearClip;
	float			farClip;

public:
	_clsCamera();
	_clsCamera(PROJECTION_TYPE projType, float horizFov, float vertFov);

	PROJECTION_TYPE	getProjectionType();
	float			getHFov();
	float			getVFov();
	float			getFDist();
	float			getNearCP();
	float			getFarCP();

	void			Zoom(float aspect);

	void			setProjectionType(PROJECTION_TYPE projType);
	void			setHFov(float fieldOfViewHoriz);
	void			setVFov(float fieldOfViewVert);
	void			setFDist(float dist);
	void			setNearCP(float nearCP);
	void			setFarCP(float farCP);

	void			GetViewMatrix(MATRIX3D &mOut);
	void			GetProjectionMatrix(MATRIX3D &mOutm);
} CAMERA3D, *LPCAMERA3D;

#include "Camera.inl"

