#pragma once
#include "YunutyEngine.h"
/// <summary>
/// Ű�� ���콺�� �Է��� ������ �ش� �Է¿� ���� �ٸ� Ŭ������ �Լ��� ȣ�����ִ� �Լ�.
///	�Է°� ������ ���Ҹ� ���´�.
/// </summary>
class InputManager : public Component
{
public:
	enum SelectedSerialNumber
	{
		One = 1,
		Two,
		Three,
		All
	};

private:
	int currentSelectedSerialNumber;

public:
	virtual void Start() override;
	virtual void Update() override;

};

