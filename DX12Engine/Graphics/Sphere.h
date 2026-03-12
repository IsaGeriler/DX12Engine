#pragma once

#include "../Graphics/Mesh.h"
#include "../Graphics/PSOManager.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Vertex.h"

class Sphere {
public:
	Mesh mesh;
	std::string shader_name = "StaticModelSphere";
	std::string pso_name = "StaticModelSpherePSO";

	void initialize(DX12Core* core, PSOManager* psos, ShaderManager* shaders, unsigned int rings, unsigned int segments, float radius) {
		// Create Sphere vertices
		std::vector<STATIC_VERTEX> vertices;
		for (int lat = 0; lat <= rings; lat++) {
			float theta = lat * std::numbers::pi_v<float> / rings;
			float sinTheta = sinf(theta);
			float cosTheta = cosf(theta);
			for (int lon = 0; lon <= segments; lon++) {
				float phi = lon * 2.0f * std::numbers::pi_v<float> / segments;
				float sinPhi = sinf(phi);
				float cosPhi = cosf(phi);
				Vec3 position(radius * sinTheta * cosPhi, radius * cosTheta,
					radius * sinTheta * sinPhi);
				Vec3 normal = position.normalize();
				float tu = 1.0f - (float)lon / segments;
				float tv = 1.0f - (float)lat / rings;
				vertices.push_back(addVertex(position, normal, tu, tv));
			}
		}

		// Create Sphere Indices
		std::vector<unsigned int> indices;
		for (int lat = 0; lat < rings; lat++) {
			for (int lon = 0; lon < segments; lon++) {
				int current = lat * (segments + 1) + lon;
				int next = current + segments + 1;
				indices.push_back(current);
				indices.push_back(next);
				indices.push_back(current + 1);
				indices.push_back(current + 1);
				indices.push_back(next);
				indices.push_back(next + 1);
			}
		}

		// Initialize mesh, shaders, and pso
		mesh.initialize(core, vertices, indices);
		shaders->load(core, shader_name, "../DX12Engine/Shaders/PlaneVS.hlsl", "../DX12Engine/Shaders/PlanePS.hlsl");
		psos->createPSO(core, pso_name, shaders->find(shader_name)->vs, shaders->find(shader_name)->ps, VertexLayoutCache::getStaticLayout());
	}

	void draw(DX12Core* core, PSOManager* psos, ShaderManager* shaders, Matrix& vp) {
		Matrix sphereWorld;
		shaders->apply(core, shader_name);
		shaders->updateConstantVS(shader_name, "staticMeshBuffer", "W", &sphereWorld);
		shaders->updateConstantVS(shader_name, "staticMeshBuffer", "VP", &vp);
		psos->bind(core, pso_name);
		mesh.draw(core);
	}
};