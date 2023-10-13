#pragma once
#include "IRangeAction.h"

/// <summary>
/// �Ϲ� ���� ���� Ŭ�������� �÷��̾� �� ���� Ŭ������ �θ� Ŭ����.
/// �⺻������ ��� ������ ���� ����� ���� �ִ�.
/// </summary>
class Unit : public IRangeAction
{
public:
	// ������Ʈ���� ������ �Լ�
	virtual void Start() override;
	virtual void Update() override;

public:
	// IRangeAction���� ������ �Լ�
	virtual void IdentifyAction(Vector3d position) override;
	virtual void AttackAction(Vector3d position) override;

public:
	// ����Լ���
};

