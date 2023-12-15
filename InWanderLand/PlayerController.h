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
private:
	PlayerController();
	~PlayerController();
	static PlayerController* instance;

public:
	static PlayerController* GetInstance();

private:
	RTSCam* m_movingSystemComponent;
	std::map<Unit*, int> playerComponentMap;
	
public:
	void MakeLeftClickMove(int unitSerialNumber);
	void MakeLeftClickAttackMove(int unitSerialNumber);

	void QSkillKeyPressed(int unitSerialNumber);

	void SetMovingSystemComponent(RTSCam* sys);
	void AddPlayerUnit(Unit* p_playerUnit); 
};

