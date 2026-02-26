#include <cstdio>

#include "../Platform/Window.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow) {
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	Window window;
	window.initialize("DX12Engine", 1024, 1024);

	while (true) {
		window.processMessages();
	}

	FreeConsole();
	return 0;
}