#pragma once
#include "Unit.h"
/// <summary>
/// Player Ŭ����. ���� Unit�� ��ӹ޴� ������ ���� ����.
/// </summary>

class RTSCam;

class Player : public Unit
{
private:
	RTSCam* rtsCamComponent;

	bool changeToMoveState;

	Vector3d m_movePosition;

public:
	virtual void Start() override;
	virtual void Update() override;

	void SetMovingSystemComponent(RTSCam* sys);

private:
	virtual void MoveEngage() override;

	void MoveStateFunction();


};

 