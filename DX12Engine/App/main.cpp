#include <cstdio>

#include "../Graphics/AnimatedMeshModel.h"
#include "../Graphics/Cube.h"
#include "../Core/MyMath.h"
#include "../Platform/Window.h"
#include "../Platform/Timer.h"
#include "../Graphics/DX12Core.h"
#include "../Graphics/Plane.h"
#include "../Graphics/PSOManager.h"
#include "../Graphics/Shader.h"
#include "../Graphics/StaticMeshModel.h"
#include "../Graphics/Sphere.h"
#include "../Graphics/Texture.h"

#define WIDTH 1920
#define HEIGHT 1080

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
	TextureManager textures;

	Plane plane;
	plane.initialize(&core, &psos, &shaders);

	Cube cube;
	cube.initialize(&core, &psos, &shaders);

	Sphere sphere;
	sphere.initialize(&core, &psos, &shaders, 32, 32, 1000.f);

	Matrix acaciaWorld;
	acaciaWorld = Matrix::scale(0.01f) * Matrix::translate(Vec3(0.f, 0.f, 5.f));
	StaticMeshModel acacia;
	acacia.initialize(&core, &psos, &shaders, "../DX12Engine/Asset/Models/acacia_003.gem");

	Matrix trex_world = Matrix::scale(Vec3(0.01f, 0.01f, 0.01f)) * Matrix::translate(Vec3(-2.5f, 0.f, -2.5f));
	AnimatedMeshModel trex;
	trex.initialize(&core, &psos, &shaders, &textures, "../DX12Engine/Asset/Models/TRex.gem");

	AnimationInstance instance;
	instance.initialize(&trex.animation);

	Matrix vp;
	Timer timer;
	float time = 0.f;

	while (true) {
		core.beginFrame();

		Matrix cubeWorld = Matrix::translate(Vec3(0.f, 0.f, 5.f));
		float dt = timer.dt();
		time += dt;

		Vec3 from = Vec3(11.f * cosf(time), 5.f, 11.f * sinf(time));
		Matrix v = Matrix::lookAt(from, Vec3(0.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f));
		Matrix p = Matrix::perspective(60.f, 1920.f/1080.f, 0.01f, 10000.f);
		vp = v * p;

		window.processMessages();
		
		if (window.keys[VK_ESCAPE]) break;
		plane.draw(&core, &psos, &shaders, vp);
		cube.draw(&core, &psos, &shaders, cubeWorld, vp);
		sphere.draw(&core, &psos, &shaders, vp);

		// Drawing multiple mesh example (no instanced mesh)
		cubeWorld = Matrix::translate(Vec3(5.f, 0.f, 0.f));
		cube.draw(&core, &psos, &shaders, cubeWorld, vp);
		cubeWorld = Matrix::translate(Vec3(-5.f, 0.f, 0.f));
		cube.draw(&core, &psos, &shaders, cubeWorld, vp);
		cubeWorld = Matrix::translate(Vec3(0.f, 0.f, -5.f));
		cube.draw(&core, &psos, &shaders, cubeWorld, vp);

		// Draw Static Model
		acacia.draw(&core, &psos, &shaders, acaciaWorld, vp);

		// Draw Animated Model
		instance.updateAnimation("run", dt);
		trex.draw(&core, &psos, &shaders, &textures, trex_world, vp, &instance);

		core.finishFrame();
	}

	core.flushGraphicsQueue();
	FreeConsole();
	return 0;
}