#pragma once

#include "Utils.h"

#include "FBXLoader.h"

#include <DirectXMath.h>
#include "SimpleMath.h"
using namespace DirectX::PackedVector;

#include <vector>

class Mesh;
class Material;
class Animation;

struct RenderInfo
{
	Mesh* mesh;
	Material* material;
	unsigned int materialIndex;
	DirectX::SimpleMath::Matrix wtm;
};

struct SkinnedRenderInfo
{
	RenderInfo renderInfo;
	std::wstring boneName;
	int animatorIndex;
};


class RenderSystem
{
public:
	static LazyObjects<RenderSystem> Instance;
	friend LazyObjects<RenderSystem>;

public:
	void ClearRenderInfo();
	void SortObject();
	void PushLightData();
	void PushCameraData();
	void Render();
	void RenderObject();
	void RenderSkinned();
	void RenderLight();
	void DrawFinal();

	void RenderForward();

	void DrawDeferredInfo();

private:
	void BoneUpdate(const SkinnedRenderInfo& skinnedRenderInfo);
	void ReadBone(FBXNode fbxNode, DirectX::SimpleMath::Matrix parentMatrix, std::wstring fbxName,
		const std::unordered_map<std::wstring, std::pair<FBXNode, std::unordered_map<std::wstring, FBXBoneInfo>>>& fbxNodeAndBoneMap);

private:
	std::vector<RenderInfo> deferredVec;
	std::vector<RenderInfo> forwardVec;
	std::vector<SkinnedRenderInfo> skinnedVec;

	//DirectX::SimpleMath::Matrix temp[256];
	BoneMatrix finalTM;

	






	std::map < std::wstring, DirectX::SimpleMath::Matrix> tempMap;
};



