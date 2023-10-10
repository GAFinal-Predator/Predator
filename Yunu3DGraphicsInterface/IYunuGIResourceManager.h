#pragma once
#include <vector>
#include <string>
#include "IUnknown.h"

#include "IMaterial.h"

// ���ҽ� �Ŵ����� �ؽ�ó, �޽ÿ� ���� �׷��Ƚ� ���ҽ����� ��������� �ҷ����ų� �Ҵ������ϰ� ���� �� ����Ѵ�.
namespace yunuGI
{
    struct IResourceManagerDesc
    {

    };
    class IResourceManager : public IUnknown
    {
    public:
        virtual std::vector<std::string> GetMeshKeys() const = 0;
        virtual std::vector<std::string> GetSkeletonKeys()const = 0;
        virtual std::vector<std::string> GetTextureKeys()const = 0;
        virtual std::vector<std::string> AnimationKeys()const = 0;
        virtual void LoadFile(const char* filePath)const = 0;
        virtual yunuGI::IMaterial* CreateMaterial(yunuGI::MaterialDesc& materialDesc)const = 0;
        virtual void UnloadResources()const = 0;
    };
}

