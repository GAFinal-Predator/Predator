#pragma once

#include "IYunuGIRenderable.h"

namespace yunuGI
{
	class IMaterial;
	class IMesh;

	class IMeshRenderer : public IRenderable
	{
	public:
		virtual void SetMaterial(unsigned int index, IMaterial* material) = 0;
		virtual IMaterial* GetMaterial(unsigned int index = 0) = 0;
		virtual void SetMesh(IMesh* mesh) = 0;
	};
}
