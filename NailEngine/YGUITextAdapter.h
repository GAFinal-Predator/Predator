#pragma once
#include <unordered_map>
#include <DirectXMath.h>
#include "YunuGraphicsInterface.h"
#include "YGRenderableAdapter.h"

namespace yunuGIAdapter
{
    class UITextAdapter : public yunuGIAdapter::RenderableAdapter, public yunuGI::IUIText
    {
    public:
        UITextAdapter() :RenderableAdapter() { }
        virtual void SetWorldTM(const yunuGI::Matrix4x4& worldTM) { };
        virtual void SetActive(bool isActive) { };
        virtual void SetText(TCHAR* text, ...) override {};
        virtual const std::string GetText() override { return std::string{}; };
        virtual void SetScreenSpace() {}
        virtual void SetWorldSpace() {}
        virtual void SetPickingMode(bool isPickingModeOn) {}
    private:
    };
}
