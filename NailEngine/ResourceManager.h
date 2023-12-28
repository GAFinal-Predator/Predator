#pragma once

#include "YunuGraphicsInterface.h"

#include "Utils.h"
#include "Struct.h"
#include "ModelData.h"

#include <wrl.h>
#include <memory>
#include <unordered_map>
#include <map>
#include <array>
#include <d3d11.h>


#include "ModelLoader.h"

class Shader;
class Mesh;
class Texture;
class Material;
class Animation;
class AnimationGroup;


class ResourceManager
{
public:
	static LazyObjects<ResourceManager> Instance;
	friend LazyObjects<ResourceManager>;

#pragma region Create
public:
	void CreateDefaultResource();
	void CreateShader(const std::wstring& shaderPath);
	yunuGI::IMaterial* CrateMaterial(std::wstring materialName);
	Material* CreateInstanceMaterial(const Material* material);
	void CreateTexture(const std::wstring& texturePath);
	std::shared_ptr<Texture>& CreateTexture(const std::wstring& texturePath, unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int bindFlag);
	std::shared_ptr<Texture>& CreateTextureFromResource(const std::wstring& texturePath, Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D);

	void LoadFBX(const char* filePath);
	void CreateAnimation(const std::vector<AnimationClip>& animationClip, const std::wstring& fbxName);

private:
	void CreateMesh(const std::wstring& mesh);
	void CreateMesh(const std::shared_ptr<Mesh>& mesh);
#pragma endregion

public:
	void PushFBXBoneInfo(const std::wstring fbxName, std::map<std::wstring, BoneInfo>& boneInfoMap);
	void PushFBXNode(const std::wstring fbxName, FBXNode* fbxNode);

#pragma region Getter
	std::shared_ptr<yunuGI::IMaterial> GetMaterial(const std::wstring& materialName);
	std::shared_ptr<yunuGI::IShader> GetShader(const std::wstring& shaderPath);
	std::shared_ptr<yunuGI::IShader> GetShader(const yunuGI::IShader* shader);
	std::shared_ptr<yunuGI::IShader> GetDeferredShader(const std::wstring& shaderPath);
	std::shared_ptr<Mesh> GetMesh(const std::wstring& meshName);
	std::shared_ptr<Texture> GetTexture(const std::wstring& textureName);
	yunuGI::FBXData* GetFBXData(const std::string fbxName);
	///std::vector<FBXBoneInfo>& GetFBXBoneData(const std::string fbxName);
	yunuGI::BoneInfo& GetBoneData(const std::string fbxName);
	std::map<std::wstring, BoneInfo>& GetFBXBoneData(const std::string fbxName);
	FBXNode* GetFBXNode(const std::wstring& fbxName);

	std::vector<yunuGI::IMesh*>& GetMeshList() { return this->meshVec; };
	std::vector<yunuGI::ITexture*> GetTextureList() { return this->textureVec; };
	std::vector<yunuGI::IMaterial*>& GetMaterialList() 
	{
		return this->materialVec; 
	};
	std::vector<yunuGI::IShader*>& GetShaderList() { return this->shaderVec; };
	std::vector<yunuGI::IAnimation*>& GetAnimationList() { return this->animationVec; };
	std::shared_ptr<AnimationGroup> GetAnimationGroup(const std::wstring& modelName);
#pragma endregion

private:
	void CreateDeferredShader(const std::wstring& shaderPath);
	void CreateDefaultShader();
	void CreateDefaultMesh();
	void CreateDefaultMaterial();
	void CreateDefaultTexture();

	void FillFBXData(const std::wstring& fbxName,FBXNode* node, yunuGI::FBXData* fbxData);
	void FillFBXBoneInfoVec(const yunuGI::BoneInfo& boneInfo, std::vector<yunuGI::BoneInfo>& boneInfoVec);
	//void CreateResourceFromFBX(FBXMeshData& meshData, std::vector<yunuGI::FBXData>& dataVec, yunuGI::FBXData& fbxData);
	

#pragma region LoadMesh
	void LoadCubeMesh();
	void LoadSphereMesh();
	void LoadRactangleMesh();
	void LoadPointMesh();
	void LoadLineMesh();
	void LoadCapsuleMesh();
	void LoadCylinderMesh();
#pragma endregion



private:
	// Mesh ����
	std::vector<yunuGI::IMesh*> meshVec;
	std::unordered_map<std::wstring, std::shared_ptr<yunuGI::IMesh>> meshMap;

	// Texture ����
	std::vector<yunuGI::ITexture*> textureVec;
	std::unordered_map<std::wstring, std::shared_ptr<yunuGI::ITexture>> textureMap;
	std::unordered_map<std::wstring, std::shared_ptr<yunuGI::ITexture>> deferredTextureMap;

	// Material ����
	std::vector<yunuGI::IMaterial*> materialVec;
	std::unordered_map<std::wstring, std::shared_ptr<yunuGI::IMaterial>> materialMap;
	// Graphics ���ο��� ��� �� Material
	std::unordered_map<std::wstring, std::shared_ptr<yunuGI::IMaterial>> instanceMaterialMap;
	std::unordered_map<std::wstring, std::shared_ptr<yunuGI::IMaterial>> deferredMaterialMap;

	// Shader ����
	std::vector<yunuGI::IShader*> shaderVec;
	std::unordered_map<std::wstring, std::shared_ptr<yunuGI::IShader>> shaderMap;
	std::unordered_map<std::wstring, std::shared_ptr<yunuGI::IShader>> deferredShaderMap;

	// Animation ����
	std::vector<yunuGI::IAnimation*> animationVec;
	std::unordered_map<std::wstring, std::shared_ptr<yunuGI::IAnimation>> animationMap;
	// �׷��Ƚ����� �ִϸ��̼��� �ؽ�ó�� Ȱ���ϰ� �; ���� �����̳�
	std::unordered_map<std::wstring, std::shared_ptr<AnimationGroup>> animationGroupMap;

	std::unordered_map<std::wstring, yunuGI::FBXData*> fbxDataMap;
	//// ���� �������� �� ���������� ������Ʈ ���� �� ���¿�
	///std::unordered_map<std::wstring, std::vector<yunuGI::BoneInfo>> fbxBoneInfoVecMap;
	// �׷��Ƚ� ���� ���ο��� ��Ű�� �ִϸ��̼ǿ� �� ������ ����� ������ �ִ� �� ����
	std::unordered_map<std::wstring, std::map<std::wstring,BoneInfo>> fbxBoneInfoMap;
	std::unordered_map<std::wstring, FBXNode*> fbxNodeMap;


	//std::unordered_map<std::wstring, std::shared_ptr<Shader>> shaderMap;
	//std::unordered_map<std::wstring, std::shared_ptr<Texture>> textureMap;
	//std::unordered_map<std::wstring, std::shared_ptr<Mesh>> meshMap;
	//std::unordered_map<std::wstring, std::shared_ptr<yunuGIAdapter::MaterialAdapter>> materialMap;
	//std::unordered_map<std::wstring, std::shared_ptr<yunuGIAdapter::MaterialAdapter>> instanceMaterialMap;
	
	//std::unordered_map<std::wstring, yunuGI::BoneInfo> fbxBoneInfoTreeMap;

	//// �׷��Ƚ����� ��Ű���� �� ���� offset matrix�� ������ �ִ� �� ����
	//std::unordered_map<std::wstring, std::vector<FBXBoneInfo>> fbxBonOffsetVecMap;
};

