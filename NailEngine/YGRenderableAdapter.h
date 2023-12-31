#pragma once
#include <unordered_map>
#include "YunuGraphicsInterface.h"
#include "IYunuGIRenderable.h"

namespace yunuGIAdapter
{
	class RenderableAdapter : virtual public yunuGI::IRenderable
	{
	public:
		RenderableAdapter() { }
		static RenderableAdapter* GetWrapper() { }

		virtual void SetColor(int materialIndex, yunuGI::Color color)  {};
		virtual void SetShader(int materialIndex, std::wstring shaderKey)  {};
		virtual void SetMaterialName(int materialIndex, std::wstring materialKey)  {};
		virtual void SetMesh(std::wstring mesh)  {};
		virtual void SetWorldTM(const yunuGI::Matrix4x4& tm) {};
		/*virtual void SetMaterial(yunuGI::IMaterial* material) override {};
		virtual yunuGI::IMaterial* GetMaterial() override { return nullptr; };*/
	private:
	};
}
