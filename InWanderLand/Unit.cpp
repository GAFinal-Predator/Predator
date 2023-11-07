#include "Unit.h"
#include "Dotween.h"
#include "ProjectileSystem.h"
#include "Projectile.h"
#include "MoveDetector.h"

void Unit::Start()
{
	unitType = "Enemy";
	m_speed = 5.0f;
	m_bulletSpeed = 5.1f;

	unitFSM = new FSM<UnitState>(UnitState::Idle);
	isRechaseCompleted = true;

	unitFSM->transitions[UnitState::Idle].push_back({ UnitState::Chase,
		[this]() { return idleToChase == true; } });

	unitFSM->transitions[UnitState::Idle].push_back({ UnitState::Attack,
		[this]() { return idleToAttack == true; } });

	unitFSM->transitions[UnitState::Chase].push_back({ UnitState::Idle,
		[this]() { return chaseToIdle == true; } });

	unitFSM->transitions[UnitState::Chase].push_back({ UnitState::ReChase,
		[this]() { return isChaseRequested == true; } });

	unitFSM->transitions[UnitState::ReChase].push_back({ UnitState::Chase,
	[this]() { return isRechaseCompleted == true; } });

	unitFSM->transitions[UnitState::Attack].push_back({ UnitState::Idle,
		[this]() { return attackToIdle == true; } });

	for (int i = 1; i < UnitState::StateEnd - 1; i++)
	{
		unitFSM->transitions[UnitState(i)].push_back({ UnitState::Idle,
			[this]() {return initToIdle == true; } });
	}

	unitFSM->engageAction[UnitState::Idle] = [this]() { IdleEngage(); };
	unitFSM->engageAction[UnitState::Chase] = [this]() { ChaseEngage(); };
	unitFSM->engageAction[UnitState::Attack] = [this]() { AttackEngage(); };


	unitFSM->updateAction[UnitState::Idle] = [this]() { IdleUpdate(); };
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
		if (unitFSM->previousState == UnitState::Attack && !isJustEntered)
		{
			transitionDelay = 0.7f;
		}
		else
		{
			transitionDelay = 0.0f;
			isJustEntered = false;
		}

		GetGameObject()->GetComponent<Dotween>()->DONothing(transitionDelay).OnComplete([=]()
			{
				if (m_opponentGameobject != nullptr)
				{
					float distance = (m_opponentGameobject->GetTransform()->GetWorldPosition()
						- GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();

					if (distance < m_IdDistance && distance > m_AtkDistance)
						idleToChase = true;

					else if (distance < m_AtkDistance)
						idleToAttack = true;
				}
			});
	}

	void Unit::ChaseTransition()
	{
		chaseToIdle = true;
	}

	void Unit::AttackTransition()
	{
		attackToIdle = true;
	}

	/// <summary>
	/// ��� ������ �ν� ������ ������ ����� �� ȣ��Ǵ� �Լ�.
	/// ex) FSM �ʱ�ȭ, transitionDelay �ʱ�ȭ.
	/// </summary>
	void Unit::InitFSM()
	{
		transitionDelay = 0.0f;
		initToIdle = true;
	}

#pragma endregion

#pragma region State Engage Functions
	void Unit::IdleEngageFunction()
	{
		GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 0, 0, 0 });

		GetGameObject()->GetComponent<Dotween>()->StopAllDotweenFunction();

		IdleTransition();

		//GetGameObject()->GetComponent<Dotween>()->DONothing(0.5f /*������*/).OnComplete([=]()
		//	{
		//	});
	}

	void Unit::AttackEngageFunction()
	{
		GetGameObject()->GetComponent<NavigationAgent>()->MoveTo(GetGameObject()->GetTransform()->GetWorldPosition());
		// ������ �����ϴ� ����. ���� ���� or ���Ÿ� (����ü) ����
		GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0, 0, 1, 0 });

		// ����ü�� �����Ǿ� �ִٸ� ���Ÿ� ������ �Ѵ�.
		ProjectileSystem::GetInstance()->Shoot(this, m_opponentGameobject->GetComponent<Unit>(), m_bulletSpeed);

		/// ���� �� m_Delay�������� ��͸� ����, �ƴϸ� �Ѿư� ���� ���Ѵ�.
		GetGameObject()->GetComponent<Dotween>()->DONothing(0.5f/*������*/).OnComplete([this]()
			{
				float distance = (m_opponentGameobject->GetTransform()->GetWorldPosition() - GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();

				if (distance < m_AtkDistance)
				{
					AttackEngageFunction();
				}
				else
				{
					AttackTransition();
				}
			});
	}

	void Unit::ChaseEngageFunction()
	{
		GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0, 1, 0, 0 });

		MoveDetector::GetInstance()->TargetMove(this, m_opponentTargetPosition);

		Vector3d betweenVector = m_opponentGameobject->GetTransform()->GetWorldPosition() - GetGameObject()->GetTransform()->GetWorldPosition();
		Vector3d directionVector = betweenVector.Normalized();

		//Vector3d endPosition = GetGameObject()->GetTransform()->GetWorldPosition() + (directionVector * chaseSpeed * Time::GetDeltaTime());
		Vector3d endPosition = m_opponentGameobject->GetTransform()->GetWorldPosition();

		float distancePerFrame = (endPosition - GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();

		//GetGameObject()->GetComponent<NavigationAgent>()->SetSpeed(distancePerFrame / Time::GetDeltaTime());
		GetGameObject()->GetComponent<NavigationAgent>()->SetSpeed(1.0f);
		//GetGameObject()->GetComponent<NavigationAgent>()->SetAcceleration(1.0f);
		GetGameObject()->GetComponent<NavigationAgent>()->MoveTo(m_opponentTargetPosition);
		//GetGameObject()->GetTransform()->SetWorldPosition(endPosition);

		float distance = (m_opponentGameobject->GetTransform()->GetWorldPosition()
			- GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();
		//else if (distance > m_IdDistance)
		//{
		//	ExitIDRangeTransition();
		//}
	}
#pragma endregion 

void Unit::MoveDetect(Vector3d newPosition)
{
	m_opponentTargetPosition = newPosition;
	isChaseRequested = true;
}

void Unit::IdleEngage()
{
	initToIdle = false;
	chaseToIdle = false;
	attackToIdle = false;
	IdleEngageFunction();
}

void Unit::ChaseEngage()
{
	idleToChase = false;
	isChaseRequested = false;
	chaseSpeed = 10.0f;
	GetGameObject()->GetComponent<NavigationAgent>()->SetRadius(0.5f);
	ChaseEngageFunction();
}

void Unit::AttackEngage()
{
	idleToAttack = false;
	AttackEngageFunction();
}

void Unit::IdleUpdate()
{

}

void Unit::ChaseUpdate()
{
	float distance = (m_opponentGameobject->GetTransform()->GetWorldPosition()
		- GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();

	if (distance < m_AtkDistance)
	{
		ChaseTransition();
	}
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

void Unit::SetOpponentGameObject(GameObject* obj)
{
	m_opponentGameobject = obj;
}

void Unit::SetOpponentTargetPosition(Vector3d pos)
{
	m_opponentTargetPosition = pos;
}

void Unit::EnterIDRange()
{
	isJustEntered = true;
}

