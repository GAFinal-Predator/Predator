#pragma once
#include "YunutyEngine.h"
#include "Unit.h"
/// <summary>
/// UnitFactory������ ���� ������ �� �� ȿ�������� �ϱ� ���� Ŭ����.
/// </summary>

using namespace yunuGI;

class UnitProductionOrder : public Component
{
protected:
	string m_objectName;
	Unit::UnitType m_unitType;
	Unit::UnitSide m_unitSide;
	int m_hp;
	int m_ap;
	float m_idRadius;
	float m_atkRadius;
	float m_unitSpeed;

	// �ӽ�. �𵨸� ���Ϸ� ��ü���ֱ�
	GameObject* m_unitGameObject;
	IAnimation* m_idleAnimation;
	IAnimation* m_walkAnimation;
	IAnimation* m_attackAnimation;
	IAnimation* m_deathAnimation;

public:
	virtual GameObject* CreateUnitWithOrder();

	virtual void SetUnitData(GameObject* fbxObject) = 0;

	virtual void SetPlayerRelatedComponents(Unit* playerUnit);
};

