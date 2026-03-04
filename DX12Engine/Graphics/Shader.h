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
	ID3DBlob* vertexShader = nullptr;
	ID3DBlob* pixelShader = nullptr;

	std::vector<ConstantBuffer> vertexShaderConstantBuffers;
	std::vector<ConstantBuffer> pixelShaderConstantBuffers;

	// Create instance of Vertex
	ScreenSpaceTriangle prim;

	// Create instance of Pipeline State Manager
	PSOManager psos;

	// Create instance of a Constant Buffer
	ConstantBuffer cbuffer;

	std::string read(std::string filename) {
		std::ifstream file(filename);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

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
		reflection->Release();
	}

	void compile(DX12Core* core) {
		std::string vertexShaderPath = "../DX12Engine/Shaders/TriangleVS.hlsl";
		std::string pixelShaderPath = "../DX12Engine/Shaders/TrianglePS.hlsl";
		
		ID3DBlob* status;

		std::string vertexShaderStr = read(vertexShaderPath);
		std::string pixelShaderStr = read(pixelShaderPath);
		
		HRESULT hr = D3DCompile(vertexShaderStr.c_str(), strlen(vertexShaderStr.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &vertexShader, &status);
		if (FAILED(hr)) { printf("%s", (char*)status->GetBufferPointer()); exit(0); }
		initializeConstantBuffers(core, vertexShader, vertexShaderConstantBuffers);
		
		hr = D3DCompile(pixelShaderStr.c_str(), strlen(pixelShaderStr.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &pixelShader, &status);
		if (FAILED(hr)) { printf("%s", (char*)status->GetBufferPointer()); exit(0); }
		initializeConstantBuffers(core, pixelShader, pixelShaderConstantBuffers);

		psos.createPSO(core, "Triangle", vertexShader, pixelShader, prim.mesh.inputLayoutDesc);
	}

	void apply(DX12Core* core) {
		for (int i = 0; i < vertexShaderConstantBuffers.size(); i++) {
			core->getCommandList()->SetGraphicsRootConstantBufferView(0, vertexShaderConstantBuffers[i].getGPUAddress());
			vertexShaderConstantBuffers[i].next();
		}
		for (int i = 0; i < pixelShaderConstantBuffers.size(); i++) {
			core->getCommandList()->SetGraphicsRootConstantBufferView(1, pixelShaderConstantBuffers[i].getGPUAddress());
			pixelShaderConstantBuffers[i].next();
		}
	}

	void initialize(DX12Core* core) {
		prim.initialize(core);
		compile(core);
	}

	void draw(DX12Core* core /* , ConstantBuffer2* cb /* ConstantBuffer1* cb */) {
		core->beginRenderPass();
		apply(core);
		psos.bind(core, "Triangle");
		prim.draw(core);
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
		updateConstant(constantBufferName, variableName, data, vertexShaderConstantBuffers);
	}

	void updateConstantPS(std::string constantBufferName, std::string variableName, void* data) {
		updateConstant(constantBufferName, variableName, data, pixelShaderConstantBuffers);
	}
};

// Implement a ShaderManager
class ShaderManager {
public:
	// Save to file - D3DWriteBlobToFile(compiledShaderCode, L"shadername.cso", false);
	// Read from file - D3DReadFileToBlob(L"shadername.cso", &shader);
	// Store the shaders in a map - std::map<std::string, Shader> shaders;
};