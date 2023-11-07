#pragma once
#include "YunutyEngine.h"
#include "FSM.h"

/// <summary>
/// �Ϲ� ���� ���� Ŭ�������� �÷��̾� �� ���� Ŭ������ �θ� Ŭ����.
/// �⺻������ ��� ������ ����� ���� �ִ�.
/// </summary>

class Timer;
class ProjectileSystem;
class Projectile;
class MoveDetector;

class Unit : public Component
{
protected:
	enum UnitState
	{
		Idle,
		Move,
		Chase,
		ReChase,
		Detect,
		Attack,
		Death,
		StateEnd
	};

	FSM<UnitState>* unitFSM;

protected:
	string unitType;

	float m_speed;
	float chaseSpeed;
	float m_bulletSpeed;

	bool idleToChase;
	bool idleToAttack;
	bool attackToIdle;
	bool chaseToIdle;
	bool isChaseRequested;
	bool isRechaseCompleted;

	bool initToIdle;

	bool isJustEntered;			// �ۿ� �ִٰ� ���� ���. 

	float m_IdDistance;
	float m_AtkDistance;

	float transitionDelay;

	// ó�� �ν� ������ ������ �� ����Ǵ� ��� ����.
	GameObject* m_opponentGameobject;
	Vector3d m_opponentPosition;		// ���� ����� ��ġ
	Vector3d m_moveTargetPosition;		// �ڽ��� �̵� ���� ��ġ
	Vector3d m_opponentTargetPosition;	// ����� �̵� ���� ��ġ

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

	void MoveDetect(Vector3d newPosition);
	void SetOpponentGameObject(GameObject* obj);
	void SetOpponentTargetPosition(Vector3d pos);
	void EnterIDRange();
};

