#pragma once
#include "YunutyEngine.h"
#include "FSM.h"
#include <list>

/// <summary>
/// �Ϲ� ���� ���� Ŭ�������� �÷��̾� �� ���� Ŭ������ �θ� Ŭ����.
/// �⺻������ ��� ������ ����� ���� �ִ�.
/// </summary>

class Timer;
class ProjectileSystem;
class Projectile;

class Unit : public Component
{
protected:
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

protected:
	string unitType;

	float m_speed;
	float m_bulletSpeed;

	bool idleToChase;
	bool idleToAttack;
	bool attackToIdle;
	bool chaseToIdle;

	bool initToIdle;

	bool isJustEntered;			// �ۿ� �ִٰ� ���� ���. 

	bool isDistanceComparingStarted;

	float m_IdDistance;
	float m_AtkDistance;

	float transitionDelay;

	float chaseUpdateDelay;

	// ó�� �ν� ������ ������ �� ����Ǵ� ��� ����.
	std::list<GameObject*> m_opponentGameObjectList;
	GameObject* m_currentTargetObject;
	Vector3d m_currentTargetPosition;		// ���� ����� ��ġ

	void IdleEngage();
	void ChaseEngage();
	void AttackEngage();

	void IdleUpdate();
	void ChaseUpdate();
	void AttackUpdate();

	void IdleEngageFunction();
	void AttackEngageFunction();
	void ChaseEngageFunction();

public:
	// ������Ʈ���� ������ �Լ�
	virtual void Start() override;
	virtual void Update() override;

public:
	// �ڽĵ鿡�� �����ֵ�, �ܺο��� ȣ���� ������ �Լ�
	string GetType() const;
	
	void SetIdRadius(float radius);
	void SetAtkRadius(float radius);


	/// ���� ���¿��� �ٸ� ���·� ���� ���� bool���� �Ǻ����ִ� �Լ�.
	void IdleTransition();
	void ChaseTransition();
	void AttackTransition();

	void InitFSM();

	void SetOpponentGameObject(GameObject* obj);
	void DeleteOpponentGameObject(GameObject* obj);
	void EnterIDRange();
};

