#include "BaseUnitEntity.h"
#include "Dotween.h"

std::string BaseUnitEntity::GetType() const
{
	return unitType;
}

void BaseUnitEntity::SetIdRadius(float radius)
{
	m_IdDistance = radius;
}

void BaseUnitEntity::SetAtkRadius(float radius)
{
	m_AtkDistance = radius;
}

/// ��� ������ �ν� ������ ������ ����� �� ȣ��Ǵ� �Լ�.
/// ex) FSM �ʱ�ȭ, transitionDelay �ʱ�ȭ.
void BaseUnitEntity::InitFSM()
{
	transitionDelay = 0.0f;
	initToIdle = true;
}

void BaseUnitEntity::SetOpponentGameObject(yunutyEngine::GameObject* obj)
{
	m_opponentGameObjectList.push_back(obj);
	if (m_currentTargetObject == nullptr)
		m_currentTargetObject = obj;
}

void BaseUnitEntity::DeleteOpponentGameObject(yunutyEngine::GameObject* obj)
{
	m_opponentGameObjectList.remove(obj);
}

void BaseUnitEntity::EnterIDRange()
{
	isJustEntered = true;
}
