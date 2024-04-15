#pragma once
#include "EnemySkillSystem.h"

class Dotween;

class SpikeSkillSystem : public EnemySkillSystem
{
private:
	float m_elapsed{ 0.0f };
	float m_spikeTrapDamageDuration{ 3.0f };

	SkillRequirements m_spikeDamageRequirement;

public:
	virtual void Start() override;
	virtual void Update() override;

	virtual void ActivateSkill(Unit::SkillEnum p_currentSkill, Vector3d p_skillPosition) override;

	void SetSpikeSkillRequirment(GameObject* p_colliderObj, GameObject* p_debugObj, Dotween* p_dotween);

	virtual void ActivateSkillOne() override;
};

