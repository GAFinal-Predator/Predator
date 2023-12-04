#pragma once
#include "YunutyEngine.h"
#include "FSM.h"
#include <list>
#include "BaseUnitEntity.h"

/// <summary>
/// �Ϲ� ���� ���� Ŭ�������� �÷��̾� �� ���� Ŭ������ �θ� Ŭ����.
/// �⺻������ ��� ������ ����� ���� �ִ�.
/// </summary>

class Timer;
class ProjectileSystem;
class Projectile;

class Enemy : public BaseUnitEntity
{
private:
	enum UnitState
	{
		Idle,
		Move,
		Chase,
		Detect,
		Attack,
		Death,
		StateEnd
	};

	FSM<UnitState>* unitFSM;

private:
	/// ���� ���¿��� �ٸ� ���·� ���� ���� bool���� �Ǻ����ִ� �Լ�.
	void IdleTransition() override;
	void ChaseTransition() override;
	void AttackTransition() override;

	virtual void IdleEngage() override;
	virtual void ChaseEngage() override;
	virtual void AttackEngage() override;

	virtual void IdleEngageFunction() override;
	virtual void AttackEngageFunction() override;
	virtual void ChaseEngageFunction() override;

	virtual void IdleUpdate() override;
	virtual void ChaseUpdate() override;
	virtual void AttackUpdate() override;

public:
	// ������Ʈ���� ������ �Լ�
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnDestroy() override;
};

