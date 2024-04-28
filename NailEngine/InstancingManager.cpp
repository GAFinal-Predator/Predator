#include "InstancingManager.h"

#include "Material.h"
#include "Mesh.h"

#include "CameraManager.h"
#include "NailCamera.h"
#include "NailAnimator.h"
#include "AnimationGroup.h"
#include "NailEngine.h"
#include "ConstantBuffer.h"
#include "ResourceManager.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "PointLight.h"
#include "ParticleSystem.h"

#include <cmath>
#include <algorithm>

LazyObjects<InstancingManager> InstancingManager::Instance;

void InstancingManager::Init()
{
	instanceTransitionDesc = std::make_shared<InstanceTransitionDesc>();
	particleBuffer = std::make_shared<ParticleBuffer>();
	lightMapUVBuffer = std::make_shared<LightMapUVBuffer>();
}

void InstancingManager::SortByCameraDirection()
{
	for (auto& each : this->staticMeshDeferredMap)
	{
		std::vector<std::shared_ptr<RenderInfo>> tempVec;

		for (auto& each2 : each.second)
		{
			tempVec.push_back(each2);
		}

		auto tempPair = std::make_pair(each.first, tempVec);
		this->staticMeshDeferredRenderVec.push_back(tempPair);
	}
	staticMeshDeferredMap.clear();

	// 우선 BoundingRadius 기준으로 정렬
	std::sort(this->staticMeshDeferredRenderVec.begin(), this->staticMeshDeferredRenderVec.end(),
		[](const auto& left, const auto& right)
		{
			auto lR = left.first.first->GetBoundingRadius();
			auto rR = right.first.first->GetBoundingRadius();

			return lR > rR;
		}
	);

	DirectX::SimpleMath::Matrix cameraWTM = CameraManager::Instance.Get().GetMainCamera()->GetWTM();
	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Quaternion quat;
	cameraWTM.Decompose(scale, quat, pos);

	DirectX::SimpleMath::Matrix rot = DirectX::XMMatrixRotationQuaternion(quat);

	DirectX::SimpleMath::Vector4 front{ 0.f,0.f,1.f,0.f };
	front = DirectX::XMVector3Rotate(front, quat);


	DirectX::SimpleMath::Vector3 cameraDirection = DirectX::SimpleMath::Vector3{ front.x,front.y,front.z };
	cameraDirection.Normalize();

	for (auto& each : this->staticMeshDeferredRenderVec)
	{
		std::sort(each.second.begin(), each.second.end(),
			[=](const auto& left, const auto& right)
			{
				auto lPos = DirectX::SimpleMath::Vector3{ left->wtm._41, left->wtm._42, left->wtm._43};
				auto rPos = DirectX::SimpleMath::Vector3{ right->wtm._41, right->wtm._42, right->wtm._43 };

				float lDot = cameraDirection.Dot(lPos);
				float rDot = cameraDirection.Dot(rPos);

				return lDot < rDot;
			}
		);
	}

	// 인덱스맵에 맵핑하기
	unsigned int instanceIDIndex = 0;
	unsigned int renderInfoIndex = 0;
	for (auto& each : staticMeshDeferredRenderVec)
	{
		staticMeshInstanceIDIndexMap.insert({ each.first,instanceIDIndex });
		instanceIDIndex++;

		renderInfoIndex = 0;

		for (auto& each2 : each.second)
		{
			staticMeshRenderInfoIndexMap.insert({ each2 ,renderInfoIndex });
			renderInfoIndex++;
		}
	}
}

void InstancingManager::RenderStaticDeferred()
{
	// 비활성화인 얘들 거르고
	// 프러스텀 밖에 있는 얘들 거르고
	// 삭제된 얘들 거르고

	// 인스턴스 버퍼의 데이터를 지움
	ClearData();

	{
		for (auto& pair : this->staticMeshDeferredMap)
		{
			auto& renderInfoVec = pair.second;

			const InstanceID& instanceID = pair.first;

			{
				//for (int i = 0; i < renderInfoVec.size(); ++i)
				int index = 0;
				for (auto& i : renderInfoVec)
				{
					if (i->mesh == nullptr) continue;

					if (i->isActive == false) continue;

					const std::shared_ptr<RenderInfo>& renderInfo = i;
					InstancingData data;
					data.wtm = renderInfo->wtm;
					AddData(instanceID, data);

					lightMapUVBuffer->lightMapUV[index].lightMapIndex = renderInfo->lightMapIndex;
					lightMapUVBuffer->lightMapUV[index].scaling = renderInfo->uvScaling;
					lightMapUVBuffer->lightMapUV[index].uvOffset = renderInfo->uvOffset;


					index++;
				}

				NailEngine::Instance.Get().GetConstantBuffer(static_cast<int>(CB_TYPE::LIGHTMAP_UV))->PushGraphicsData(lightMapUVBuffer.get(),
					sizeof(LightMapUVBuffer),
					static_cast<int>(CB_TYPE::LIGHTMAP_UV), false);

				if (renderInfoVec.size() != 0)
				{
					auto& buffer = _buffers[instanceID];
					if (buffer->GetCount() > 0)
					{
						ExposureBuffer exposurrBuffer;
						exposurrBuffer.diffuseExposure = (*renderInfoVec.begin())->mesh->GetDiffuseExposure();
						exposurrBuffer.ambientExposure = (*renderInfoVec.begin())->mesh->GetAmbientExposure();;
						NailEngine::Instance.Get().GetConstantBuffer(static_cast<int>(CB_TYPE::EXPOSURE))->PushGraphicsData(&exposurrBuffer,
							sizeof(ExposureBuffer),
							static_cast<int>(CB_TYPE::EXPOSURE), false);

						(*renderInfoVec.begin())->material->PushGraphicsData();
						buffer->PushData();
						(*renderInfoVec.begin())->mesh->Render((*renderInfoVec.begin())->materialIndex, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, true, buffer->GetCount(), buffer);
					}
				}
			}
		}
	}

	{
		for (auto& pair : this->staticMeshDeferredRenderVec)
		{
			auto& renderInfoVec = pair.second;

			const InstanceID& instanceID = pair.first;

			{
				int index = 0;
				for (auto& i : renderInfoVec)
				{
					if (i == nullptr) continue;

					if (i->mesh == nullptr) continue;

					if (i->isActive == false) continue;

					auto& frustum = CameraManager::Instance.Get().GetMainCamera()->GetFrustum();
					auto aabb = i->mesh->GetBoundingBox(i->wtm, i->materialIndex);

					if (frustum.Contains(aabb) == DirectX::ContainmentType::DISJOINT)
					{
						continue;
					}

					//if ((i->mesh->GetName() == L"SM_Bush_001") || (i->mesh->GetName() == L"SM_Bush_002"))
					//{

					//}

					const std::shared_ptr<RenderInfo>& renderInfo = i;
					InstancingData data;
					data.wtm = renderInfo->wtm;
					AddData(instanceID, data);

					lightMapUVBuffer->lightMapUV[index].lightMapIndex = renderInfo->lightMapIndex;
					lightMapUVBuffer->lightMapUV[index].scaling = renderInfo->uvScaling;
					lightMapUVBuffer->lightMapUV[index].uvOffset = renderInfo->uvOffset;


					index++;
				}

				NailEngine::Instance.Get().GetConstantBuffer(static_cast<int>(CB_TYPE::LIGHTMAP_UV))->PushGraphicsData(lightMapUVBuffer.get(),
					sizeof(LightMapUVBuffer),
					static_cast<int>(CB_TYPE::LIGHTMAP_UV), false);

				if (renderInfoVec.size() != 0)
				{
					auto& buffer = _buffers[instanceID];
					if (buffer->GetCount() > 0)
					{
						ExposureBuffer exposurrBuffer;
						exposurrBuffer.diffuseExposure = (*renderInfoVec.begin())->mesh->GetDiffuseExposure();
						exposurrBuffer.ambientExposure = (*renderInfoVec.begin())->mesh->GetAmbientExposure();;
						NailEngine::Instance.Get().GetConstantBuffer(static_cast<int>(CB_TYPE::EXPOSURE))->PushGraphicsData(&exposurrBuffer,
							sizeof(ExposureBuffer),
							static_cast<int>(CB_TYPE::EXPOSURE), false);

						(*renderInfoVec.begin())->material->PushGraphicsData();
						buffer->PushData();
						(*renderInfoVec.begin())->mesh->Render((*renderInfoVec.begin())->materialIndex, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, true, buffer->GetCount(), buffer);
					}
				}
			}
		}
	}
}

void InstancingManager::RenderStaticForward()
{

	// 비활성화인 얘들 거르고
	// 프러스텀 밖에 있는 얘들 거르고
	// 삭제된 얘들 거르고

	// 인스턴스 버퍼의 데이터를 지움
	ClearData();

	//std::map<InstanceID, std::vector<std::shared_ptr<RenderInfo>>> cache;

	//for (auto& each : renderInfo)
	//{
	//	InstanceID instanceID = std::make_pair((unsigned __int64)each->mesh, (unsigned __int64)each->material);

	//	cache[instanceID].push_back(each);
	//}

	for (auto& pair : this->staticMeshForwardCache)
	{
		std::set<std::shared_ptr<RenderInfo>>& renderInfoVec = pair.second;

		const InstanceID& instanceID = pair.first;

		//if (renderInfoVec.size() == 1)
		//{
		//	MatrixBuffer matrixBuffer;
		//	matrixBuffer.WTM = renderInfoVec[0].wtm;
		//	matrixBuffer.VTM = NailCamera::Instance.Get().GetVTM();
		//	matrixBuffer.PTM = NailCamera::Instance.Get().GetPTM();
		//	matrixBuffer.WVP = matrixBuffer.WTM * matrixBuffer.VTM * matrixBuffer.PTM;
		//	matrixBuffer.WorldInvTrans = matrixBuffer.WTM.Invert().Transpose();
		//	NailEngine::Instance.Get().GetConstantBuffer(0)->PushGraphicsData(&matrixBuffer, sizeof(MatrixBuffer), 0);
		//
		//	auto mesh = std::static_pointer_cast<Mesh>(ResourceManager::Instance.Get().GetMesh(renderInfoVec[0].mesh->GetName()));
		//
		//	std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(renderInfoVec[0].material->GetName()))->PushGraphicsData();
		//	for (int i = 0; i < mesh->GetMaterialCount(); ++i)
		//	{
		//		renderInfoVec[0].mesh->Render(i);
		//	}
		//}
		//else
		{
			//for (int i = 0; i < renderInfoVec.size(); ++i)
			for (auto& i : renderInfoVec)
			{
				if (i->mesh == nullptr) continue;
				if (i->isActive == false) continue;

				auto& frustum = CameraManager::Instance.Get().GetMainCamera()->GetFrustum();
				auto aabb = i->mesh->GetBoundingBox(i->wtm, i->materialIndex);

				if (frustum.Contains(aabb) == DirectX::ContainmentType::DISJOINT)
				{
					continue;
				}

				const std::shared_ptr<RenderInfo>& renderInfo = i;
				InstancingData data;
				data.wtm = renderInfo->wtm;
				AddData(instanceID, data);
			}

			if (renderInfoVec.size() != 0)
			{
				if ((*renderInfoVec.begin())->mesh == nullptr) continue;
				auto& buffer = _buffers[instanceID];
				if (buffer->GetCount() > 0)
				{
					(*renderInfoVec.begin())->material->PushGraphicsData();
					auto test = (*renderInfoVec.begin())->mesh->GetMaterialCount();
					buffer->PushData();
					(*renderInfoVec.begin())->mesh->Render((*renderInfoVec.begin())->materialIndex, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, true, buffer->GetCount(), buffer);
				}
			}
		}
	}
}

void InstancingManager::RenderStaticShadow()
{
	ClearData();

	for (auto& pair : this->staticMeshDeferredMap)
	{
		auto& renderInfoVec = pair.second;

		const InstanceID& instanceID = pair.first;

		{
			for (auto& i : renderInfoVec)
			{
				if (i->lightMapIndex != -1) continue;

				if (i->isActive == false) continue;

				//auto& frustum = CameraManager::Instance.Get().GetMainCamera()->GetFrustum();
				//auto aabb = i->mesh->GetBoundingBox(i->wtm * CameraManager::Instance.Get().GetMainCamera()->GetVTM(), i->materialIndex);

				//if (frustum.Contains(aabb) == DirectX::ContainmentType::DISJOINT)
				//{
				//	continue;
				//}

				const std::shared_ptr<RenderInfo>& renderInfo = i;
				InstancingData data;
				data.wtm = renderInfo->wtm;
				AddData(instanceID, data);
			}

			if (renderInfoVec.size() != 0)
			{
				auto& buffer = _buffers[instanceID];

				auto opacityMap = (*renderInfoVec.begin())->material->GetTexture(yunuGI::Texture_Type::OPACITY);
				if (opacityMap)
				{
					static_cast<Texture*>(opacityMap)->Bind(static_cast<unsigned int>(yunuGI::Texture_Type::OPACITY));
					MaterialBuffer materialBuffer;
					materialBuffer.useTexture[static_cast<unsigned int>(yunuGI::Texture_Type::OPACITY)] = 1;
					NailEngine::Instance.Get().GetConstantBuffer(static_cast<int>(CB_TYPE::MATERIAL))->PushGraphicsData(&materialBuffer, sizeof(MaterialBuffer), static_cast<int>(CB_TYPE::MATERIAL));
				}
				else
				{
					MaterialBuffer materialBuffer;
					materialBuffer.useTexture[static_cast<unsigned int>(yunuGI::Texture_Type::OPACITY)] = 0;
					NailEngine::Instance.Get().GetConstantBuffer(static_cast<int>(CB_TYPE::MATERIAL))->PushGraphicsData(&materialBuffer, sizeof(MaterialBuffer), static_cast<int>(CB_TYPE::MATERIAL));
				}

				buffer->PushData();
				(*renderInfoVec.begin())->mesh->Render((*renderInfoVec.begin())->materialIndex, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, true, buffer->GetCount(), buffer);
			}
		}
	}
}

void InstancingManager::RenderStaticPointLightShadow(DirectX::SimpleMath::Matrix& lightWTM, PointLight* light)
{
	ClearData();

	for (auto& pair : this->staticMeshDeferredMap)
	{
		auto& renderInfoVec = pair.second;

		const InstanceID& instanceID = pair.first;

		{
			for (auto& i : renderInfoVec)
			{
				if (i->lightMapIndex != -1) continue;

				if (i->isActive == false) continue;

				// 만일 포인트라이트의 범위 안에 있는 오브젝트가 아니라면 렌더링되지 않게 컬링
				auto aabb = i->mesh->GetBoundingBox(i->wtm, i->materialIndex);

				if (light->GetBoundingSphere(lightWTM).Intersects(aabb) == false)
				{
					continue;
				}

				//auto& frustum = CameraManager::Instance.Get().GetMainCamera()->GetFrustum();
				//auto aabb = i->mesh->GetBoundingBox(i->wtm * CameraManager::Instance.Get().GetMainCamera()->GetVTM(), i->materialIndex);

				//if (frustum.Contains(aabb) == DirectX::ContainmentType::DISJOINT)
				//{
				//	continue;
				//}

				const std::shared_ptr<RenderInfo>& renderInfo = i;
				InstancingData data;
				data.wtm = renderInfo->wtm;
				AddData(instanceID, data);
			}

			if (renderInfoVec.size() != 0)
			{
				auto& buffer = _buffers[instanceID];
				buffer->PushData();
				(*renderInfoVec.begin())->mesh->Render((*renderInfoVec.begin())->materialIndex, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, true, buffer->GetCount(), buffer);
			}
		}
	}
}

void InstancingManager::RenderSkinnedPointLightShadow(DirectX::SimpleMath::Matrix& lightWTM, PointLight* light)
{
	ClearData();

	for (auto& pair : this->skinnedMeshCache)
	{
		const std::set<std::shared_ptr<SkinnedRenderInfo>>& renderInfoVec = pair.second;

		const InstanceID instanceID = pair.first;

		{
			int descIndex = 0;
			for (auto& i : renderInfoVec)
			{
				if (i->renderInfo.isActive == false) continue;

				auto aabb = i->renderInfo.mesh->GetBoundingBox(i->renderInfo.wtm, i->renderInfo.materialIndex);

				if (light->GetBoundingSphere(lightWTM).Intersects(aabb) == false)
				{
					continue;
				}


				const RenderInfo& renderInfo = i->renderInfo;
				InstancingData data;
				data.wtm = renderInfo.wtm;
				AddData(instanceID, data);
				this->instanceTransitionDesc->transitionDesc[descIndex++] = i->animator->GetTransitionDesc();
			}

			NailEngine::Instance.Get().GetConstantBuffer(static_cast<int>(CB_TYPE::INST_TRANSITION))->PushGraphicsData(this->instanceTransitionDesc.get(),
				sizeof(InstanceTransitionDesc), static_cast<int>(CB_TYPE::INST_TRANSITION));

			auto animationGroup = ResourceManager::Instance.Get().GetAnimationGroup((*renderInfoVec.begin())->modelName);
			animationGroup->Bind();

			if (renderInfoVec.size() != 0)
			{
				auto& buffer = _buffers[instanceID];
				if (buffer->GetCount() > 0)
				{
					buffer->PushData();
					(*renderInfoVec.begin())->renderInfo.mesh->Render((*renderInfoVec.begin())->renderInfo.materialIndex, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, true, buffer->GetCount(), buffer);
				}
			}
		}
	}
}

void InstancingManager::RegisterStaticDeferredData(std::shared_ptr<RenderInfo>& renderInfo)
{
	//InstanceID instanceID = std::make_pair((unsigned __int64)renderInfo->mesh, (unsigned __int64)renderInfo->material);
	InstanceID instanceID = std::make_pair(renderInfo->mesh, renderInfo->material);

	auto renderInfoIter = this->staticMeshRenderInfoIndexMap.find(renderInfo);
	if (renderInfoIter != this->staticMeshRenderInfoIndexMap.end())
	{
		auto instanceIter = this->staticMeshInstanceIDIndexMap.find(instanceID);
		if (instanceIter != this->staticMeshInstanceIDIndexMap.end())
		{
			this->staticMeshDeferredRenderVec[instanceIter->second].second.push_back(renderInfo);
		}
		else
		{
			// 원래 vector안에는 있었지만 새로운 머터리얼이 적용되어 정보가 없는 경우
			std::vector<std::shared_ptr<RenderInfo>> tempVec;
			tempVec.push_back(renderInfo);
			this->staticMeshDeferredRenderVec.push_back(std::make_pair(instanceID, tempVec));
		}
	}
	else
	{
		this->staticMeshDeferredMap[instanceID].insert(renderInfo);
	}

	if (_buffers.find(instanceID) == _buffers.end())
	{
		_buffers[instanceID] = std::make_shared<InstanceBuffer>();
	}
}

void InstancingManager::RegisterStaticForwardData(std::shared_ptr<RenderInfo>& renderInfo)
{
	//InstanceID instanceID = std::make_pair((unsigned __int64)renderInfo->mesh, (unsigned __int64)renderInfo->material);
	InstanceID instanceID = std::make_pair(renderInfo->mesh, renderInfo->material);

	this->staticMeshForwardCache[instanceID].insert(renderInfo);

	if (_buffers.find(instanceID) == _buffers.end())
	{
		_buffers[instanceID] = std::make_shared<InstanceBuffer>();
	}
}

void InstancingManager::RegisterSkinnedData(std::shared_ptr<SkinnedRenderInfo>& renderInfo)
{
	//InstanceID instanceID = std::make_pair((unsigned __int64)renderInfo->renderInfo.mesh, (unsigned __int64)renderInfo->renderInfo.material);
	InstanceID instanceID = std::make_pair(renderInfo->renderInfo.mesh, renderInfo->renderInfo.material);

	this->skinnedMeshCache[instanceID].insert(renderInfo);

	if (_buffers.find(instanceID) == _buffers.end())
	{
		_buffers[instanceID] = std::make_shared<InstanceBuffer>();
	}
}

void InstancingManager::PopStaticDeferredData(std::shared_ptr<RenderInfo>& renderInfo)
{
	//InstanceID instanceID = std::make_pair((unsigned __int64)renderInfo->mesh, (unsigned __int64)renderInfo->material);
	InstanceID instanceID = std::make_pair(renderInfo->mesh, renderInfo->material);


	// 인스턴스 인덱스 맵에 있는지 검사
	auto instanceIter = this->staticMeshInstanceIDIndexMap.find(instanceID);
	if (instanceIter != this->staticMeshInstanceIDIndexMap.end())
	{
		// 이미 vector에 있다는 뜻
		auto renderInfoIter = this->staticMeshRenderInfoIndexMap.find(renderInfo);
		if (renderInfoIter != this->staticMeshRenderInfoIndexMap.end())
		{
			if (!this->staticMeshDeferredRenderVec[instanceIter->second].second.empty())
			{
				this->staticMeshDeferredRenderVec[instanceIter->second].second[renderInfoIter->second] = nullptr;
			}
		}
	}
	else
	{
		auto iter = this->staticMeshDeferredMap.find(instanceID);

		if (iter != this->staticMeshDeferredMap.end())
		{
			this->staticMeshDeferredMap[instanceID].erase(renderInfo);

			if (this->staticMeshDeferredMap[instanceID].empty())
			{
				this->staticMeshDeferredMap.erase(instanceID);
			}
		}
	}
}

void InstancingManager::PopStaticForwardData(std::shared_ptr<RenderInfo>& renderInfo)
{
	//InstanceID instanceID = std::make_pair((unsigned __int64)renderInfo->mesh, (unsigned __int64)renderInfo->material);
	InstanceID instanceID = std::make_pair(renderInfo->mesh, renderInfo->material);

	auto iter = this->staticMeshForwardCache.find(instanceID);
	if (iter != this->staticMeshForwardCache.end())
	{
		this->staticMeshForwardCache[instanceID].erase(renderInfo);
		if (this->staticMeshForwardCache[instanceID].empty())
		{
			this->staticMeshForwardCache.erase(instanceID);
		}
	}
}

void InstancingManager::PopSkinnedData(std::shared_ptr<SkinnedRenderInfo>& renderInfo)
{
	//InstanceID instanceID = std::make_pair((unsigned __int64)renderInfo->renderInfo.mesh, (unsigned __int64)renderInfo->renderInfo.material);
	InstanceID instanceID = std::make_pair(renderInfo->renderInfo.mesh, renderInfo->renderInfo.material);

	auto iter = this->skinnedMeshCache.find(instanceID);
	if (iter != this->skinnedMeshCache.end())
	{
		this->skinnedMeshCache[instanceID].erase(renderInfo);
		if (this->skinnedMeshCache[instanceID].empty())
		{
			this->skinnedMeshCache.erase(instanceID);
		}
	}
}

void InstancingManager::RegisterParticleRenderInfo(ParticleSystem* particleSystem, std::list<yunuGI::ParticleRenderInfo>* particleInfoList)
{
	this->particleRenderInfoMap.insert({ particleSystem, particleInfoList });
}

void InstancingManager::RenderSkinned()
{
	ClearData();

	for (auto& pair : this->skinnedMeshCache)
	{
		const std::set<std::shared_ptr<SkinnedRenderInfo>>& renderInfoVec = pair.second;

		const InstanceID instanceID = pair.first;

		{
			int descIndex = 0;
			int index = 0;
			for (auto& i : renderInfoVec)
			{
				if (i->renderInfo.isActive == false) continue;

				auto& frustum = CameraManager::Instance.Get().GetMainCamera()->GetFrustum();
				auto aabb = i->renderInfo.mesh->GetBoundingBox(i->renderInfo.wtm, i->renderInfo.materialIndex);

				if (frustum.Intersects(aabb) == false)
				{
					continue;
				}

				const RenderInfo& renderInfo = i->renderInfo;
				InstancingData data;
				data.wtm = renderInfo.wtm;
				AddData(instanceID, data);
				this->instanceTransitionDesc->transitionDesc[descIndex++] = i->animator->GetTransitionDesc();

				lightMapUVBuffer->lightMapUV[index].lightMapIndex = renderInfo.lightMapIndex;
				lightMapUVBuffer->lightMapUV[index].scaling = renderInfo.uvScaling;
				lightMapUVBuffer->lightMapUV[index].uvOffset = renderInfo.uvOffset;

				index++;
			}

			NailEngine::Instance.Get().GetConstantBuffer(static_cast<int>(CB_TYPE::LIGHTMAP_UV))->PushGraphicsData(lightMapUVBuffer.get(),
				sizeof(LightMapUVBuffer),
				static_cast<int>(CB_TYPE::LIGHTMAP_UV), false);

			NailEngine::Instance.Get().GetConstantBuffer(static_cast<int>(CB_TYPE::INST_TRANSITION))->PushGraphicsData(this->instanceTransitionDesc.get(),
				sizeof(InstanceTransitionDesc), static_cast<int>(CB_TYPE::INST_TRANSITION));

			auto animationGroup = ResourceManager::Instance.Get().GetAnimationGroup((*renderInfoVec.begin())->modelName);
			animationGroup->Bind();

			if (renderInfoVec.size() != 0)
			{
				if ((*renderInfoVec.begin())->renderInfo.mesh == nullptr) continue;

				ExposureBuffer exposurrBuffer;
				exposurrBuffer.diffuseExposure = (*renderInfoVec.begin())->renderInfo.mesh->GetDiffuseExposure();
				exposurrBuffer.ambientExposure = (*renderInfoVec.begin())->renderInfo.mesh->GetAmbientExposure();;
				NailEngine::Instance.Get().GetConstantBuffer(static_cast<int>(CB_TYPE::EXPOSURE))->PushGraphicsData(&exposurrBuffer,
					sizeof(ExposureBuffer),
					static_cast<int>(CB_TYPE::EXPOSURE), false);

				auto& buffer = _buffers[instanceID];
				if (buffer->GetCount() > 0)
				{
					(*renderInfoVec.begin())->renderInfo.material->PushGraphicsData();
					buffer->PushData();
					(*renderInfoVec.begin())->renderInfo.mesh->Render((*renderInfoVec.begin())->renderInfo.materialIndex, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, true, buffer->GetCount(), buffer);
				}
			}
		}
	}
}

void InstancingManager::ClearData()
{
	for (auto& pair : _buffers)
	{
		pair.second->ClearData();
	}
}

void InstancingManager::PopParticleSystem(ParticleSystem* system)
{
	particleRenderInfoMap.erase(system);
}

void InstancingManager::RenderParticle()
{
	MatrixBuffer matrixBuffer;
	matrixBuffer.VTM = CameraManager::Instance.Get().GetMainCamera()->GetVTM();
	matrixBuffer.PTM = CameraManager::Instance.Get().GetMainCamera()->GetPTM();

	for (auto& each : this->particleRenderInfoMap)
	{
		auto& tempList = each.second;

		int index = 0;
		for (auto& each2 : *tempList)
		{
			this->particleBuffer->particleDesc[index].pos = reinterpret_cast<DirectX::SimpleMath::Vector3&>(each2.position);
			this->particleBuffer->particleDesc[index].scale = each2.curScale;
			index++;
		}

		if (index != 0)
		{
			static_cast<Material*>(each.first->GetMaterial())->PushGraphicsData();
			static_cast<GeometryShader*>(ResourceManager::Instance.Get().GetShader(L"ParticleGS.cso").get())->Bind();

			NailEngine::Instance.Get().GetConstantBuffer(static_cast<int>(CB_TYPE::PARTICLE))->PushGraphicsData(this->particleBuffer.get(),
				sizeof(ParticleBuffer),
				static_cast<int>(CB_TYPE::PARTICLE), true);

			matrixBuffer.WTM = DirectX::SimpleMath::Matrix::Identity;
			NailEngine::Instance.Get().GetConstantBuffer(static_cast<int>(CB_TYPE::MATRIX))->PushGraphicsData(&matrixBuffer, sizeof(MatrixBuffer), static_cast<int>(CB_TYPE::MATRIX), true);

			ResourceManager::Instance.Get().GetMesh(L"Point")->Render(0, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, true, index);
		}
	}

	static_cast<GeometryShader*>(ResourceManager::Instance.Get().GetShader(L"ParticleGS.cso").get())->UnBind();
}

void InstancingManager::AddData(const InstanceID& id, InstancingData& instancingData)
{
	_buffers[id]->AddData(instancingData);
}

