#pragma once
#include "YunutyEngine.h"
/// <summary>
/// GetGameObject->GetComponent<IRangeAction>�� ���� �Լ�ȣ�� �� ����. 
/// </summary>
class RangeSystem : public Component
{
private:
    GameObject* m_unit;
    string m_type;
public:
    void SetUnitGameObject(GameObject* obj);
    void SetRangeType(string p_type);
public:
    virtual void OnCollisionEnter2D(const Collision2D& collision) override;
};

