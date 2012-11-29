
#include "stdafx.h"
#include "Render.h"

// ============================================================================
// _clsViewport class partial implementation:

void _clsViewport::InitValues(_clsViewport *obj, RENDER_MODE rMode)
{
	obj->hDCOutput		= NULL;
	obj->hBmpOutput		= NULL;
	obj->hBmpOriginal	= NULL;

	//obj->setSize(uVpWidth, uVpHeight);	
	obj->rMode = rMode;
}

_clsViewport::_clsViewport() 
{
	InitValues(this, RM_WIREFRAME);
}

_clsViewport::_clsViewport(RENDER_MODE rMode)
{
	InitValues(this, rMode);
}

//_clsViewport::_clsViewport(LONG uVpWidth, LONG uVpHeight, RENDER_MODE rMode)
//{
//	InitValues(this, uVpWidth, uVpHeight, rMode);
//}
//
//_clsViewport::_clsViewport(const SIZE &szVp, RENDER_MODE rMode)
//{
//	InitValues(this, szVp.cx, szVp.cy, rMode);
//}

_clsViewport::~_clsViewport() 
{ 
	DeleteObject(hBmpOutput);
	DeleteDC(hDCOutput);
}

BOOL _clsViewport::setSize(LONG uVpWidth, LONG uVpHeight)
{
	if (uVpWidth <= 0 || uVpHeight <= 0) return FALSE;
		
	if (uVpWidth != getWidth() || uVpHeight != getHeight())
	{
		HDC hDCScr = GetDC(NULL);
		
		if (hDCOutput != NULL) 
		{
			SelectObject(hDCOutput, hBmpOriginal);
			DeleteObject(hBmpOutput);
			DeleteDC(hDCOutput);
		}
		hDCOutput = CreateCompatibleDC(hDCScr);
		hBmpOutput = CreateCompatibleBitmap(hDCScr, uVpWidth, uVpHeight);

		if ((hBmpOriginal = (HBITMAP)SelectObject(hDCOutput, hBmpOutput)) == NULL)
		{
			DeleteObject(hBmpOutput);	hBmpOutput	= NULL;
			DeleteDC(hDCOutput);		hDCOutput	= NULL;
		}

		ReleaseDC(NULL, hDCScr);
	}
	return TRUE;
}


inline COLORREF AddColor(COLORREF a, COLORREF b)
{
	return RGB(
			min(RED(a)   + RED(b)   , 255U), 
			min(GREEN(a) + GREEN(b) , 255U), 
			min(BLUE(a)  + BLUE(b)  , 255U)
		);
}
// Is used as predicate for std::sort algorithm; provides a > b comparison
bool ZDepthComparator(const DIRECTPOLY3D &a, const DIRECTPOLY3D &b) 
{
	return (a.first.z + a.second.z + a.third.z) > (b.first.z + b.second.z + b.third.z);
}
BOOL _clsViewport::Render(LPSCENE3D lpScene, LPCAMERA3D lpCamera, HDC hDCScreen) const 
{
	BOOL bResult = hDCScreen != NULL && lpScene != NULL && lpCamera != NULL;

	if (bResult)
	{
		SCENEPOLY scenePolygonData;

		MATRIX4	cameraMatrix; 	  lpCamera->GetViewMatrix(cameraMatrix);
		MATRIX4	projectionMatrix; lpCamera->GetProjectionMatrix(projectionMatrix);

		OBJECTS_LIST lightData; lpScene->getObjects(lightData, CLS_LIGHT);
		OBJECTS_LIST meshData;  lpScene->getObjects(meshData, CLS_MESH);
		__foreach(OBJECTS_LIST::const_iterator, mesh_object, meshData)
		{
			LPMESH3D mesh = static_cast<LPMESH3D>(*mesh_object);
			mesh->Transform();

			VERT_LIST transformedVertexData;
			VERT_LIST projectedVertexData;

			mesh->getVertexCacheData(transformedVertexData);

			// camera projection transformations
			projectedVertexData = transformedVertexData;
			__foreach(VERT_LIST::iterator, vertex, projectedVertexData)
			{
				Matrix3DTransformCoord(cameraMatrix, *vertex, *vertex);
				
				float w = projectionMatrix._34 != .0F ? vertex->z / projectionMatrix._34 : 1.0F;
				Matrix3DTransformCoord(projectionMatrix, *vertex, *vertex);
				*vertex /= w;
			}

			// Polygon filtering
			POLY_LIST polygonData;

			mesh->getPolygonData(polygonData);
			__foreach(POLY_LIST::const_iterator, polygon, polygonData)
			{
				DIRECTPOLY3D outputPolygonData;

				outputPolygonData.first  = projectedVertexData[polygon->first];
				outputPolygonData.second = projectedVertexData[polygon->second];
				outputPolygonData.third  = projectedVertexData[polygon->third];
				if ( (-1.0F <= outputPolygonData.first.x  && outputPolygonData.first.x  <= 1.0F && -1.0F <= outputPolygonData.first.y  && outputPolygonData.first.y  <= 1.0F 
				||	  -1.0F <= outputPolygonData.second.x && outputPolygonData.second.x <= 1.0F && -1.0F <= outputPolygonData.second.y && outputPolygonData.second.y <= 1.0F
				||    -1.0F <= outputPolygonData.third.x  && outputPolygonData.third.x  <= 1.0F && -1.0F <= outputPolygonData.third.y  && outputPolygonData.third.y  <= 1.0F)
				&&	    .0F <= outputPolygonData.first.z  && outputPolygonData.first.z  <= projectionMatrix._34
				&&	    .0F <= outputPolygonData.second.z && outputPolygonData.second.z <= projectionMatrix._34
				&&      .0F <= outputPolygonData.third.z  && outputPolygonData.third.z  <= projectionMatrix._34
				) {
					if ((rMode & RM_SHADED) != 0)
					{
						float cosCam = Vector3DMultS(polygon->Normal(projectedVertexData, 2), VECTOR3D(.0F, .0F, 1.0F));
						if (!(-1.0F <= cosCam && cosCam < .0F)) continue;
					}

					outputPolygonData.fillColorRef = mesh->getColor();
					if (!lightData.empty())
					{

						COLORREF lightedSummaryColor = RGB(0, 0, 0);
						__foreach(OBJECTS_LIST::const_iterator, light_object, lightData)
						{
							LPOMNILIGHT3D light = static_cast<LPOMNILIGHT3D>(*light_object); 

							lightedSummaryColor = AddColor(lightedSummaryColor,
								light->AffectPolygonColor(*polygon, transformedVertexData, mesh->getColor())
								);
						}

						float indicentIllumination = 1.0F - mesh->getSelfIllumination();
						outputPolygonData.fillColorRef = AddColor(
								RGB(
									RED(lightedSummaryColor)   * indicentIllumination,
									GREEN(lightedSummaryColor) * indicentIllumination,
									BLUE(lightedSummaryColor)  * indicentIllumination
								),
								RGB(
									RED(mesh->getColor())   * mesh->getSelfIllumination(),
									GREEN(mesh->getColor()) * mesh->getSelfIllumination(),
									BLUE(mesh->getColor())  * mesh->getSelfIllumination(),
								)
							);
					}
					outputPolygonData.strokeColorRef = (rMode & RM_WIREFRAME) != 0 ? mesh->getColor() : outputPolygonData.fillColorRef;

					scenePolygonData.push_back(outputPolygonData);
				}
			}
		}
		sort(scenePolygonData.begin(), scenePolygonData.end(), ZDepthComparator);

		// Rasterizing
		RECT viewportRect = {0, 0, getWidth(), getHeight()};
		HBRUSH hBrCurrent = CreateSolidBrush(lpScene->getAmbientColor());
		HBRUSH hBrOld	  = (HBRUSH)SelectObject(hDCOutput, hBrCurrent);

		FillRect(hDCOutput, &viewportRect, hBrCurrent);

		SelectObject(hDCOutput, hBrOld);
		DeleteObject(hBrCurrent);

		MATRIX4 viewportMatrix;
		viewportMatrix.SetIdentity();
		viewportMatrix._41 = (FLOAT)viewportRect.right / 2;
		viewportMatrix._42 = (FLOAT)viewportRect.bottom / 2;
		viewportMatrix._11 =  viewportMatrix._41;
		viewportMatrix._22 = -viewportMatrix._42;

		__foreach(SCENEPOLY::iterator, polygon, scenePolygonData)
		{
			Matrix3DTransformCoord(viewportMatrix, polygon->first,  polygon->first);
			Matrix3DTransformCoord(viewportMatrix, polygon->second, polygon->second);
			Matrix3DTransformCoord(viewportMatrix, polygon->third,  polygon->third);

			POINT vert2DDrawBuffer[3];

			vert2DDrawBuffer[0].x = (LONG)polygon->first.x;
			vert2DDrawBuffer[0].y = (LONG)polygon->first.y;

			vert2DDrawBuffer[1].x = (LONG)polygon->second.x;
			vert2DDrawBuffer[1].y = (LONG)polygon->second.y;

			vert2DDrawBuffer[2].x = (LONG)polygon->third.x;
			vert2DDrawBuffer[2].y = (LONG)polygon->third.y;

			hBrCurrent  = (rMode & RM_SHADED) != 0 
									? CreateSolidBrush(polygon->fillColorRef) 
									: (HBRUSH)GetStockObject(NULL_BRUSH);
			hBrOld	   = (HBRUSH)SelectObject(hDCOutput, hBrCurrent);
			
			HPEN   hPenCurrent = CreatePen(PS_SOLID, 1, polygon->strokeColorRef);		
			HPEN   hPenOld     = (HPEN)SelectObject(hDCOutput, hPenCurrent);

			Polygon(hDCOutput, vert2DDrawBuffer, 3);

			SelectObject(hDCOutput, hBrOld);
			SelectObject(hDCOutput, hPenOld);
			if ((rMode & RM_SHADED) != 0) DeleteObject(hBrCurrent);
			DeleteObject(hPenCurrent);
		}

		BITMAP info;
		bResult &= (GetObject(GetCurrentObject(hDCScreen, OBJ_BITMAP), sizeof(BITMAP), &info) == sizeof(BITMAP))
			&& StretchBlt(hDCScreen, 0, 0, info.bmWidth, info.bmHeight, hDCOutput, 0, 0, viewportRect.right, viewportRect.bottom, SRCCOPY);
	}

	return bResult;
}

// ============================================================================
// _clsRenderPool class partial implementation:

DWORD WINAPI _clsRenderPool::Render(LPVOID renderInfo)
{
	LPTHREAD_DATA	vp = (LPTHREAD_DATA)renderInfo;

	HFONT			hFontOld,
					hFontCur = CreateFont(
									-FRAME_FONT_SIZE,
									0, 0, 0,
									FW_THIN,
									0, 0, 0,
									DEFAULT_CHARSET,
									OUT_DEFAULT_PRECIS,
									CLIP_DEFAULT_PRECIS,
									ANTIALIASED_QUALITY,
									VARIABLE_PITCH,
									FRAME_FONT_FAMILY
								);

	HPEN			hPenOld,
					hPenCur = CreatePen(
									PS_INSIDEFRAME, 
									FRAME_STROKE_WIDTH, 
									FRAME_STROKE_COLORREF
								);
	while ( WaitForMultipleObjects(2, (LPEVENT)&vp->tcEvents, FALSE, INFINITE) != WAIT_OBJECT_0 + 1 )
	{
		clock_t	time = clock();

		vp->lpViewport->Render(*(vp->lppScene), vp->lpCamera, vp->hDCScreen);
		if ( vp->bIsActive ) 
		{
			SIZE szVp;
			vp->lpViewport->getSize(szVp);
			hPenOld = (HPEN)SelectObject(vp->hDCScreen, hPenCur);
			HBRUSH hBrOld = (HBRUSH)SelectObject(vp->hDCScreen, GetStockObject(NULL_BRUSH));
			Rectangle(vp->hDCScreen, 1, 1, szVp.cx - 1, szVp.cy - 1);
			SelectObject(vp->hDCScreen, hPenOld);
			SelectObject(vp->hDCScreen, hBrOld);
		}
		hFontOld = (HFONT)SelectObject(vp->hDCScreen, hFontCur); 
		SetBkMode(vp->hDCScreen, TRANSPARENT);
		SetTextColor(vp->hDCScreen, FRAME_FONT_COLOR);
		tstring tsCameraName = vp->lpCamera->getName();
		TextOut(
			vp->hDCScreen, 
			FRAME_STROKE_WIDTH + 2, 
			FRAME_STROKE_WIDTH, 
			tsCameraName.c_str(), 
			(INT)tsCameraName.length()
		);
		SelectObject(vp->hDCScreen, hFontOld);

		time = CLOCKS_PER_FRAME - clock() + time;
		if ( time > 0 ) Sleep(time);

		ResetEvent(vp->tcEvents.doRender);
		SetEvent(vp->tcEvents.jobDone);
	};

	DeleteObject(hPenCur);	
	DeleteObject(hFontCur);
	return SHUTDOWN_ON_DEMAND;
}

_clsRenderPool::_clsRenderPool() : evTrigger(NULL) { }
_clsRenderPool::_clsRenderPool(LPSCENE3D lpScene) : evTrigger(NULL) { setRenderScene(lpScene); }
_clsRenderPool::~_clsRenderPool() 
{ 
	for (size_t i = 0, max = tdlViewports.size(); i < max; i++) delViewport(i);
}

DWORD _clsRenderPool::addViewport(
	LPVIEWPORT	lpViewport,
	LPCAMERA3D	lpCamera, 
	HDC			hDCScreen
) {
	DWORD dwResultID = ((DWORD)0U);
	
	if (lpViewport != NULL && lpCamera != NULL && hDCScreen != NULL)
	{
		LPTHREAD_DATA thData = NULL;
		try
		{
			FillMemory(thData = new THREAD_DATA(), sizeof(THREAD_DATA), NULL);

			thData->lpViewport	= lpViewport;
			thData->bIsActive	= FALSE;
			thData->lppScene	= &lpScene;
			thData->lpCamera	= lpCamera;
			thData->hDCScreen	= hDCScreen;

			thData->tcEvents.doRender	= evTrigger != NULL ? evTrigger : CreateEvent(NULL, FALSE, FALSE, NULL);
			thData->tcEvents.jobDone	= CreateEvent(0, FALSE, FALSE, NULL);
			thData->tcEvents.shutDown	= CreateEvent(0, FALSE, FALSE, NULL);
			if (thData->tcEvents.jobDone == NULL || thData->tcEvents.shutDown == NULL) throw E_FAIL;

			thData->hThread = CreateThread(0, 0, Render, thData, CREATE_SUSPENDED, &dwResultID);
			if(thData->hThread == NULL) throw E_FAIL;

			evTrigger = thData->tcEvents.doRender;

			ResumeThread(thData->hThread);
			tdlViewports.push_back(thData);
		}
		catch (DWORD)
		{
			if (tdlViewports.empty()) CloseHandle(thData->tcEvents.doRender);
			CloseHandle(thData->tcEvents.shutDown);
			CloseHandle(thData->tcEvents.jobDone);

			delete thData;
		}
	}

	return dwResultID;
}

BOOL _clsRenderPool::delViewport(size_t uVpIndex)
{
	if ( uVpIndex >= tdlViewports.size() ) return FALSE;
	
	SetEvent(tdlViewports[uVpIndex]->tcEvents.shutDown);
	if (WaitForSingleObject(tdlViewports[uVpIndex]->hThread, THREAD_WAIT_TIMEOUT) == WAIT_TIMEOUT)
	{
		TerminateThread(tdlViewports[uVpIndex]->hThread, SHUTDOWN_ON_TERMINATE);
	}

	CloseHandle(tdlViewports[uVpIndex]->hThread);
	CloseHandle(tdlViewports[uVpIndex]->tcEvents.shutDown);
	CloseHandle(tdlViewports[uVpIndex]->tcEvents.jobDone);

	delete tdlViewports[uVpIndex];

	tdlViewports.erase(tdlViewports.begin() + uVpIndex);
	evlStates.erase(evlStates.begin() + uVpIndex);

	if (tdlViewports.empty())
	{
		CloseHandle(evTrigger);
		evTrigger = NULL;
	}
		
	return TRUE;
}

BOOL _clsRenderPool::setViewportScreen(size_t uVpIndex, HDC hDcNew, HDC &hDcOld)
{
	if (uVpIndex >= tdlViewports.size()) return FALSE;

	hDcOld = tdlViewports[uVpIndex]->hDCScreen;
	tdlViewports[uVpIndex]->hDCScreen = hDcNew;
	
	return TRUE;
}
