#define WIN32_LEAN_AND_MEAN
#include "Alice.h"
#include "Scene.h"
LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg){
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd){
	//register window
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hIcon = NULL;
	wndclass.hIconSm = NULL;
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = GLWindowProc;
	wndclass.lpszClassName = L"AWindow";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	ATOM atom = RegisterClassEx(&wndclass);
	if (!atom){
		return 0;
	}
	RECT rect = { 0,0,1280,720 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	int viewportWidth = rect.right - rect.left;
	int viewportHeight = rect.bottom - rect.top;
	//create window
	HWND hwnd = CreateWindowEx(NULL, L"AWindow", L"Alice Rendering Window", WS_OVERLAPPEDWINDOW,
		100, 100, viewportWidth, viewportHeight, NULL, NULL, hInstance, NULL);
	viewportWidth = 1280;
	viewportHeight = 720;
	//show window
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	HDC dc = GetDC(hwnd);
	HARC rc = aCreateRenderContext(dc);
	aMakeCurrent(dc, rc);
	OnViewportChanged(1280, 720);
	InitScene();
	MSG msg;
	while (true){
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)){
			if (msg.message == WM_QUIT){
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (Render()) {
			aSwapBuffers(dc);
		}
		Sleep(1);
	}
	return 0;
}