#pragma once

// ============================================================================
// Partial implementation of CScene class:

inline void CScene::InitDefaultValues(CScene *obj)
{
	CONTENT_CLASS clsObjLst;

	clsObjLst.first = CLS_CAMERA;
	obj->objects.insert(clsObjLst);
	clsObjLst.first = CLS_MESH;
	obj->objects.insert(clsObjLst);
	clsObjLst.first = CLS_LIGHT;
	obj->objects.insert(clsObjLst);

	obj->ambientColor = RGB(0, 0, 0);
}

inline bool CScene::AddObject(const LPOBJECT3D lpObject)
{
	CONTENT::iterator finder = objects.find(lpObject->clsID());

	bool bResult = lpObject != NULL	&& finder != objects.end();
 	if ( bResult ) finder->second.push_back(lpObject);

	return bResult;
}

inline bool CScene::DeleteObject(const LPOBJECT3D lpObject)
{
	size_t objIndex;
	return findObjectIndex(lpObject, &objIndex)
			&& DeleteObject(lpObject->clsID(), objIndex);	
}

inline LPOBJECT3D CScene::getFirstObject()
{
	CLASS_ID idFound;

	if ( getFirstObject(&idFound) )
		return objects.at(idFound).at(0);

	return NULL;
}

inline LPOBJECT3D CScene::getObject(CLASS_ID objID, size_t objIndex)
{
	CONTENT::iterator finder = objects.find(objID);

	if ( finder != objects.end() && objIndex < finder->second.size() ) 
		return finder->second[objIndex];

	return NULL;
}

inline LPOBJECT3D CScene::getObject(size_t objID)
{
	CLASS_ID clsID; 
	UINT_PTR objIndex;

	if (findObjectIndex(objID, &clsID, &objIndex)) 
		return objects[clsID][objIndex];

	return NULL;
}

inline size_t CScene::getObjectClassCount(CLASS_ID clsID)	{ return objects[clsID].size(); }

inline COLORREF CScene::getAmbientColor()					{ return ambientColor; }

inline VOID CScene::setAmbientColor(COLORREF color)			{ ambientColor = color; }