/* Preprocessor Directives */
#undef UNICODE
#include <windows.h>  // Win32 API

#include "data.h"
#include "display.h"
#include "resource.h"  // resources for your application

#include <iostream>	
#pragma warning(disable: 4996)	//ignore sprintf() error

#define ID_AREABUTTON	1010	//id of area button
#define ID_EDITLEN		1020	//length edit box
#define ID_EDITHEI		1030	//Height edit box
#define FLUID_DATA_DIR "C:\\Users\\ycc\\Desktop\\Fulid_type.csv"
#define PIPE_DATA_DIR "C:\\Users\\ycc\\Desktop\\Pipe_Size.csv"

/* Function Prototypes */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);  //the window procedure
/* A callback function is passed (by reference) to another function */

/* Global variables */
const std::vector<FluidType> fluid_type_list = ReadFluidData(FLUID_DATA_DIR);
const std::vector<PipeSize> pipe_size_list = ReadPipeData(PIPE_DATA_DIR);

/* Functions */
/*****************************************************************
 * Function    | WinMain()
 * Description | Entry point for our application, we create and
 *             |  register a window class and then call CreateWindow
 * Inputs      | None
 * Output      | Integer value 0
 *****************************************************************/
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR szCmdLine,
	int iCmdShow) {
	static char  szAppName[] = "My Window";

	HWND        hwnd, lhWnd;
	WNDCLASSEX  wndclass;
	MSG msg;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;


	/*  Register a new window class with Windows  */
	RegisterClassEx(&wndclass);

	/*  Create a window based on our new class  */
	hwnd = CreateWindow(szAppName,
		"PipeSeller",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800, // initial width
		600, // initial height
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	/* Show and update our window  */
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);


	/*  Retrieve and process any queued messages until we get WM_QUIT  */
	/* Recall that Windows uses a messaging system to notify window of */
	/*  user actions												   */
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);  // send message to WndProc
	}
	return msg.wParam;
}


/*****************************************************************
 * Function    | WinProc()
 * Description | Whenever anything happens to your window, Windows
 *             |  will call this function telling you what has happened.
 *             |  The message parameter contains the message sent
 * Inputs      | None
 * Output      | Integer value 0
 *****************************************************************/
LRESULT CALLBACK WndProc(HWND hwnd,
	UINT iMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (iMsg)
	{
	case WM_PAINT: {
		PAINTSTRUCT ps;
		const HDC& hdc = BeginPaint(hwnd, &ps);
		HFONT hFont = CreateFont(
			16, 0, 0, 0,
			FW_NORMAL,
			FALSE, FALSE, FALSE,
			ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, "Courier New");
		SelectObject(hdc, hFont);

		const char* tips = "Fluid name:";
		TextOut(hdc, 0, 0, tips, strlen(tips));
		DrawPipeData(hdc, pipe_size_list);
		//DrawFluidData(hdc, fluid_type_list);

		DeleteObject(hFont);
		EndPaint(hwnd, &ps);
		break;
	}

#define ID_CONFIRM_BUTTON 1001


		// 在窗口过程的 WM_CREATE 消息处理器中
	case WM_CREATE:
	{
		for (int i = 0; i < fluid_type_list.size(); i++) {
			HWND hwndRadioButton = CreateWindow(
				"BUTTON",
				fluid_type_list[i].name.c_str(),
				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				0, 20 + i * 20, 230, 15,
				hwnd,
				(HMENU)(i),
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
				NULL);
		}
			HWND hwndButton = CreateWindow(
				"BUTTON",   // 预定义的按钮类名
				"Confirm", // 按钮文本
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, // 样式
				10, 20 + fluid_type_list.size() * 20, 100, 30,    // 位置和大小
				hwnd,         // 父窗口句柄
				(HMENU)ID_CONFIRM_BUTTON, // 菜单句柄或子窗口ID
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
				NULL);        // 创建参数
		break;
	}

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		if (wmId == ID_CONFIRM_BUTTON) {
			for (int i = 0; i < fluid_type_list.size(); i++) {
				if (IsDlgButtonChecked(hwnd, i)) {
					MessageBox(hwnd, ("你选择了 " + fluid_type_list[i].name).c_str(), "提示", MB_OK);
					break;
				}
			}
		}
		break;
	}

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, iMsg, wParam, lParam);
	}
	return 0;
}