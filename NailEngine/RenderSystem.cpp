#include "RenderSystem.h"

#include "NailEngine.h"
#include "Struct.h"
#include "ConstantBuffer.h"

#include "ResourceManager.h"
#include "Material.h"
#include "Mesh.h"

#include "NailCamera.h"
#include "RenderableManager.h"
#include "IRenderable.h"

#include "ILight.h"
#include "LightManager.h"
#include "PointLight.h"

#include "NailCamera.h"
#include "RenderTargetGroup.h"

#include "Utils.h"

#include "ResourceBuilder.h"
#include "SwapChain.h"
#include "Device.h"

LazyObjects<RenderSystem> RenderSystem::Instance;

void RenderSystem::ClearRenderInfo()
{
	deferredVec.clear();
	forwardVec.clear();
}

void RenderSystem::SortObject()
{
	auto& renderableSet = RenderableManager::Instance.Get().GetRenderableSet();

	for (auto& e : renderableSet)
	{
		auto mesh = e->GetMesh();
		for (int i = 0; i < mesh->GetMaterialCount(); ++i)
		{
			RenderInfo renderInfo;
			renderInfo.mesh = mesh;
			renderInfo.material = e->GetMaterial(i);
			renderInfo.materialIndex = i;
			renderInfo.wtm = e->GetWorldTM();

			if (e->GetMaterial(i)->GetPixelShader()->GetShaderInfo().shaderType == yunuGI::ShaderType::Deferred)
			{
				this->deferredVec.emplace_back(renderInfo);
			}
			else if (e->GetMaterial(i)->GetPixelShader()->GetShaderInfo().shaderType == yunuGI::ShaderType::Forward)
			{
				this->forwardVec.emplace_back(renderInfo);
			}
		}
	}
}

void RenderSystem::PushLightData()
{
	auto& lightSet = LightManager::Instance.Get().GetLightList();
	LightParams params;
	params.lightCount = LightManager::Instance.Get().GetLightCount();

	unsigned int i = 0;

	for (auto& e : lightSet)
	{
		params.lights[i] = e->GetLightInfo();

		if (e->GetLightInfo().lightType == static_cast<unsigned int>(LightType::Directional))
		{
			params.lights[i].direction = e->GetLightInfo().direction;
		}
		else if (e->GetLightInfo().lightType == static_cast<unsigned int>(LightType::Point))
		{
			params.lights[i].range = e->GetLightInfo().range;
		}
		else if(e->GetLightInfo().lightType == static_cast<unsigned int>(LightType::Spot))
		{
			params.lights[i].range = e->GetLightInfo().range;
			params.lights[i].angle = e->GetLightInfo().angle;
			params.lights[i].direction = e->GetLightInfo().direction;
		}
		
		i++;
	}

	NailEngine::Instance.Get().GetConstantBuffer(2)->PushGraphicsData(&params, sizeof(LightParams), 2);
}

void RenderSystem::PushCameraData()
{
	CameraBuffer buffer;
	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Quaternion quat;
	NailCamera::Instance.Get().GetWTM().Decompose(scale, quat, pos);
	buffer.position = pos;
	NailEngine::Instance.Get().GetConstantBuffer(3)->PushGraphicsData(&buffer, sizeof(CameraBuffer), 3);
}

void RenderSystem::Render()
{
	ClearRenderInfo();
	SortObject();

	PushCameraData();
	PushLightData();

	// ������Ʈ ����
	RenderObject();
	
	// ����Ʈ ����
	RenderLight();

	// Final ���
	DrawFinal();

	RenderForward();

	// ���۵� ���� ���
	DrawDeferredInfo();

	// ���۵�� SRV UnBind
	std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(L"Deferred_DirectionalLight"))->UnBindGraphicsData();
	std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(L"Deferred_Final"))->UnBindGraphicsData();
}

void RenderSystem::RenderObject()
{
	auto& renderTargetGroup = NailEngine::Instance.Get().GetRenderTargetGroup();
	renderTargetGroup[static_cast<int>(RENDER_TARGET_TYPE::G_BUFFER)]->OMSetRenderTarget();

	for (auto& e : this->deferredVec)
	{
		MatrixBuffer matrixBuffer;
		matrixBuffer.WTM = e.wtm;
		matrixBuffer.VTM = NailCamera::Instance.Get().GetVTM();
		matrixBuffer.PTM = NailCamera::Instance.Get().GetPTM();
		matrixBuffer.WVP = matrixBuffer.WTM * matrixBuffer.VTM * matrixBuffer.PTM;
		matrixBuffer.WorldInvTrans = matrixBuffer.WTM.Invert().Transpose();
		NailEngine::Instance.Get().GetConstantBuffer(0)->PushGraphicsData(&matrixBuffer, sizeof(MatrixBuffer), 0);

		auto mesh = std::static_pointer_cast<Mesh>(ResourceManager::Instance.Get().GetMesh(e.mesh->GetName()));

		std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(e.material->GetName()))->PushGraphicsData();
		mesh->Render(e.materialIndex);
	}
	
	//renderTargetGroup[static_cast<int>(RENDER_TARGET_TYPE::G_BUFFER)]->UnBind();
}

void RenderSystem::RenderLight()
{
	auto& renderTargetGroup = NailEngine::Instance.Get().GetRenderTargetGroup();
	auto& lightSet = LightManager::Instance.Get().GetLightList();
	// ������ ����
	renderTargetGroup[static_cast<int>(RENDER_TARGET_TYPE::LIGHTING)]->OMSetRenderTarget();
	for (auto& e : lightSet)
	{
		// Point Light�� ��� ���� Sphere Mesh�� ������ ���������ο� �ѱ��.
		// �̶� WVP�� �ʿ��ϱ⿡ ������ۿ� �о�־�� �Ѵ�.
		if (e->GetLightInfo().lightType == static_cast<unsigned int>(LightType::Point))
		{
			MatrixBuffer matrixBuffer;
			matrixBuffer.WTM = std::static_pointer_cast<PointLight>(e)->GetWorldTM();
			matrixBuffer.VTM = NailCamera::Instance.Get().GetVTM();
			matrixBuffer.PTM = NailCamera::Instance.Get().GetPTM();
			matrixBuffer.WVP = matrixBuffer.WTM * matrixBuffer.VTM * matrixBuffer.PTM;
			matrixBuffer.WorldInvTrans = std::static_pointer_cast<PointLight>(e)->GetWorldTM().Invert().Transpose();
			NailEngine::Instance.Get().GetConstantBuffer(0)->PushGraphicsData(&matrixBuffer, sizeof(MatrixBuffer), 0);
		}

		std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(e->GetMaterialName()))->PushGraphicsData();
		auto mesh = ResourceManager::Instance.Get().GetMesh(e->GetMeshName());
		for (int i = 0; i < mesh->GetMaterialCount(); ++i)
		{
			mesh->Render(i);
		}
	}

	//renderTargetGroup[static_cast<int>(RENDER_TARGET_TYPE::LIGHTING)]->UnBind();
}

void RenderSystem::DrawFinal()
{
	// ���� Ÿ�� �並 ���ε��ϰ�, ���� ���ٽ� ���۸� ��� ���ձ� �ܰ迡 ���ε��Ѵ�.
	ResourceBuilder::Instance.Get().device->GetDeviceContext().Get()->OMSetRenderTargets(1,
		ResourceBuilder::Instance.Get().swapChain->GetRTV().GetAddressOf(),
		ResourceBuilder::Instance.Get().swapChain->GetDSV().Get());


	std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(L"Deferred_Final"))->PushGraphicsData();
	ResourceManager::Instance.Get().GetMesh(L"Rectangle")->Render();
}

void RenderSystem::RenderForward()
{
	for (auto& e : this->forwardVec)
	{
		MatrixBuffer matrixBuffer;
		matrixBuffer.WTM = e.wtm;
		matrixBuffer.VTM = NailCamera::Instance.Get().GetVTM();
		matrixBuffer.PTM = NailCamera::Instance.Get().GetPTM();
		matrixBuffer.WVP = matrixBuffer.WTM * matrixBuffer.VTM * matrixBuffer.PTM;
		matrixBuffer.WorldInvTrans = matrixBuffer.WTM.Invert().Transpose();
		NailEngine::Instance.Get().GetConstantBuffer(0)->PushGraphicsData(&matrixBuffer, sizeof(MatrixBuffer), 0);

		auto mesh = std::static_pointer_cast<Mesh>(ResourceManager::Instance.Get().GetMesh(e.mesh->GetName()));

		std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(e.material->GetName()))->PushGraphicsData();
		mesh->Render(e.materialIndex);
	}
}

void RenderSystem::DrawDeferredInfo()
{
	for (int i = 0; i < 6; i++)
	{
		DirectX::SimpleMath::Matrix matSclae = DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(1280 / 10.f, 800 / 10.f, 1.f));
		DirectX::SimpleMath::Matrix matRotation = DirectX::SimpleMath::Matrix::CreateRotationX(0.f);
		matRotation *= DirectX::SimpleMath::Matrix::CreateRotationY(0.f);
		matRotation *= DirectX::SimpleMath::Matrix::CreateRotationZ(0.f);
		DirectX::SimpleMath::Matrix matTranslation = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-576.f + (i * 128.f), 360.f, 1.f));
		DirectX::SimpleMath::Matrix wtm = matSclae * matRotation * matTranslation;

		if (i == 0)
			std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(L"DeferredPosition"))->PushGraphicsData();
		else if (i == 1)
			std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(L"DeferredNormal"))->PushGraphicsData();
		else if (i == 2)
			std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(L"DeferredColor"))->PushGraphicsData();
		else if (i == 3)
			std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(L"DeferredDepth"))->PushGraphicsData();
		else if (i == 4)
			std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(L"DeferredDiffuseLight"))->PushGraphicsData();
		else if (i == 5)
			std::static_pointer_cast<Material>(ResourceManager::Instance.Get().GetMaterial(L"DeferredSpecularLight"))->PushGraphicsData();
		//else if (i == 6)
		//{
		//	matSclae = DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(1280 / 10.f, 800 / 10.f, 1.f));
		//	matTranslation = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-865.f + (i * 200), 424.f, 500.f));
		//	wtm = matSclae * matRotation * matTranslation;
		//	ResourceManager::Instance.Get().GetMaterial(L"ShadowTarget")->PushGraphicsData();
		//}

		MatrixBuffer matrixBuffer;
		matrixBuffer.WVP = wtm * DirectX::SimpleMath::Matrix::Identity * NailCamera::Instance.Get().GetVTMOrtho();

		NailEngine::Instance.Get().GetConstantBuffer(0)->PushGraphicsData(&matrixBuffer, sizeof(MatrixBuffer), 0);
		ResourceManager::Instance.Get().GetMesh(L"Rectangle")->Render();
	}
}
