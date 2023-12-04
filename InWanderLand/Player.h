#pragma once
#include "BaseUnitEntity.h"
#include "FSM.h"
/// <summary>
/// Player 클래스. 추후 Unit을 상속받는 구조로 수정 예정.
/// </summary>

class RTSCam;

class Player : public BaseUnitEntity
{
private:
	enum PlayerState
	{
		Idle,
		Move,
		Chase,
		Detect,
		Attack,
		Death,
		StateEnd
	};

	FSM<PlayerState>* playerFSM;

	RTSCam* rtsCamComponent;

	bool changeToMove;
	bool moveToIdle;
	bool isMoveStarted;
	//bool moveToChase;		// Move -> Idle -> Chase 순으로 해보자

	Vector3d m_tempPosition;		// MoveTo호출 도중 SetPosition 해주면 어떻게 될까 확인용.
	bool tempCheck;

	Vector3d m_movePosition;		// 클릭 시 이동해야 할 위치를 담아주는 멤버
	Vector3d m_previousPosition;	// 이동 중인지를 판단하기 위한 이전 프레임에서의 위치 (현재 위치와 비교)
	
	float distanceOffset;

private:
	/// 현재 상태에서 다른 상태로 가기 위한 bool값을 판별해주는 함수.
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
	// 컴포넌트에서 가져온 함수
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnDestroy() override;

	void SetMovingSystemComponent(RTSCam* sys);

private:
	void MoveEngage();
	void MoveEngageFunction();

	void MoveUpdate();


};

 