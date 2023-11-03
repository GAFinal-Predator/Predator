#include "Unit.h"
#include "Dotween.h"
#include "ProjectileSystem.h"
#include "Projectile.h"

void Unit::Start()
{
	unitType = "Enemy";
	m_speed = 3.0f;
	m_bulletSpeed = 5.1f;

	transitionTriggerVector.push_back(idleToChase);
	transitionTriggerVector.push_back(idleToAttack);
	transitionTriggerVector.push_back(chaseToIdle);
	transitionTriggerVector.push_back(attackToIdle);
	InitTriggers();

	unitFSM = new FSM<UnitState>(UnitState::Idle);
	 
	unitFSM->transitions[UnitState::Idle].push_back({ UnitState::Chase,
		[this]() { return idleToChase == true; } });

	unitFSM->transitions[UnitState::Idle].push_back({ UnitState::Attack,
		[this]() { return idleToAttack == true; } });

	unitFSM->transitions[UnitState::Chase].push_back({ UnitState::Idle,
		[this]() { return chaseToIdle == true; } });

	unitFSM->transitions[UnitState::Attack].push_back({ UnitState::Idle,
		[this]() { return attackToIdle == true; } });

	unitFSM->engageAction[UnitState::Idle] = [this]() { IdleEngage(); };
	unitFSM->engageAction[UnitState::Chase] = [this]() { ChaseEngage(); };
	unitFSM->engageAction[UnitState::Attack] = [this]() { AttackEngage(); };


	unitFSM->updateAction[UnitState::Chase] = [this]() { ChaseUpdate(); };
	unitFSM->updateAction[UnitState::Attack] = [this]() { AttackUpdate(); };

}

void Unit::Update()
{
	unitFSM->UpdateState();

	// �ν� ���� ���� ������ �ȴٸ�, ��ǥ�� �ϴ�(����) ������Ʈ�� ��ġ ������ ��� �޾ƿ;��Ѵ�.
	// �׷��� �Ǹ� �̵��߿� �ش� ���� ������Ʈ�� ��ġ�� �ٲ� �׿� �°� �ڿ������� �̵��� �� ���� ������?
	if (m_opponentGameobject != nullptr)
		m_opponentPosition = m_opponentGameobject->GetTransform()->GetWorldPosition();
}

#pragma region TransitionFuctions
	void Unit::IdleTransition()
	{
		// Chase�� �� ������, Attack���� �� ������ �Ǻ�
		// -> idleToChase�� idleToAttack �̳�!
		float distance = (m_opponentPosition - GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();

		if (distance < m_IdDistance && distance > m_AtkDistance)
			idleToChase = true;
		
		if (distance <= m_AtkDistance)
			idleToAttack = true;
		
	}

	void Unit::ChaseTransition()
	{
		chaseToIdle = true;
	}

	void Unit::AttackTransition()
	{
		attackToIdle = true;
	}
#pragma endregion

void Unit::SetOpponentGameObject(GameObject* obj)
{
	m_opponentGameobject = obj;
}

void Unit::EnterIDRange()
{
	isJustEntered = true;
}

void Unit::ExitIDRange()
{
	isJustEntered = false;
}

#pragma region State Engage Functions
	void Unit::IdleEngageFunction()
	{
		GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 0, 0, 0 });

		GetGameObject()->GetComponent<Dotween>()->StopAllDotweenFunction();
	
		IdleTransition();
	}

	void Unit::AttackEngageFunction()
	{
		// ������ �����ϴ� ����. ���� ���� or ���Ÿ� (����ü) ����
		GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0, 0, 1, 0 });

		// ����ü�� �����Ǿ� �ִٸ� ���Ÿ� ������ �Ѵ�.
		ProjectileSystem::GetInstance()->Shoot(this, m_opponentGameobject->GetComponent<Unit>(), m_bulletSpeed);

		/// ���� �� m_Delay�������� ��͸� ����, �ƴϸ� �Ѿư� ���� ���Ѵ�.
		GetGameObject()->GetComponent<Dotween>()->DONothing(0.5f/*������*/).OnComplete([this]()
			{
				float distance = (m_opponentGameobject->GetTransform()->GetWorldPosition() - GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();

				if (distance > m_AtkDistance)
				{
					AttackEngageFunction();
				}
				else
				{
					AttackTransition();
				}
			});
	}
#pragma endregion 

#pragma region State Update Functions
	void Unit::ChaseUpdateFunction()
	{
		GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0, 1, 0, 0 });

		Vector3d betweenVector = m_opponentGameobject->GetTransform()->GetWorldPosition() - GetGameObject()->GetTransform()->GetWorldPosition();
		Vector3d directionVector = betweenVector.Normalized();

		Vector3d endPosition = GetGameObject()->GetTransform()->GetWorldPosition() + (directionVector * m_speed * Time::GetDeltaTime());

		GetGameObject()->GetComponent<NavigationAgent>()->SetSpeed(1.0f);
		GetGameObject()->GetComponent<NavigationAgent>()->MoveTo(m_opponentPosition);

		float distance = (m_opponentPosition - GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();

		if (distance < m_AtkDistance)
		{
			ChaseTransition();
		}
		//else if (distance > m_IdDistance)
		//{
		//	ExitIDRangeTransition();
		//}
	}
#pragma endregion 

void Unit::IdleEngage()
{
	InitTriggers();
	IdleEngageFunction();
}

void Unit::ChaseEngage()
{
	InitTriggers();
}

void Unit::AttackEngage()
{
	InitTriggers();

	AttackEngageFunction();
}

void Unit::ChaseUpdate()
{
	ChaseUpdateFunction();
}

void Unit::AttackUpdate()
{
}

std::string Unit::GetType() const
{
	return unitType;
}

void Unit::SetIdRadius(float radius)
{
	m_IdDistance = radius;
}

void Unit::SetAtkRadius(float radius)
{
	m_AtkDistance = radius;
}

void Unit::InitTriggers()
{
	for (auto e : transitionTriggerVector)
	{
		e = false;
	}
}
