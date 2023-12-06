#pragma once

#include <Windows.h>
#include <wrl.h>
#include <memory>
#include <vector>

#include "Utils.h"

class Device;
class SwapChain;
class ConstantBuffer;
class RenderTargetGroup;

class NailEngine
{
public:
	static LazyObjects<NailEngine> Instance;
	friend LazyObjects<NailEngine>;

public:
	void Init(UINT64 hWnd);
	void Render();

public:
	void SetResolution(unsigned int width, unsigned int height);

	std::shared_ptr<ConstantBuffer>& GetConstantBuffer(unsigned int index);
	std::vector<std::shared_ptr<RenderTargetGroup>>& GetRenderTargetGroup() { return this->renderTargetGroup; }
private:
	void CreateConstantBuffer();
	void CreateRenderTargetGroup();

private:
	HWND hWnd;
	unsigned int width;
	unsigned int height;

	std::shared_ptr<Device> device;
	std::shared_ptr<SwapChain> swapChain;

	std::vector<std::shared_ptr<ConstantBuffer>> constantBuffers;
	std::vector<std::shared_ptr<RenderTargetGroup>> renderTargetGroup;
};

