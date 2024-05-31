#include "Interactable_TriggerSphere.h"

#include "InteractableData.h"
#include "DebugMeshes.h"

#include "Unit.h"

void Interactable_TriggerSphere::Start()
{
	AttachDebugMesh(GetGameObject(), DebugMeshType::Sphere, yunuGI::Color::green());
	auto col = GetGameObject()->AddComponent<physics::SphereCollider>();
	col->SetRadius(0.5);

	auto ts = GetGameObject()->GetTransform();
	ts->SetWorldPosition(initPos);
	ts->SetWorldRotation(initRotation);
	ts->SetWorldScale(initScale);
}

void Interactable_TriggerSphere::Update()
{
	
}

void Interactable_TriggerSphere::OnTriggerEnter(physics::Collider* collider)
{
	if (Unit* colliderUnitComponent = collider->GetGameObject()->GetComponent<Unit>();
		colliderUnitComponent != nullptr &&
		colliderUnitComponent->IsPlayerUnit())
	{
		triggerStay.insert(collider);
		OnInteractableTriggerEnter();
	}
}

void Interactable_TriggerSphere::OnTriggerExit(physics::Collider* collider)
{
	if (Unit* colliderUnitComponent = collider->GetGameObject()->GetComponent<Unit>();
		colliderUnitComponent != nullptr &&
		colliderUnitComponent->IsPlayerUnit())
	{
		if (triggerStay.size() == 1)
		{
			OnInteractableTriggerExit();
		}
		triggerStay.erase(collider);
	}
}

void Interactable_TriggerSphere::SetDataFromEditorData(const application::editor::InteractableData& data)
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
}

yunutyEngine::coroutine::Coroutine Interactable_TriggerSphere::DoInteraction()
{
	/// 해당 Coroutine 은 호출되지 않을 것입니다.
	co_return;
}
