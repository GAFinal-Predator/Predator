#pragma once

#include "YunuGIMatrix4x4.h"
#include "YunuGIColor.h"
#include "IUnknown.h"

namespace yunuGI
{
    class IRenderable : virtual public yunuGI::IUnknown
    {
    public:
        virtual void SetWorldTM(const Matrix4x4& tm) =0;
        virtual void SetActive(bool isActive) =0;

        // �Ʒ� �Լ��� ���� ���� ������� ������ �°� ������ �ؾ���.
        virtual void SetColor(int materialIndex, yunuGI::Color color) =0;
        virtual void SetShader(int materialIndex, std::wstring shaderKey) =0;
        virtual void SetMaterialName(int materialIndex, std::wstring materialKey) =0;
    };
}
