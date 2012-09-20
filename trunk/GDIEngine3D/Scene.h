
#pragma once

#include "Basis.h"

// ============================================================================
// _clsScene class declaration
typedef class _clsScene : public IUnknown {
private:
	CONTENT		objects;
	COLORREF	ambientColor;

	static void InitDefaultValues(_clsScene *obj);

public:
	_clsScene();
	~_clsScene();

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
} SCENE3D, *LPSCENE3D;

#include "Scene.inl"