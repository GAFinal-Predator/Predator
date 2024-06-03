#pragma once
#include "YunutyEngine.h"
#include "SkillType.h"

class Unit;
class Skill
{
protected:
    std::weak_ptr<Unit> owner;
public:
    virtual coroutine::Coroutine operator()() = 0;
    virtual SkillType::Enum GetSkillType() { return SkillType::NONE; };

    virtual void OnFinishInterrupted() {};
    virtual void OnFinishSuccessful() {};

    friend Unit;
};
