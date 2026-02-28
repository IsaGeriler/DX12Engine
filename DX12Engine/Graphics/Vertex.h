#pragma once

#include "DX12Core.h"
#include "Mesh.h"
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
		vertices[0].position = Vec3(0, 1.0f, 0);
		vertices[0].colour = Colour(0, 1.0f, 0);
		vertices[1].position = Vec3(-1.0f, -1.0f, 0);
		vertices[1].colour = Colour(1.0f, 0, 0);
		vertices[2].position = Vec3(1.0f, -1.0f, 0);
		vertices[2].colour = Colour(0, 0, 1.0f);
		mesh.initialize(core, &vertices, sizeof(PRIM_VERTEX), 3);
	}
};