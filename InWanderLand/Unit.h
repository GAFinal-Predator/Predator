#pragma once
#include "YunutyEngine.h"
#include "FSM.h"
//#include <memory>
#include <list>

using namespace yunuGI;

/// <summary>
/// ���ֵ��� �����ϴ� ���.
/// �⺻������ ��� ������ BaseUnitEntity�� ����Ѵ�.
/// </summary>

class Unit : public Component
{
public:
	// ��� �� ������ : ���������� Death�� StateEnd�� ������� �� ���� ��!
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

	enum class AttackType
	{
		Melee,
		Ranged
	};

	enum class UnitType
	{
		Warrior,
		Magician,
		Healer,
		MeleeEnemy,
		Boss
	};

	enum class UnitSide
	{
		Player,
		Enemy
	};

private:
	FSM<UnitState> unitFSM{UnitState::Idle};

	UnitType m_unitType;
	UnitSide m_unitSide;
	AttackType m_attackType;

	int m_hp;
	int m_ap;
	
	float m_speed;
	float m_bulletSpeed;

	float chaseUpdateDelay;
	// ������ ���� ȸ������ ���氢���� ��Ÿ���ϴ�. ������ currentRotation�� 0���϶� ����(���� X ����), 90���϶� ����(���� z ����)�� ���մϴ�.
	float currentRotation{270};
	// ������ �ٶ���� �ϴ� ȸ������ ���氢���� ��Ÿ���ϴ�. ������ ȸ���ӵ��� ���� ȸ������ desiredRotation�� ��ġ�ϰ� �ٲߴϴ�.
	float desiredRotation{270};
	float rotationSpeed{ 500 };


	// ���� �������� ���
	UnitState currentOrder = UnitState::Idle;

	float m_idDistance;
	float m_atkDistance;

	float idleToChaseDelay = 1.5f;
	float idleElapsed;

	float moveFunctionElapsed;
	float moveFunctionCallDelay = 0.1f;

	float chaseFunctionElapsed;
	float chaseFunctionCallDelay = 0.1f;

	float attackStartDelay = 0.3f;
	float attackFunctionElapsed;
	float attackFunctionCallDelay = 0.5f;
	bool isAttackStarted = false;

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
	std::list<Unit*> m_recognizedThisList;	// ���� �� ������Ʈ�� �������� �ν��� �ٸ� ���ֵ��� ����Ʈ.

	yunutyEngine::GameObject* m_currentTargetObject;		// Attack�̳� Chase �� ����� ����  ������Ʈ
	Vector3d m_currentMovePosition;							// ���� ����� ��ġ

	Vector3d m_currentSkillPosition;

private:
	/// <summary>
	/// �ִϸ��̼� ���� ���
	/// </summary>
	IAnimation* m_idleAnimation;
	IAnimation* m_walkAnimation;
	IAnimation* m_attackAnimation;
	IAnimation* m_deathAnimation;

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
	void LookAt(Vector3d destination);
	
	void DetermineCurrentTargetObject();
	void ReportUnitDeath();												// this ������ �׾��ٴ� ������ ����
	void IdentifiedOpponentDeath(yunutyEngine::GameObject* obj);		// ��� ������ �׾��� ��� ó���� ������ ���� �Լ�

public:
	virtual void Start() override;
	virtual void Update() override;

	UnitType GetUnitType() const;
	UnitSide GetUnitSide() const;
	void SetUnitType(UnitType type);
	void SetUnitSide(UnitSide side);

	void SetUnitHp(int p_Hp);
	void SetUnitAp(int p_Ap);
	void SetIdRadius(float radius);
	void SetAtkRadius(float radius);
	void SetUnitSpeed(float speed);

	void SetIdleAnimation(IAnimation* idleAnim);
	void SetWalkAnimation(IAnimation* walkAnim);
	void SetAttackAnimation(IAnimation* attackAnim);
	void SetDeathAnimation(IAnimation* deathAnim);

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

