#pragma once
#include "YunutyEngine.h"
#include "ContentsObservee.h"

class Unit;

class CursorDetector : public Component, public ContentsObservee
{
public:
	virtual void OnTriggerEnter(physics::Collider* collider) override;
	virtual void OnTriggerExit(physics::Collider* collider) override;

	virtual void Start() override;
	virtual void Update() override;

	void EraseUnitFromContainer(Unit* p_unit);

	Unit* GetCurrentOnMouseUnit() const;
	virtual Component* GetComponent() override { return this; }

private:
	Unit* m_currentSelectedUnit{ nullptr };
	std::set<Unit*> m_onMouseUnits;

	GameObject* m_SelectEffectObject;

	void DetermineClosestUnit();
};

