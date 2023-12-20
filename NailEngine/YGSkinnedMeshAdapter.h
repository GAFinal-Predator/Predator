#pragma once
#include "YunuGraphicsInterface.h"
#include "IYunuGIRenderable.h"
#include "YGRenderableAdapter.h"

#include "RenderableManager.h"
#include "SKinnedMesh.h"
#include "MaterialWrapper.h"

#include <memory>

namespace yunuGIAdapter
{
	class SkinnedMeshAdapter : public yunuGIAdapter::RenderableAdapter, public yunuGI::ISkinnedMesh
	{
	public:
		SkinnedMeshAdapter() :RenderableAdapter()
		{
			renderable = std::make_shared<SkinnedMesh>();
			RenderableManager::Instance.Get().PushSkinnedRenderableObject(renderable);

			std::shared_ptr<MaterialWrapper> material = std::make_shared<MaterialWrapper>(false);
			material->SetRenderable(this->renderable);
			//renderable->SetMaterial(0, material->GetMaterial());
			this->materialVec.emplace_back(material);
		}

		~SkinnedMeshAdapter()
		{
			RenderableManager::Instance.Get().PopSkinnedRenderableObject(renderable);
		}

		virtual void SetBone(std::wstring fbxName) override
		{
			renderable->SetBone(fbxName);
		}

		virtual void SetWorldTM(const yunuGI::Matrix4x4& worldTM)
		{
			renderable->SetWorldTM(reinterpret_cast<const DirectX::SimpleMath::Matrix&>(worldTM));
		};

		virtual void SetActive(bool isActive) {};

		virtual void SetMesh(yunuGI::IMesh* mesh) override
		{
			renderable->SetMesh(reinterpret_cast<Mesh*>(mesh));
		};

		virtual void SetAnimatorIndex(int animatorIndex) override
		{
			renderable->SetAnimatorIndex(animatorIndex);
		};

		virtual void SetPickingMode(bool isPickingModeOn) {}

		virtual void SetMaterial(unsigned int index, yunuGI::IMaterial* material) override
		{
			std::shared_ptr<Material> materialPtr = std::shared_ptr<Material>(reinterpret_cast<Material*>(material));

			// ���ο� Material�̶��
			if (index + 1 > this->materialVec.size())
			{
				std::shared_ptr<MaterialWrapper> tempMaterial = std::make_shared<MaterialWrapper>();
				tempMaterial->SetRenderable(this->renderable);
				this->materialVec.emplace_back(tempMaterial);

				if (this->materialVec.back()->IsOrigin())
				{
					this->materialVec.back()->original = materialPtr;
				}
				else
				{
					this->materialVec.back()->variation = materialPtr;
				}
			}
			else
			{
				if (this->materialVec[index]->IsOrigin())
				{
					this->materialVec[index]->original = materialPtr;
				}
				else
				{
					this->materialVec[index]->variation = materialPtr;
				}
			}

			renderable->SetMaterial(index, material);
		};

		virtual yunuGI::IMaterial* GetMaterial(unsigned int index = 0)override
		{
			return this->materialVec[index].get();
		};

	private:
		std::shared_ptr<SkinnedMesh> renderable;
		std::vector<std::shared_ptr<MaterialWrapper>> materialVec;
	};
}
