#pragma once
#include "UnitBuff.h"

// 아군은 공격속도 증진, 적군은 공격속도 감소
class UnitBuffHanselDebuff : public UnitBuff
{
public:
    UnitBuffHanselDebuff();
    std::shared_ptr<float> attackSpeedAdder;
    std::shared_ptr<float> attackDamageMultiplier;
    virtual UIEnumID GetUIEnumID() { return UIEnumID::CharInfo_Buff_HanselDebuff; };
    virtual UnitBuffType GetBuffType() { return UnitBuffType::HanselDebuff; };
private:
    std::weak_ptr<VFXAnimator> buffEffectAnimator;

    virtual void OnStart();
    virtual void OnEnd();
    virtual void OnOverlap(UnitBuff&& overlapping) override;
    virtual void OnPause() override;
    virtual void OnResume() override;
    virtual coroutine::Coroutine EffectCoroutine() override;
};