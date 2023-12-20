#pragma once
#include "YunutyEngine.h"
#include "Unit.h"
/// <summary>
/// UnitFactory������ ���� ������ �� �� ȿ�������� �ϱ� ���� Ŭ����.
/// </summary>

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
	yunuGI::Color m_unitColor;


public:
	virtual void SetUnitData() = 0;
	//virtual void SetUnitGameObject(GameObject* obj) = 0;
	virtual GameObject* CreateUnitToOrder();

	virtual void SetPlayerRelatedComponents(Unit* playerUnit);
};

