#pragma once
#include "MagicianAutoAttackProjectile.h"
#include "GameObjectPool.h"
#include "DebugMeshes.h"
#include "StaticMeshRenderer.h"

class MagicianAutoAttackProjectilePool :
	public GameObjectPool<MagicianAutoAttackProjectile>, public SingletonClass<MagicianAutoAttackProjectilePool>
{
public:
	virtual void ObjectInitializer(MagicianAutoAttackProjectile* projectile) override
	{
		const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
		auto capsuleMesh = _resourceManager->GetMesh(L"Capsule");

		auto projectileComponent = projectile->GetGameObject()->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		projectileComponent->GetGI().SetMesh(capsuleMesh);
		projectileComponent->GetGI().SetMaterial(0, GetColoredDebugMaterial(yunuGI::Color::red(), false));
	}
};

