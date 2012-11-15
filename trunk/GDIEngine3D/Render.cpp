
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

// Is used as predicate for std::sort algorithm; provides a > b comparison
bool ZDepthComparator(const pair <DIRECTPOLY3D, UINT> &a, const pair <DIRECTPOLY3D, UINT> &b) 
{
	return (a.first.first.z + a.first.second.z + a.first.third.z) 
			> (b.first.first.z + b.first.second.z + b.first.third.z);
}

BOOL _clsViewport::Render(LPSCENE3D lpScene, LPCAMERA3D lpCamera, HDC hDCScreen) const 
{
	BOOL bResult = hDCScreen != NULL && lpScene != NULL && lpCamera != NULL;

	if (bResult)
	{
		HPEN			hPenCurrent, 
						hPenOld;
		HBRUSH			hBrCurrent,
						hBrOld;

		RECT			viewportRect			= {0, 0, getWidth(), getHeight()};
		size_t			sceneObjCount,
						sceneLightCount,
						scenePolyCount,
						sceneLightedPolyCount,
						objVertCount,
						objEdgeCount,
						objPolyCount;

		LPOMNILIGHT3D	lightToRender;
		LPMESH3D		objToRender;
		LPVECTOR3D		objVertBuffer;
		LPEDGE3D		objEdgeBuffer;
		LPPOLY3D		objPolyBuffer;

		MATRIX4		cameraMatrix,
						projectionMatrix,
						viewportMatrix;
	
		SCENEPOLY		scenePolyBuffer;
		LPCOLORREF		scenePolyColorBuffer;
		POINT			vert2DDrawBuffer[3];

		// Filling lpViewport with scene ambient color
		hBrCurrent	= CreateSolidBrush(lpScene->getAmbientColor());
		hBrOld		= (HBRUSH)SelectObject(hDCOutput, hBrCurrent);
		FillRect(hDCOutput, &viewportRect, hBrCurrent);
		SelectObject(hDCOutput, hBrOld);
		DeleteObject(hBrCurrent);

		sceneObjCount	= lpScene->getObjectClassCount(CLS_MESH);

		if ( rMode != RM_WIREFRAME ) 
		{
			scenePolyCount = 0;
			for (size_t i = 0; i < sceneObjCount; i++) 
				scenePolyCount += ((LPMESH3D)lpScene->getObject(CLS_MESH, i))->getPolygonsCount();

			sceneLightCount			= lpScene->getObjectClassCount(CLS_LIGHT);
			scenePolyColorBuffer	= new COLORREF[scenePolyCount];
			sceneLightedPolyCount	= 0;
		}

		lpCamera->GetViewMatrix(cameraMatrix);
		lpCamera->GetProjectionMatrix(projectionMatrix);

		viewportMatrix.SetIdentity();
		viewportMatrix._22 = -1.0F;
		viewportMatrix._41 = (FLOAT)viewportRect.right / 2;
		viewportMatrix._42 = (FLOAT)viewportRect.bottom / 2;

		// Drawing objects
		for ( UINT i = 0; i < sceneObjCount; i++ ) 
		{
			objToRender	= (LPMESH3D)lpScene->getObject(CLS_MESH, i);

			LPVECTOR3D objVertBufferRaw;
			objToRender->getVertexCacheDataRaw(objVertBufferRaw, objVertCount);
			objVertBuffer = new VECTOR3D[objVertCount];
			CopyMemory(objVertBuffer, objVertBufferRaw, objVertCount * sizeof(VECTOR3D));

			// calculate lighting here
			if ( rMode != RM_WIREFRAME ) 
			{
				objToRender->getPolygonDataRaw(objPolyBuffer, objPolyCount);
				size_t lightTo	= sceneLightedPolyCount + objPolyCount; // number of polygons to light

				for (size_t j = sceneLightedPolyCount; j < lightTo; j++) {
					VECTOR3D normal(objPolyBuffer[j - sceneLightedPolyCount].Normal(objVertBuffer, 2));
					Vector3DNormalize(normal, normal);
					if ( !sceneLightCount ) {
						scenePolyColorBuffer[j] = objToRender->getColor();
						continue;
					}
					else
						scenePolyColorBuffer[j] = 0;

					for (UINT k = 0; k < sceneLightCount; k++) 	{
						lightToRender = (LPOMNILIGHT3D)lpScene->getObject(CLS_LIGHT, k);
						FLOAT power = lightToRender->getPower();
						COLORREF lightColor	= lightToRender->getColor();
						if ( power == 0 || lightColor == BLACK)
							continue;

						FLOAT ratio = Vector3DMultS(normal, lightToRender->getForwardLookDirection());
						if (ratio < -.0)
							ratio = power - ratio;
						else
							if (ratio < .0)
								ratio = max(power / 3.3f, (FLOAT)DARK_SIDE);
							else
								ratio = (FLOAT)DARK_SIDE;

						COLORREF newColor = objToRender->getColor();								
						UINT red	= (UINT)(min((RED(newColor) + RED(lightColor))/2,     255) * ratio);
						UINT green	= (UINT)(min((GREEN(newColor) + GREEN(lightColor))/2, 255) * ratio);
						UINT blue	= (UINT)(min((BLUE(newColor) + BLUE(lightColor))/2,   255) * ratio);

						if (scenePolyColorBuffer[j] != BLACK) {
							newColor = RGB(
								(BYTE)min((red	+ RED(scenePolyColorBuffer[j])), 255),
								(BYTE)min((green + GREEN(scenePolyColorBuffer[j])), 255),
								(BYTE)min((blue + BLUE(scenePolyColorBuffer[j])), 255)
							);
						}
						else {
							newColor = RGB( 
								red > 255	? 255 : red,
								green > 255	? 255 : green,
								blue > 255	? 255 : blue 
							);
						}
						scenePolyColorBuffer[j] = newColor;
					}
				}
			}

			// camera projection transformations
			for ( UINT j = 0; j < objVertCount; j++ ) 
			{
				Matrix3DTransformCoord(cameraMatrix, objVertBuffer[j], objVertBuffer[j]);
				
				float z = objVertBuffer[j].z;
				Matrix3DTransformCoord(projectionMatrix, objVertBuffer[j],	objVertBuffer[j]);
				objVertBuffer[j] *= projectionMatrix._34 / z;

				Matrix3DTransformCoord(viewportMatrix, objVertBuffer[j], objVertBuffer[j]);
			}

			if ( rMode != RM_WIREFRAME ) 
			{
				// filling a part of scene Buf
				for (UINT j = 0; j < objPolyCount; j++) {
					VECTOR3D normal(objPolyBuffer[j].Normal(objVertBuffer, 1));
					float cosCam = Vector3DMultS(normal, VECTOR3D(0,0,1)) 
													/ Vector3DLength(normal);
					if (cosCam >= -1 && cosCam < 0) {
						DIRECTPOLY3D tmp;
						tmp.first	= objVertBuffer[ objPolyBuffer[j].first ];
						tmp.second	= objVertBuffer[ objPolyBuffer[j].second ];
						tmp.third	= objVertBuffer[ objPolyBuffer[j].third ];
						tmp.colorRef = scenePolyColorBuffer[j + sceneLightedPolyCount];
						scenePolyBuffer.push_back(pair<DIRECTPOLY3D,int>(tmp,i));
					}
				}
				sceneLightedPolyCount += objPolyCount;
			}
			else 
			{
				objToRender->getEdgeDataRaw(objEdgeBuffer, objEdgeCount);
				hPenCurrent	= CreatePen(PS_SOLID, 1, objToRender->getColor());
				hPenOld		= (HPEN)SelectObject(hDCOutput, hPenCurrent);
				for ( UINT j = 0; j < objEdgeCount; j++ ) 
				{
					if ( objVertBuffer[objEdgeBuffer[j].first].z	>= .0F
						&& objVertBuffer[objEdgeBuffer[j].first].z	<= projectionMatrix._34
						&& objVertBuffer[objEdgeBuffer[j].second].z >= .0F
						&& objVertBuffer[objEdgeBuffer[j].second].z <= projectionMatrix._34
						&& objVertBuffer[objEdgeBuffer[j].first].x	>= .0F
						&& objVertBuffer[objEdgeBuffer[j].first].x	<= viewportRect.right
						&& objVertBuffer[objEdgeBuffer[j].first].y	>= .0F
						&& objVertBuffer[objEdgeBuffer[j].first].y	<= viewportRect.bottom
						&& objVertBuffer[objEdgeBuffer[j].second].x	>= .0F
						&& objVertBuffer[objEdgeBuffer[j].second].x	<= viewportRect.right
						&& objVertBuffer[objEdgeBuffer[j].second].y	>= .0F
						&& objVertBuffer[objEdgeBuffer[j].second].y	<= viewportRect.bottom
					) { 
						vert2DDrawBuffer[0].x 
							= (LONG)objVertBuffer[objEdgeBuffer[j].first].x;
						vert2DDrawBuffer[0].y 
							= (LONG)objVertBuffer[objEdgeBuffer[j].first].y;
				
						vert2DDrawBuffer[1].x 
							= (LONG)objVertBuffer[objEdgeBuffer[j].second].x;
						vert2DDrawBuffer[1].y 
							= (LONG)objVertBuffer[objEdgeBuffer[j].second].y;

						Polyline(hDCOutput, vert2DDrawBuffer, 2);
					}
				}
				SelectObject(hDCOutput, hPenOld);
				DeleteObject(hPenCurrent);
			}

			delete[] objVertBuffer;
		}

		if ( rMode != RM_WIREFRAME ) 
		{
			sort(scenePolyBuffer.begin(), scenePolyBuffer.end(), ZDepthComparator);

			scenePolyCount	= scenePolyBuffer.size();
			for (UINT i = 0; i < scenePolyCount; i++ ) 
			{
				if ( scenePolyBuffer[i].first.first.z    > .0F
					&& scenePolyBuffer[i].first.first.z  < projectionMatrix._34
					&& scenePolyBuffer[i].first.second.z > .0F
					&& scenePolyBuffer[i].first.second.z < projectionMatrix._34
					&& scenePolyBuffer[i].first.third.z  > .0F
					&& scenePolyBuffer[i].first.third.z  < projectionMatrix._34

					&& scenePolyBuffer[i].first.first.x  >= .0F
					&& scenePolyBuffer[i].first.first.x  <= viewportRect.right
					&& scenePolyBuffer[i].first.second.x >= .0F
			   		&& scenePolyBuffer[i].first.second.x <= viewportRect.right
					&& scenePolyBuffer[i].first.third.x  >= .0F
			   		&& scenePolyBuffer[i].first.third.x  <= viewportRect.right
				  
					&& scenePolyBuffer[i].first.first.y  >= .0F
			   		&& scenePolyBuffer[i].first.first.y  <= viewportRect.bottom
					&& scenePolyBuffer[i].first.second.y >= .0F
			   		&& scenePolyBuffer[i].first.second.y <= viewportRect.bottom
					&& scenePolyBuffer[i].first.third.y  >= .0F
			   		&& scenePolyBuffer[i].first.third.y  <= viewportRect.bottom
				) { 
					objToRender	= (LPMESH3D)lpScene->getObject(
												CLS_MESH, 
												scenePolyBuffer[i].second
											);

					hBrCurrent = CreateSolidBrush(scenePolyBuffer[i].first.colorRef);
					if ( rMode == RM_SHADEDWF ) 
						hPenCurrent = CreatePen(PS_SOLID, 1, objToRender->getColor());
					else
						hPenCurrent = CreatePen(PS_SOLID, 1, scenePolyBuffer[i].first.colorRef);
					
					hPenOld = (HPEN)SelectObject(hDCOutput, hPenCurrent);
					hBrOld	= (HBRUSH)SelectObject(hDCOutput, hBrCurrent);

					vert2DDrawBuffer[0].x = (LONG)scenePolyBuffer[i].first.first.x;
					vert2DDrawBuffer[0].y = (LONG)scenePolyBuffer[i].first.first.y;

					vert2DDrawBuffer[1].x = (LONG)scenePolyBuffer[i].first.second.x;
					vert2DDrawBuffer[1].y = (LONG)scenePolyBuffer[i].first.second.y;

					vert2DDrawBuffer[2].x = (LONG)scenePolyBuffer[i].first.third.x;
					vert2DDrawBuffer[2].y = (LONG)scenePolyBuffer[i].first.third.y;

					Polygon(hDCOutput, vert2DDrawBuffer, 3);

					SelectObject(hDCOutput, hBrOld);
					SelectObject(hDCOutput, hPenOld);
					DeleteObject(hBrCurrent);
					DeleteObject(hPenCurrent);
				}
			}
			delete[] scenePolyColorBuffer;
		}

		BITMAP info;
		bResult &= (GetObject(GetCurrentObject(hDCScreen, OBJ_BITMAP), sizeof(BITMAP), &info) == sizeof(BITMAP))
			&& StretchBlt(hDCScreen, 0, 0, info.bmWidth, info.bmHeight, hDCOutput, 0, 0, viewportRect.right, viewportRect.bottom, SRCCOPY);
	}

	return bResult;
}

//VOID SetViewportDefaultView(LPVIEWPORT vp, VIEW_TYPE vt)
//{
//	VECTOR3D		defCamPos;
//	FLOAT			perspCoords = VIEW_DISTANCE_DEFAULT;
//
//	if ( vp != NULL )
//	{
//		LPCAMERA3D cam = vp->getCamera();
//		switch ( vt )
//		{
//			case VIEW_LEFT:
//				defCamPos.y = perspCoords;
//				break;
//
//			case VIEW_RIGHT:
//
//				defCamPos.y = -perspCoords;
//				break;
//
//			case VIEW_FRONT:
//				defCamPos.x = perspCoords;
//				break;	
//
//			case VIEW_BACK:
//				defCamPos.x = -perspCoords;
//				break;	
//
//			case VIEW_TOP:
//				defCamPos.z = perspCoords;
//				break;
//
//			case VIEW_BOTTOM:
//
//				defCamPos.z = -perspCoords;
//				break;
//
//			case VIEW_PERSPECTIVE:
//				perspCoords = sqrt((perspCoords * perspCoords) / 3);
//				defCamPos.x = perspCoords;
//				defCamPos.y = perspCoords;
//				defCamPos.z = perspCoords;
//				cam->setProjectionType(PT_CENTRAL);
//				break;
//		}
//
//		cam->Translate(defCamPos);
//	}
//}

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
