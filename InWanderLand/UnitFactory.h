#pragma once
#include "YunutyEngine.h"
/// <summary>
/// �ս��� ������ ���� �� �ֵ��� ���ִ� Ŭ����.
/// ����Ʈ ������ unit ��ü�� �����ϰ�, ������ ���Ѵٸ� GetComponent�� �������ش�.
/// </summary>

enum UnitType
{
	PLAYER,
	ENEMY,
};

class UnitFactory
{
public:
	UnitFactory();

private:
	yunuGI::Color playerColor;
	yunuGI::Color enemyColor;

	float defaultPlayerAtkRadius;
	float defaultPlayerIDRadius;

	float defaultEnemyAtkRadius;
	float defaultEnemyIDRadius;

public:
	yunutyEngine::GameObject* CreateUnit(UnitType unitType, yunutyEngine::NavigationField* navField);

};

