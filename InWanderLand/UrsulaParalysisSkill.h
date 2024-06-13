#pragma once
#include "Skill.h"

struct POD_UrsulaParalysisSkill
{
    float skillCoolTime = 2.0f;
    float skillCost = 10.0f;
    float skillRange = 6.f;
    float skillRadius = 1.f;
    float skillDamage = 5.f;
    float skillParalysisTime = 3.0f;
    float knockBackDuration = 2.0f;
    TO_JSON(POD_UrsulaParalysisSkill)
        FROM_JSON(POD_UrsulaParalysisSkill)
};

class UrsulaParalysisSkill : public Skill
{
public:
    UrsulaParalysisSkill() {}
    virtual SkillType::Enum GetSkillType() { return SkillType::Enum::URSULA_W; }
    virtual float GetCastRange() override { return pod.skillRange; }
    virtual coroutine::Coroutine operator()()override;
    virtual void OnInterruption()override;

    static POD_UrsulaParalysisSkill pod;

private:
    float colliderEffectRatio;

    coroutine::Coroutine SpawningFieldEffect(std::weak_ptr<UrsulaParalysisSkill> skill);
    std::weak_ptr<coroutine::Coroutine> effectColliderCoroutine;
    std::weak_ptr<UnitAcquisitionSphereCollider> damageCollider;
    std::weak_ptr<UnitAcquisitionSphereCollider> knockBackCollider;
    std::weak_ptr<ManagedFBX> tentacleObject;
    std::weak_ptr<ManagedFBX> waveObject;
};






template <>
bool SkillPodFieldPreEncoding<SkillType::Enum::URSULA_W>(json& data)
{
    application::FieldPreEncoding<boost::pfr::tuple_size_v<POD_UrsulaParalysisSkill>>(UrsulaParalysisSkill::pod, data["POD"]);
    return true;
}


template <>
bool SkillPodFieldPostEncoding<SkillType::Enum::URSULA_W>(json& data)
{
    application::FieldPostEncoding<boost::pfr::tuple_size_v<POD_UrsulaParalysisSkill>>(UrsulaParalysisSkill::pod, data["POD"]);
    return true;
}

template <>
bool SkillPodFieldPreDecoding<SkillType::Enum::URSULA_W>(const json& data)
{
    application::FieldPreDecoding<boost::pfr::tuple_size_v<POD_UrsulaParalysisSkill>>(UrsulaParalysisSkill::pod, data["POD"]);
    return true;
}

template <>
bool SkillPodFieldPostDecoding<SkillType::Enum::URSULA_W>(const json& data)
{
    application::FieldPostDecoding<boost::pfr::tuple_size_v<POD_UrsulaParalysisSkill>>(UrsulaParalysisSkill::pod, data["POD"]);
    return true;
}
