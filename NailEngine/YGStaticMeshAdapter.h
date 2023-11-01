#pragma once
#include "YunuGraphicsInterface.h"
#include "IYunuGIRenderable.h"
#include "YGRenderableAdapter.h"

#include "RenderableManager.h"
#include "StaticMesh.h"
#include "MaterialWrapper.h"

#include <memory>

namespace yunuGIAdapter
{
	class StaticMeshAdapter : public yunuGIAdapter::RenderableAdapter, public yunuGI::IStaticMeshRenderer
	{
	public:
		StaticMeshAdapter() :RenderableAdapter() 
		{
			renderable = std::make_shared<StaticMesh>();
			RenderableManager::Instance.Get().PushRenderableObject(renderable);

			std::shared_ptr<MaterialWrapper> material = std::make_shared<MaterialWrapper>();
			material->SetRenderable(this->renderable);
			this->materialVec.emplace_back(material);
		}

		~StaticMeshAdapter()
		{
			RenderableManager::Instance.Get().PopRenderableObject(renderable);
		}

		virtual void SetWorldTM(const yunuGI::Matrix4x4& worldTM)
		{
			renderable->SetWorldTM(reinterpret_cast<const DirectX::SimpleMath::Matrix&>(worldTM));
		};

		virtual void SetActive(bool isActive) {};

		virtual void SetMesh(const std::wstring& meshName) override
		{
			renderable->SetMeshName(meshName);
		};

		virtual void SetPickingMode(bool isPickingModeOn) {}

		virtual void SetMaterial(unsigned int index, yunuGI::IMaterial* material) override
		{
			// ���ο� Material�̶��
			if (index + 1 > this->materialVec.size())
			{
				std::shared_ptr<MaterialWrapper> tempMaterial = std::make_shared<MaterialWrapper>();
				tempMaterial->SetRenderable(this->renderable);
				this->materialVec.emplace_back(tempMaterial);

				if (this->materialVec.back()->IsOrigin())
				{
					this->materialVec.back()->original = reinterpret_cast<yunuGIAdapter::MaterialAdapter*>(material);
				}
				else
				{
					this->materialVec.back()->variation = reinterpret_cast<yunuGIAdapter::MaterialAdapter*>(material);
				}
			}
			else
			{
				if (this->materialVec[index]->IsOrigin())
				{
					this->materialVec[index]->original = reinterpret_cast<yunuGIAdapter::MaterialAdapter*>(material);
				}
				else
				{
					this->materialVec[index]->variation = reinterpret_cast<yunuGIAdapter::MaterialAdapter*>(material);
				}
			}

			renderable->SetMaterial(index, material);
		};

		virtual yunuGI::IMaterial* GetMaterial(unsigned int index = 0)override
		{
			return this->materialVec[index].get();
		};

	private:
		std::shared_ptr<StaticMesh> renderable;
		std::vector<std::shared_ptr<MaterialWrapper>> materialVec;

	};
}
