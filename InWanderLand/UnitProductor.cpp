#include "InWanderLand.h"
#include "UnitProductor.h"
#include "RangeSystem.h"
#include "PlayerController.h"
#include "Dotween.h"
#include "DebugMeshes.h"
#include "Unit_TemplateData.h"
#include "SingleNavigationField.h"

void UnitProductor::SetUnitComponentMembers()
{
	//m_unitGameObject->GetTransform()->SetWorldPosition(m_startPosition);
	auto frontDebugObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	AttachDebugMesh(frontDebugObject, DebugMeshType::Cube, yunuGI::Color::black(), true);
	frontDebugObject->GetTransform()->SetLocalScale({ 0.5, 0.5, 0.5 });
	frontDebugObject->GetTransform()->SetWorldPosition(m_unitGameObject->GetTransform()->GetLocalRotation().Forward() * -1);
	frontDebugObject->SetParent(m_unitGameObject);

	/// Unit Member Setting
	m_unitComponent->GetGameObject()->setName(m_objectName);
	m_unitComponent->SetUnitType(m_unitType);
	m_unitComponent->SetUnitSide(m_unitSide);

	m_unitComponent->SetUnitMaxHp(m_healthPoint);
	m_unitComponent->SetUnitAp(m_autoAttackDamage);

	m_unitComponent->SetAtkRadius(m_atkRadius);
	m_unitComponent->SetIdRadius(m_idRadius);
	m_unitComponent->SetUnitSpeed(m_unitSpeed);

	m_unitComponent->unitAnimations = m_baseUnitAnimations;

	m_unitComponent->SetAttackDelay(m_attackDelay);

	m_unitComponent->SetMaxAggroNumber(m_maxAggroNumber);

	m_unitComponent->SetFbxName(m_unitFbxName);

	///// + 플레이어 유닛일 경우 특수 처리
	//if (m_unitSide == Unit::UnitSide::Player)
	//	SetPlayerRelatedComponents(m_unitComponent);
}

void UnitProductor::AddRangeSystemComponent() const
{
	auto unitRangeSystemObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();

	// 2-1. RangeSystem Component
	RangeSystem* rangeSystemComponent = unitRangeSystemObject->AddComponent<RangeSystem>();
	rangeSystemComponent->SetOwnerUnitComponent(m_unitComponent);

	// 2-2. RangeSystem Collider
	auto rangesystemCollider = unitRangeSystemObject->AddComponent<physics::SphereCollider>();
	rangesystemCollider->SetRadius(m_idRadius);
	unitRangeSystemObject->AddComponent<physics::RigidBody>()->SetAsKinematic(true);
	unitRangeSystemObject->SetParent(m_unitGameObject);
}

void UnitProductor::AddColliderComponent() const 
{
	auto unitCollider = m_unitGameObject->AddComponent<physics::SphereCollider>();	// 빈 껍데기에 
	unitCollider->SetRadius(lengthUnit);
	//m_unitGameObject->AddComponent<physics::RigidBody>()->SetAsKinematic(true);

	auto unitColliderDebugObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	AttachDebugMesh(unitColliderDebugObject, DebugMeshType::Sphere, yunuGI::Color::green(), false);
	unitColliderDebugObject->SetParent(m_unitGameObject);
	unitColliderDebugObject->GetTransform()->SetWorldScale(Vector3d(lengthUnit, lengthUnit, lengthUnit));
	m_unitComponent->SetStaticMeshComponent(unitColliderDebugObject->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>());
}

void UnitProductor::AddNavigationComponent() 
{
	m_navField = &SingleNavigationField::Instance();
	auto unitNavigationComponent = m_unitGameObject->AddComponent<NavigationAgent>();
	unitNavigationComponent->AssignToNavigationField(m_navField);
	unitNavigationComponent->SetRadius(0.3f);
	m_unitComponent->SetNavField(m_navField);
}

void UnitProductor::AddDotweenComponent() const
{
	/// 6. Dotween 추가
	m_unitComponent->dotween = m_unitGameObject->AddComponent<Dotween>();
	m_unitComponent->knockBackTimer = m_unitGameObject->AddComponent<Timer>();
}

std::string UnitProductor::GetUnitFBXName() const
{
	return m_unitFbxName;
}

bool UnitProductor::SelectUnitProductorByFbxName(std::string p_name)
{
	return (m_unitFbxName == p_name);
}

void UnitProductor::SetPlayerRelatedComponents(Unit* playerUnit)
{
	playerUnit->SetPlayerSerialNumber(m_unitType);
	playerUnit->SetSkillPreviewType(qSkillPreviewType, wSkillPreviewType);
	PlayerController::SingleInstance().AddPlayerUnit(playerUnit);
}

void UnitProductor::MappingUnitData(application::editor::POD_Unit_TemplateData p_podData)
{
	//m_unitType = static_cast<Unit::UnitType>(p_podData.unitType);
	m_healthPoint = p_podData.m_healthPoint;
	m_manaPoint = p_podData.m_manaPoint;
	m_autoAttackDamage = p_podData.m_autoAttackDamage;
	m_criticalHitProbability = p_podData.m_criticalHitProbability;
	m_criticalHitMultiplier = p_podData.m_criticalHitMultiplier;
	m_defensePoint = p_podData.m_defensePoint;
	m_dodgeProbability = p_podData.m_dodgeProbability;
	m_criticalDamageDecreaseMultiplier = p_podData.m_criticalDamageDecreaseMultiplier;
	m_idRadius = p_podData.m_idRadius;
	m_atkRadius = p_podData.m_atkRadius;
	m_unitSpeed = p_podData.m_unitSpeed;
	m_attackDelay = p_podData.m_attackDelay;
}

void UnitProductor::PushWaveData(Vector3d startPos, float delay)
{
	m_waveDelayQueue.push({ startPos, delay });
	if (!isWaveUnitCreated)
	{
		m_duration = delay;
		isWaveUnitCreated = true;
	}
}

void UnitProductor::Update()
{
	if (isWaveTimerStarted && !(m_waveDelayQueue.empty()))
	{
		m_elapsed += Time::GetDeltaTime();

		if (m_elapsed >= m_duration)
		{
			CreateUnit(m_waveDelayQueue.front().first);
			m_waveDelayQueue.pop();
			m_elapsed = 0.0f;

			if (m_waveDelayQueue.empty())
				isWaveTimerStarted = false;
			else
				m_duration = m_waveDelayQueue.front().second;
		}
	}
}

void UnitProductor::Start()
{
	/// 원래는 플레이어 유닛이 들어오면 ok지만 우선은 실행 시 wave Trigger On
	isWaveTimerStarted = true;
}

