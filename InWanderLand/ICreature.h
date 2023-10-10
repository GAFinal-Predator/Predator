#pragma once
#include "Component.h"

/// <summary>
/// ũ��ó�� ��� ĳ���͵��� ���� �θ��̴�.
/// ��� ĳ���͵��� ����� �ൿ���� ������ �ִ�.
/// </summary>

enum class State
{
	IDLE,
	MOVE,
	CHASE,
	ATTACK,
	DIE,
};

class ICreature : public yunutyEngine::Component
{
public:
	virtual void Idle() = 0;
	virtual void Move() = 0;
	virtual void Attack() = 0;
	virtual void Chase() = 0;
	virtual void Die() = 0;
};