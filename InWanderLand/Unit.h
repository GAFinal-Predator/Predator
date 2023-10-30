#pragma once
#include "YunutyEngine.h"
#include "FSM.h"

/// <summary>
/// �Ϲ� ���� ���� Ŭ�������� �÷��̾� �� ���� Ŭ������ �θ� Ŭ����.
/// �⺻������ ��� ������ ���� ����� ���� �ִ�.
/// </summary>

class Timer;
class ProjectileSystem;
class Projectile;

class Unit : public Component
{
private:
	enum UnitState
	{
		Idle,
		Move,
		Chase,
		Detect,
		Attack,
		StateEnd
	};

	FSM<UnitState>* unitFSM;

protected:
	float m_speed;
	float m_bulletSpeed;

	bool isInIDRange;
	bool isInAtkRange;
	bool isOutAtkRange;
	bool isOutIDRange;

	float m_IdDistance;
	float m_AtkDistance;

	// ó�� �ν� ������ ������ �� ����Ǵ� ��� ����.
	GameObject* m_opponentGameobject;
	Vector3d m_opponentPosition;

	void IdleEngage();
	void ChaseEngage();
	void AttackEngage();

	void ChaseUpdate();
	void AttackUpdate();

	void AttackTransition();

	void IdleFunction();
	void AttackFunction();
	void ChaseFunction();

public:
	// ������Ʈ���� ������ �Լ�
	virtual void Start() override;
	virtual void Update() override;

public:
	// �ڽĵ鿡�� �����ֵ�, �ܺο��� ȣ���� ������ �Լ�
	string GetType() const;
	void SetIdRadius(float radius);
	void SetAtkRadius(float radius);

	void IdentifyTransition();
	void ExitIDRangeTransition();
	void SetOpponentGameObject(GameObject* obj);

protected:
	string unitType;

};

