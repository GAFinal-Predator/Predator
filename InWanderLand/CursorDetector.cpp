#include "CursorDetector.h"
#include "Unit.h"
#include "DebugMeshes.h"
#include "UnitProductor.h"
#include "ContentsLayer.h"
#include "Application.h"

void CursorDetector::OnTriggerEnter(physics::Collider* collider)
{
	if (Unit* colliderUnitComponent = collider->GetGameObject()->GetComponent<Unit>();
		colliderUnitComponent != nullptr && colliderUnitComponent->GetUnitSide() == Unit::UnitSide::Enemy)
	{
		m_onMouseUnits.insert(colliderUnitComponent);
		
		DetermineClosestUnit();
	}
}

void CursorDetector::OnTriggerExit(physics::Collider* collider)
{
	if (Unit* colliderUnitComponent = collider->GetGameObject()->GetComponent<Unit>();
		colliderUnitComponent != nullptr && colliderUnitComponent->GetUnitSide() == Unit::UnitSide::Enemy)
	{
		m_onMouseUnits.erase(colliderUnitComponent);
		DetermineClosestUnit();
	}
}

void CursorDetector::Start()
{
	m_SelectEffectObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	AttachDebugMesh(m_SelectEffectObject, DebugMeshType::Sphere, yunuGI::Color::red(), true);
	m_SelectEffectObject->GetTransform()->SetWorldScale({ UNIT_LENGTH * 1.5f, 0.3f, UNIT_LENGTH * 1.5f });
	
	m_SelectEffectObject->SetSelfActive(false);
	application::contents::ContentsLayer* contentsLayer = dynamic_cast<application::contents::ContentsLayer*>(application::Application::GetInstance().GetContentsLayer());
	contentsLayer->RegisterToEditorObjectContainer(m_SelectEffectObject);

}

void CursorDetector::Update()
{
	if (m_currentSelectedUnit != nullptr)
	{
		m_SelectEffectObject->GetTransform()->SetWorldPosition(m_currentSelectedUnit->GetTransform()->GetWorldPosition() + Vector3d(0, 0.1f, 0));
	}

	for (auto itr = m_onMouseUnits.begin(); itr != m_onMouseUnits.end();)
	{
		if ((*itr)->GetGameObject()->GetSelfActive() == false)
		{
			itr = m_onMouseUnits.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void CursorDetector::EraseUnitFromContainer(Unit* p_unit)
{
	m_onMouseUnits.erase(p_unit);
	DetermineClosestUnit();
}

void CursorDetector::DetermineClosestUnit()
{
	float shortestDistance;
	bool isCalculateStarted{ false };
	if (!m_onMouseUnits.empty())
	{
		for (auto e : m_onMouseUnits)
		{
			float nowComparingDistance = (GetTransform()->GetWorldPosition() - e->GetTransform()->GetWorldPosition()).Magnitude();
			if (!isCalculateStarted)
			{
				shortestDistance = nowComparingDistance;
				m_currentSelectedUnit = e;
				isCalculateStarted = true;
			}
			else if (shortestDistance > nowComparingDistance)
			{
				shortestDistance = nowComparingDistance;
				m_currentSelectedUnit = e;
			}
		}

		/// Select 관련 오브젝트를 m_currentSelectedUnit 위치에 띄우는 로직
		m_SelectEffectObject->GetTransform()->SetWorldPosition(m_currentSelectedUnit->GetTransform()->GetWorldPosition() + Vector3d(0, 0.1f, 0));
		m_SelectEffectObject->SetSelfActive(true);
	}
	else
	{
		m_currentSelectedUnit = nullptr;
		/// Select 관련 오브젝트를 비활성화 하는 로직
		m_SelectEffectObject->SetSelfActive(false);
	}
	
}
