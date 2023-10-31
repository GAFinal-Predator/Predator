#include "Unit.h"
#include "Dotween.h"
#include "ProjectileSystem.h"
#include "Projectile.h"

void Unit::Start()
{
	unitType = "Enemy";
	m_speed = 5.0f;
	m_bulletSpeed = 1.0f;
	isInIDRange = false;
	isInAtkRange = false;
	isOutIDRange = false;
	isOutAtkRange = false;
	
	unitFSM = new FSM<UnitState>(UnitState::Idle);

	unitFSM->transitions[UnitState::Idle].push_back({ UnitState::Chase,
		[this]() { return isInIDRange == true; } });

	unitFSM->transitions[UnitState::Chase].push_back({ UnitState::Attack,
		[this]() { return isInAtkRange == true; } });

	unitFSM->transitions[UnitState::Chase].push_back({ UnitState::Idle,
	[this]() { return isOutIDRange == true; } });

	unitFSM->transitions[UnitState::Attack].push_back({ UnitState::Chase,
	[this]() { return isOutAtkRange == true; } });

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
}

void Unit::IdentifyTransition()
{
	isInIDRange = true;			// FSM Transition Activated
}

void Unit::ExitIDRangeTransition()
{
	isOutIDRange = true;
}

void Unit::SetOpponentGameObject(GameObject* obj)
{
	m_opponentGameobject = obj;
}

void Unit::AttackTransition()
{
	isInAtkRange = true;
}

void Unit::IdleFunction()
{
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 0, 0, 0 });

	GetGameObject()->GetComponent<Dotween>()->StopAllDotweenFunction();
}

void Unit::AttackFunction()
{
	// ������ �����ϴ� ����. ���� ���� or ���Ÿ� (����ü) ����
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0, 0, 1, 0 });

	// ����ü�� �����Ǿ� �ִٸ� ���Ÿ� ������ �Ѵ�.
	ProjectileSystem::GetInstance()->Shoot(this, m_opponentGameobject->GetTransform()->GetWorldPosition(), m_bulletSpeed);

	/// ���� �� m_Delay�������� ��͸� ����, �ƴϸ� �Ѿư� ���� ���Ѵ�.
	GetGameObject()->GetComponent<Dotween>()->DONothing(1.5f/*������*/).OnComplete([this]()
		{
			float distance = (m_opponentGameobject->GetTransform()->GetWorldPosition() - GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();
			if (distance > m_AtkDistance)
			{
				isOutAtkRange = true;
			}
			else
			{
				AttackFunction();
			}
		});
}

void Unit::ChaseFunction()
{
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0, 1, 0, 0 });

	float distance = (m_opponentGameobject->GetTransform()->GetWorldPosition() - GetGameObject()->GetTransform()->GetTransform()->GetWorldPosition()).Magnitude();

	Vector3d betweenVector = m_opponentGameobject->GetTransform()->GetWorldPosition() - GetGameObject()->GetTransform()->GetWorldPosition();
	Vector3d directionVector = betweenVector.Normalized();

	Vector3d endPosition = GetGameObject()->GetTransform()->GetWorldPosition() + directionVector * (distance - m_AtkDistance);

	GetGameObject()->GetComponent<Dotween>()->DOMove(endPosition, distance / m_speed).OnComplete([=]()
		{
			float distance = (m_opponentGameobject->GetTransform()->GetWorldPosition() - GetGameObject()->GetTransform()->GetWorldPosition()).Magnitude();
			if (distance > m_AtkDistance)
			{
				ChaseFunction();
			}
			else
			{
				AttackTransition();
			}
		});
}

void Unit::IdleEngage()
{
	isOutIDRange = false;
	IdleFunction();
}

void Unit::ChaseEngage()
{
	isInIDRange = false;	// ���°� �ٲ�� �ش� ��� bool ������ �ٽ� �ʱ�ȭ ���ش�.
	isOutAtkRange = false;
	ChaseFunction();
}

void Unit::AttackEngage()
{
	isInAtkRange = false;
	AttackFunction();
}

void Unit::ChaseUpdate()
{
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
