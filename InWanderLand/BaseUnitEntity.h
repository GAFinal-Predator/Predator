#pragma once
#include "YunutyEngine.h"
#include <list>

/// <summary>
/// ���ֵ��� �����ϴ� ���.
/// �⺻������ ��� ������ BaseUnitEntity�� ����Ѵ�.
/// </summary>

class BaseUnitEntity : public Component
{
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
	std::list<yunutyEngine::GameObject*> m_opponentGameObjectList;
	yunutyEngine::GameObject* m_currentTargetObject;
	Vector3d m_currentTargetPosition;		// ���� ����� ��ġ

protected:
	/// ���� ���¿��� �ٸ� ���·� ���� ���� bool���� �Ǻ����ִ� �Լ�.
	/// ���ο��� fsm�� �Լ��� ȣ���ؾ��ϴ� �ڽĵ��� ������ ���� ��!
	virtual void ChaseTransition() = 0;
	virtual void AttackTransition() = 0;

	virtual void IdleEngage() = 0;
	virtual void ChaseEngage() = 0;
	virtual void AttackEngage() = 0;

	virtual void IdleEngageFunction() = 0;
	virtual void ChaseEngageFunction() = 0;
	virtual void AttackEngageFunction() = 0;

	virtual void IdleUpdate() = 0;
	virtual void ChaseUpdate() = 0;
	virtual void AttackUpdate() = 0;

public:
	virtual void IdleTransition() = 0;

	string GetType() const;

	void SetIdRadius(float radius);
	void SetAtkRadius(float radius);

	void InitFSM();

	void SetOpponentGameObject(yunutyEngine::GameObject* obj);
	void DeleteOpponentGameObject(yunutyEngine::GameObject* obj);
	void EnterIDRange();
};

