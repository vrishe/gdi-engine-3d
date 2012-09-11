
#pragma once

#include "Basis.h"

// ============================================================================
// CScene class declaration

class CScene {
private:
	CONTENT		objects;
	COLORREF	ambientColor;

	static void InitDefaultValues(CScene *obj);

public:
	CScene();
	~CScene();

	bool AddObject(const LPOBJECT3D lpObject);
	bool DeleteObject(CLASS_ID clsID, size_t objIndex); 
	bool DeleteObject(size_t objID);
	bool DeleteObject(const LPOBJECT3D lpObject);
	void Clear();

	bool findObjectIndex(const LPOBJECT3D lpObject, size_t *objIndex = NULL);
	bool findObjectIndex(
				size_t objID, 
				CLASS_ID *objClsID	= NULL, 
				size_t *objIndex	= NULL
			);

	size_t		getObjectClassCount(CLASS_ID clsID);

	bool		getFirstObject(CLASS_ID *objID);
	LPOBJECT3D	getFirstObject();
	LPOBJECT3D	getObject(CLASS_ID objID, size_t objIndex);
	LPOBJECT3D	getObject(size_t objID);
	COLORREF	getAmbientColor();

	VOID		setAmbientColor(COLORREF color);
};
typedef CScene SCENE3D, *LPSCENE3D;

#include "Scene.inl"