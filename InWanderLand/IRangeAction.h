#pragma once
#include "YunutyEngine.h"

class IRangeAction :
    public Component
{
public:
    virtual void IdentifyAction(Vector3d position) = 0;          // �ν� ���� ���� ������ �� ȣ��� �Լ�
    virtual void AttackAction(Vector3d position) = 0;            // ���� ���� ���� ������ �� ȣ��� �Լ�
};

