#pragma once

#include "ITexture.h"

#include "Resource.h"

#include <string>
#include <wrl.h>

#include <d3d11.h>

class Texture : public yunuGI::ITexture, public Resource
{
public:
	void LoadTexture(const std::wstring& texturePath);
	void Bind(unsigned int slot);
	void UnBind(unsigned int slot);

	void CreateFromResource(Microsoft::WRL::ComPtr<ID3D11Texture2D>& tex2D);
	void CreateTexture(const std::wstring& texturePath, unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int bindFlag);

#pragma region Getter
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetRTV() { return this->RTV; }
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetSRV() { return this->SRV; }
#pragma endregion

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
};

