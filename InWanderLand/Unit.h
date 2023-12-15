#pragma once
#include "YunutyEngine.h"
#include "FSM.h"
//#include <memory>
#include <list>

/// <summary>
/// 유닛들이 공유하는 멤버.
/// 기본적으로 모든 유닛은 BaseUnitEntity를 상속한다.
/// </summary>

class Unit : public Component
{
private:
	// 사용 시 주의점 : 마지막에는 Death와 StateEnd가 순서대로 들어가 있을 것!
	enum class UnitState
	{
		Idle,
		Move,
		Chase,
		Attack,
		AttackMove,
		QSkill,
		Death,
		StateEnd
	};

	FSM<UnitState> unitFSM{UnitState::Idle};
	string unitType;

	int m_hp;
	int m_ap;
	
	float m_speed;
	float m_bulletSpeed;

	float chaseUpdateDelay;

	// 지금 수행중인 명령
	UnitState currentOrder = UnitState::Idle;

	float m_idDistance;
	float m_atkDistance;

	float idleToChaseDelay = 1.5f;
	float idleElapsed;

	float moveFunctionElapsed;
	float moveFunctionCallDelay = 0.1f;

	float chaseFunctionElapsed;
	float chaseFunctionCallDelay = 0.1f;

	float attackFunctionElapsed;
	float attackFunctionCallDelay = 0.3f;

	float deathFunctionElapsed;
	float deathAnimationDelay = 1.5f;

	float qSkillFunctionStartElapsed;
	float qSkillFunctionStartedElapsed;
	float qSkillStartDelay = 1.0f;
	float qSkillAnimationDuration = 1.0f;
	bool isSkillStarted;

	bool isAttackMoving;

	int playerSerialNumber;

	std::list<yunutyEngine::GameObject*> m_opponentObjectList;		
	std::list<Unit*> m_recognizedThisList;	// 현재 이 오브젝트를 적군으로 인식한 다른 유닛들의 리스트.

	yunutyEngine::GameObject* m_currentTargetObject;		// Attack이나 Chase 때 사용할 적군  오브젝트
	Vector3d m_currentMovePosition;							// 현재 상대의 위치

	Vector3d m_currentSkillPosition;

private:
	void IdleEngage();
	void MoveEngage();
	void AttackMoveEngage();
	void AttackEngage();
	void ChaseEngage();
	void QSkillEngage();
	void DeathEngage();

	void IdleUpdate();
	void MoveUpdate();
	void AttackMoveUpdate();
	void ChaseUpdate();
	void AttackUpdate();
	void QSkillUpdate();
	void DeathUpdate();

	void IdleEngageFunction();
	void MoveEngageFunction();
	void AttackMoveEngageFunction();
	void ChaseEngageFunction();
	void AttackEngageFunction();
	void DeathEngageFunction();

	void IdleUpdateFunction();
	void MoveUpdateFunction();
	void AttackMoveUpdateFunction();
	void ChaseUpdateFunction();
	void AttackUpdateFunction();
	void QSkillUpdateFunction();
	void DeathUpdateFunction();

	void StopMove();
	void DetermineCurrentTargetObject();
	void ReportUnitDeath();												// this 유닛이 죽었다는 정보를 전달
	void IdentifiedOpponentDeath(yunutyEngine::GameObject* obj);		// 상대 유닛이 죽었을 경우 처리할 내용을 담은 함수

public:
	virtual void Start() override;
	virtual void Update() override;

	string GetType() const;
	void SetType(string type);

	void SetUnitHp(int p_Hp);
	void SetUnitAp(int p_Ap);
	void SetIdRadius(float radius);
	void SetAtkRadius(float radius);
	void SetUnitSpeed(float speed);

	void SetPlayerSerialNumber();
	int GetPlayerSerialNumber() const;

	void SetCurrentOrderMove();
	void SetCurrentOrderAttackMove();

	int GetUnitAp() const;
	void Damaged(GameObject* opponentObject, int opponentAp);

	void OrderMove(Vector3d position);
	void OrderAttackMove(Vector3d position);
	void OrderQSkill(Vector3d position);

	void AddToOpponentObjectList(yunutyEngine::GameObject* obj);
	void DeleteFromOpponentObjectList(yunutyEngine::GameObject* obj);
};

