
#include "stdafx.h"
#include "Mesh.h"

// ============================================================================
// Partial implementation of _clsMesh class:

_clsMesh::_clsMesh(MESH_ID meshTypeId) 
	: OBJECT3D(CLS_MESH), IColorable() { setMeshID(meshTypeId); }

_clsMesh::_clsMesh(MESH_ID meshTypeId, COLORREF c) 
	: OBJECT3D(CLS_MESH), IColorable(c) { setMeshID(meshTypeId); }

void _clsMesh::Transform()
{
	MATRIX3D	mTransScalePos(true),
				mLocalScale(true),
				mLocalRot(true);
	
	flushVertices();

	GetLocalScaleMatrix(mLocalScale);
	GetRotationMatrix(mLocalRot);
	GetMoveMatrix(mTransScalePos);
	GetScaleMatrix(mTransScalePos);
	
	__foreach(VERT_LIST::iterator, vertex, vertices)
	{
		Matrix3DTransformNormal(mLocalScale, *vertex, *vertex);		
		Matrix3DTransformNormal(mLocalRot, *vertex, *vertex);		
		Matrix3DTransformCoord(mTransScalePos, *vertex, *vertex);
	}
}