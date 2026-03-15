#pragma once

#include <fstream>
#include <sstream>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/Vertex.h"
#include "../Graphics/PSOManager.h"

#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

class Shader {
public:
	// Vertex and Pixel Shader
	ID3DBlob* vs = nullptr;
	ID3DBlob* ps = nullptr;

	std::vector<ConstantBuffer> vsConstantBuffers;
	std::vector<ConstantBuffer> psConstantBuffers;

	// Create instance of Pipeline State Manager
	PSOManager psos;

	// Create instance of a Constant Buffer
	ConstantBuffer cbuffer;

	// Texture Binding Points
	std::map<std::string, int> textureBindPoints;

	void initializeConstantBuffers(DX12Core* core, ID3DBlob* shader, std::vector<ConstantBuffer> &buffers) {
		ID3D12ShaderReflection* reflection;
		D3DReflect(shader->GetBufferPointer(), shader->GetBufferSize(), IID_PPV_ARGS(&reflection));
		D3D12_SHADER_DESC desc;
		reflection->GetDesc(&desc);

		for (unsigned int i = 0; i < desc.ConstantBuffers; i++) {
			ConstantBuffer buffer;
			ID3D12ShaderReflectionConstantBuffer* constantBuffer = reflection->GetConstantBufferByIndex(i);
			D3D12_SHADER_BUFFER_DESC cbDesc;
			constantBuffer->GetDesc(&cbDesc);
			buffer.name = cbDesc.Name;
			unsigned int totalSize = 0;

			for (unsigned int j = 0; j < cbDesc.Variables; j++) {
				ID3D12ShaderReflectionVariable* var = constantBuffer->GetVariableByIndex(j);
				D3D12_SHADER_VARIABLE_DESC vDesc;
				var->GetDesc(&vDesc);
				ConstantBufferVariable bufferVariable;
				bufferVariable.offset = vDesc.StartOffset;
				bufferVariable.size = vDesc.Size;
				buffer.constantBufferData.insert({ vDesc.Name, bufferVariable });
				totalSize += bufferVariable.size;
			}
			buffer.initialize(core, totalSize);
			buffers.push_back(buffer);
		}

		// Bind Texture Slots from Shader Reflection
		for (int i = 0; i < desc.BoundResources; i++) {
			D3D12_SHADER_INPUT_BIND_DESC bindDesc;
			reflection->GetResourceBindingDesc(i, &bindDesc);

			if (bindDesc.Type == D3D_SIT_TEXTURE) {
				textureBindPoints.insert({ bindDesc.Name, bindDesc.BindPoint });
			}
		}

		reflection->Release();
	}

	void loadVertexShader(DX12Core* core, std::string vs_hlsl) {
		ID3DBlob* status;
		HRESULT hr = D3DCompile(vs_hlsl.c_str(), strlen(vs_hlsl.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &vs, &status);
		if (FAILED(hr)) { printf("%s", (char*)status->GetBufferPointer()); exit(0); }
		initializeConstantBuffers(core, vs, vsConstantBuffers);
	}

	void loadPixelShader(DX12Core* core, std::string ps_hlsl) {
		ID3DBlob* status;
		HRESULT hr = D3DCompile(ps_hlsl.c_str(), strlen(ps_hlsl.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &ps, &status);
		if (FAILED(hr)) { printf("%s", (char*)status->GetBufferPointer()); exit(0); }
		initializeConstantBuffers(core, ps, psConstantBuffers);
	}

	void apply(DX12Core* core) {
		for (int i = 0; i < vsConstantBuffers.size(); i++) {
			core->getCommandList()->SetGraphicsRootConstantBufferView(0, vsConstantBuffers[i].getGPUAddress());
			vsConstantBuffers[i].next();
		}

		for (int i = 0; i < psConstantBuffers.size(); i++) {
			core->getCommandList()->SetGraphicsRootConstantBufferView(1, psConstantBuffers[i].getGPUAddress());
			psConstantBuffers[i].next();
		}
	}

	void updateConstant(std::string constantBufferName, std::string variableName, void* data, std::vector<ConstantBuffer>& buffers) {
		for (int i = 0; i < buffers.size(); i++) {
			if (buffers[i].name == constantBufferName) {
				buffers[i].update(variableName, data);
				return;
			}
		}
	}

	void updateConstantVS(std::string constantBufferName, std::string variableName, void* data) {
		updateConstant(constantBufferName, variableName, data, vsConstantBuffers);
	}

	void updateConstantPS(std::string constantBufferName, std::string variableName, void* data) {
		updateConstant(constantBufferName, variableName, data, psConstantBuffers);
	}

	void updateTexturePS(DX12Core* core, std::string texture_name, int heapOffset) {
		UINT bindPoint = textureBindPoints[texture_name];
		D3D12_GPU_DESCRIPTOR_HANDLE	handle = core->srvHeap.gpuHandle;
		handle.ptr += ((UINT64)(heapOffset - bindPoint) * (UINT64)(core->srvHeap.incrementSize));
		core->getCommandList()->SetGraphicsRootDescriptorTable(2, handle);
	}

	void free() {
		// Free Vertex Shader
		if (vs != nullptr) vs->Release();
		for (ConstantBuffer cb : vsConstantBuffers) cb.free();

		// Free Pixel Shader
		if (ps != nullptr) ps->Release();
		for (ConstantBuffer cb : psConstantBuffers) cb.free();
	}
};

// Implement a ShaderManager
class ShaderManager {
public:
	// Save to file - D3DWriteBlobToFile(compiledShaderCode, L"shadername.cso", false);
	// Read from file - D3DReadFileToBlob(L"shadername.cso", &shader);
	// Store the shaders in a map - std::map<std::string, Shader> shaders;
public:
	std::map<std::string, Shader> shaders;  // <shader_name, Shader>

	~ShaderManager() {
		for (auto it = shaders.begin(); it != shaders.end(); ) {
			it->second.free();
			shaders.erase(it++);
		}
	}

	// Read Shader from filepath
	std::string read(std::string filename) {
		std::ifstream file(filename);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	void load(DX12Core* core, std::string shader_name, std::string vs_hlsl, std::string ps_hlsl) {
		std::map<std::string, Shader>::iterator it = shaders.find(shader_name);
		if (it != shaders.end()) return;

		// Add the Shader to the Shaders Map
		Shader shader;
		shader.loadVertexShader(core, read(vs_hlsl));
		shader.loadPixelShader(core, read(ps_hlsl));
		shaders.insert({ shader_name, shader });
	}

	void apply(DX12Core* core, std::string shader_name) {
		shaders[shader_name].apply(core);
	}

	void updateConstantVS(std::string shader_name, std::string constantBufferName, std::string variableName, void* data) {
		shaders[shader_name].updateConstantVS(constantBufferName, variableName, data);
	}

	void updateConstantPS(std::string shader_name, std::string constantBufferName, std::string variableName, void* data) {
		shaders[shader_name].updateConstantPS(constantBufferName, variableName, data);
	}

	void updateTexturePS(DX12Core* core, std::string shader_name, std::string texture_name, int heapOffset) {
		shaders[shader_name].updateTexturePS(core, texture_name, heapOffset);
	}

	Shader* find(std::string shader_name) {
		return &shaders[shader_name];
	}
};