#include "ModelLoader.h"

#include <filesystem>
#include <iostream>

#include "Animation.h"

#include "ResourceManager.h"

LazyObjects<ModelLoader> ModelLoader::Instance;

FBXNode* ModelLoader::LoadModel(const char* filePath)
{
	this->texturePath = std::filesystem::path(filePath).parent_path().wstring()
		+ L"/" + std::filesystem::path(filePath).stem().wstring() + L".fbm/";

	// Assimp Importer ��ü ����
	Assimp::Importer importer;

	// Load Flag
	unsigned int flag = aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
		aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
		aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes |
		aiProcess_SortByPType | aiProcess_LimitBoneWeights;

	const aiScene* scene = importer.ReadFile(filePath, flag);

	if (!scene) {
		assert(FALSE);
	}

	FBXNode* fbxNode = new FBXNode;
	ParseNode(scene->mRootNode, scene, fbxNode, this->ConvertToCloumnMajor(scene->mRootNode->mTransformation));

	FillVertexBoneIndexAndWeight(scene, scene->mRootNode, fbxNode);

	ResourceManager::Instance.Get().PushFBXBoneInfo(std::filesystem::path(filePath).stem().wstring(), this->boneInfoMap);

	ResourceManager::Instance.Get().PushFBXNode(std::filesystem::path(filePath).stem().wstring(), fbxNode);

	if (scene->HasAnimations())
	{
		AddHasAnimation(fbxNode);
		LoadAnimation(scene, std::filesystem::path(filePath).stem().wstring());
	}

	///LoadAnimation(scene, std::filesystem::path(filePath).stem().wstring());

	boneInfoMap.clear();
	animationClipVec.clear();

	return fbxNode;
}

void ModelLoader::ParseNode(const aiNode* node, const aiScene* scene, FBXNode* fbxNode, DirectX::SimpleMath::Matrix parentMatrix)
{
	fbxNode->nodeName = this->aiStringToWString(node->mName);
	fbxNode->transformMatrix = this->ConvertToCloumnMajor(node->mTransformation);
	//fbxNode->worldMatrix = parentMatrix * fbxNode->transformMatrix;
	fbxNode->worldMatrix = fbxNode->transformMatrix * parentMatrix;
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		FBXMeshData fbxMeshData;

		unsigned int meshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[meshIndex];

		fbxMeshData.meshName = this->aiStringToWString(mesh->mName);

		for (int j = 0; j < mesh->mNumVertices; ++j)
		{
			aiVector3D vertexPos = mesh->mVertices[j];
			DirectX::SimpleMath::Vector3 dvertex{ vertexPos.x,vertexPos.y,vertexPos.z };

			aiVector3D uv = mesh->mTextureCoords[0][j];
			DirectX::SimpleMath::Vector2 duv{ uv.x, uv.y };

			aiVector3D normal = mesh->mNormals[j];
			DirectX::SimpleMath::Vector3 dnormal{ normal.x, normal.y, normal.z };

			Vertex vertex = { dvertex, DirectX::SimpleMath::Vector4{1.f,1.f,1.f,1.f}, duv, dnormal, DirectX::SimpleMath::Vector3{1.f,0.f,0.f} };

			fbxMeshData.vertex.emplace_back(vertex);
		}

		for (int j = 0; j < mesh->mNumFaces; ++j)
		{
			aiFace face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; ++k)
			{
				fbxMeshData.indices.emplace_back(face.mIndices[k]);
			}
		}

		ParseMaterial(scene, mesh, fbxMeshData);

		unsigned int boneIndex = 0;

		for (int b = 0; b < mesh->mNumBones; ++b)
		{
			BoneInfo boneInfo;
			boneInfo.offset = this->ConvertToCloumnMajor(mesh->mBones[b]->mOffsetMatrix);
			//boneInfo.index = boneIndex++;
			for (int w = 0; w < mesh->mBones[b]->mNumWeights; ++w)
			{
				aiBone* bone = mesh->mBones[b];
				std::wstring boneName = this->aiStringToWString(bone->mName);

				this->boneInfoMap.insert({ boneName, boneInfo });
			}
		}

		for (auto& i : this->boneInfoMap)
		{
			i.second.index = boneIndex++;
		}

		fbxNode->meshVec.emplace_back(fbxMeshData);
	}

	fbxNode->child.resize(node->mNumChildren);
	for (int i = 0; i < node->mNumChildren; ++i)
	{
		FBXNode* fbxChildNode = new FBXNode;
		fbxNode->child[i] = fbxChildNode;
		ParseNode(node->mChildren[i], scene, fbxChildNode, fbxNode->worldMatrix);
	}
}


void ModelLoader::ParseMaterial(const aiScene* scene, const aiMesh* mesh, FBXMeshData& fbxMeshData)
{
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// Material Name
	fbxMeshData.material.materialName = aiStringToWString(material->GetName());

	// Albedo Texture
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			std::wstring _path = aiStringToWString(path);
			std::filesystem::path pathName(_path);
			pathName.filename().wstring();

			fbxMeshData.material.albedoMap = this->texturePath + pathName.filename().wstring();
		}
	}

	// Normal Texture
	if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
	{
		aiString path;
		if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
		{
			std::wstring _path = aiStringToWString(path);
			std::filesystem::path pathName(_path);
			pathName.filename().wstring();

			fbxMeshData.material.normalMap = this->texturePath + pathName.filename().wstring();
		}
	}
}

void ModelLoader::AddHasAnimation(FBXNode* fbxNode)
{
	fbxNode->hasAnimation = true;

	for (int i = 0; i < fbxNode->child.size(); ++i)
	{
		AddHasAnimation(fbxNode->child[i]);
	}
}

void ModelLoader::LoadAnimation(const aiScene* scene, const std::wstring& fbxName)
{
	for (int i = 0; i < scene->mNumAnimations; ++i)
	{
		aiAnimation* animation = scene->mAnimations[i];

		AnimationClip animationClip;
		animationClip.name = this->aiStringToWString(animation->mName);
		animationClip.duration = animation->mDuration / animation->mTicksPerSecond;
		animationClip.totalFrame = animation->mDuration;

		animationClip.keyFrameInfoVec.resize(this->boneInfoMap.size());

		int maxKeyCount = 0;

		for (int j = 0; j < animation->mNumChannels; ++j)
		{
			aiNodeAnim* nodeAnim = animation->mChannels[j];

			auto iter = this->boneInfoMap.find(this->aiStringToWString(nodeAnim->mNodeName));

			if (iter != this->boneInfoMap.end())
			{
				unsigned int boneIndex = iter->second.index;

				animationClip.keyFrameInfoVec[boneIndex].resize(nodeAnim->mNumPositionKeys);

				if (nodeAnim->mNumPositionKeys >= maxKeyCount)
				{
					maxKeyCount = nodeAnim->mNumPositionKeys;
				}

				for (int k = 0; k < nodeAnim->mNumPositionKeys; ++k)
				{
					aiVectorKey vectorKey = nodeAnim->mPositionKeys[k];

					animationClip.keyFrameInfoVec[boneIndex][k].pos = DirectX::SimpleMath::Vector3{ vectorKey.mValue.x, vectorKey.mValue.y, vectorKey.mValue.z};
				}

				for (int k = 0; k < nodeAnim->mNumRotationKeys; ++k)
				{
					aiQuatKey quatKey = nodeAnim->mRotationKeys[k];

					animationClip.keyFrameInfoVec[boneIndex][k].rot = DirectX::SimpleMath::Quaternion{ quatKey.mValue.x, quatKey.mValue.y, quatKey.mValue.z, quatKey.mValue.w };
				}

				for (int k = 0; k < nodeAnim->mNumScalingKeys; ++k)
				{
					aiVectorKey vectorKey = nodeAnim->mScalingKeys[k];

					animationClip.keyFrameInfoVec[boneIndex][k].scale = DirectX::SimpleMath::Vector3{ vectorKey.mValue.x,vectorKey.mValue.y, vectorKey.mValue.z };
				}
			}
		}

		for (int r = 0; r < animationClip.keyFrameInfoVec.size(); ++r)
		{
			KeyFrameInfo keyFrameInfo = animationClip.keyFrameInfoVec[r].back();
			animationClip.keyFrameInfoVec[r].resize(maxKeyCount, keyFrameInfo);
		}

		this->animationClipVec.emplace_back(animationClip);
	}

	ResourceManager::Instance.Get().CreateAnimation(this->animationClipVec, fbxName);
}

void ModelLoader::FillVertexBoneIndexAndWeight(const aiScene* scene, const aiNode* node, FBXNode* fbxNode)
{
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		unsigned int meshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[meshIndex];

		for (int b = 0; b < mesh->mNumBones; b++)
		{
			for (int w = 0; w < mesh->mBones[b]->mNumWeights; ++w)
			{
				auto iter = this->boneInfoMap.find(this->aiStringToWString(mesh->mBones[b]->mName));

				if (iter != this->boneInfoMap.end())
				{
					fbxNode->meshVec[i].vertex[mesh->mBones[b]->mWeights[w].mVertexId].FillBoneIndexWeight(
						iter->second.index,
						mesh->mBones[b]->mWeights[w].mWeight
					);
				}
			}
		}
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		FillVertexBoneIndexAndWeight(scene, node->mChildren[i], fbxNode->child[i]);
	}
}

std::wstring ModelLoader::aiStringToWString(const aiString& str)
{
	std::string tempStr = str.C_Str();
	return std::wstring{ tempStr.begin(), tempStr.end() };
}

DirectX::SimpleMath::Matrix ModelLoader::ConvertToCloumnMajor(const aiMatrix4x4& matrix)
{
	DirectX::SimpleMath::Matrix colMajor{
		matrix.a1, matrix.b1, matrix.c1,matrix.d1,
		matrix.a2, matrix.b2, matrix.c2,matrix.d2,
		matrix.a3, matrix.b3, matrix.c3,matrix.d3,
		matrix.a4, matrix.b4, matrix.c4,matrix.d4,
	};

	return colMajor;
}
