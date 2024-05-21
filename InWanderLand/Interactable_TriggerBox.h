/// 2024. 05. 20 김상준
/// Interactable 의 trigger 용 InteractableComponent
/// Box

#pragma once

#include "IInteractableComponent.h"

class Interactable_TriggerBox
	: public IInteractableComponent
{
public:
	virtual void Start() override;
	virtual void Update() override;

	virtual void OnTriggerEnter(physics::Collider* collider) override
	{
		if (Unit* colliderUnitComponent = collider->GetGameObject()->GetComponent<Unit>();
			colliderUnitComponent != nullptr &&
			colliderUnitComponent->GetUnitSide() == Unit::UnitSide::Player)
		{
			triggerStay.insert(collider);
			OnInteractableTriggerEnter(); 
		}
	}

	virtual void OnTriggerExit(physics::Collider* collider) override
	{
		if (Unit* colliderUnitComponent = collider->GetGameObject()->GetComponent<Unit>();
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

	virtual void SetDataFromEditorData(const application::editor::InteractableData& data) override;

	virtual yunutyEngine::coroutine::Coroutine DoInteraction() override;

private:
	std::unordered_set<physics::Collider*> triggerStay = std::unordered_set<physics::Collider*>();
	Vector3d initPos = Vector3d(0, 0, 0);
	Quaternion initRotation = Quaternion();
	Vector3d initScale = Vector3d(1, 1, 1);
};

