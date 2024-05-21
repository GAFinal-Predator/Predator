#include "LocalTimeEntityManager.h"
#include "LocalTimeEntity.h"

LocalTimeEntityManager::~LocalTimeEntityManager()
{
	int a = 0;
}

void LocalTimeEntityManager::Start()
{
}

void LocalTimeEntityManager::OnContentsStop()
{
	this->SetActive(false);
	m_entityVector.clear();
}

void LocalTimeEntityManager::RegisterLocalTimeEntity(LocalTimeEntity* p_entity)
{
	m_entityVector.push_back(p_entity);
}

void LocalTimeEntityManager::ReportTacticModeEngaged()
{
	for (auto each : m_entityVector)
	{
		each->SetLocalTimeScale(0.0f);
	}
}

void LocalTimeEntityManager::ReportTacticModeEnded()
{
	for (auto each : m_entityVector)
	{
		each->SetLocalTimeScale(1.0f);
	}
}

void LocalTimeEntityManager::SetLocalTimeScaleDirectly(LocalTimeEntity* p_entity, float p_scale)
{
	p_entity->SetLocalTimeScale(p_scale);
}
