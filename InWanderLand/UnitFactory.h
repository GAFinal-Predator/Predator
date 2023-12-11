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

	float defaultPlayerSpeed;
	float defaultEnemySpeed;

	int playerHp;
	int playerAp;
	int enemyHp;
	int enemyAp;

public:
	yunutyEngine::GameObject* CreateUnit(UnitType unitType, yunutyEngine::NavigationField* navField, Vector3d startPosition);
	void SetIDRadius(float idRadius);
	void SetAtkRadius(float atkRadius);
	void SetPlayerHp(int p_hp);
	void SetPlayerAp(int p_ap);
	void SetEnemyHp(int p_hp);
	void SetEnemyAp(int p_ap);
};

