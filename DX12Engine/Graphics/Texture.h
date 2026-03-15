#pragma once

#include <map>
#include <string>

#include "../Graphics/DX12Core.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../ThirdParty/stb/stb_image.h"

class Texture {
public:
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	ID3D12Resource* tex;
	int heapOffset;

	void load(DX12Core* core, std::string texture_filename) {
		int width = 0, height = 0, channels = 0;
		unsigned char* texels = stbi_load(texture_filename.c_str(), &width, &height, &channels, 0);

		if (channels == 3) {
			channels = 4;
			unsigned char* texelsWithAlpha = new unsigned char[width * height * channels];
			for (int i = 0; i < (width * height); i++) {
				texelsWithAlpha[(i * 4)] = texels[(i * 3)];
				texelsWithAlpha[(i * 4) + 1] = texels[(i * 3) + 1];
				texelsWithAlpha[(i * 4) + 2] = texels[(i * 3) + 2];
				texelsWithAlpha[(i * 4) + 3] = 255;
			}
			// Initialize texture using width, height, channels, an, texelsWithAlpha
			initialize(core, texelsWithAlpha, width, height, channels);
			delete[] texelsWithAlpha;
		}
		else {
			// Initialize texture using width, height, channels, and texels
			initialize(core, texels, width, height, channels);
		}
		stbi_image_free(texels);
	}

	void initialize(DX12Core* core, unsigned char* data, int width, int height, int channels) {
		// Allocate GPU Memory
		D3D12_HEAP_PROPERTIES heapDesc;
		memset(&heapDesc, 0, sizeof(D3D12_HEAP_PROPERTIES));
		heapDesc.Type = D3D12_HEAP_TYPE_DEFAULT;

		D3D12_RESOURCE_DESC textureDesc;
		memset(&textureDesc, 0, sizeof(D3D12_RESOURCE_DESC));
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		core->device->CreateCommittedResource(&heapDesc, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&tex));

		// Copy to upload heap & to GPU memory
		D3D12_RESOURCE_DESC desc = tex->GetDesc();
		unsigned long long size;
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
		core->device->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, NULL, NULL, &size);

		unsigned int alignedWith = ((width * channels) + 255) & ~255;
		core->uploadResource(tex, data, alignedWith * height, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &footprint);

		// Create shader resource view for texture (store position in SRV heap)
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = core->srvHeap.getNextCPUHandle();
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		
		core->device->CreateShaderResourceView(tex, &srvDesc, srvHandle);
		heapOffset = core->srvHeap.used - 1;
	}
};

class TextureManager {
public:
	std::map<std::string, Texture> textures;

	void loadTexture(DX12Core* core, std::string texture_name, std::string texture_filename) {
		// Check if the texture exists
		std::map<std::string, Texture>::iterator iter = textures.find(texture_name);
		if (iter != textures.end()) return;

		// Insert texture to the map
		Texture texture;
		texture.load(core, texture_filename);
		textures.insert({ texture_name, texture });
	}

	int findTexture(std::string texture_name) {
		return textures[texture_name].heapOffset;
	}
};