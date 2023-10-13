#pragma once
#include "Unit.h"
/// <summary>
/// Player Ŭ����. ���� Unit�� ��ӹ޴� ������ ���� ����.
/// </summary>
class Player : public Unit
{
public:
	Player();

private:


public:
	virtual void Start() override;
	virtual void Update() override;

public:
	virtual void IdentifyAction(Vector3d position) override;
	virtual void AttackAction(Vector3d position) override;

};

