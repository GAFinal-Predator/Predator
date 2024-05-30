#include "Interactable_SpikeTrap.h"

#include "InteractableData.h"
#include "DebugMeshes.h"

#include "Unit.h"
#include "GameManager.h"

void Interactable_SpikeTrap::Start()
{
	auto ts = GetGameObject()->GetTransform();
	ts->SetWorldPosition(initPos);
	ts->SetWorldRotation(initRotation);
	ts->SetWorldScale(initScale);

	auto rendererObj = GetGameObject()->AddGameObject();
	AttachDebugMesh(rendererObj, DebugMeshType::Cube, yunuGI::Color::green());
	rendererObj->GetTransform()->SetLocalScale(Vector3d(2.5, 2.5, 2.5));
	auto boxCollider = GetGameObject()->AddComponent<physics::BoxCollider>();
	boxCollider->SetHalfExtent(Vector3d(1.25, 1.25, 1.25));

	for (auto each : GetGameObject()->GetChildren())
	{
		auto renderer = each->GetComponent<graphics::StaticMeshRenderer>();
		if (renderer)
		{
			mesh = each;
			break;
		}
	}
}

void Interactable_SpikeTrap::OnTriggerEnter(physics::Collider* collider)
{
	if (Unit* colliderUnitComponent = collider->GetGameObject()->GetComponent<Unit>();
		GameManager::Instance().IsBattleSystemOperating() &&
		colliderUnitComponent != nullptr &&
		colliderUnitComponent->GetUnitSide() == Unit::UnitSide::Player)
	{
		triggerStay.insert(collider);
		OnInteractableTriggerEnter();

		/// colliderUnitComponent 를 통해 Unit 경직 만드는 로직 추가
		colliderUnitComponent;

		/// 대미지도 주려면 주기
		colliderUnitComponent->Damaged(damage);
		yunutyEngine::SoundSystem::PlaySoundfile("sounds/trap/Damage_Zone.wav");
	}
}

void Interactable_SpikeTrap::OnTriggerExit(physics::Collider* collider)
{
	if (Unit* colliderUnitComponent = collider->GetGameObject()->GetComponent<Unit>();
		GameManager::Instance().IsBattleSystemOperating() &&
		colliderUnitComponent != nullptr &&
		colliderUnitComponent->GetUnitSide() == Unit::UnitSide::Player)
	{
		if (triggerStay.size() == 1)
		{
			OnInteractableTriggerExit();
		}
		triggerStay.erase(collider);
	}
}

void Interactable_SpikeTrap::SetDataFromEditorData(const application::editor::InteractableData& data)
{
	initPos.x = data.pod.position.x;
	initPos.y = data.pod.position.y;
	initPos.z = data.pod.position.z;
	initRotation.w = data.pod.rotation.w;
	initRotation.x = data.pod.rotation.x;
	initRotation.y = data.pod.rotation.y;
	initRotation.z = data.pod.rotation.z;
	initScale.x = data.pod.scale.x;
	initScale.y = data.pod.scale.y;
	initScale.z = data.pod.scale.z;
	damage = data.pod.templateData->pod.damage;
}
