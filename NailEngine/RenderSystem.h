#pragma once

#include "Utils.h"
#include "Struct.h"
#include "ModelData.h"

#include "DXTK/SpriteBatch.h"
#include "DXTK/CommonStates.h"

#include <DirectXMath.h>
#include "SimpleMath.h"
using namespace DirectX::PackedVector;



#include "IShader.h"
#include "Material.h"

#include "UIImage.h"
#include "UIText.h"

#include <memory>
#include <vector>
#include <unordered_set>
#include <set>
#include <map>

/// D2D 관련 헤더
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib,"d2d1")
#pragma comment(lib, "dwrite")


class Mesh;
class Material;
class yunuGI::IShader;
class Animation;
class NailAnimator;

class RenderSystem
{
public:
	static LazyObjects<RenderSystem> Instance;
	friend LazyObjects<RenderSystem>;
	
	void Finalize();

public:
	void Init();
	void CreateD2D();

	void Render();
	void PushLightData();
	void PushCameraData();
	void RenderObject();
	void RenderSkinned();
	void RenderShadow();
	void RenderPointLightShadow();
	void RenderLight();
	void RenderFinal();
	void RenderBackBuffer();
	void RenderUI();

	void RenderForward();

	void DrawDeferredInfo();

public:
	void PushStaticRenderableObject(nail::IRenderable* renderable);
	void PopStaticRenderableObject(nail::IRenderable* renderable);

	void PushSkinnedRenderableObject(nail::IRenderable* renderable);
	void PopSkinnedRenderableObject(nail::IRenderable* renderable);

	void PushUIObject(std::shared_ptr<nail::IRenderable> renderable);
	void PopUIObject(std::shared_ptr<nail::IRenderable> renderable);

	void PushTextObject(std::shared_ptr<nail::IRenderable> renderable);
	void PopTextObject(std::shared_ptr<nail::IRenderable> renderable);

	void ReSortUIObject(int layer, std::shared_ptr<nail::IRenderable> ui);

	void ReSortRenderInfo(nail::IRenderable* renderable, int index);

	void RegisterRenderInfo(nail::IRenderable* renderable, std::shared_ptr<RenderInfo> renderInfo);
	void RegisterSkinnedRenderInfo(nail::IRenderable* renderable, std::shared_ptr<SkinnedRenderInfo> renderInfo);

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> QueryBrush(std::shared_ptr<UIText> uiText);
	Microsoft::WRL::ComPtr<IDWriteTextFormat> QueryTextFormat(std::shared_ptr<UIText> uiText);

private:
	std::set<std::shared_ptr<RenderInfo>> deferredSet;
	std::set<std::shared_ptr<RenderInfo>> forwardSet;
	std::set<std::shared_ptr<SkinnedRenderInfo>> skinnedSet;
	
	BoneMatrix finalTM;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::CommonStates> commonStates;

	yunuGI::IShader* ps = nullptr;
	yunuGI::IShader* vs = nullptr;

private:
	//std::unordered_set<std::shared_ptr<IRenderable>> staticRenderableSet;

	std::map<nail::IRenderable*, std::vector<std::shared_ptr<RenderInfo>>> staticMeshRenderInfoMap;
	std::map<nail::IRenderable*, std::vector<std::shared_ptr<SkinnedRenderInfo>>> skinnedMeshRenderInfoMap;

	std::multiset<std::shared_ptr<nail::IRenderable>, CompareSmartPtr> UIImageSet;
	std::set<std::shared_ptr<nail::IRenderable>> UITextSet;

	/// D2D 관련 멤버 변수
	Microsoft::WRL::ComPtr<ID2D1Factory> d2dFactory;
	Microsoft::WRL::ComPtr<IDWriteFactory> wFactory;
	Microsoft::WRL::ComPtr<IDXGISurface> surface;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> d2dRT;

	std::unordered_map<yunuGI::Color, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> brushMap;
	std::map<std::wstring, Microsoft::WRL::ComPtr<IDWriteTextFormat>> wFormatMap;

	DirectX::SimpleMath::Vector3 mainCamPos;
};



