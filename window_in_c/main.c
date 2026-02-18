#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdio.h>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

HWND button;
HWND button_2;

HWND hwnd1 = NULL;
HWND hwnd2 = NULL;
HWND hwnd3 = NULL;

int screen_width = 500;
int screen_height = 500;
int window_pos_x = 0;
int window_pos_y = 0;

int WINAPI wWinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, PWSTR cmd_line, int cmd_show) {
	const wchar_t CLASS_NAME[] = L"Sample window class";
	
	WNDCLASS wc = {0};
	
	wc.lpfnWndProc = &WindowProc;
	wc.hInstance = h_instance;
	wc.lpszClassName = CLASS_NAME;
	
	RegisterClass(&wc);
	
	hwnd1 = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Balls",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		window_pos_x, window_pos_y, screen_width, screen_height,
		NULL,
		NULL,
		h_instance,
		NULL
	);
	
	hwnd2 = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Balls2",
		WS_OVERLAPPEDWINDOW,
		150, 150, 100, 100,
		NULL,
		NULL,
		h_instance,
		NULL
		
	);
	
	hwnd3 = CreateWindow(L"Edit", L"0", WS_VISIBLE | SS_LEFT, 1750, 20, 150, 150, NULL, NULL, h_instance, NULL);
	
	
	if (!hwnd1 || !hwnd2) {
		return 0;
	}
	
	ShowWindow(hwnd1, cmd_show);
	ShowWindow(hwnd2, cmd_show);
	ShowWindow(hwnd3, cmd_show);
	
	MSG msg = {0};
	
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 0;
}

int color_counter = 0;
int direction = 1;
int width_dir = 1;
int height_dir = 1;
int pos_x_dir = 1;
int pos_y_dir = 1;

int click_counter = 0;

int multiplier = 1;

COLORREF current_bg_color;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	

	
	
	
	switch (msg) {
		
		case WM_CREATE:{
			SetTimer(hwnd, 1, 16, NULL);
			
			return 0;
		}break;
		
		
		case WM_TIMER:{
			
			wchar_t buffer[16];
			
			_itow(click_counter, buffer, 30);
			
			SetWindowText(hwnd3, buffer);
			
			if (hwnd == hwnd1) {
				window_pos_x += 8 * pos_x_dir * multiplier;
				window_pos_y += 8 * pos_y_dir * multiplier;
				
				if (window_pos_x >= 1920 - 500){
					pos_x_dir = -1;
					multiplier += 0.001;
				}
				if (window_pos_x <= 0){
					pos_x_dir = 1;
					multiplier += 0.001;
				}
				if (window_pos_y >= 1040- 500){
					pos_y_dir = -1;
					multiplier += 0.001;
				}
				if (window_pos_y <= 0) {
					pos_y_dir = 1;
					multiplier += 0.001;
				}
				
				current_bg_color = RGB(((window_pos_x - 0)*(255 - 0))/((1920) - 0) + 0,((window_pos_y - 0)*(255 - 0))/((1080) - 0) + 0, 0);
				InvalidateRect(hwnd1, NULL, TRUE);
				SetWindowPos(hwnd1, NULL, window_pos_x, window_pos_y, screen_width, screen_height, SWP_NOZORDER); 
			}
		}break;
		
		case WM_LBUTTONDOWN:{
			if (hwnd == hwnd2){
				SetWindowPos(hwnd2, NULL, rand() % 1820, rand() % 1000, 100, 100, SWP_NOZORDER);
				click_counter += 1;
			}	
			return 0;
		}break;

	
		
		
		case WM_COMMAND:{
			int ID = LOWORD(w_param);
			int notif = HIWORD(w_param);
			
			return 0;
		}break;
		
		case WM_MOUSEMOVE: {
			InvalidateRect(hwnd1, NULL, TRUE);
			return 0;
		}break;
		
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		} break;

		case WM_PAINT: {	
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			int random_red = rand() % 256;
			int random_blue = rand() % 256;
			HBRUSH brush = CreateSolidBrush(RGB(random_red, color_counter, random_blue));
			HBRUSH bg_brush = CreateSolidBrush(current_bg_color);
			
			//FillRect(hdc, &r, brush);
			if (color_counter == 255){
				direction = -1;
			}
			if (color_counter == 0){
				direction = 1;
			}
				
			RECT bg_rect;
			GetWindowRect(hwnd1, &bg_rect);
			
			bg_rect.right -= bg_rect.left;
			bg_rect.bottom -= bg_rect.top;
			bg_rect.left = bg_rect.top = 0;
			
			color_counter += 1 * direction;
			FillRect(hdc, &bg_rect, bg_brush);
			
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);
			
			int width = 50;
			int height = 50;
			
			RECT r;
			r.left = pt.x;
			r.top = pt.y;
			r.right = pt.x + width;
			r.bottom = pt.y + height;
			
			FillRect(hdc, &r, brush);
			
			
			DeleteObject(brush);
			DeleteObject(bg_brush);
			EndPaint(hwnd, &ps);
			return 0;
		} break;
		default: break;
	}
	
	return DefWindowProc(hwnd, msg, w_param, l_param);
}
