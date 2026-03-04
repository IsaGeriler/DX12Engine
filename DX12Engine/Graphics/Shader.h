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

	void compile(DX12Core* core) {
		std::string vertexShaderPath = "../DX12Engine/Shaders/TriangleVS.hlsl";
		std::string pixelShaderPath = "../DX12Engine/Shaders/TrianglePS.hlsl";
		
		ID3DBlob* status;

		std::string vertexShaderStr = read(vertexShaderPath);
		std::string pixelShaderStr = read(pixelShaderPath);
		
		HRESULT hr = D3DCompile(vertexShaderStr.c_str(), strlen(vertexShaderStr.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &vertexShader, &status);
		hr = D3DCompile(pixelShaderStr.c_str(), strlen(pixelShaderStr.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &pixelShader, &status);

		if (FAILED(hr)) { (char*)status->GetBufferPointer(); }
		psos.createPSO(core, "Triangle", vertexShader, pixelShader, prim.mesh.inputLayoutDesc);
	}

	void initialize(DX12Core* core) {
		prim.initialize(core);
		cbuffer.initialize(core, sizeof(ConstantBuffer1), 2);
		compile(core);
	}

	void draw(DX12Core* core, ConstantBuffer1* cb) {
		core->beginRenderPass();
		cbuffer.update(cb, sizeof(ConstantBuffer1), core->frameIndex());
		core->getCommandList()->SetGraphicsRootConstantBufferView(1, cbuffer.getGPUAddress(core->frameIndex()));
		psos.bind(core, "Triangle");
		prim.draw(core);
	}
};

// Implement a ShaderManager
class ShaderManager {
public:
	// Save to file - D3DWriteBlobToFile(compiledShaderCode, L"shadername.cso", false);
	// Read from file - D3DReadFileToBlob(L"shadername.cso", &shader);
	// Store the shaders in a map - std::map<std::string, Shader> shaders;
};