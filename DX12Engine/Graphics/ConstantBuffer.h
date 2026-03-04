#pragma once

#include <map>

#include "../Graphics/DX12Core.h"

// Constant Buffers are hard to manage, adapt to code reflection!
struct ConstantBufferVariable {
	unsigned int offset;
	unsigned int size;
};

class ConstantBuffer {
public:
	// Create resource to store constant buffer
	ID3D12Resource* constantBuffer;
	unsigned char* buffer;
	unsigned int cbSizeInBytes;

	unsigned int maxDrawCalls;
	unsigned int offsetIndex;

	// Constant Buffer Name
	std::string name;
	std::map<std::string, ConstantBufferVariable> constantBufferData;  // <Name, Offset from start and size>

	void initialize(DX12Core* core, unsigned int sizeInBytes, unsigned int _maxDrawCalls = 1024) {
		cbSizeInBytes = (sizeInBytes + 255) & ~255;
		unsigned int cbSizeInBytesAligned = cbSizeInBytes * _maxDrawCalls;
		
		maxDrawCalls = _maxDrawCalls;
		offsetIndex = 0;
		
		HRESULT hr;
		D3D12_HEAP_PROPERTIES heapprops = {};
		heapprops.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapprops.CreationNodeMask = 1;
		heapprops.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC cbDesc = {};
		cbDesc.Width = cbSizeInBytesAligned;
		cbDesc.Height = 1;
		cbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbDesc.DepthOrArraySize = 1;
		cbDesc.MipLevels = 1;
		cbDesc.SampleDesc.Count = 1;
		cbDesc.SampleDesc.Quality = 0;
		cbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		hr = core->device->CreateCommittedResource(
			&heapprops, D3D12_HEAP_FLAG_NONE,
			&cbDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL,
			IID_PPV_ARGS(&constantBuffer)
		);
		hr = constantBuffer->Map(0, NULL, (void**)&buffer);
	}

	void update(std::string name, void* data) {
		auto it = constantBufferData.find(name);
		if (it == constantBufferData.end()) return;

		ConstantBufferVariable cbVariable = it->second;
		unsigned int offset = offsetIndex * cbSizeInBytes;
		memcpy(&buffer[offset + cbVariable.offset], data, cbVariable.size);
	}

	D3D12_GPU_VIRTUAL_ADDRESS getGPUAddress() const {
		return (constantBuffer->GetGPUVirtualAddress() + (offsetIndex * cbSizeInBytes));
	}

	// Ring Buffer
	void next() {
		offsetIndex++;
		if (offsetIndex >= maxDrawCalls) offsetIndex = 0;
	}
};