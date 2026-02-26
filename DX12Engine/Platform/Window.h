#pragma once

#include <string>
#include <Windows.h>

#define NOMINMAX
#define WINDOW_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define WINDOW_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

class Window {
public:
	HWND hwnd;			  // Handle to a window
	HINSTANCE hInstance;  // Handle to an instance

	std::string name;			 // Window name
	unsigned int width, height;  // Window width and height

	bool keys[256];
	bool mouseButtons[3];

	int mouse_x;
	int mouse_y;

	void initialize(std::string window_name, unsigned int window_width, unsigned int window_height, unsigned int window_x = 0, unsigned int window_y = 0);
	void updateMouse(int x, int y);
	void processMessages();
};