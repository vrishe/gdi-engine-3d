
#pragma once

#include "Basis.h"
#include "SharedDef.h"

// ============================================================================
// CCamera class

class CCamera : public CObject {
private:
	static void InitDefaultValues(CCamera *cam);

protected:
	PROJECTION_TYPE projectionType;

	float			hFOV;		// FOV is an angle measured in radians
	float			vFOV;
	float			fDist;
	float			nearClip;
	float			farClip;

public:
	CCamera();
	CCamera(PROJECTION_TYPE projType, float horizFov, float vertFov);

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
};
typedef CCamera CAMERA3D, *LPCAMERA3D;

#include "Camera.inl"

