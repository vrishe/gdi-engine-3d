
#include "stdafx.h"
#include "Scene.h"

// ============================================================================
// Partial implementation of CScene class:

CScene::CScene() 
{ 
	InitDefaultValues(this);
}
CScene::~CScene() { }

bool CScene::DeleteObject(CLASS_ID clsID, size_t objIndex)
{
	CONTENT::iterator finder = objects.find(clsID);
	
	bool bResult = finder != objects.end() && objIndex < finder->second.size();
	if ( bResult ) finder->second.erase(finder->second.begin() + objIndex);

	return bResult;
}

bool CScene::DeleteObject(size_t objID)
{
	CONTENT::iterator finder;
	CLASS_ID objClsID;
	size_t objIndex;
	bool bResult 
		= findObjectIndex(objID, &objClsID, &objIndex);
	if ( bResult ) 
	{
		finder = objects.find(objClsID);
		bResult = finder != objects.end();
		if ( bResult ) 
			finder->second.erase(finder->second.begin() + objIndex);
	}
	return bResult;
}

void CScene::Clear()
{
	objects.clear();
	
	InitDefaultValues(this);
}

bool CScene::findObjectIndex(const LPOBJECT3D lpObject, size_t *objIndex)
{
	bool bResult = lpObject != NULL;
	if ( !bResult ) return bResult;
	CONTENT::iterator finder = objects.find(lpObject->clsID());
	bResult = finder != objects.end();
	size_t objCount;
 	if ( bResult ) 
	{
		objCount	= finder->second.size();
		bResult		= false;
		for ( 
			size_t i = 0;
			i < objCount;
			i++ 
		) {
			if ( finder->second[i] == lpObject ) 
			{
				if (objIndex != NULL)  *objIndex = i;
				i		= objCount;
				bResult = true;
			}
		}
	}
	return bResult;
}

bool CScene::findObjectIndex(size_t objID, CLASS_ID *objClsID, size_t *objIndex)
{
	CONTENT::iterator finder = objects.begin();
	size_t	i,
			listCount;

	while ( finder != objects.end() )
	{
		listCount = finder->second.size();
		i = 0;
		while ( i < listCount ) 
		{
			if ( finder->second[i]->objID() == objID ) 
			{
				if ( objClsID != NULL ) *objClsID	= finder->first;
				if ( objIndex != NULL )	*objIndex	= i;
				return true;
			}
			i++;
		}
		finder++;
	}
	return false;
}

bool CScene::getFirstObject(CLASS_ID *objID)
{
	CONTENT::iterator i = objects.begin();
	while ( i != objects.end() )
	{
		if ( i->second.size() > 0 ) 
		{
			if (objID != NULL) *objID = i->first;
			return true;
		}
		i++;
	}
	return false;
}

