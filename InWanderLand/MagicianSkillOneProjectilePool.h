#pragma once
#include "GameObjectPool.h"
#include "DebugMeshes.h"
#include "StaticMeshRenderer.h"
#include "MagicianAutoAttackProjectile.h"
#include "ContentsLayer.h"
#include "Application.h"

class MagicianSkillOneProjectilePool :
	public GameObjectPool<MagicianAutoAttackProjectile>, public SingletonClass<MagicianSkillOneProjectilePool>
{
public:
	virtual void ObjectInitializer(MagicianAutoAttackProjectile* projectile) override
	{
		const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
		auto capsuleMesh = _resourceManager->GetMesh(L"Capsule");

		auto projectileComponent = projectile->GetGameObject()->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		projectileComponent->GetGI().SetMesh(capsuleMesh);
		projectileComponent->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 0, 0, 0 });

		application::contents::ContentsLayer* contentsLayer = dynamic_cast<application::contents::ContentsLayer*>(application::Application::GetInstance().GetContentsLayer());
		contentsLayer->RegisterToEditorObjectVector(projectile->GetGameObject());
	}
};

