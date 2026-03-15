#pragma once

#include <string>
#include <vector>

#include "../Graphics/DX12Core.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/PSOManager.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Vertex.h"
#include "../ThirdParty/GEMLoader.h"

class StaticMeshModel {
public:
	std::vector<Mesh*> meshes;
	std::string shader_name = "AcaciaShader";
	std::string pso_name = "AcaciaPSO";
	std::string pso_wireframe_name = "AcaciaWireframePSO";

	void initialize(DX12Core* core, PSOManager* psos, ShaderManager* shaders, std::string filename) {
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		loader.load(filename, gemmeshes);

		for (unsigned int i = 0; i < gemmeshes.size(); i++) {
			Mesh* mesh = new Mesh();
			std::vector<STATIC_VERTEX> vertices;
			for (unsigned int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
				STATIC_VERTEX vertex;
				memcpy(&vertex, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				vertices.emplace_back(vertex);
			}
			mesh->initialize(core, vertices, gemmeshes[i].indices);
			meshes.emplace_back(mesh);
		}
		shaders->load(core, shader_name, "../DX12Engine/Shaders/PlaneVS.hlsl", "../DX12Engine/Shaders/UntexturedPS.hlsl");
		psos->createPSO(core, pso_name, shaders->find(shader_name)->vs, shaders->find(shader_name)->ps, VertexLayoutCache::getStaticLayout());
		psos->createPSO(core, pso_wireframe_name, shaders->find(shader_name)->vs, shaders->find(shader_name)->ps, VertexLayoutCache::getStaticLayout(), true);
	}

	void draw(DX12Core* core, PSOManager* psos, ShaderManager* shaders, Matrix& world, Matrix& vp, bool showWireframe) {
		core->beginRenderPass();
		shaders->apply(core, shader_name);
		shaders->updateConstantVS(shader_name, "staticMeshBuffer", "W", &world);
		shaders->updateConstantVS(shader_name, "staticMeshBuffer", "VP", &vp);
		
		if (showWireframe) psos->bind(core, pso_wireframe_name);
		else psos->bind(core, pso_name);

		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i]->draw(core);
	}
};