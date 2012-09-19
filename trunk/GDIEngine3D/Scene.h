
#pragma once

#include "Basis.h"

// ============================================================================
// CScene class declaration
class CScene : public IUnknown {
private:
	CONTENT		objects;
	COLORREF	ambientColor;

	static void InitDefaultValues(CScene *obj);

public:
	CScene();
	~CScene();

	bool AddObject(const LPOBJECT3D lpObject);
	bool DeleteObject(CLASS_ID clsID, size_t objIndex); 
	bool DeleteObject(const LPOBJECT3D lpObject);
	void Clear();

	bool findObjectIndex(const LPOBJECT3D lpObject, size_t *objIndex = NULL) const;

	size_t		getObjectClassCount(CLASS_ID clsID)			const;
	LPOBJECT3D	getFirstObject(CLASS_ID clsID)				const;
	LPOBJECT3D	getObject(CLASS_ID objID, size_t objIndex)	const;
	COLORREF	getAmbientColor()							const;

	VOID		setAmbientColor(COLORREF color);
};
typedef CScene SCENE3D, *LPSCENE3D;

#include "Scene.inl"