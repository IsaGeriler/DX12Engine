#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler.lib")

class DescriptorHeap {
public:
	ID3D12DescriptorHeap* heap;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;  // writing on the CPU, like a CPU pointer
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;  // reading on the GPU, like a GPU pointer
	unsigned int incrementSize;
	int used;								// Keep track of used slots as loading textures

	void initialize(ID3D12Device5* device, int num) {
		D3D12_DESCRIPTOR_HEAP_DESC uavcbvHeapDesc = {};
		uavcbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		uavcbvHeapDesc.NumDescriptors = num;
		uavcbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		device->CreateDescriptorHeap(&uavcbvHeapDesc, IID_PPV_ARGS(&heap));

		cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
		gpuHandle = heap->GetGPUDescriptorHandleForHeapStart();
		incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		used = 0;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE getNextCPUHandle() {
		if (used > 0) cpuHandle.ptr += incrementSize;
		used++;
		return cpuHandle;
	}
};