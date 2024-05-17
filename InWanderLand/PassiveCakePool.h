#pragma once
#include "PassiveCake.h"
#include "DebugMeshes.h"
#include "UnitProductor.h"
#include "PermanentObservee.h"

class PassiveCakePool :
	public GameObjectPool<PassiveCake>, public SingletonComponent<PassiveCakePool>, public PermanentObservee
{
public:
	virtual void ObjectInitializer(PassiveCake* passiveCake) override
	{
		auto dotween = passiveCake->GetGameObject()->AddComponent<Dotween>();
		passiveCake->SetDotweenComponent(dotween);

		auto cakeMeshObject = Scene::getCurrentScene()->AddGameObject();
		auto cakeMesh = AttachDebugMesh(cakeMeshObject, DebugMeshType::Cube);
		cakeMesh->GetGI().SetMaterial(0, GetColoredDebugMaterial(yunuGI::Color::white(), false));
		cakeMesh->SetActive(false);
		passiveCake->SetMesh(cakeMesh);
		cakeMeshObject->GetTransform()->SetWorldScale({ UNIT_LENGTH, UNIT_LENGTH, UNIT_LENGTH });

		auto cakeCollider = passiveCake->GetGameObject()->AddComponent<physics::BoxCollider>();
		cakeCollider->SetHalfExtent({ UNIT_LENGTH * 0.5f, UNIT_LENGTH * 0.5f, UNIT_LENGTH * 0.5f });
		passiveCake->SetCollider(cakeCollider);

		passiveCake->GetGameObject()->AddComponent<physics::RigidBody>()->SetAsKinematic(true);
		cakeCollider->SetActive(false);
	}

	virtual void Start() override;
	virtual void OnContentsStop() override;
};

void PassiveCakePool::Start()
{
}

void PassiveCakePool::OnContentsStop()
{
	this->SetActive(false);
	poolObjects.clear();
	expendableObjects.clear();
}
