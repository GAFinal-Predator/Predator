#pragma once
#include "IResource.h"

namespace yunuGI
{
	enum class ShaderType
	{
		Forward,
		Deferred,
		Light
	};

	enum class Rasterizer
	{
		Wire,
		Solid
	};

	struct ShaderInfo
	{
		ShaderType shaderType;
		Rasterizer rasterizer;
	};

	class IShader : virtual public IResource
	{
	public:
		virtual const ShaderInfo& GetShaderInfo() const = 0;

	private:

	protected:
		ShaderInfo shaderInfo;
	};
}