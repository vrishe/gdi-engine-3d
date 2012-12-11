// Test.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "Test.h"

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;								// ������� ���������
TCHAR szTitle[MAX_LOADSTRING];					// ����� ������ ���������
TCHAR szWindowClass[MAX_LOADSTRING];			// ��� ������ �������� ����

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���������� ��� �����.
	MSG msg;
	HACCEL hAccelTable;

	// ������������� ���������� �����
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST));

	// ���� ��������� ���������:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
//
//  �����������:
//
//    ��� ������� � �� ������������� ���������� ������ � ������, ���� �����, ����� ������ ���
//    ��� ��������� � ��������� Win32, �� �������� ������� RegisterClassEx'
//    ������� ���� ��������� � Windows 95. ����� ���� ������� ����� ��� ����,
//    ����� ���������� �������� "������������" ������ ������ � ���������� �����
//    � ����.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 500, 500, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ������������ ��������� � ������� ����.
//
//  WM_COMMAND	- ��������� ���� ����������
//  WM_PAINT	-��������� ������� ����
//  WM_DESTROY	 - ������ ��������� � ������ � ���������.
//
//

HSCENE    scene    = SceneCreate();
HCAMERA	  camera   = CameraCreate(PT_CENTRAL, (FLOAT)(M_PI / 2.0), (FLOAT)(M_PI / 2.0));

HVIEWPORT viewport = ViewportCreate(RM_SHADED);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		{
			SceneAmbientColorSet(scene, RGB(100, 120, 170));

			SCENE_OBJECT test_omni1 = SceneOmniLightCreate(scene, 1.3F, RGB(255,   0,   0));
			SCENE_OBJECT test_omni2 = SceneOmniLightCreate(scene, 1.3F, RGB(  0, 255,   0));
			SCENE_OBJECT test_omni3 = SceneOmniLightCreate(scene, 1.3F, RGB(  0,   0, 255));
			SceneObjectTranslate(scene, test_omni1, 90.0F,    .0F,  300.0F);
			SceneObjectTranslate(scene, test_omni2, 90.0F,    .0F, -300.0F);
			SceneObjectTranslate(scene, test_omni3, 90.0F, 300.0F,     .0F);

			SCENE_OBJECT test_pyramid1 = SceneConeCreate(scene, 1.0F, 1.0F, .5F, 4, RGB(128, 128, 128));
			SceneObjectTranslate(scene, test_pyramid1, 2.5F, .0F, .0F);
			//SceneObjectRotate(scene, test_pyramid1, .0F, (FLOAT)M_PI / 4.0, .0F);

			//SCENE_OBJECT test_sphere1 = SceneSphereCreate(scene, 15.5F, .0F, .0F, .0F, 16, RGB(255, 255, 255));
			//SCENE_OBJECT test_sphere2 = SceneSphereCreate(scene,   .5F, .0F, .0F, .0F,  6, RGB(  0, 255,   0));
			//SCENE_OBJECT test_sphere3 = SceneSphereCreate(scene, 50.2F, .0F, .0F, .0F, 25, RGB(255, 255, 255));
			//SCENE_OBJECT test_sphere4 = SceneSphereCreate(scene, 88.0F, .0F, .0F, .0F, 48, RGB(255, 255, 255));
			//SceneObjectTranslate(scene, test_sphere1,  90.0F,   .0F,    .0F);
			///SceneObjectTranslate(scene, test_sphere2,   2.0F, -1.0F,    .0F);
			//SceneObjectTranslate(scene, test_sphere3, 150.0F, 20.0F,  50.0F);
			//SceneObjectTranslate(scene, test_sphere4, 250.0F, 40.0F, -70.0F);

			//SceneMeshSelfIlluminationSet(scene, test_sphere2, 1.0F);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		ViewportSizeSet(viewport, ps.rcPaint.right, ps.rcPaint.bottom);
		ViewportRender(viewport, scene, camera, hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���������� ��������� ��� ���� "� ���������".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
