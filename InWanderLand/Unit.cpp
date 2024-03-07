#include "InWanderLand.h"
#include "Unit.h"
#include "PlayerController.h"
#include "AttackSystem.h"
#include "InputManager.h"
#include "PlayerSkillSystem.h"
#include "Dotween.h"
#include "TacticModeSystem.h"


void Unit::Start()
{
	m_initialAutoAttackDamage = m_autoAttackDamage;
	m_bulletSpeed = 5.1f;
	chaseUpdateDelay = 0.1f;

	dotween = GetGameObject()->GetComponent<Dotween>();
	m_navAgentComponent = GetGameObject()->GetComponent<NavigationAgent>();
	m_animatorComponent = GetGameObject()->GetComponent<yunutyEngine::graphics::Animator>();

	unitFSM.transitions[UnitState::Idle].push_back({ UnitState::Move,
		[this]() { return currentOrder == UnitState::Move; } });

	unitFSM.transitions[UnitState::Idle].push_back({ UnitState::AttackMove,
		[this]() { return currentOrder == UnitState::AttackMove || (unitFSM.previousState == UnitState::Attack && isAttackMoving); } });

	unitFSM.transitions[UnitState::Idle].push_back({ UnitState::Chase,
		[this]() { return (m_currentTargetUnit != nullptr && idleElapsed >= idleToChaseDelay) && m_currentTargetUnit->currentOrder != UnitState::Death; } });

	unitFSM.transitions[UnitState::Move].push_back({ UnitState::Idle,
		[this]() { return (GetGameObject()->GetTransform()->GetWorldPosition() - m_currentMovePosition).Magnitude() < 0.3f; } });

	unitFSM.transitions[UnitState::Move].push_back({ UnitState::AttackMove,
		[this]() { return currentOrder == UnitState::AttackMove; } });

	unitFSM.transitions[UnitState::AttackMove].push_back({ UnitState::Idle,
		[this]() { return (GetGameObject()->GetTransform()->GetWorldPosition() - m_currentMovePosition).Magnitude() < 0.3f; } });

	unitFSM.transitions[UnitState::AttackMove].push_back({ UnitState::Move,
		[this]() { return currentOrder == UnitState::Move; } });

	unitFSM.transitions[UnitState::AttackMove].push_back({ UnitState::Chase,
		[this]() { return m_currentTargetUnit != nullptr; } });

	unitFSM.transitions[UnitState::Chase].push_back({ UnitState::Idle,
		[this]() { return m_currentTargetUnit == nullptr; } });

	unitFSM.transitions[UnitState::Chase].push_back({ UnitState::Move,
		[this]() { return currentOrder == UnitState::Move; } });

	unitFSM.transitions[UnitState::Chase].push_back({ UnitState::Attack,
		[this]() { return (GetGameObject()->GetTransform()->GetWorldPosition()
			- m_currentTargetUnit->GetTransform()->GetWorldPosition()).Magnitude() <= m_atkDistance + 0.4f; } });

	unitFSM.transitions[UnitState::Attack].push_back({ UnitState::Idle,
		[this]()
		{
			return m_currentTargetUnit == nullptr || 
				(GetGameObject()->GetTransform()->GetWorldPosition() - m_currentTargetUnit->GetTransform()->GetWorldPosition()).Magnitude() > m_atkDistance + 0.4f;
		} });

	unitFSM.transitions[UnitState::Attack].push_back({ UnitState::Move,
		[this]() { return currentOrder == UnitState::Move; } });

	unitFSM.transitions[UnitState::Skill].push_back({ UnitState::Idle,
		[this]() { return currentOrder == UnitState::Idle; } });

	for (int i = static_cast<int>(UnitState::Idle); i < static_cast<int>(UnitState::Skill); i++)
	{
		unitFSM.transitions[static_cast<UnitState>(i)].push_back({ UnitState::Skill,
		[this]() { return currentOrder == UnitState::Skill || (TacticModeSystem::SingleInstance().IsTacticModeActivated(this)
			&& TacticModeSystem::SingleInstance().isTacticModeStarted); } });
	}

	for (int i = static_cast<int>(UnitState::Idle); i < static_cast<int>(UnitState::Paralysis); i++)
	{
		unitFSM.transitions[static_cast<UnitState>(i)].push_back({ UnitState::Paralysis,
		[this]() { return currentOrder == UnitState::Paralysis; } });
	}

	unitFSM.transitions[static_cast<UnitState>(UnitState::Paralysis)].push_back({ UnitState::Idle,
		[this]() { return currentOrder == UnitState::Idle; } });

	for (int i = static_cast<int>(UnitState::Idle); i < static_cast<int>(UnitState::Death); i++)
	{
		unitFSM.transitions[static_cast<UnitState>(i)].push_back({ UnitState::Death,
		[this]() { return m_healthPoint <= 0; } });
	}

	unitFSM.engageAction[UnitState::Idle] = [this]() { IdleEngage(); };
	unitFSM.engageAction[UnitState::Move] = [this]() { MoveEngage(); };
	unitFSM.engageAction[UnitState::AttackMove] = [this]() { AttackMoveEngage(); };
	unitFSM.engageAction[UnitState::Chase] = [this]() { ChaseEngage(); };
	unitFSM.engageAction[UnitState::Attack] = [this]() { AttackEngage(); };
	unitFSM.engageAction[UnitState::Skill] = [this]() { SkillEngage(); };
	unitFSM.engageAction[UnitState::Paralysis] = [this]() { ParalysisEngage(); };
	unitFSM.engageAction[UnitState::Death] = [this]() { DeathEngage(); };

	unitFSM.updateAction[UnitState::Idle] = [this]() { IdleUpdate(); };
	unitFSM.updateAction[UnitState::Move] = [this]() { MoveUpdate(); };
	unitFSM.updateAction[UnitState::AttackMove] = [this]() { AttackMoveUpdate(); };
	unitFSM.updateAction[UnitState::Chase] = [this]() { ChaseUpdate(); };
	unitFSM.updateAction[UnitState::Attack] = [this]() { AttackUpdate(); };
	unitFSM.updateAction[UnitState::Skill] = [this]() { SkillUpdate(); };
	unitFSM.updateAction[UnitState::Death] = [this]() { DeathUpdate(); };

	m_animatorComponent->GetGI().Play(unitAnimations.m_idleAnimation);
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
	m_staticMeshRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color::white());
	m_animatorComponent->GetGI().ChangeAnimation(unitAnimations.m_idleAnimation, animationLerpDuration, animationTransitionSpeed);

	DetermineCurrentTargetObject();

	idleToChaseDelay = 0.0f;

	if (unitFSM.previousState == UnitState::Attack && m_currentTargetUnit != nullptr && !isJustHitByQSkill)
		idleToChaseDelay = 1.0f;

	if (unitFSM.previousState == UnitState::AttackMove)
		isAttackMoving = false;

	StopMove();
}

void Unit::MoveEngage()
{
	currentOrder = UnitState::Move;

	moveFunctionElapsed = 0.0f;

	Vector3d mouseXZVector = Vector3d(m_currentMovePosition.x, 0, m_currentMovePosition.z);

	//dotween->DOLookAt(mouseXZVector, rotationTime, false);

	m_staticMeshRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color::blue());

	m_navAgentComponent->SetSpeed(m_speed);

	m_animatorComponent->GetGI().ChangeAnimation(unitAnimations.m_walkAnimation, animationLerpDuration, animationTransitionSpeed);
}

void Unit::AttackMoveEngage()
{
	currentOrder = UnitState::AttackMove;

	moveFunctionElapsed = 0.0f;

	m_staticMeshRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color::gray());

	m_navAgentComponent->SetSpeed(m_speed);

	m_animatorComponent->GetGI().ChangeAnimation(unitAnimations.m_walkAnimation, animationLerpDuration, animationTransitionSpeed);
}

void Unit::AttackEngage()
{
	currentOrder = UnitState::Attack;

	m_staticMeshRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color::red());

	m_animatorComponent->GetGI().ChangeAnimation(unitAnimations.m_idleAnimation, animationLerpDuration, animationTransitionSpeed);

	attackFunctionElapsed = 0.0f;
	attackAnimationFrameCheckNumber = 0;
	isAttackStarted = false;
	dotween->DOLookAt(m_currentTargetUnit->GetTransform()->GetWorldPosition(), rotationTime, false);
	CheckCurrentAnimation(unitAnimations.m_idleAnimation);


	StopMove();
}

void Unit::ChaseEngage()
{
	currentOrder = UnitState::Chase;

	m_staticMeshRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color::green());

	dotween->DOLookAt(m_currentTargetUnit->GetTransform()->GetWorldPosition(), rotationTime, false);

	m_animatorComponent->GetGI().ChangeAnimation(unitAnimations.m_walkAnimation, animationLerpDuration, animationTransitionSpeed);
}

void Unit::SkillEngage()
{
	currentOrder = UnitState::Skill;
	qSkillFunctionStartElapsed = 0.0f;

	/// 전술모드 동작 여부를 확인한다
	if (TacticModeSystem::SingleInstance().IsTacticModeActivated(this))
	{
		TacticModeSystem::SingleInstance().CallQueueFunction(this);
	}

	GetGameObject()->GetComponent<PlayerSkillSystem>()->SkillActivate(m_currentSelectedSkill, m_currentSkillPosition);


	StopMove();
}

void Unit::ParalysisEngage()
{
	m_animatorComponent->GetGI().ChangeAnimation(unitAnimations.m_paralysisAnimation, animationLerpDuration, animationTransitionSpeed);
}

void Unit::DeathEngage()
{
	currentOrder = UnitState::Death;

	deathFunctionElapsed = 0.0f;

	m_animatorComponent->GetGI().ChangeAnimation(unitAnimations.m_deathAnimation, animationLerpDuration, animationTransitionSpeed);
	m_opponentObjectList.clear();

	ReportUnitDeath();

	StopMove();
}
#pragma endregion


#pragma region State Update()
void Unit::IdleUpdate()
{
	CheckCurrentAnimation(unitAnimations.m_idleAnimation);

	idleElapsed += Time::GetDeltaTime();

	// 데미지를 입으면 공격한 상대의 정보를 list에 등록하고 쫓아가기
}

void Unit::MoveUpdate()
{
	CheckCurrentAnimation(unitAnimations.m_walkAnimation);

	moveFunctionElapsed += Time::GetDeltaTime();

	//Vector3d mouseXZVector = Vector3d(m_currentMovePosition.x, 0, m_currentMovePosition.z);

	//LookAt(mouseXZVector);

	if (moveFunctionElapsed >= moveFunctionCallDelay)
	{
		moveFunctionElapsed = 0.0f;

		m_navAgentComponent->MoveTo(m_currentMovePosition);
	}
}

void Unit::AttackMoveUpdate()
{
	CheckCurrentAnimation(unitAnimations.m_walkAnimation);

	moveFunctionElapsed += Time::GetDeltaTime();

	Vector3d mouseXZVector = Vector3d(m_currentMovePosition.x, 0, m_currentMovePosition.z);

	//LookAt(mouseXZVector);

	if (moveFunctionElapsed >= moveFunctionCallDelay)
	{
		moveFunctionElapsed = 0.0f;

		m_navAgentComponent->MoveTo(m_currentMovePosition);
	}
}

void Unit::AttackUpdate()
{
	/// AttackState에서는 공격 후 다음 공격까지 남은 시간 동안 idleAnimation을 적용하므로 다른 방식으로 체크하는게 좋겠다.
	//CheckCurrentAnimation(m_attackAnimation);

	attackFunctionElapsed += Time::GetDeltaTime();

	//LookAt(m_currentTargetUnit->GetTransform()->GetWorldPosition());

	if (isAttackAnimationOperating)
	{
		attackAnimationFrameCheckNumber++;

		if (attackAnimationFrameCheckNumber >= unitAnimations.m_attackAnimation->GetTotalFrame())
		{
			m_animatorComponent->GetGI().ChangeAnimation(unitAnimations.m_idleAnimation, animationLerpDuration, animationTransitionSpeed);
			attackAnimationFrameCheckNumber = 0;
			isAttackAnimationOperating = false;
			dotween->DOLookAt(m_currentTargetUnit->GetTransform()->GetWorldPosition(), rotationTime, false);
		}
	}

	if (attackFunctionElapsed >= attackFunctionCallDelay/* || !isAttackStarted*/)
	{
		isAttackStarted = true;
		isAttackAnimationOperating = true;
		attackFunctionElapsed = 0.0f;
		m_animatorComponent->GetGI().ChangeAnimation(unitAnimations.m_attackAnimation, animationLerpDuration, animationTransitionSpeed);
		GetGameObject()->GetComponent<AttackSystem>()->Attack(m_currentTargetUnit);
		CheckCurrentAnimation(unitAnimations.m_attackAnimation);
	}
}

void Unit::SkillUpdate()
{
	qSkillFunctionStartElapsed += Time::GetDeltaTime();
	if (qSkillFunctionStartElapsed >= qSkillAnimationDuration)
	{
		isSkillStarted = false;
		currentOrder = UnitState::Idle;
		// 여기서 leftClickFunction을 스킬 사용 못하게 해야 한다....
		/// 전술모드 추가에 따른 조건식 추가.
		PlayerController::GetInstance()->SetLeftClickMove();
	}
}

void Unit::ChaseUpdate()
{
	CheckCurrentAnimation(unitAnimations.m_walkAnimation);

	chaseFunctionElapsed += Time::GetDeltaTime();

	//LookAt(m_currentTargetUnit->GetTransform()->GetWorldPosition());

	if (chaseFunctionElapsed >= chaseFunctionCallDelay)
	{
		chaseFunctionElapsed = 0.0f;

		DetermineCurrentTargetObject();

		m_navAgentComponent->MoveTo(m_currentTargetUnit->GetTransform()->GetWorldPosition());
	}
}

void Unit::DeathUpdate()
{
	CheckCurrentAnimation(unitAnimations.m_deathAnimation);

	deathFunctionElapsed += Time::GetDeltaTime();

	if (deathFunctionElapsed >= deathAnimationDelay)
	{
		deathFunctionElapsed = 0.0f;

		m_navAgentComponent->SetRadius(0.0f);
		m_navAgentComponent->SetActive(false);
		GetGameObject()->SetSelfActive(false);
	}
}
#pragma endregion

void Unit::CheckCurrentAnimation(yunuGI::IAnimation* currentStateAnimation)
{
	if (m_animatorComponent->GetGI().GetCurrentAnimation() != currentStateAnimation)
	{
		m_animatorComponent->GetGI().ChangeAnimation(currentStateAnimation, animationLerpDuration, animationTransitionSpeed);
	}
}

void Unit::StopMove()
{
	m_navAgentComponent->MoveTo(GetGameObject()->GetTransform()->GetWorldPosition());
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
	m_healthPoint = p_Hp;
}

void Unit::SetUnitAp(int p_Ap)
{
	m_autoAttackDamage = p_Ap;
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

void Unit::SetAttackDelay(float p_delay)
{
	attackFunctionCallDelay = p_delay;
}

Unit::UnitType Unit::GetPlayerSerialNumber() const
{
	return playerSerialNumber;
}

Unit::UnitState Unit::GetCurrentUnitState() const
{
	return currentOrder;
}

SkillPreviewSystem::SkillPreviewMesh Unit::GetSkillPreviewType(SkillEnum p_currentSkillType) const
{
	switch (p_currentSkillType)
	{
		case Unit::SkillEnum::Q:
			return m_qSkillPreviewType;
			break;
		case Unit::SkillEnum::W:
			return m_wSkillPreviewType;
			break;
		default:
			break;
	}
}

void Unit::SetCurrentOrderMove()
{
	currentOrder = UnitState::Move;
}

void Unit::SetCurrentOrderAttackMove()
{
	currentOrder = UnitState::AttackMove;
}

int Unit::GetUnitDamage() const
{
	return m_autoAttackDamage;
}

void Unit::Damaged(Unit* opponentUnit, float opponentDmg)
{
	AddToOpponentObjectList(opponentUnit);
	DetermineHitDamage(opponentDmg);
	m_healthPoint -= m_finalHitDamage;
	// ui로 표시되는, 혹은 최종 남은 체력은 반올림할 것인가 혹은 내림할 것인가는 아래에 구현.
}

void Unit::Damaged(float dmg)
{
	//DetermineHitDamage(dmg);
	//m_healthPoint -= m_finalHitDamage;
	m_healthPoint -= dmg;
}

void Unit::Heal(float healingPoint)
{
	// 최대 체력이면 x
	m_healthPoint += healingPoint;
}

void Unit::IncreaseAttackPower(float p_attackPowerIncrease)
{
	/// 우선은 기본 공격력만 증가하는 로직입니다.
	if(m_autoAttackDamage + p_attackPowerIncrease <= m_maxAutoAttackDamage)
		m_autoAttackDamage += p_attackPowerIncrease;
	else
	{
		m_autoAttackDamage = m_maxAutoAttackDamage;
	}
}

void Unit::IncreaseAttackSpeed(float p_attackSpeedIncrease)
{
	if (m_minAttackDelay < attackFunctionCallDelay / p_attackSpeedIncrease)
	{
		attackFunctionCallDelay /= p_attackSpeedIncrease;
	}

	else
	{
		attackFunctionCallDelay = m_minAttackDelay;
	}
}

void Unit::SetUnitDamageToZero(bool p_bool)
{
	if (p_bool == true)
	{
		m_beforeBlindDamage = m_autoAttackDamage;
		m_autoAttackDamage = 0;
	}

	else
	{
		m_autoAttackDamage = m_beforeBlindDamage;
	}
}

void Unit::MultipleUnitSpeed(float p_mul)
{
	m_speed *= p_mul;
}

float Unit::DetermineAttackDamage(float p_damage)
{
	m_finalAttackDamage = p_damage;

	std::srand(std::time(0));

	m_randcriticalHitNumber = std::rand() % 100 + 1;
	if (m_randcriticalHitNumber <= m_criticalHitProbability)
	{
		m_finalAttackDamage *= m_criticalHitMultiplier;
	}

	return m_finalAttackDamage;
}

void Unit::SetStaticMeshComponent(yunutyEngine::graphics::StaticMeshRenderer* p_stcMesh)
{
	m_staticMeshRenderer = p_stcMesh;
}

void Unit::ChangeCurrentOpponentUnitForced(Unit* p_unit)
{
	m_currentTargetUnit = p_unit;
}

void Unit::DetermineHitDamage(float p_onceCalculatedDmg)
{
	m_finalHitDamage = (m_defensePoint / 10.0f) / (1 - m_criticalDamageDecreaseMultiplier) / (1 - m_dodgeProbability);
}

void Unit::DetermineCurrentTargetObject()
{
	bool isDistanceComparingStarted = false;

	float tempShortestDistance = 0.0f;

	for (auto e : m_opponentObjectList)
	{
		float distance = (GetGameObject()->GetTransform()->GetWorldPosition() - e->GetTransform()->GetWorldPosition()).Magnitude();

		if ((!isDistanceComparingStarted || tempShortestDistance > distance) && e->currentOrder != UnitState::Death)
		{
			tempShortestDistance = distance;
			m_currentTargetUnit = e;
			isDistanceComparingStarted = true;
		}
	}
}

void Unit::ReportUnitDeath()
{
	for (auto e : m_recognizedThisList)
	{
		// 죽은 유닛이 아닌 죽은 유닛을 list에 갖고 있는 유닛의 함수 호출
		e->IdentifiedOpponentDeath(this);
	}
}

void Unit::IdentifiedOpponentDeath(Unit* p_unit)
{
	/// 죽은 유닛이 현재 타겟으로 지정한 유닛이라면
	if (m_currentTargetUnit == p_unit)
		m_currentTargetUnit = nullptr;

	/// 적군을 담고 있는 list에서 죽은 오브젝트 유닛을 빼준다.
	m_opponentObjectList.remove(p_unit);
}

void Unit::SetPlayerSerialNumber(UnitType serialNum)
{
	playerSerialNumber = serialNum;
}

void Unit::OrderMove(Vector3d position)
{
	m_currentMovePosition = position;
	currentOrder = UnitState::Move;
	dotween->DOLookAt(position, rotationTime, false);
}

void Unit::OrderAttackMove(Vector3d position)
{
	m_currentMovePosition = position;
	currentOrder = UnitState::AttackMove;
	dotween->DOLookAt(position, rotationTime, false);

	PlayerController::GetInstance()->SetLeftClickMove();
	// 다음 클릭은 Move로 바꿀 수 있도록 function 재정의.

	isAttackMoving = true;
}

void Unit::OrderSkill(SkillEnum p_skillNum, Vector3d position)
{
	currentOrder = UnitState::Skill;
	m_skillPosition = position;
	m_currentSelectedSkill = p_skillNum;
	dotween->DOLookAt(position, rotationTime, false);

	PlayerController::GetInstance()->SetLeftClickEmpty();

	m_currentSkillPosition = position;
}

void Unit::OrderSkill(SkillEnum p_skillNum)
{
	/// warrior 2nd active skill 처럼 캐릭터의 회전이 필요 없는 스킬
	currentOrder = UnitState::Skill;
	m_currentSelectedSkill = p_skillNum;

	PlayerController::GetInstance()->SetLeftClickEmpty();
}

void Unit::SetSkillDuration(float p_duration)
{
	qSkillAnimationDuration = p_duration;
}

void Unit::SetSkillPreviewType(SkillPreviewSystem::SkillPreviewMesh p_qskill, SkillPreviewSystem::SkillPreviewMesh p_wskill)
{
	m_qSkillPreviewType = p_qskill;
	m_wSkillPreviewType = p_wskill;
}

void Unit::SetMaxAggroNumber(int p_num)
{
	m_maxAggroNumber = p_num;
}

void Unit::AddToOpponentObjectList(Unit* p_unit)
{
	if (p_unit->currentOrder != UnitState::Death && this->currentOrder != UnitState::Death && p_unit->m_currentAggroNumber < p_unit->m_maxAggroNumber)
	{
		m_opponentObjectList.push_back(p_unit);
		p_unit->m_currentAggroNumber++;

		if (m_currentTargetUnit == nullptr)
			m_currentTargetUnit = p_unit;

		p_unit->AddToRecognizeList(this);
	}
}

void Unit::DeleteFromOpponentObjectList(Unit* p_unit)
{
	m_opponentObjectList.remove(p_unit);
	p_unit->m_currentAggroNumber--;
	assert(p_unit->m_currentAggroNumber >= 0);

	if (m_currentTargetUnit == p_unit)
		m_currentTargetUnit = nullptr;

	p_unit->DeleteFromRecognizeList(this);
}

void Unit::AddToRecognizeList(Unit* p_unit)
{
	m_recognizedThisList.push_back(p_unit);
}

void Unit::DeleteFromRecognizeList(Unit* p_unit)
{
	m_recognizedThisList.remove(p_unit);
}

void Unit::SetNavField(NavigationField* p_navField)
{
	m_unitNavField = p_navField;
}

NavigationField*  Unit::GetNavField() const
{
	return m_unitNavField;
}

void Unit::MakeUnitPushedState(bool p_isCrushed)
{
	isJustHitByQSkill = p_isCrushed;
}

void Unit::MakeUnitParalysisState()
{
	currentOrder = UnitState::Paralysis;
}

void Unit::MakeUnitParalysisEnd()
{
	currentOrder = UnitState::Idle;
}

bool Unit::GetJustCrushedState() const
{
	return isJustHitByQSkill;
}

bool Unit::IsUnitDead() const
{
	if (m_healthPoint <= 0)
		return true;

	else
		return false;
}

