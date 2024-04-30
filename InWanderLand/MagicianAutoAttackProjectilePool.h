#pragma once
#include "MagicianAutoAttackProjectile.h"
#include "GameObjectPool.h"
#include "DebugMeshes.h"
#include "StaticMeshRenderer.h"
#include "Dotween.h"
#include "ContentsObservee.h"

class MagicianAutoAttackProjectilePool :
	public GameObjectPool<MagicianAutoAttackProjectile>, public Component, public SingletonComponent<MagicianAutoAttackProjectilePool>, public ContentsObservee
{
public:
	virtual void ObjectInitializer(MagicianAutoAttackProjectile* projectile) override
	{
		//const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
		//auto capsuleMesh = _resourceManager->GetMesh(L"Capsule");

		//auto projectileComponent = projectile->GetGameObject()->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		//projectileComponent->GetGI().SetMesh(capsuleMesh);
		//projectileComponent->GetGI().SetMaterial(0, GetColoredDebugMaterial(yunuGI::Color::red(), false));	
		auto bulletGameObject = yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX("SM_Fork");
		bulletGameObject->SetParent(projectile->GetGameObject());

		projectile->GetGameObject()->AddComponent<Dotween>();
		//bulletGameObject->GetTransform()->SetLocalRotation(Quaternion(Vector3d( 90, 0, 0 )));
		//bulletGameObject->GetTransform()->SetLocalRotation(projectile->GetGameObject()->GetTransform()->GetWorldRotation());
	}
	virtual void Start() override;
	virtual void PlayFunction() override;
	virtual void StopFunction() override;
};

void MagicianAutoAttackProjectilePool::Start()
{
	isSingletonComponent = true;
}

void MagicianAutoAttackProjectilePool::PlayFunction()
{
	this->SetActive(true);
	if (isOncePaused)
	{
		Start();
	}
}

void MagicianAutoAttackProjectilePool::StopFunction()
{
	poolObjects.clear();
	expendableObjects.clear();
}

