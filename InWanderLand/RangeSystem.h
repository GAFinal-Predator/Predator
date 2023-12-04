#pragma once
#include "YunutyEngine.h"
/// <summary>
/// GetGameObject->GetComponent<IRangeAction>�� ���� �Լ�ȣ�� �� ����. 
/// </summary>

class BaseUnitEntity;

class RangeSystem : public CircleCollider2D
{
private:
    BaseUnitEntity* m_unitComponent;
    float m_tempIDRadius;
    float m_tempAtkRadius;

public:   
    void SetOwnerUnitComponent(BaseUnitEntity* unitComponent);

public:
    virtual void Start() override;
    virtual void OnCollisionEnter2D(const Collision2D& collision) override;
    virtual void OnCollisionExit2D(const Collision2D& collision) override;
};

