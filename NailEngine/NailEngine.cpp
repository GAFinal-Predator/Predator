#include "NailEngine.h"

#include <assert.h>

#include "ResourceBuilder.h"
#include "Device.h"
#include "SwapChain.h"
#include "ConstantBuffer.h"

#include "Struct.h"
#include "ResourceManager.h"

#include "RenderSystem.h"

LazyObjects<NailEngine> NailEngine::Instance;

void NailEngine::Init(UINT64 hWnd)
{
	this->hWnd = reinterpret_cast<HWND>(hWnd);

	this->device = std::make_shared<Device>();
	this->device->Init();
	ResourceBuilder::Instance.Get().device = this->device;

	this->swapChain = std::make_shared<SwapChain>();
	this->swapChain->Init(this->hWnd, this->width, this->height);
	ResourceBuilder::Instance.Get().swapChain = this->swapChain;

	ResourceManager::Instance.Get().CreateDefaultResource();
	CreateConstantBuffer();
}

void NailEngine::Render()
{
	
	// Begin
	ResourceBuilder::Instance.Get().device->GetDeviceContext().Get()->RSSetViewports(1, &ResourceBuilder::Instance.Get().swapChain->GetViewPort());

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

	depthStencilDesc.DepthEnable = true; // ���� ���� ��� ����
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // ���� ���� ������ ����
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS; // ���� ���� ���ϴ� �Լ�

	depthStencilDesc.StencilEnable = false; // ���ٽ� ���� ��� ����
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK; // ���ٽ� ���� ���� ���� ����ũ
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK; // ���ٽ� ���� �� ���� ����ũ

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; // ���ٽ� �׽�Ʈ�� �����ϸ� ������ ����
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; // ���ٽ� �׽�Ʈ�� ���������� ���� �׽�Ʈ�� ������ ��쿡 ������ ����
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; // ���ٽ� �׽�Ʈ�� ���� �׽�Ʈ ��� ������ ��쿡 ������ ����
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // ���ٽ� �׽�Ʈ�� ����� �Լ�

	depthStencilDesc.BackFace = depthStencilDesc.FrontFace; // ��� ���ٽ��� 

	ID3D11DepthStencilState* pDepthStencilState;
	HRESULT hr = ResourceBuilder::Instance.Get().device->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);

	ResourceBuilder::Instance.Get().device->GetDeviceContext().Get()->OMSetDepthStencilState(pDepthStencilState, 1);

	const float red[] = { 1.f, 0.f, 0.f, 1.f };

	// ���� Ÿ�� �並 ���ε��ϰ�, ���� ���ٽ� ���۸� ��� ���ձ� �ܰ迡 ���ε��Ѵ�.
	ResourceBuilder::Instance.Get().device->GetDeviceContext().Get()->OMSetRenderTargets(1,
		ResourceBuilder::Instance.Get().swapChain->GetRTV().GetAddressOf(),
		ResourceBuilder::Instance.Get().swapChain->GetDSV().Get());

	// ���� Ÿ�ٺ並 ���� ������ ������ �ȼ��� �� �ʱ�ȭ�Ͽ� �����.
	ResourceBuilder::Instance.Get().device->GetDeviceContext().Get()->ClearRenderTargetView(ResourceBuilder::Instance.Get().swapChain->GetRTV().Get(), red);
	// ���� ���ٽǺ並 �ʱ�ȭ�Ѵ�.
	ResourceBuilder::Instance.Get().device->GetDeviceContext().Get()->ClearDepthStencilView(ResourceBuilder::Instance.Get().swapChain->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	ResourceBuilder::Instance.Get().device->GetDeviceContext().Get()->RSSetState(0);

	RenderSystem::Instance.Get().RenderObject();

	// End
	ResourceBuilder::Instance.Get().swapChain->GetSwapChain().Get()->Present(1, 0);

	for (auto& iter : this->constantBuffers)
	{
		iter->Clear();
	}
}

void NailEngine::SetResolution(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
}

std::shared_ptr<ConstantBuffer>& NailEngine::GetConstantBuffer(unsigned int index)
{
	return this->constantBuffers[index];
}

void NailEngine::CreateConstantBuffer()
{
	{
		std::shared_ptr<ConstantBuffer> _constantBuffer = std::make_shared<ConstantBuffer>();
		_constantBuffer->CraeteConstantBuffer(sizeof(MatrixBuffer), 256);
		this->constantBuffers.emplace_back(_constantBuffer);
	}

	{
		std::shared_ptr<ConstantBuffer> _constantBuffer = std::make_shared<ConstantBuffer>();
		_constantBuffer->CraeteConstantBuffer(sizeof(MaterialBuffer), 256);
		this->constantBuffers.emplace_back(_constantBuffer);
	}
}
