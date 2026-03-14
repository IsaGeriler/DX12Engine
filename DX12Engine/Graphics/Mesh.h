#pragma once

#include "../Graphics/DX12Core.h"
#include "../Graphics/Vertex.h"
#include "../Core/MyMath.h"

class Mesh {
public:
	// Create buffer and upload vertices to GPU
	// Vertex and Index Buffers
	ID3D12Resource* vertexBuffer;
	ID3D12Resource* indexBuffer;

	// Create view member variable
	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;

	// Input Layout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;

	unsigned int numMeshIndices;

	void initialize(DX12Core* core, void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices) {
		// Specify vertex buffer will be in GPU memory heap
		D3D12_HEAP_PROPERTIES heapprops;
		memset(&heapprops, 0, sizeof(D3D12_HEAP_PROPERTIES));
		heapprops.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapprops.CreationNodeMask = 1;
		heapprops.VisibleNodeMask = 1;

		// Create vertex buffer on heap
		D3D12_RESOURCE_DESC vbDesc;
		memset(&vbDesc, 0, sizeof(D3D12_RESOURCE_DESC));
		vbDesc.Width = numVertices * vertexSizeInBytes;
		vbDesc.Height = 1;
		vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vbDesc.DepthOrArraySize = 1;
		vbDesc.MipLevels = 1;
		vbDesc.SampleDesc.Count = 1;
		vbDesc.SampleDesc.Quality = 0;
		vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// Allocate memory
		HRESULT hr;
		hr = core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &vbDesc, D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&vertexBuffer));

		// Copy vertices using our helper function
		core->uploadResource(vertexBuffer, vertices, numVertices * vertexSizeInBytes, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		// Add Index Buffer creation on GPU memory
		D3D12_RESOURCE_DESC ibDesc;
		memset(&ibDesc, 0, sizeof(D3D12_RESOURCE_DESC));
		ibDesc.Width = numIndices * sizeof(unsigned int);
		ibDesc.Height = 1;
		ibDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		ibDesc.DepthOrArraySize = 1;
		ibDesc.MipLevels = 1;
		ibDesc.SampleDesc.Count = 1;
		ibDesc.SampleDesc.Quality = 0;
		ibDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// Allocate memory
		hr = core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &ibDesc, D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&indexBuffer));

		// Copy indices using our helper function
		core->uploadResource(indexBuffer, indices, numIndices * sizeof(unsigned int), D3D12_RESOURCE_STATE_INDEX_BUFFER);

		// Fill in view in helper function
		vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vbView.StrideInBytes = vertexSizeInBytes;
		vbView.SizeInBytes = numVertices * vertexSizeInBytes;

		// Create view to Index Buffer
		ibView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R32_UINT;
		ibView.SizeInBytes = numIndices * sizeof(unsigned int);

		numMeshIndices = numIndices;
	}

	// Initialize a static mesh
	void initialize(DX12Core* core, std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices) {
		initialize(core, &vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size());
		inputLayoutDesc = VertexLayoutCache::getStaticLayout();
	}

	// Initialize an animated mesh
	void initialize(DX12Core* core, std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices) {
		initialize(core, &vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size());
		inputLayoutDesc = VertexLayoutCache::getAnimatedLayout();
	}

	void draw(DX12Core* core) {
		core->getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core->getCommandList()->IASetVertexBuffers(0, 1, &vbView);
		core->getCommandList()->IASetIndexBuffer(&ibView);
		core->getCommandList()->DrawIndexedInstanced(numMeshIndices, 1, 0, 0, 0);
	}
};