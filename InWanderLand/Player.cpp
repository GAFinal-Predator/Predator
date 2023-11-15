#include "Player.h"
#include "Dotween.h"
#include "RTSCam.h"

void Player::Start()
{
	unitType = "Player";
	m_speed = 2.0f;
	m_bulletSpeed = 5.1f;


	idleToChase = false;
	idleToAttack = false;
	chaseToIdle = false;
	attackToIdle = false;
	changeToMoveState = false;

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

	for (int i = 1; i < UnitState::StateEnd - 1; i++)
	{
		if (i != UnitState::Death)
		{
			unitFSM->transitions[UnitState(i)].push_back({ UnitState::Move,
			[this]() {return changeToMoveState == true; } });
		}
	}

	unitFSM->engageAction[UnitState::Idle] = [this]() { IdleEngage(); };
	unitFSM->engageAction[UnitState::Move] = [this]() { MoveEngage(); };
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
	if (!(m_opponentGameObjectList.empty()))
	{
		m_currentTargetObject = m_opponentGameObjectList.front();
		m_currentTargetPosition = m_currentTargetObject->GetTransform()->GetWorldPosition();
	}

	rtsCamComponent->groundLeftClickCallback = [=](Vector3d position)
	{
		m_movePosition = position;
		changeToMoveState = true;
	};
}

void Player::SetMovingSystemComponent(RTSCam* sys)
{
	rtsCamComponent = sys;
}

void Player::MoveEngage()
{
	changeToMoveState = false;
	MoveStateFunction();
}

void Player::MoveStateFunction()
{
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 1, 1, 0 });

	GetGameObject()->GetComponent<NavigationAgent>()->MoveTo(m_movePosition);

}

