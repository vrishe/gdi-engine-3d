#pragma once

// ============================================================================
// Partial implementation of CScene class:

inline void CScene::InitDefaultValues(CScene *obj)
{
	obj->objects[CLS_MESH];
	obj->objects[CLS_LIGHT];

	obj->ambientColor = RGB(0, 0, 0);
}

inline bool CScene::AddObject(const LPOBJECT3D lpObject)
{
	CONTENT::iterator finder = lpObject != NULL ? objects.find(lpObject->clsID()) : objects.end();

 	if ( finder != objects.end() ) 
	{
		finder->second.push_back(lpObject);
		return true;
	}

	return false;
}

inline bool CScene::DeleteObject(CLASS_ID clsID, size_t objIndex)
{
	CONTENT::iterator finder = objects.find(clsID);
	
	if ( finder != objects.end() && objIndex < finder->second.size() ) 
	{
		finder->second.erase(finder->second.begin() + objIndex);
		return true;
	}

	return false;
}

inline bool CScene::DeleteObject(const LPOBJECT3D lpObject)
{
	size_t objIndex;
	return findObjectIndex(lpObject, &objIndex)
			&& DeleteObject(lpObject->clsID(), objIndex);	
}

inline void CScene::Clear()
{
	objects.clear();
	
	InitDefaultValues(this);
}

inline LPOBJECT3D CScene::getObject(CLASS_ID clsID, size_t objIndex) const
{
	CONTENT::const_iterator finder = objects.find(clsID);

	if ( finder != objects.end() && objIndex < finder->second.size() ) 
		return finder->second[objIndex];

	return NULL;
}

inline size_t CScene::getObjectClassCount(CLASS_ID clsID) const 
{ 
	CONTENT::const_iterator finder = objects.find(clsID);

	if ( finder != objects.end() ) return finder->second.size(); 
	return SIZE_MAX;
}

inline LPOBJECT3D CScene::getFirstObject(CLASS_ID clsID) const
{
	CONTENT::const_iterator finder = objects.find(clsID);

	return finder != objects.end() ? *(finder->second.begin()) : NULL;
}

inline COLORREF CScene::getAmbientColor() const { return ambientColor; }

inline VOID	CScene::setAmbientColor(COLORREF color) { ambientColor = color; }
