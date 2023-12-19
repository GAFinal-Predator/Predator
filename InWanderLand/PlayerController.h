#pragma once
#include "YunutyEngine.h"
#include <map>
/// <summary>
/// UnitFactory���� Player�� ���� ���, PlayerController�� �ش� Player�� ����Ѵ�.
/// ���� ��ȣ�� ����Ͽ� Ű�Է����� ������ �� �ֵ��� �غ���!
/// </summary>

class RTSCam;
class Unit;

class PlayerController
{
public:
	enum class OrderType
	{
		Move,
		AttackMove,
		QSkill,
	};

private:
	PlayerController();
	~PlayerController();
	static PlayerController* instance;

public:
	static PlayerController* GetInstance();

private:
	RTSCam* m_movingSystemComponent;
	std::map<Unit*, int> playerComponentMap;
	int previousSerialNumber = 0;

	void SelectFunctionByOrderType(int unitSerialNumber, OrderType p_orderType);

public:
	void ApplyCurrentPlayerOrder(int unitSerialNumber, OrderType orderType);
	void SetMovingSystemComponent(RTSCam* sys);
	void AddPlayerUnit(Unit* p_playerUnit);

	void SetLeftClickEmpty();

};

