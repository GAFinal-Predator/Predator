#include "InWanderLand.h"
#include "BurnEffect.h"

void BurnEffect::SetEdgeColor(yunuGI::Color color)
{
	this->edgeColor = color;
}

void BurnEffect::SetEdgeThickness(float edgeThickness)
{
	this->edgeThickness = edgeThickness;
}

void BurnEffect::SetSpeed(float speed)
{
	this->speed = speed;
}

void BurnEffect::DisAppear()
{
	isDisAppear = true;
	isDone = false;
	isFirst = true;
}

void BurnEffect::Appear()
{
	isAppear = true;
	isDone = false;
	isFirst = true;
}

bool BurnEffect::IsDone()
{
	return this->isDone;
}

void BurnEffect::Start()
{
	const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();

	auto rootObj = GetGameObject();

	for (auto& each : rootObj->GetChildren())
	{
		yunutyEngine::graphics::SkinnedMesh* renderer = nullptr;
		renderer = each->GetComponent<yunutyEngine::graphics::SkinnedMesh>();

		if (renderer)
		{
			this->renderer = renderer;

			for (int i = 0; i < renderer->GetGI().GetMaterialCount(); ++i)
			{
				auto material = renderer->GetGI().GetMaterial(i, false);

				this->originMaterialVec.push_back(material);

				std::wstring burnMaterialName = L"Burn_";
				burnMaterialName += material->GetName();

				auto burnMaterial = _resourceManager->CloneMaterial(burnMaterialName, material);
				burnMaterial->SetTexture(yunuGI::Texture_Type::Temp0, _resourceManager->GetTexture(L"Texture/Dissolve.jpg"));
				burnMaterial->SetPixelShader(_resourceManager->GetShader(L"DissolvePS.cso"));

				renderer->GetGI().SetMaterial(i, burnMaterial);
				this->burnMaterialVec.push_back(renderer->GetGI().GetMaterial(i));

				renderer->GetGI().SetMaterial(i, originMaterialVec.back());
			}
		}
	}
}

void BurnEffect::Update()
{
	if (isAppear && !isDone)
	{
		if (isFirst)
		{
			this->amount = 1.f;
			isFirst = false;

			for (int i = 0; i < originMaterialVec.size(); ++i)
			{
				renderer->GetGI().SetMaterial(i, burnMaterialVec[i]);
				renderer->GetGI().GetMaterial(i)->SetColor(edgeColor);
				renderer->GetGI().GetMaterial(i)->SetFloat(1, this->edgeThickness);
			}
		}

		// 1 ~ 0 -> 나타남
		amount -= (speed * Time::GetDeltaTime());
		
		for (int i = 0; i < originMaterialVec.size(); ++i)
		{
			renderer->GetGI().GetMaterial(i)->SetFloat(0,amount);
		}

		if (amount <= 0.f)
		{
			// 효과가 끝남
			Reset();
		}

	}
	else if(isDisAppear && !isDone)
	{
		if (isFirst)
		{
			this->amount = 0.f;
			isFirst = false;

			for (int i = 0; i < originMaterialVec.size(); ++i)
			{
				renderer->GetGI().SetMaterial(i, burnMaterialVec[i]);
				renderer->GetGI().GetMaterial(i)->SetColor(edgeColor);
				renderer->GetGI().GetMaterial(i)->SetFloat(1, this->edgeThickness);
			}
		}

		// 0 ~ 1 -> 사라짐
		amount += (speed * Time::GetDeltaTime());

		for (int i = 0; i < originMaterialVec.size(); ++i)
		{
			renderer->GetGI().GetMaterial(i)->SetFloat(0, amount);
		}

		if (amount >= 1.f)
		{
			// 효과가 끝남
			Reset();
		}
	}
}

void BurnEffect::PlayFunction()
{

}

void BurnEffect::StopFunction()
{
	if (GetGameObject()->GetSelfActive())
		GetGameObject()->SetSelfActive(false);
}

void BurnEffect::Reset()
{
	isDisAppear = false;
	isAppear = false;
	isFirst = true;
	isDone = true;
	
	for (int i = 0; i < originMaterialVec.size(); ++i)
	{
		renderer->GetGI().SetMaterial(i, originMaterialVec[i], true);
	}
}
