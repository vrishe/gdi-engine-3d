
#include "stdafx.h"
#include "Scene.h"

// ============================================================================
// Partial implementation of _clsScene class:

_clsScene::_clsScene() 
{ 
	InitDefaultValues(this);
}
_clsScene::~_clsScene() { }

bool _clsScene::findObjectIndex(const LPOBJECT3D lpObject, size_t *objIndex) const
{
	CONTENT::const_iterator finder = lpObject != NULL ? objects.find(lpObject->getClassID()) : objects.end();

 	if ( finder != objects.end() ) 
	{
		for (size_t i = 0, max = finder->second.size(); i < max; i++) 
		{
			if ( finder->second[i] == lpObject ) 
			{
				if (objIndex != NULL)  *objIndex = i;
				return true;
			}
		}
	}
	return false;
}

