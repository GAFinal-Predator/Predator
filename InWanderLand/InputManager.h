#pragma once
#include "YunutyEngine.h"
/// <summary>
/// Ű�� ���콺�� �Է��� ������ �ش� �Է¿� ���� �ٸ� Ŭ������ �Լ��� ȣ�����ִ� �Լ�.
///	�Է°� ������ ���Ҹ� ���´�.
/// </summary>
class InputManager : public Component
{
private:
	int currentSelectedSerialNumber;

public:
	void Start() override;
	void Update() override;

};

