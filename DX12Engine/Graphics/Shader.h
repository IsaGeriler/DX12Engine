#pragma once

#include <fstream>
#include <sstream>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include "../Graphics/Vertex.h"
#include "../Graphics/PSOManager.h"

#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

class Shader {
public:
	// Vertex and Pixel Shader
	ID3DBlob* vertexShader;
	ID3DBlob* pixelShader;

	// Create instance of Vertex
	ScreenSpaceTriangle prim;

	// Create instance of Pipeline State Manager
	PSOManager psos;

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
		compile(core);
	}

	void draw(DX12Core* core) {
		core->beginRenderPass();
		psos.bind(core, "Triangle");
		prim.draw(core);
	}
};