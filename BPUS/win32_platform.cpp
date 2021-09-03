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
					process_button(BUTTON_UP, VK_UP);
					process_button(BUTTON_DOWN, VK_DOWN);
					process_button(BUTTON_LEFT, VK_LEFT);
					process_button(BUTTON_RIGHT, VK_RIGHT);
					process_button(BUTTON_W, 'W');
					process_button(BUTTON_A, 'A');
					process_button(BUTTON_S, 'S');
					process_button(BUTTON_D, 'D');
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