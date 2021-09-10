#include <Windows.h>
#include "utils.cpp"

global_variable bool running = true;

struct RenderState {
	int height, width;
	void* memory;

	BITMAPINFO bitmapinfo;
};

global_variable RenderState renderState;

#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		running = false;
		break;

	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		renderState.width = rect.right - rect.left;
		renderState.height = rect.bottom - rect.top;

		int size = renderState.width * renderState.height * sizeof(unsigned int);

		if (renderState.memory) VirtualFree(renderState.memory, 0, MEM_RELEASE);
		renderState.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		renderState.bitmapinfo.bmiHeader.biSize = sizeof(renderState.bitmapinfo.bmiHeader);
		renderState.bitmapinfo.bmiHeader.biWidth = renderState.width;
		renderState.bitmapinfo.bmiHeader.biHeight = renderState.height;
		renderState.bitmapinfo.bmiHeader.biPlanes = 1;
		renderState.bitmapinfo.bmiHeader.biBitCount = 32;
		renderState.bitmapinfo.bmiHeader.biCompression = BI_RGB;
	} break;

	default:
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// Create Window Class
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpszClassName = L"Game Window Class";
	windowClass.lpfnWndProc = window_callback;

	// Register Class
	RegisterClass(&windowClass);

	// Create Window
	HWND window = CreateWindow(windowClass.lpszClassName, L"Bad Piggies Ultimate Sandbox", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	Input input = {};

	float deltatime = 0.1666666f;
	LARGE_INTEGER frameBeginTime;
	QueryPerformanceCounter(&frameBeginTime);

	float performanceFrequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performanceFrequency = (float)perf.QuadPart;
	}

	init();

	while (running) {
		// Input
		MSG message;

		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.buttons[i].changed = false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			switch (message.message)
			{
			case WM_KEYUP:
			case WM_KEYDOWN: {
				u32 vk_code = (u32)message.wParam;
				bool isdown = ((message.lParam & (1 << 31)) == 0);

#define process_button(b, vk)\
case vk: {\
input.buttons[b].changed = isdown != input.buttons[b].isdown;\
input.buttons[b].isdown = isdown;\
} break;

				switch (vk_code)
				{
					process_button(BUTTON_A, 'A');
					process_button(BUTTON_B, 'B');
					process_button(BUTTON_C, 'C');
					process_button(BUTTON_D, 'D');
					process_button(BUTTON_E, 'E');
					process_button(BUTTON_F, 'F');
					process_button(BUTTON_G, 'G');
					process_button(BUTTON_H, 'H');
					process_button(BUTTON_I, 'I');
					process_button(BUTTON_J, 'J');
					process_button(BUTTON_K, 'K');
					process_button(BUTTON_L, 'L');
					process_button(BUTTON_M, 'M');
					process_button(BUTTON_N, 'N');
					process_button(BUTTON_O, 'O');
					process_button(BUTTON_P, 'P');
					process_button(BUTTON_Q, 'Q');
					process_button(BUTTON_R, 'R');
					process_button(BUTTON_S, 'S');
					process_button(BUTTON_T, 'T');
					process_button(BUTTON_U, 'U');
					process_button(BUTTON_V, 'V');
					process_button(BUTTON_W, 'W');
					process_button(BUTTON_X, 'X');
					process_button(BUTTON_Y, 'Y');
					process_button(BUTTON_Z, 'Z');

					process_button(BUTTON_1, '1');
					process_button(BUTTON_2, '2');
					process_button(BUTTON_3, '3');
					process_button(BUTTON_4, '4');
					process_button(BUTTON_5, '5');
					process_button(BUTTON_6, '6');
					process_button(BUTTON_7, '7');
					process_button(BUTTON_8, '8');
					process_button(BUTTON_9, '9');
					process_button(BUTTON_0, '0');

					process_button(BUTTON_NUMPAD_0, VK_NUMPAD0);
					process_button(BUTTON_NUMPAD_1, VK_NUMPAD1);
					process_button(BUTTON_NUMPAD_2, VK_NUMPAD2);
					process_button(BUTTON_NUMPAD_3, VK_NUMPAD3);
					process_button(BUTTON_NUMPAD_4, VK_NUMPAD4);
					process_button(BUTTON_NUMPAD_5, VK_NUMPAD5);
					process_button(BUTTON_NUMPAD_6, VK_NUMPAD6);
					process_button(BUTTON_NUMPAD_7, VK_NUMPAD7);
					process_button(BUTTON_NUMPAD_8, VK_NUMPAD8);
					process_button(BUTTON_NUMPAD_9, VK_NUMPAD9);

					process_button(BUTTON_SPACE, VK_SPACE);
					process_button(BUTTON_ENTER, VK_RETURN);
					process_button(BUTTON_UP, VK_UP);
					process_button(BUTTON_DOWN, VK_DOWN);
					process_button(BUTTON_LEFT, VK_LEFT);
					process_button(BUTTON_RIGHT, VK_RIGHT);
					process_button(BUTTON_SHIFT, VK_SHIFT);
					process_button(BUTTON_CTRL, VK_CONTROL);
					process_button(BUTTON_ALT, VK_MENU);
					process_button(BUTTON_TAB, VK_TAB);
				}
			} break;

			default:
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}

		//Simulate
		simulate(&input, deltatime);

		//Render
		StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frameEndTime;
		QueryPerformanceCounter(&frameEndTime);
		deltatime = (float)(frameEndTime.QuadPart - frameBeginTime.QuadPart) / performanceFrequency;
		frameBeginTime = frameEndTime;
	}
}