#pragma once
#include "Component.h"

/// <summary>
/// IUnit�� ��� ĳ���͵��� ���� �θ��̴�.
/// ��� ĳ���͵��� ����� �ൿ���� ������ �ִ�.
/// </summary>

enum class State
{
	IDLE,
	MOVE,
	ATTACKMOVE,
	CHASE,
	ATTACK,
	DIE,
};

enum class UnitType
{
	PLAYER, // �ӽ�
	WARRIOR,
	ARCHER,
};

enum class AttackType
{
	MELEE,
	RANGE,
};

class IUnit : public yunutyEngine::Component
{
public:
	virtual void Idle() = 0;
	virtual void Move() = 0;
	virtual void Attack() = 0;
	virtual void AttackMove() = 0;
	virtual void Chase() = 0;
	virtual void Damaged(float damage) = 0;
	virtual void Hit() = 0;
	virtual void Die() = 0;
};