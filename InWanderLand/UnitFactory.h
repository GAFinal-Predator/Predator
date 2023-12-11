#pragma once
#include "YunutyEngine.h"
/// <summary>
/// 손쉽게 유닛을 만들 수 있도록 해주는 클래스.
/// 디폴트 값으로 unit 객체를 생성하고, 조정을 원한다면 GetComponent로 설정해준다.
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

