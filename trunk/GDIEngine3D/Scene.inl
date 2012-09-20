#pragma once

// ============================================================================
// Partial implementation of _clsScene class:

inline void _clsScene::InitDefaultValues(_clsScene *obj)
{
	obj->objects[CLS_MESH];
	obj->objects[CLS_LIGHT];

	obj->ambientColor = RGB(0, 0, 0);
}

inline bool _clsScene::AddObject(const LPOBJECT3D lpObject)
{
	CONTENT::iterator finder = lpObject != NULL ? objects.find(lpObject->getClassID()) : objects.end();

 	if ( finder != objects.end() ) 
	{
		finder->second.push_back(lpObject);
		return true;
	}

	return false;
}

inline bool _clsScene::DeleteObject(CLASS_ID clsID, size_t objIndex)
{
	CONTENT::iterator finder = objects.find(clsID);
	
	if ( finder != objects.end() && objIndex < finder->second.size() ) 
	{
		finder->second.erase(finder->second.begin() + objIndex);
		return true;
	}

	return false;
}

inline bool _clsScene::DeleteObject(const LPOBJECT3D lpObject)
{
	size_t objIndex;
	return findObjectIndex(lpObject, &objIndex)
			&& DeleteObject(lpObject->getClassID(), objIndex);	
}

inline void _clsScene::Clear()
{
	objects.clear();
	
	InitDefaultValues(this);
}

inline LPOBJECT3D _clsScene::getObject(CLASS_ID clsID, size_t objIndex) const
{
	CONTENT::const_iterator finder = objects.find(clsID);

	if ( finder != objects.end() && objIndex < finder->second.size() ) 
		return finder->second[objIndex];

	return NULL;
}

inline size_t _clsScene::getObjectClassCount(CLASS_ID clsID) const 
{ 
	CONTENT::const_iterator finder = objects.find(clsID);

	if ( finder != objects.end() ) return finder->second.size(); 
	return SIZE_MAX;
}

inline LPOBJECT3D _clsScene::getFirstObject(CLASS_ID clsID) const
{
	CONTENT::const_iterator finder = objects.find(clsID);

	return finder != objects.end() ? *(finder->second.begin()) : NULL;
}

inline COLORREF _clsScene::getAmbientColor() const { return ambientColor; }

inline VOID	_clsScene::setAmbientColor(COLORREF color) { ambientColor = color; }
