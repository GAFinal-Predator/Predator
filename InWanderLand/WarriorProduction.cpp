#include "WarriorProduction.h"

void WarriorProduction::SetUnitData(GameObject* fbxObject)
{
	m_objectName = "Warrior";
	m_unitType = Unit::UnitType::Warrior;
	m_unitSide = Unit::UnitSide::Player;
	m_hp = 1050;
	m_ap = 10;
	m_idRadius = 5.0f;
	m_atkRadius = 1.5f;
	m_unitSpeed = 1.5f;

	auto rsrcManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();

	/// Setting Animation Related Members
	m_unitGameObject = fbxObject;

	auto animator = m_unitGameObject->GetComponent<yunutyEngine::graphics::Animator>();

	auto& animList = rsrcManager->GetAnimationList();

	for (auto each : animList)
	{
		if (each->GetName() == L"root|000.Idle")
		{
			m_idleAnimation = each;
			m_idleAnimation->SetLoop(true);
			animator->GetGI().PushAnimation(m_idleAnimation);
			animator->GetGI().Play(m_idleAnimation);
		}
		else if (each->GetName() == L"root|001-2.Walk")
		{
			m_walkAnimation = each;
			m_walkAnimation->SetLoop(true);
			animator->GetGI().PushAnimation(m_walkAnimation);
		}
		else if (each->GetName() == L"root|003-1.NormalAttack_L")
		{
			m_attackAnimation = each;
			m_attackAnimation->SetLoop(true);
			animator->GetGI().PushAnimation(m_attackAnimation);
		}
		else if (each->GetName() == L"root|012.Death")
		{
			m_deathAnimation = each;
			m_deathAnimation->SetLoop(false);
			animator->GetGI().PushAnimation(m_deathAnimation);
		}
	}
}

GameObject* WarriorProduction::CreateUnitWithOrder()
{
	auto unitGameObject = UnitProductionOrder::CreateUnitWithOrder();
	return unitGameObject;
}

