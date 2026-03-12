#include <cstdio>

#include "../Graphics/Cube.h"
#include "../Core/MyMath.h"
#include "../Platform/Window.h"
#include "../Platform/Timer.h"
#include "../Graphics/DX12Core.h"
#include "../Graphics/Plane.h"
#include "../Graphics/PSOManager.h"
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

	PSOManager psos;
	ShaderManager shaders;

	Plane plane;
	plane.initialize(&core, &psos, &shaders);

	Cube cube;
	cube.initialize(&core, &psos, &shaders);

	Matrix vp;
	Timer timer;
	float time = 0.f;

	while (true) {
		time += timer.dt();

		Vec3 from = Vec3(11.f * cosf(time), 5.f, 11.f * sinf(time));
		Matrix v = Matrix::lookAt(from, Vec3(0.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f));
		Matrix p = Matrix::perspective(60.f, 1.7777778f, 0.01f, 10000.f);
		vp = v * p;

		core.beginFrame();
		window.processMessages();
		
		if (window.keys[VK_ESCAPE]) break;
		plane.draw(&core, &psos, &shaders, vp);
		cube.draw(&core, &psos, &shaders, vp);
		
		core.finishFrame();
	}

	core.flushGraphicsQueue();
	FreeConsole();
	return 0;
}