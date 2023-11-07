#pragma once
#include "Unit.h"
/// <summary>
/// Player Ŭ����. ���� Unit�� ��ӹ޴� ������ ���� ����.
/// </summary>
class Player : public Unit
{
private:
	enum PlayerState
	{
		Idle,
		Move,
		Chase,

	};

public:
	virtual void Start() override;
	virtual void Update() override;

};

 