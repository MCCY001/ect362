#undef UNICODE
#include <windows.h>
#include <iostream>	

#include "ect362.h"

#pragma warning(disable: 4996)

#define FLUID_DATA_DIR "C:\\Users\\ycc\\Desktop\\Fulid_type.csv"
#define PIPE_DATA_DIR "C:\\Users\\ycc\\Desktop\\Pipe_Size.csv"
#define CUSTOMER_INFO_DIR "C:\\Users\\ycc\\Desktop\\customer.txt"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); 

/* global variables */
static std::vector<FluidType> fluid_type_list = ReadFluidData(FLUID_DATA_DIR);
static std::vector<PipeSize> pipe_size_list = ReadPipeData(PIPE_DATA_DIR);
static FluidType selected_fluid;
static PipeSelectionResult result;
bool result_valid = false;

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

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName,
		"PipeSeller",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800, 
		600, 
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

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

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
		const char* tips2 = "input maximum tanker truck volume in ft3 here:";
		const char* tips3 = "input maximum tanker fill time ft/s here:";
		const char* tips4 = "input customer id here:";
		TextOut(hdc, 0, 0, tips, strlen(tips));
		TextOut(hdc, 250, 0, tips2, strlen(tips2));
		TextOut(hdc, 250, 75, tips3, strlen(tips3));
		TextOut(hdc, 250, 150, tips4, strlen(tips4));
		//DrawPipeData(hdc, pipe_size_list);
		//DrawFluidData(hdc, fluid_type_list);

		if (result_valid) {
			DisplaySelectedData(hdc, result);
			result_valid = false;  // 重置结果标记
		}

		DeleteObject(hFont);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_CREATE:
	{
		for (int i = 0; i < fluid_type_list.size(); i++) {
			HWND hwndRadioButton = CreateWindow(
				"BUTTON",
				fluid_type_list[i].name.c_str(),
				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				0, 20 + i * 20, 230, 15,
				hwnd,
				(HMENU)(FLUID_TYPE_PREFIX + i),
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
				NULL);
		}

		HWND max_tanker_truck_volume = CreateWindow(
			"EDIT",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			250, 30, 300, 20,
			hwnd,
			(HMENU)MAXIMUM_TANKER_TRUCK_VOLUME_EDIT,
			(HINSTANCE)0,
			NULL);

		HWND max_tanker_fill_time = CreateWindow(
			"EDIT",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			250, 100, 300, 20,
			hwnd,
			(HMENU)MAXIMUM_TANKER_FILL_TIME_EDIT,
			(HINSTANCE)0,
			NULL);

		HWND customer_id = CreateWindow(
			"EDIT",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			250, 170, 300, 20,
			hwnd,
			(HMENU)CUSTOMER_ID_EDIT,
			(HINSTANCE)0,
			NULL);

		HWND hwndButton = CreateWindow(
			"BUTTON",
			"Confirm",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10, 20 + fluid_type_list.size() * 20, 100, 30,
			hwnd,
			(HMENU)ID_CONFIRM_BUTTON,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		char buffer[256] = {'\0'};
		switch (wmId) {
		case ID_CONFIRM_BUTTON:
			for (int i = 0; i < fluid_type_list.size(); i++) {
				if (IsDlgButtonChecked(hwnd, FLUID_TYPE_PREFIX + i)) {
					selected_fluid = fluid_type_list[i];
					MessageBox(hwnd, ("你选择了 " + fluid_type_list[i].name).c_str(), "提示", MB_OK);
					break;
				}
			}
			GetWindowText(GetDlgItem(hwnd, MAXIMUM_TANKER_TRUCK_VOLUME_EDIT), buffer, 256);
			double max_tank_volume = atof(buffer);
			GetWindowText(GetDlgItem(hwnd, MAXIMUM_TANKER_FILL_TIME_EDIT), buffer, 256);
			double max_fill_time = atof(buffer);
			GetWindowText(GetDlgItem(hwnd, CUSTOMER_ID_EDIT), buffer, 256);
			unsigned int customer_id = atoi(buffer);
			result = Calculate(pipe_size_list, selected_fluid, max_tank_volume, max_fill_time);
			CustomerOrder current_order{ customer_id, max_tank_volume, max_fill_time, result.actual_pipe };
			WriteCustomerToFile(current_order, CUSTOMER_INFO_DIR);
			result_valid = true;
			InvalidateRect(hwnd, NULL, TRUE);
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