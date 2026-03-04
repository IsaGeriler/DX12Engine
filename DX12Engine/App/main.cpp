#include <cstdio>

#include "../Core/MyMath.h"
#include "../Platform/Window.h"
#include "../Platform/Timer.h"
#include "../Graphics/DX12Core.h"
#include "../Graphics/Shader.h"

#define WIDTH 1024
#define HEIGHT 1024

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow) {
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	Window window;
	window.initialize("DX12Engine", WIDTH, HEIGHT);

	DX12Core core;
	core.initialize(window.hwnd, WIDTH, HEIGHT);

	Shader shader;
	shader.initialize(&core);

	Timer timer;
	Vec4 lights[4];
	float time = 0.f;

	while (true) {
		time += timer.dt();

		shader.updateConstantVS("bufferName", "time", &time);
		shader.updateConstantPS("bufferName", "time", &time);

		for (int i = 0; i < 4; i++) {
			float angle = time + (i * std::numbers::pi_v<float> / 2.f);
			lights[i] = Vec4(
				WIDTH / 2.f + (cosf(angle) * (WIDTH * 0.3f)),
				HEIGHT / 2.f + (sinf(angle) * (HEIGHT * 0.3f)),
				0.f, 0.f
			);
		}

		shader.updateConstantVS("bufferName", "lights", lights);
		shader.updateConstantPS("bufferName", "lights", lights);

		core.beginFrame();
		window.processMessages();
		
		if (window.keys[VK_ESCAPE]) break;
		
		shader.draw(&core);
		core.finishFrame();
	}

	core.flushGraphicsQueue();
	FreeConsole();
	return 0;
}