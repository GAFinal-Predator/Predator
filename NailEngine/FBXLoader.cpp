#include "FBXLoader.h"

#include <iostream>

#include "Mesh.h"

#include <codecvt>
#include <locale>
#include <filesystem>


LazyObjects<FBXLoader> FBXLoader::Instance;

FBXNode FBXLoader::LoadFBXData(const char* filePath)
{
	this->texturePath = std::filesystem::path(filePath).parent_path().wstring() 
		+ L"/" + std::filesystem::path(filePath).stem().wstring() + L".fbm/";

	// Assimp Importer ��ü ����
	Assimp::Importer importer;

	// Load Flag
	unsigned int flag;
	flag = aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals |
		aiProcess_MakeLeftHanded |
		aiProcess_FlipWindingOrder |
		aiProcess_FlipUVs |
		aiProcess_SplitLargeMeshes |
		aiProcess_PreTransformVertices |
		aiProcess_MakeLeftHanded;

	const aiScene* scene = importer.ReadFile(filePath, flag);

	if (!scene) {
		std::cerr << "�� �ε� ����!" << std::endl;
	}



	// Mesh Data Load
	FBXNode rootNode;
	ParseNode(scene->mRootNode, scene, rootNode);

	return rootNode;
}

void FBXLoader::ParseNode(const aiNode* node, const aiScene* scene, FBXNode& fbxNode)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) 
	{
		FBXMeshData data;

		unsigned int meshIndex = node->mMeshes[i];
		const aiMesh* mesh = scene->mMeshes[meshIndex];

		std::wstring meshName = aiStringToWString(mesh->mName.C_Str());

		data.meshName = meshName;
		for (int j = 0; j < mesh->mNumVertices; ++j)
		{
			// ���ؽ� ��ġ (Position)
			aiVector3D vertex = mesh->mVertices[j];
			DirectX::SimpleMath::Vector3 dvertex{ vertex.x,vertex.y,vertex.z};

			// ���ؽ� ���� (Color) - �밳 ������ ����
			///aiColor4D color = mesh->mColors[0][i]; // ù ��° ���� ä��

			// UV ��ǥ (Texture Coordinates)
			aiVector3D uv = mesh->mTextureCoords[0][j]; // ù ��° UV ä��
			DirectX::SimpleMath::Vector2 duv{ uv.x, uv.y };

			// ���� (Normal)
			aiVector3D normal = mesh->mNormals[j];
			DirectX::SimpleMath::Vector3 dnormal{ normal.x, normal.y, normal.z };

			// ź��Ʈ (Tangent)
			///aiVector3D tangent = mesh->mTangents[i];

			data.vertex.emplace_back(Vertex{ dvertex , DirectX::SimpleMath::Vector4(1.f,1.f,1.f,1.f), duv, dnormal, DirectX::SimpleMath::Vector3(1.f,0.f,0.f) });
		}

		/// �ε��� ä���
		for (int j = 0; j < mesh->mNumFaces; ++j)
		{
			aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; ++k)
			{
				data.indices.emplace_back(face.mIndices[k]);
			}
		}

		// Material Load
		ParseMaterial(scene, mesh, data);

		fbxNode.meshVec.emplace_back(data);
	}

	// Parse Child
	fbxNode.child.resize(node->mNumChildren);
	for (unsigned int i = 0; i < node->mNumChildren; ++i) 
	{
		ParseNode(node->mChildren[i], scene, fbxNode.child[i]);
	}
}

void FBXLoader::ParseMaterial(const aiScene* scene, const aiMesh* mesh, FBXMeshData& meshData)
{
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// Material Name
	meshData.material.materialName = aiStringToWString(material->GetName().C_Str());

	// Albedo Texture
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			meshData.material.albedoMap = this->texturePath + aiStringToWString(path.C_Str());
		}
	}
}

std::wstring FBXLoader::aiStringToWString(std::string str)
{
	std::wstring wstr;
	for (char c : str) {
		wstr += static_cast<wchar_t>(c);
	}
	return wstr;
}

DirectX::SimpleMath::Matrix FBXLoader::ConvertToCloumnMajor(aiMatrix4x4 matrix)
{
	DirectX::SimpleMath::Matrix colMajor{
		matrix.a1, matrix.b1, matrix.c1,matrix.d1,
		matrix.a2, matrix.b2, matrix.c2,matrix.d2,
		matrix.a3, matrix.b3, matrix.c3,matrix.d3,
		matrix.a4, matrix.b4, matrix.c4,matrix.d4,
	};

	return colMajor;
}
