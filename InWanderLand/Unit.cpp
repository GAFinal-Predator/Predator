#include "Unit.h"
#include "ProjectileSystem.h"
#include "PlayerController.h"
#include "InputManager.h"

void Unit::Start()
{
	m_speed = 2.0f;
	m_bulletSpeed = 5.1f;

	chaseUpdateDelay = 0.1f;

	unitFSM.transitions[UnitState::Idle].push_back({ UnitState::Move,
		[this]() { return currentOrder == UnitState::Move; } });

	unitFSM.transitions[UnitState::Idle].push_back({ UnitState::AttackMove,
		[this]() { return currentOrder == UnitState::AttackMove || (unitFSM.previousState == UnitState::Attack && isAttackMoving); } });

	unitFSM.transitions[UnitState::Idle].push_back({ UnitState::Chase,
		[this]() { return (m_currentTargetObject != nullptr && idleElapsed >= idleToChaseDelay) && m_currentTargetObject->GetComponent<Unit>()->currentOrder != UnitState::Death; } });

	unitFSM.transitions[UnitState::Move].push_back({ UnitState::Idle,
		[this]() { return (GetGameObject()->GetTransform()->GetWorldPosition() - m_currentMovePosition).Magnitude() < 0.3f; } });

	unitFSM.transitions[UnitState::Move].push_back({ UnitState::AttackMove,
		[this]() { return currentOrder == UnitState::AttackMove; } });

	unitFSM.transitions[UnitState::AttackMove].push_back({ UnitState::Idle,
		[this]() { return (GetGameObject()->GetTransform()->GetWorldPosition() - m_currentMovePosition).Magnitude() < 0.3f; } });

	unitFSM.transitions[UnitState::AttackMove].push_back({ UnitState::Move,
		[this]() { return currentOrder == UnitState::Move; } });

	unitFSM.transitions[UnitState::AttackMove].push_back({ UnitState::Chase,
		[this]() { return m_currentTargetObject != nullptr; } });

	unitFSM.transitions[UnitState::Chase].push_back({ UnitState::Idle,
		[this]() { return m_currentTargetObject == nullptr; } });

	unitFSM.transitions[UnitState::Chase].push_back({ UnitState::Move,
		[this]() { return currentOrder == UnitState::Move; } });

	unitFSM.transitions[UnitState::Chase].push_back({ UnitState::Attack,
		[this]() { return (GetGameObject()->GetTransform()->GetWorldPosition() - m_currentTargetObject->GetTransform()->GetWorldPosition()).Magnitude() <= m_atkDistance + 0.4f; } });

	unitFSM.transitions[UnitState::Attack].push_back({ UnitState::Idle,
		[this]() { return m_currentTargetObject == nullptr; } });

	unitFSM.transitions[UnitState::Attack].push_back({ UnitState::Move,
		[this]() { return currentOrder == UnitState::Move; } });

	unitFSM.transitions[UnitState::QSkill].push_back({ UnitState::Idle,
		[this]() { return currentOrder == UnitState::Idle; } });

	for (int i = static_cast<int>(UnitState::Idle); i < static_cast<int>(UnitState::QSkill); i++)
	{
		unitFSM.transitions[static_cast<UnitState>(i)].push_back({ UnitState::QSkill,
		[this]() { return currentOrder == UnitState::QSkill; } });
	}

	for (int i = static_cast<int>(UnitState::Idle); i < static_cast<int>(UnitState::Death); i++)
	{
		unitFSM.transitions[static_cast<UnitState>(i)].push_back({ UnitState::Death,
		[this]() { return m_hp <= 0; } });
	}

	unitFSM.engageAction[UnitState::Idle] = [this]() { IdleEngage(); };
	unitFSM.engageAction[UnitState::Move] = [this]() { MoveEngage(); };
	unitFSM.engageAction[UnitState::AttackMove] = [this]() { AttackMoveEngage(); };
	unitFSM.engageAction[UnitState::Chase] = [this]() { ChaseEngage(); };
	unitFSM.engageAction[UnitState::Attack] = [this]() { AttackEngage(); };
	unitFSM.engageAction[UnitState::QSkill] = [this]() { QSkillEngage(); };
	unitFSM.engageAction[UnitState::Death] = [this]() { DeathEngage(); };

	unitFSM.updateAction[UnitState::Idle] = [this]() { IdleUpdate(); };
	unitFSM.updateAction[UnitState::Move] = [this]() { MoveUpdate(); };
	unitFSM.updateAction[UnitState::AttackMove] = [this]() { AttackMoveUpdate(); };
	unitFSM.updateAction[UnitState::Chase] = [this]() { ChaseUpdate(); };
	unitFSM.updateAction[UnitState::Attack] = [this]() { AttackUpdate(); };
	unitFSM.updateAction[UnitState::QSkill] = [this]() { QSkillUpdate(); };
	unitFSM.updateAction[UnitState::Death] = [this]() { DeathUpdate(); };
}

void Unit::Update()
{
	unitFSM.UpdateState();
}

Unit::UnitType Unit::GetUnitType() const
{
	return m_unitType;
}

Unit::UnitSide Unit::GetUnitSide() const
{
	return m_unitSide;
}

#pragma region State Engage()
void Unit::IdleEngage()
{
	currentOrder = UnitState::Idle;
	idleElapsed = 0.0f;
	IdleEngageFunction();
}

void Unit::MoveEngage()
{
	currentOrder = UnitState::Move;

	moveFunctionElapsed = 0.0f;

	GetGameObject()->GetComponent<NavigationAgent>()->SetSpeed(m_speed);

	MoveEngageFunction();
}

void Unit::AttackMoveEngage()
{
	currentOrder = UnitState::AttackMove;

	moveFunctionElapsed = 0.0f;

	GetGameObject()->GetComponent<NavigationAgent>()->SetSpeed(m_speed);

	AttackMoveEngageFunction();
}

void Unit::AttackEngage()
{
	currentOrder = UnitState::Attack;

	attackFunctionElapsed = 0.0f;
	isAttackStarted = false;

	AttackEngageFunction();
}

void Unit::ChaseEngage()
{
	currentOrder = UnitState::Chase;

	ChaseEngageFunction();
}

void Unit::QSkillEngage()
{
	currentOrder = UnitState::QSkill;
	qSkillFunctionStartElapsed = 0.0f;
	qSkillFunctionStartedElapsed = 0.0f;

	//GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().SetCurrentFrame(0);
	//GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().Play(m_deathAnimation);

	StopMove();
}

void Unit::DeathEngage()
{
	currentOrder = UnitState::Death;

	deathFunctionElapsed = 0.0f;

	DeathEngageFunction();
}
#pragma endregion


#pragma region State Update()
void Unit::IdleUpdate()
{
	idleElapsed += Time::GetDeltaTime();

	IdleUpdateFunction();
	// �������� ������ ������ ����� ������ list�� ����ϰ� �Ѿư���
}

void Unit::MoveUpdate()
{
	moveFunctionElapsed += Time::GetDeltaTime();

	Vector3d mouseXZVector = Vector3d(m_currentMovePosition.x, 0, m_currentMovePosition.z);

	LookAt(mouseXZVector);

	if (moveFunctionElapsed >= moveFunctionCallDelay)
		MoveUpdateFunction();
}

void Unit::AttackMoveUpdate()
{
	moveFunctionElapsed += Time::GetDeltaTime();

	Vector3d mouseXZVector = Vector3d(m_currentMovePosition.x, 0, m_currentMovePosition.z);

	LookAt(mouseXZVector);

	if (moveFunctionElapsed >= moveFunctionCallDelay)
		AttackMoveUpdateFunction();
}

void Unit::AttackUpdate()
{
	attackFunctionElapsed += Time::GetDeltaTime();

	LookAt(m_currentTargetObject->GetTransform()->GetWorldPosition());

	if (attackFunctionElapsed >= attackFunctionCallDelay)
	{
		if (!isAttackStarted)
		{
			GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().SetCurrentFrame(0);
			GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().Play(m_attackAnimation);
			isAttackStarted = true;
		}
		AttackUpdateFunction();
	}
}

void Unit::QSkillUpdate()
{
	qSkillFunctionStartElapsed += Time::GetDeltaTime();

	if (isSkillStarted)
	{
		qSkillFunctionStartedElapsed += Time::GetDeltaTime();
		if (qSkillFunctionStartedElapsed >= qSkillAnimationDuration)
		{
			isSkillStarted = false;
			currentOrder = UnitState::Idle;
			// ���⼭ leftClickFunction�� ��ų ��� ���ϰ� �ؾ� �Ѵ�....
			PlayerController::GetInstance()->SetLeftClickMove();
		}
	}

	if (qSkillFunctionStartElapsed >= qSkillStartDelay)
		QSkillUpdateFunction();
}

void Unit::ChaseUpdate()
{
	chaseFunctionElapsed += Time::GetDeltaTime();

	LookAt(m_currentTargetObject->GetTransform()->GetWorldPosition());

	if (chaseFunctionElapsed >= chaseFunctionCallDelay)
		ChaseUpdateFunction();
}

void Unit::DeathUpdate()
{
	deathFunctionElapsed += Time::GetDeltaTime();

	if (deathFunctionElapsed >= deathAnimationDelay)
		DeathUpdateFunction();
}
#pragma endregion


#pragma region Engage Functions
/// <summary>
/// �ֿ� ��� : m_currentTargetObject�� nullptr�� ���·� ������ ��Ȳ�� ���� ������.
/// </summary>
void Unit::IdleEngageFunction()
{
	GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().SetCurrentFrame(0);
	GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().Play(m_idleAnimation);

	DetermineCurrentTargetObject();

	idleToChaseDelay = 0.0f;

	if (unitFSM.previousState == UnitState::Attack && m_currentTargetObject == nullptr)
		idleToChaseDelay = 1.0f;

	if (unitFSM.previousState == UnitState::AttackMove)
		isAttackMoving = false;

	StopMove();
}

void Unit::MoveEngageFunction()
{
	GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().SetCurrentFrame(0);

	GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().Play(m_walkAnimation);
}

void Unit::AttackMoveEngageFunction()
{
	GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().SetCurrentFrame(0);

	GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().Play(m_walkAnimation);
}

void Unit::ChaseEngageFunction()
{
	GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().SetCurrentFrame(0);

	GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().Play(m_walkAnimation);
}

void Unit::AttackEngageFunction()
{
	StopMove();
	
	//// Idle Animation ���� �� Delay�ְ� ���
	//GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().SetCurrentFrame(0);
	//GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().Play(m_idleAnimation);

	// �ٷ� ���
	ProjectileSystem::GetInstance()->Shoot(this, m_currentTargetObject->GetComponent<Unit>(), m_bulletSpeed);
	GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().SetCurrentFrame(0);
	GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().Play(m_attackAnimation);

}

void Unit::DeathEngageFunction()
{
	GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().SetCurrentFrame(0);
	GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>()->GetGI().Play(m_deathAnimation);

	m_opponentObjectList.clear();

	ReportUnitDeath();

	StopMove();
}
#pragma endregion

#pragma region Update Functions
void Unit::IdleUpdateFunction()
{

}

void Unit::MoveUpdateFunction()
{
	moveFunctionElapsed = 0.0f;

	GetGameObject()->GetComponent<NavigationAgent>()->MoveTo(m_currentMovePosition);
}

void Unit::AttackMoveUpdateFunction()
{
	moveFunctionElapsed = 0.0f;

	GetGameObject()->GetComponent<NavigationAgent>()->MoveTo(m_currentMovePosition);
}

void Unit::ChaseUpdateFunction()
{
	chaseFunctionElapsed = 0.0f;

	DetermineCurrentTargetObject();

	GetGameObject()->GetComponent<NavigationAgent>()->MoveTo(m_currentTargetObject->GetTransform()->GetWorldPosition());
}

void Unit::AttackUpdateFunction()
{
	attackFunctionElapsed = 0.0f;

	//if (m_currentTargetObject->GetComponent<Unit>()->GetUnitCurrentState() == UnitState::Death)

	//if(m_attackType == AttackType::Melee)

	ProjectileSystem::GetInstance()->Shoot(this, m_currentTargetObject->GetComponent<Unit>(), m_bulletSpeed);
}

void Unit::QSkillUpdateFunction()
{
	qSkillFunctionStartElapsed = 0.0f;
	isSkillStarted = true;

	// ��ų ������Ʈ ���� (���� ��ų ���)

}

void Unit::DeathUpdateFunction()
{
	deathFunctionElapsed = 0.0f;

	GetGameObject()->GetComponent<NavigationAgent>()->SetRadius(0.0f);
	GetGameObject()->GetComponent<NavigationAgent>()->SetActive(false);
	GetGameObject()->SetSelfActive(false);
	//GetGameObject()->SetSelfActive(false);
}

#pragma endregion

void Unit::StopMove()
{
	GetGameObject()->GetComponent<NavigationAgent>()->MoveTo(GetGameObject()->GetTransform()->GetWorldPosition());
}

void Unit::SetUnitType(UnitType type)
{
	m_unitType = type;
}

void Unit::SetUnitSide(UnitSide side)
{
	m_unitSide = side;
}

void Unit::SetUnitHp(int p_Hp)
{
	m_hp = p_Hp;
}

void Unit::SetUnitAp(int p_Ap)
{
	m_ap = p_Ap;
}

void Unit::SetIdRadius(float radius)
{
	m_idDistance = radius;
}

void Unit::SetAtkRadius(float radius)
{
	m_atkDistance = radius;
}

void Unit::SetUnitSpeed(float speed)
{
	m_speed = speed;
}

void Unit::SetIdleAnimation(IAnimation* idleAnim)
{
	m_idleAnimation = idleAnim;
}

void Unit::SetWalkAnimation(IAnimation* walkAnim)
{
	m_walkAnimation = walkAnim;
}

void Unit::SetAttackAnimation(IAnimation* attackAnim)
{
	m_attackAnimation = attackAnim;
}

void Unit::SetDeathAnimation(IAnimation* deathAnim)
{
	m_deathAnimation = deathAnim;
}

void Unit::LookAt(Vector3d destination)
{
	// ����, ���� �Ǻ�.
	Vector3d tempDistanceVec = destination - GetGameObject()->GetTransform()->GetWorldPosition();
	Vector3d distanceVec = Vector3d(tempDistanceVec.x, 0, tempDistanceVec.z);
	bool isUnitFliped = false;
	Vector3d forwardVector = GetGameObject()->GetTransform()->GetWorldRotation().Forward();
	forwardVector.y = 0.0f;
	forwardVector *= -1;

	Vector3d axis = Vector3d::Cross(forwardVector, distanceVec);

	float localRotationSpeed = rotationSpeed;

	if (axis.y < 0)
	{
		localRotationSpeed *= -1;
		//i	sUnitFliped = true;
	}
	
	// �������� �ݴ� ������ ������ ��� ���� bool�� ����
	float dotted = Vector3d::Dot(forwardVector, distanceVec);
	if (dotted < 0)
	{
		isUnitFliped = true;
	}
	else
	{
		isUnitFliped = false;
	}

	// ���� ���� - forward�� distance�� ������� ������ ���� �ȴٸ� ��������.
	if (axis.Magnitude() >= 0.5 || isUnitFliped == true)
	{
		currentRotation += localRotationSpeed * Time::GetDeltaTime();
		GetGameObject()->GetTransform()->rotation = Quaternion({ 0, currentRotation, 0 });
	}

	//Vector3d directionVector = (destination - GetGameObject()->GetTransform()->GetWorldPosition()).Normalized();
	//Vector3d finalDirectionVector = Vector3d(directionVector.x, GetTransform()->GetWorldPosition().y, directionVector.z);

	//GetGameObject()->GetTransform()->SetWorldRotation(Quaternion::MakeWithForwardUp(-1 * finalDirectionVector, GetTransform()->GetWorldRotation().Up()));
}

int Unit::GetPlayerSerialNumber() const
{
	return playerSerialNumber;
}

void Unit::SetCurrentOrderMove()
{
	currentOrder = UnitState::Move;
}

void Unit::SetCurrentOrderAttackMove()
{
	currentOrder = UnitState::AttackMove;
}

int Unit::GetUnitAp() const
{
	return m_ap;
}

void Unit::Damaged(GameObject* opponentObject, int opponentAp)
{
	if (currentOrder == UnitState::Idle)
		AddToOpponentObjectList(opponentObject);

	m_hp -= opponentAp;
}

void Unit::DetermineCurrentTargetObject()
{
	bool isDistanceComparingStarted = false;

	float tempShortestDistance = 0.0f;

	for (auto e : m_opponentObjectList)
	{
		float distance = (GetGameObject()->GetTransform()->GetWorldPosition() - e->GetTransform()->GetWorldPosition()).Magnitude();

		if ((!isDistanceComparingStarted || tempShortestDistance > distance) && e->GetComponent<Unit>()->currentOrder != UnitState::Death)
		{
			tempShortestDistance = distance;
			m_currentTargetObject = e;
			isDistanceComparingStarted = true;
		}
	}
}

void Unit::ReportUnitDeath()
{
	for (auto e : m_recognizedThisList)
	{
		// ���� ������ �ƴ� ���� ������ list�� ���� �ִ� ������ �Լ� ȣ��
		e->IdentifiedOpponentDeath(this->GetGameObject());
	}
}

void Unit::IdentifiedOpponentDeath(yunutyEngine::GameObject* diedOpponent)
{
	/// ���� ������ ���� Ÿ������ ������ �����̶��
	if (m_currentTargetObject == diedOpponent)
		m_currentTargetObject = nullptr;

	/// ������ ��� �ִ� list���� ���� ������Ʈ ������ ���ش�.
	m_opponentObjectList.remove(diedOpponent);
}

void Unit::SetPlayerSerialNumber()
{
	static int localSerialNumber = 1;
	playerSerialNumber = localSerialNumber;
	localSerialNumber++;
}

void Unit::OrderMove(Vector3d position)
{
	m_currentMovePosition = position;
	currentOrder = UnitState::Move;
}

void Unit::OrderAttackMove(Vector3d position)
{
	m_currentMovePosition = position;
	currentOrder = UnitState::AttackMove;

	PlayerController::GetInstance()->SetLeftClickMove();
	// ���� Ŭ���� Move�� �ٲ� �� �ֵ��� function ������.

	isAttackMoving = true;
}

void Unit::OrderQSkill(Vector3d position)
{
	currentOrder = UnitState::QSkill;

	PlayerController::GetInstance()->SetLeftClickMove();

	m_currentSkillPosition = position;
}

void Unit::AddToOpponentObjectList(yunutyEngine::GameObject* opponent)
{
	if (opponent->GetComponent<Unit>()->currentOrder != UnitState::Death && this->currentOrder != UnitState::Death)
	{
		m_opponentObjectList.push_back(opponent);

		if (m_currentTargetObject == nullptr)
			m_currentTargetObject = opponent;

		opponent->GetComponent<Unit>()->m_recognizedThisList.push_back(this);
	}
}

void Unit::DeleteFromOpponentObjectList(yunutyEngine::GameObject* obj)
{
	m_opponentObjectList.remove(obj);

	if (m_currentTargetObject == obj)
		m_currentTargetObject = nullptr;

	obj->GetComponent<Unit>()->m_recognizedThisList.remove(this);
}

