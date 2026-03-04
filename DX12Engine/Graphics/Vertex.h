#pragma once

#include "../Graphics/DX12Core.h"
#include "../Graphics/Mesh.h"
#include "../Core/MyMath.h"

struct PRIM_VERTEX {
	Vec3 position;
	Colour colour;
};

class ScreenSpaceTriangle {
public:
	PRIM_VERTEX vertices[3];
	Mesh mesh;

	void initialize(DX12Core* core) {
		vertices[0].position = Vec3(0.f, 1.f, 0.f);
		vertices[0].colour = Colour(0.f, 1.f, 0.f);
		vertices[1].position = Vec3(-1.f, -1.f, 0.f);
		vertices[1].colour = Colour(1.f, 0.f, 0.f);
		vertices[2].position = Vec3(1.f, -1.f, 0.f);
		vertices[2].colour = Colour(0.f, 0.f, 1.f);
		mesh.initialize(core, &vertices[0], sizeof(PRIM_VERTEX), 3);
	}

	void draw(DX12Core* core) {
		mesh.draw(core);
	}
};