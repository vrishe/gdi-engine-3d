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

inline LPOBJECT3D _clsScene::DeleteObject(CLASS_ID clsID, size_t objIndex)
{
	CONTENT::iterator finder = objects.find(clsID);
	
	if ( finder != objects.end() && objIndex < finder->second.size() ) 
	{
		LPOBJECT3D removal = finder->second.at(objIndex);
		finder->second.erase(finder->second.begin() + objIndex);
		return removal;
	}

	return NULL;
}

inline bool _clsScene::DeleteObject(const LPOBJECT3D lpObject)
{
	size_t objIndex;
	if (findObjectIndex(lpObject, &objIndex))
	{
		DeleteObject(lpObject->getClassID(), objIndex);	
		return true;
	}

	return false;
}

inline void _clsScene::Clear(OBJECTS_LIST &removal)
{
	getObjectsBulkVector(removal, true);

	objects.clear();
	InitDefaultValues(this);
}

inline void _clsScene::getObjectsBulkVector(OBJECTS_LIST &bulk, bool preClean) const
{
	if (preClean) bulk.clear();

	__foreach(CONTENT::const_iterator, entry, objects)
	{
		bulk.insert(bulk.end(), entry->second.begin(), entry->second.end());
	}
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
