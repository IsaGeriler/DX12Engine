#pragma once

#include <string>
#include <vector>

#include "../Graphics/Animation.h"
#include "../Graphics/DX12Core.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/PSOManager.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Vertex.h"
#include "../ThirdParty/GEMLoader.h"

class AnimatedMeshModel {
public:
	std::vector<Mesh*> meshes;
	Animation animation;

	std::string shader_name = "AnimatedMeshShader";
	std::string pso_name = "AnimatedMeshPSO";

	void initialize(DX12Core* core, PSOManager* psos, ShaderManager* shaders, std::string filename) {
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		GEMLoader::GEMAnimation gemanimation;
		loader.load(filename, gemmeshes, gemanimation);

		// Load Vertices and Indices
		for (int i = 0; i < gemmeshes.size(); i++) {
			Mesh* mesh = new Mesh();
			std::vector<ANIMATED_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
				ANIMATED_VERTEX vertex;
				memcpy(&vertex, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
				vertices.emplace_back(vertex);
			}
			mesh->initialize(core, vertices, gemmeshes[i].indices);
			meshes.emplace_back(mesh);
		}
		shaders->load(core, shader_name, "../DX12Engine/Shaders/AnimatedMeshVS.hlsl", "../DX12Engine/Shaders/AnimatedMeshPS.hlsl");
		psos->createPSO(core, pso_name, shaders->find(shader_name)->vs, shaders->find(shader_name)->ps, VertexLayoutCache::getAnimatedLayout());
		memcpy(&animation.skeleton.globalInverse, &gemanimation.globalInverse, 16 * sizeof(float));

		// Load Bones
		for (int i = 0; i < gemanimation.bones.size(); i++) {
			Bone bone;
			bone.name = gemanimation.bones[i].name;
			memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
			bone.parentIndex = gemanimation.bones[i].parentIndex;
			animation.skeleton.bones.emplace_back(bone);
		}

		// Load Animation Data
		for (int i = 0; i < gemanimation.animations.size(); i++) {
			std::string name = gemanimation.animations[i].name;
			AnimationSequence animation_sequence;
			animation_sequence.ticksPerSeconds = gemanimation.animations[i].ticksPerSecond;

			for (int n = 0; n < gemanimation.animations[i].frames.size(); n++) {
				AnimationFrame animation_frame;
				for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++) {
					Vec3 position;
					memcpy(&position, &gemanimation.animations[i].frames[n].positions[index], sizeof(Vec3));
					animation_frame.positions.emplace_back(position);

					Quaternion rotation;
					memcpy(&rotation, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
					animation_frame.rotations.emplace_back(rotation);

					Vec3 scale;
					memcpy(&scale, &gemanimation.animations[i].frames[n].scales[index], sizeof(Vec3));
					animation_frame.scales.emplace_back(scale);
				}
				animation_sequence.frames.emplace_back(animation_frame);
			}
			animation.animations.insert({ name, animation_sequence });
		}
	}

	void draw(DX12Core* core, PSOManager* psos, ShaderManager* shaders, Matrix& world, Matrix& vp, AnimationInstance* instance) {
		core->beginRenderPass();
		shaders->apply(core, shader_name);

		shaders->updateConstantVS(shader_name, "animatedMeshBuffer", "W", &world);
		shaders->updateConstantVS(shader_name, "animatedMeshBuffer", "VP", &vp);
		shaders->updateConstantVS(shader_name, "animatedMeshBuffer", "bones", instance->matrices);

		psos->bind(core, pso_name);

		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i]->draw(core);
		}
	}
};