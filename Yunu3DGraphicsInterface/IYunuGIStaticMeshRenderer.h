#pragma once
#include "IYunuMeshRenderer.h"
#include "YunuGIMatrix4x4.h"
#include "IUnknown.h"

#include <string>

namespace yunuGI
{
	class IStaticMeshRenderer : public yunuGI::IMeshRenderer
	{
	public:
		virtual void SetPickingMode(bool isPickingModeOn) = 0;
		virtual void SetMaterial(unsigned int index, IMaterial* material) {};
		virtual IMaterial* GetMaterial(unsigned int index = 0) { return nullptr; };
		
		virtual void SetLightMapUVOffset(float x, float y) = 0;
		virtual void SetLightMapUVScaling(float x, float y) = 0;
		virtual void SetLightMapUVIndex(unsigned int index) = 0;

	};

	struct IStaticMeshRendererDesc
	{
	};
}
