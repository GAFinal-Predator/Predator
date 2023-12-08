#pragma once
#include "YunutyEngine.h"
#include "FSM.h"
//#include <memory>
#include <list>

/// <summary>
/// ���ֵ��� �����ϴ� ���.
/// �⺻������ ��� ������ BaseUnitEntity�� ����Ѵ�.
/// </summary>

class Unit : public Component
{
private:
	enum class UnitState
	{
		Idle,
		Move,
		Chase,
		Attack,
		AttackMove,
		Death,
		StateEnd
	};
	FSM<UnitState> unitFSM{UnitState::Idle};
	string unitType;

	float m_speed;
	float m_bulletSpeed;

	float chaseUpdateDelay;

	// ���� �������� ���
	UnitState currentOrder=UnitState::Idle;

	float m_IdDistance;
	float m_AtkDistance;

	float idleToChaseDelay = 1.5f;
	float idleElapsed;

	float MoveUpdateDelay;

	float moveFunctionElapsed;
	float moveFunctionCallDelay = 0.1f;

	float chaseFunctionElapsed;
	float chaseFunctionCallDelay = 0.1f;

	float attackFunctionElapsed;
	float attackFunctionCallDelay = 0.3f;

	// ó�� �ν� ������ ������ �� ����Ǵ� ��� ����.
	std::list<yunutyEngine::GameObject*> m_opponentGameObjectList;
	yunutyEngine::GameObject* m_currentTargetObject;
	Vector3d m_currentMovePosition;		// ���� ����� ��ġ

private:
	/// ���� ���¿��� �ٸ� ���·� ���� ���� bool���� �Ǻ����ִ� �Լ�.
	/// ���ο��� fsm�� �Լ��� ȣ���ؾ��ϴ� �ڽĵ��� ������ ���� ��!

	void IdleEngage();
	void MoveEngage();
	void AttackMoveEngage();
	void AttackEngage();
	void ChaseEngage();
	void DeathEngage();

	void IdleEngageFunction();
	void MoveEngageFunction();
	void AttackMoveEngageFunction();
	void ChaseEngageFunction();
	void AttackEngageFunction();
	void DeathEngageFunction();

	void IdleUpdate();
	void MoveUpdate();
	void AttackMoveUpdate();
	void ChaseUpdate();
	void AttackUpdate();

	void IdleUpdateFunction();
	void MoveUpdateFunction();
	void AttackMoveUpdateFunction();
	void ChaseUpdateFunction();
	void AttackUpdateFunction();

	void StopPosition();

public:
	virtual void Start() override;
	virtual void Update() override;

	string GetType() const;
	void SetType(string type);

	void SetIdRadius(float radius);
	void SetAtkRadius(float radius);
	void SetUnitSpeed(float speed);

	void OrderMove(Vector3d position);
	void OrderAttackMove(Vector3d position);
	void SetOpponentGameObject(yunutyEngine::GameObject* obj);
	void DeleteOpponentGameObject(yunutyEngine::GameObject* obj);
	void EnterIDRange(GameObject* opponentObject);
};

