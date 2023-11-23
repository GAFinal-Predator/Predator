#pragma once
#include <vector>
#include <string>
#include "IUnknown.h"

#include "IMaterial.h"

#include "FBXData.h"

// ���ҽ� �Ŵ����� �ؽ�ó, �޽ÿ� ���� �׷��Ƚ� ���ҽ����� ��������� �ҷ����ų� �Ҵ������ϰ� ���� �� ����Ѵ�.
namespace yunuGI
{
    struct IResourceManagerDesc
    {

    };
    class IResourceManager : public IUnknown
    {
    public:
        virtual void LoadFile(const char* filePath)const = 0;
        virtual yunuGI::IMaterial* CreateMaterial(std::wstring materialName)const = 0;
        virtual yunuGI::IMaterial* GetMaterial(const std::wstring& materialName)const = 0;
        virtual yunuGI::IMesh* GetMesh(const std::wstring& meshName)const = 0;
        virtual bool GetFBXData(const std::string& fbxName, std::vector<yunuGI::FBXData>& dataVec)const = 0;
        virtual bool GetFBXBoneData(const std::string& fbxName, yunuGI::BoneInfo& boneInfo)const = 0;
        virtual void UnloadResources()const = 0;

        virtual std::vector<IShader*>& GetShaderList()const = 0;
        virtual std::vector<IMesh*>& GetMeshList()const = 0;
        virtual std::vector<IMaterial*>& GetMaterialhList()const = 0;
    };
}

