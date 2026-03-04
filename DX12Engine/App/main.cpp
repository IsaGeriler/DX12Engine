#include <cstdio>

#include "../Platform/Window.h"
#include "../Graphics/DX12Core.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Shader.h"

#include "../Graphics/Vertex.h"
#include "../Graphics/PSOManager.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow) {
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	Window window;
	window.initialize("DX12Engine", 1024, 1024);

	DX12Core core;
	core.initialize(window.hwnd, 1024, 1024);

	Shader shader;
	shader.initialize(&core);

	while (true) {
		core.beginFrame();
		// core.beginRenderPass();
		window.processMessages();
		if (window.keys[VK_ESCAPE]) break;
		shader.draw(&core);
		core.finishFrame();
	}

	core.flushGraphicsQueue();
	FreeConsole();
	return 0;
}