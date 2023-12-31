#pragma once
#include "IResource.h"

#include "YunuGIColor.h"

namespace yunuGI
{
	class IShader;
	class ITexture;

	enum class Texture_Type
	{
		ALBEDO,
		NORMAL,
		ARM,
		HEIGHT,
		EMISSION,
		Temp0,
		Temp1,
		Temp2,
	};

	class IMaterial : virtual public IResource
	{
	public:
		virtual void SetVertexShader(const yunuGI::IShader* shader) = 0;
		virtual void SetPixelShader(const yunuGI::IShader* shader) = 0;
		virtual void SetTexture(Texture_Type textureType, const yunuGI::ITexture* texture) = 0;
		virtual void SetColor(const yunuGI::Color& color) = 0;
		virtual void SetInt(int index, int val) = 0;

		virtual const yunuGI::IShader* GetPixelShader() const = 0;

	private:

	};
}