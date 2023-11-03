#include "Player.h"
#include "Dotween.h"

void Player::Start()
{
	unitType = "Player";
	m_speed = 3.0f;
	m_bulletSpeed = 5.1f;
	idleToChase = false;
	idleToAttack = false;
	chaseToIdle = false;
	attackToIdle = false;

	unitFSM = new FSM<UnitState>(UnitState::Idle);
	IdleEngage();		// ���� ���� �� �����Լ� ȣ�� ����... �׷��� �ϴ� ȣ�� ���� �����.

	unitFSM->transitions[UnitState::Idle].push_back({ UnitState::Chase,
		[this]() { return idleToChase == true; } });

	unitFSM->transitions[UnitState::Idle].push_back({ UnitState::Attack,
		[this]() { return idleToChase == true; } });

	unitFSM->transitions[UnitState::Chase].push_back({ UnitState::Idle,
		[this]() { return chaseToIdle == true; } });

	unitFSM->transitions[UnitState::Chase].push_back({ UnitState::Attack,
		[this]() { return idleToAttack == true; } });

	unitFSM->transitions[UnitState::Attack].push_back({ UnitState::Idle,
		[this]() { return attackToIdle == true; } });

	unitFSM->engageAction[UnitState::Idle] = [this]() { IdleEngage(); };
	unitFSM->engageAction[UnitState::Chase] = [this]() { ChaseEngage(); };
	unitFSM->engageAction[UnitState::Attack] = [this]() { AttackEngage(); };

	unitFSM->updateAction[UnitState::Chase] = [this]() { ChaseUpdate(); };
	unitFSM->updateAction[UnitState::Attack] = [this]() { AttackUpdate(); };
}

void Player::Update()
{
	unitFSM->UpdateState();

	// �ν� ���� ���� ������ �ȴٸ�, ��ǥ�� �ϴ�(����) ������Ʈ�� ��ġ ������ ��� �޾ƿ;��Ѵ�.
	// �׷��� �Ǹ� �̵��߿� �ش� ���� ������Ʈ�� ��ġ�� �ٲ� �׿� �°� �ڿ������� �̵��� �� ���� ������?
	if (m_opponentGameobject != nullptr)
		m_opponentPosition = m_opponentGameobject->GetTransform()->GetWorldPosition();
}

