#include "Interactable_TriggerBox.h"

#include "InteractableData.h"
#include "DebugMeshes.h"

void Interactable_TriggerBox::Start()
{
	auto ts = GetGameObject()->GetTransform();
	ts->SetWorldPosition(initPos);
	ts->SetWorldRotation(initRotation);
	ts->SetWorldScale(initScale);

	AttachDebugMesh(GetGameObject(), DebugMeshType::Cube, yunuGI::Color::green());

	GetGameObject()->AddComponent<physics::BoxCollider>();
}

void Interactable_TriggerBox::Update()
{

}

void Interactable_TriggerBox::SetDataFromEditorData(const application::editor::InteractableData& data)
{
	initPos.x = data.pod.position.x;
	initPos.y = data.pod.position.y;
	initPos.z = data.pod.position.z;
	initRotation.w = data.pod.rotation.w;
	initRotation.x = data.pod.rotation.x;
	initRotation.y = data.pod.rotation.y;
	initRotation.z = data.pod.rotation.z;
	initScale.x = data.pod.scale.x;
	initScale.x = data.pod.scale.y;
	initScale.x = data.pod.scale.z;
}

yunutyEngine::coroutine::Coroutine Interactable_TriggerBox::DoInteraction()
{
	/// 해당 Coroutine 은 호출되지 않을 것입니다.
	co_return;
}
