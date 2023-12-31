#include "PixelShader.h"

#include <regex>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <assert.h>

#include <d3d11shader.h>

#include "ResourceBuilder.h"
#include "Device.h"

void PixelShader::CreateShader(const std::wstring& shaderPath)
{
	unsigned int _compileFlag = 0;
#ifdef _DEBUG
	_compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	::D3DReadFileToBlob(shaderPath.c_str(), psBuffer.GetAddressOf());

	//if (FAILED(::D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
	//	, "main", "ps_5_0", _compileFlag, NULL, psBuffer.GetAddressOf(), errorMSG.GetAddressOf())))
	//{
	//	if (errorMSG)
	//	{
	//		::MessageBoxA(nullptr, "Failed to compile shader!", nullptr, MB_OK);
	//		assert(true);
	//	}
	//	else
	//	{
	//		::MessageBoxA(nullptr, "Shader path not found!", nullptr, MB_OK);
	//		assert(true);
	//	}
	//}

	if (FAILED(ResourceBuilder::Instance.Get().device->GetDevice()->CreatePixelShader(
		psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(),
		nullptr,
		ps.GetAddressOf())))
	{
		::MessageBoxA(nullptr, "Failed to create pixel shader!", nullptr, MB_OK);
		assert(true);
	}

	std::filesystem::path filePath(shaderPath);
	std::wstring fileName = filePath.stem().wstring();
	std::wstring hlslPath{L"Shader/"};
	hlslPath = hlslPath + fileName + L".hlsl";

	//CreateRasterizerState();
	CreateShaderState(hlslPath);
	CreateSamplerState();
}

void PixelShader::Bind()
{
	ResourceBuilder::Instance.Get().device->GetDeviceContext()->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());

	ResourceBuilder::Instance.Get().device->GetDeviceContext()->RSSetState(this->rasterizerState.Get());

	ResourceBuilder::Instance.Get().device->GetDeviceContext()->OMSetDepthStencilState(this->depthStencilState.Get(), 1);

	ResourceBuilder::Instance.Get().device->GetDeviceContext()->PSSetShader(ps.Get(), nullptr, 0);

	ResourceBuilder::Instance.Get().device->GetDeviceContext()->OMSetBlendState(this->blendState.Get(), nullptr, 0xFFFFFFFF);
}

void PixelShader::UnBind()
{
	ResourceBuilder::Instance.Get().device->GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);
}

//void PixelShader::CreateRasterizerState()
//{
//	//ID3D11ShaderReflection* pShaderReflection = nullptr;
//
//	//D3DReflect(
//	//	psBuffer->GetBufferPointer(),
//	//	psBuffer->GetBufferSize(),
//	//	IID_ID3D11ShaderReflection,
//	//	(void**)&pShaderReflection
//	//);
//
//	//ID3D11ShaderReflectionVariable* pVariable = pShaderReflection->GetVariableByName("A");
//
//	//if (pVariable) {
//	//	D3D11_SHADER_VARIABLE_DESC varDesc;
//	//	HRESULT hr = pVariable->GetDesc(&varDesc);
//	//}
//}

void PixelShader::CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ResourceBuilder::Instance.Get().device->GetDevice()->CreateSamplerState(&samplerDesc, this->samplerState.GetAddressOf());
}

void PixelShader::CreateShaderState(const std::wstring& shaderPath)
{
	std::ifstream file(shaderPath);

	std::string fileContent((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

	std::regex commentRegex("//.*?ShaderInfo");
	if (!std::regex_search(fileContent, commentRegex))
	{
		// hlsl안에 ShaderInfo라는 주석이 없기에 assert
		assert(FALSE);
	}

	CreateShaderType(fileContent);
	CreateRasterizerState(fileContent);
	CreateDepthStencilState(fileContent);
	CreateBlendState(fileContent);
}

void PixelShader::CreateShaderType(const std::string& fileContent)
{
	std::regex commentRegex("// ShaderType : (\\w+)");
	std::smatch matches;
	if (std::regex_search(fileContent, matches, commentRegex))
	{
		if (matches.size() >= 2)
		{
			std::string shaderType = matches[1].str();
			if (shaderType == "Forward")
			{
				this->shaderInfo.shaderType = yunuGI::ShaderType::Forward;
			}
			else if (shaderType == "Deferred")
			{
				this->shaderInfo.shaderType = yunuGI::ShaderType::Deferred;
			}
			else if (shaderType == "Light")
			{
				this->shaderInfo.shaderType = yunuGI::ShaderType::Light;
			}
			else
			{
				// 뒤에 입력이 없었으니 assert
				assert(FALSE);
			}
		}
		else
		{
			// ShaderType : 이라는 문자열을 hlsl파일 안에 정의하지 않았으니 assert
			assert(FALSE);
		}
	}
}

void PixelShader::CreateRasterizerState(const std::string& fileContent)
{
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.f;

	std::regex commentRegex("// RasterType : (\\w+)");
	std::smatch matches;
	if (std::regex_search(fileContent, matches,commentRegex))
	{
		if (matches.size() >= 2)
		{
			std::string shaderType = matches[1].str();
			if (shaderType == "Solid")
			{
				this->shaderInfo.rasterizer = yunuGI::Rasterizer::Solid;
				rasterDesc.FillMode = D3D11_FILL_SOLID;
			}
			else if (shaderType == "Wireframe")
			{
				this->shaderInfo.rasterizer = yunuGI::Rasterizer::Wire;
				rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
			}
			else
			{
				// 뒤에 입력이 없었으니 assert
				assert(FALSE);
			}
		}
		else
		{
			// RasterType : 이라는 문자열을 hlsl파일 안에 정의하지 않았으니 assert
			assert(FALSE);
		}
	}

	commentRegex = std::regex("// CullType : (\\w+)");
	if (std::regex_search(fileContent, matches, commentRegex))
	{
		if (matches.size() >= 2)
		{
			std::string shaderType = matches[1].str();
			if (shaderType == "CullBack")
			{
				rasterDesc.CullMode = D3D11_CULL_BACK;
			}
			else if (shaderType == "CullFront")
			{
				rasterDesc.CullMode = D3D11_CULL_FRONT;
			}
			else if (shaderType == "CullNone")
			{
				rasterDesc.CullMode = D3D11_CULL_NONE;
			}
			else
			{
				// 뒤에 입력이 없었으니 assert
				assert(FALSE);
			}
		}
		else
		{
			// CullType : 이라는 문자열을 hlsl파일 안에 정의하지 않았으니 assert
			assert(FALSE);
		}
	}

	ResourceBuilder::Instance.Get().device->GetDevice()->CreateRasterizerState(&rasterDesc, this->rasterizerState.GetAddressOf());
}

void PixelShader::CreateDepthStencilState(const std::string& fileContent)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	
	depthStencilDesc.DepthEnable = true; // 깊이 버퍼 사용 여부
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS; // 깊이 값을 비교하는 함수
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 깊이 값을 쓰는지 여부

	depthStencilDesc.StencilEnable = false; // 스텐실 버퍼 사용 여부
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK; // 스텐실 값을 읽을 때의 마스크
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK; // 스텐실 값을 쓸 때의 마스크

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; // 스텐실 테스트에 실패하면 수행할 동작
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; // 스텐실 테스트는 성공했으나 깊이 테스트는 실패한 경우에 수행할 동작
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; // 스텐실 테스트와 깊이 테스트 모두 성공한 경우에 수행할 동작
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 스텐실 테스트에 사용할 함수

	depthStencilDesc.BackFace = depthStencilDesc.FrontFace; // 양면 스텐실을 

	std::regex commentRegex("// DepthType : (\\w+)");
	std::smatch matches;
	if (std::regex_search(fileContent, matches, commentRegex))
	{
		if (matches.size() >= 2)
		{
			std::string shaderType = matches[1].str();
			if (shaderType == "Less")
			{
				depthStencilDesc.DepthEnable = true; 
				depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS; 
				
			}
			else if (shaderType == "LessEqual")
			{
				depthStencilDesc.DepthEnable = true; 
				depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; 
			}
			else if (shaderType == "Greater")
			{
				depthStencilDesc.DepthEnable = true;
				depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
			}
			else if (shaderType == "GreaterEqual")
			{
				depthStencilDesc.DepthEnable = true;
				depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
			}
			else if (shaderType == "NoDepthTest")
			{
				depthStencilDesc.DepthEnable = false;
				depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
			}
			else if (shaderType == "NoDepthTestNoWrite")
			{
				depthStencilDesc.DepthEnable = false;
				//depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
				depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			}
			else if (shaderType == "LessNoWrite")
			{
				depthStencilDesc.DepthEnable = true;
				depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
				depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			}
			else
			{
				// 뒤에 입력이 없었으니 assert
				assert(FALSE);
			}
		}
		else
		{
			// DepthType : 이라는 문자열을 hlsl파일 안에 정의하지 않았으니 assert
			assert(FALSE);
		}
	}

	ResourceBuilder::Instance.Get().device->GetDevice()->CreateDepthStencilState(&depthStencilDesc, this->depthStencilState.GetAddressOf());
}

void PixelShader::CreateBlendState(const std::string& fileContent)
{
	D3D11_BLEND_DESC blendDesc{};

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;

	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	std::regex commentRegex("// BlendType : (\\w+)");
	std::smatch matches;
	if (std::regex_search(fileContent, matches, commentRegex))
	{
		if (matches.size() >= 2)
		{
			std::string shaderType = matches[1].str();
			if (shaderType == "Default")
			{

			}
			else if (shaderType == "AlphaBlend")
			{
				blendDesc.RenderTarget[0].BlendEnable = true;
				blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
				blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
			}
			else
			{
				// 뒤에 입력이 없으니 assert
				assert(FALSE);
			}
		}
		else
		{
			// BlendType : 이라는 문자열을 hlsl파일 안에 정의하지 않았으니 assert
			assert(FALSE);
		}
	}

	ResourceBuilder::Instance.Get().device->GetDevice()->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
}
