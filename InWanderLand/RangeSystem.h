#pragma once
#include "YunutyEngine.h"
/// <summary>
/// GetGameObject->GetComponent<IRangeAction>�� ���� �Լ�ȣ�� �� ����. 
/// </summary>

class Unit;

class RangeSystem : public CircleCollider2D
{
private:
    Unit* m_unitComponent;
    float m_tempIDRadius;
    float m_tempAtkRadius;

public:   
    void SetUnitComponent(Unit* unitComponent);
    void SetIdRadius(float radius);
    void SetAtkRadius(float radius);

public:
    virtual void Start() override;
    virtual void OnCollisionEnter2D(const Collision2D& collision) override;
    virtual void OnCollisionExit2D(const Collision2D& collision) override;
};

