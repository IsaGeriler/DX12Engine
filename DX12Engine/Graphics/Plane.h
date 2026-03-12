#pragma once

#include "../Graphics/Mesh.h"
#include "../Graphics/PSOManager.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Vertex.h"

class Plane {
public:
	Mesh mesh;
	std::string shader_name = "StaticModelPlane";
	std::string pso_name = "StaticModelPlanePSO";

	void initialize(DX12Core* core, PSOManager* psos, ShaderManager* shaders) {
		std::vector<STATIC_VERTEX> vertices;
		// vertices.push_back(addVertex(Vec3(-15, 0, -15), Vec3(0, 1, 0), 0, 0));
		// vertices.push_back(addVertex(Vec3(15, 0, -15), Vec3(0, 1, 0), 1, 0));
		// vertices.push_back(addVertex(Vec3(-15, 0, 15), Vec3(0, 1, 0), 0, 1));
		// vertices.push_back(addVertex(Vec3(15, 0, 15), Vec3(0, 1, 0), 1, 1));

		vertices.push_back(addVertex(Vec3(-1, 0, -1), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(1, 0, -1), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-1, 0, 1), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(1, 0, 1), Vec3(0, 1, 0), 1, 1));

		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		
		mesh.initialize(core, vertices, indices);
		shaders->load(core, shader_name, "../DX12Engine/Shaders/PlaneVS.hlsl", "../DX12Engine/Shaders/PlanePS.hlsl");
		psos->createPSO(core, pso_name, shaders->find(shader_name)->vs, shaders->find(shader_name)->ps, VertexLayoutCache::getStaticLayout());
	}

	void draw(DX12Core* core, PSOManager* psos, ShaderManager* shaders, Matrix& vp) {
		Matrix planeWorld;
		core->beginRenderPass();
		shaders->updateConstantVS(shader_name, "staticMeshBuffer", "W", &planeWorld);
		shaders->updateConstantVS(shader_name, "staticMeshBuffer", "VP", &vp);
		shaders->apply(core, shader_name);
		psos->bind(core, pso_name);
		mesh.draw(core);
	}
};