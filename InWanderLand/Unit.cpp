#include "InWanderLand.h"
#include "Unit.h"
#include "PlayerController.h"
#include "AttackSystem.h"
#include "RangedAttackSystem.h"
#include "InputManager.h"
#include "PlayerSkillSystem.h"
#include "ChessTrapSkillSystem.h"
#include "SpikeSkillSystem.h"
#include "BossSkillSystem.h"
#include "TacticModeSystem.h"
#include "IAnimation.h"
#include "VFXAnimator.h"
#include "SkillPreviewSystem.h"

#include "BurnEffect.h"
#include "DebuggingMeshPool.h"
#include "StatusEffect.h"
#include "CinematicManager.h"
#include "UnitBuff.h"
#include "wanderResources.h"
#include "FBXPool.h"
#include "ProjectileType.h"
#include "RTSCam.h"

coroutine::Coroutine ShowPath(const std::vector<Vector3d> paths)
{
    for (int i = 0; i < paths.size(); i++)
    {
        DebugBeacon::PlaceBeacon(paths[i], yunuGI::Color::green(), Vector3d::one * 0.5, 0.5);
    }
    co_return;
}
float getDeltaAngle(float difference) {
    while (difference < -180) difference += 360;
    while (difference > 180) difference -= 360;
    return difference;
}
bool Unit::pauseAll{ false };
std::weak_ptr<Unit> Unit::GetClosestEnemy()
{
    auto minIt = std::min_element(attackRange.lock()->GetEnemies().begin(), attackRange.lock()->GetEnemies().end(), [this](const Unit* const a, const Unit* const b)
        {
            return (GetTransform()->GetWorldPosition() - a->GetTransform()->GetWorldPosition()).MagnitudeSqr() <
                (GetTransform()->GetWorldPosition() - b->GetTransform()->GetWorldPosition()).MagnitudeSqr();
        });
    if (minIt != attackRange.lock()->GetEnemies().end())
        return (*minIt)->GetWeakPtr<Unit>();

    minIt = std::min_element(acquisitionRange.lock()->GetEnemies().begin(), acquisitionRange.lock()->GetEnemies().end(), [this](const Unit* const a, const Unit* const b)
        {
            return (GetTransform()->GetWorldPosition() - a->GetTransform()->GetWorldPosition()).MagnitudeSqr() <
                (GetTransform()->GetWorldPosition() - b->GetTransform()->GetWorldPosition()).MagnitudeSqr();
        });
    if (minIt != acquisitionRange.lock()->GetEnemies().end())
        return (*minIt)->GetWeakPtr<Unit>();

    return std::weak_ptr<Unit>();
}
void Unit::Update()
{
    if (referenceDisableNavAgent.BeingReferenced())
    {
        navAgentEnableFrameCount = 0;
        navAgentComponent.lock()->SetActive(false);
    }
    else
    {
        // navAgent 복구는 한 프레임 이후에 진행된다.
        if (navAgentEnableFrameCount >= 1)
            navAgentComponent.lock()->SetActive(!referenceDisableNavAgent.BeingReferenced());
        navAgentEnableFrameCount++;
    }
    navObstacle.lock()->SetActive(referenceEnableNavObstacle.BeingReferenced());
    //navObstacle.lock()->SetActive(false);

    if (!referenceBlockFollowingNavAgent.BeingReferenced())
        GetTransform()->SetWorldPosition(navAgentComponent.lock()->GetTransform()->GetWorldPosition());
    if (!referenceBlockRotation.BeingReferenced())
        UpdateRotation();
    // UI가 유닛을 따라다니게 만듬
    auto& offset = unitTemplateData->pod.statusBarOffset;
    if (!unitStatusUI.expired())
        unitStatusUI.lock()->GetTransform()->SetWorldPosition(Vector3d{ offset.x,offset.y,0 }
    + UIManager::Instance().GetUIPosFromWorld(GetTransform()->GetWorldPosition()));
    // 버프 효과 적용
    for (auto& [buffID, buff] : buffs)
    {
        buff.get()->OnUpdate();
        buff.get()->durationLeft -= Time::GetDeltaTime() * localBuffTimeScale;
        if (buff.get()->durationLeft < 0 || currentHitPoint <= 0.0f)
            buff.get()->OnEnd();
    }
    std::erase_if(buffs, [](std::pair<const UnitBuffType, std::shared_ptr<UnitBuff>>& pair)
        { return pair.second->durationLeft < 0; });
    // behaviour tree에 맞게 동작
    unitBehaviourTree.Update();
    auto navPos = navAgentComponent.lock()->GetTransform()->GetWorldPosition();
    auto navSpeed = navAgentComponent.lock()->GetSpeed();
    lastPosition = GetTransform()->GetWorldPosition();
    // 재생중인 애니메이션이 없다면 기본 애니메이션 출력
    // 어떤게 기본 애니메이션인지는 행동트리의 상태에 따라 바뀔 수 있다.
    if ((animatorComponent.lock()->IsDone() || blendWithDefaultAnimTrigger) && defaultAnimationType != UnitAnimType::None)
    {
        blendWithDefaultAnimTrigger = false;
        PlayAnimation(defaultAnimationType);
    }
}

void Unit::OnDestroy()
{
    if (!unitStatusUI.expired())
        Scene::getCurrentScene()->DestroyGameObject(unitStatusUI.lock()->GetGameObject());
    if (!navAgentComponent.expired())
        Scene::getCurrentScene()->DestroyGameObject(navAgentComponent.lock()->GetGameObject());
}

template<>
void Unit::OnStateEngage<UnitBehaviourTree::Death>()
{
    onStateEngage[UnitBehaviourTree::Death]();
    PlayAnimation(UnitAnimType::Death);
    enableNavObstacleByState.reset();
    disableNavAgentByState = referenceDisableNavAgent.Acquire();
    defaultAnimationType = UnitAnimType::None;
    coroutineDeath = StartCoroutine(DeathCoroutine());
}
template<>
void Unit::OnStateExit<UnitBehaviourTree::Death>()
{
    onStateExit[UnitBehaviourTree::Death]();
    disableNavAgentByState.reset();
}
template<>
void Unit::OnStateEngage<UnitBehaviourTree::Paralysis>()
{
    onStateEngage[UnitBehaviourTree::Paralysis]();
    PlayAnimation(UnitAnimType::Paralysis, Animation::PlayFlag_::Blending | Animation::PlayFlag_::Repeat);
}
template<>
void Unit::OnStateExit<UnitBehaviourTree::Paralysis>()
{
    onStateExit[UnitBehaviourTree::Paralysis]();
    PlayAnimation(UnitAnimType::Paralysis, Animation::PlayFlag_::Blending | Animation::PlayFlag_::Repeat);
}
template<>
void Unit::OnStateEngage<UnitBehaviourTree::Knockback>()
{
    onStateEngage[UnitBehaviourTree::Knockback]();
    blockFollowingNavAgentByState = referenceBlockFollowingNavAgent.Acquire();
}
template<>
void Unit::OnStateExit<UnitBehaviourTree::Knockback>()
{
    onStateExit[UnitBehaviourTree::Knockback]();
    blockFollowingNavAgentByState.reset();
    OrderHold();
}
template<>
void Unit::OnStateEngage<UnitBehaviourTree::Pause>()
{
    onStateEngage[UnitBehaviourTree::Pause]();
    defaultAnimationType = UnitAnimType::Idle;
    enableNavObstacleByState = referenceEnableNavObstacle.Acquire();
    disableNavAgentByState = referenceDisableNavAgent.Acquire();
    //PlayAnimation(UnitAnimType::Idle, Animation::PlayFlag_::Blending | Animation::PlayFlag_::Repeat);
    animatorComponent.lock()->Pause();
}
template<>
void Unit::OnStateExit<UnitBehaviourTree::Pause>()
{
    onStateExit[UnitBehaviourTree::Pause]();
    //PlayAnimation(UnitAnimType::Idle, Animation::PlayFlag_::Blending | Animation::PlayFlag_::Repeat);
    animatorComponent.lock()->Resume();
    enableNavObstacleByState.reset();
    disableNavAgentByState.reset();
}
template<>
void Unit::OnStateEngage<UnitBehaviourTree::Reviving>()
{
    onStateEngage[UnitBehaviourTree::Reviving]();
    enableNavObstacleByState = referenceEnableNavObstacle.Acquire();
    disableNavAgentByState = referenceDisableNavAgent.Acquire();
    coroutineRevival = StartCoroutine(RevivalCoroutine());
}
template<>
void Unit::OnStateExit<UnitBehaviourTree::Reviving>()
{
    onStateExit[UnitBehaviourTree::Reviving]();
    enableNavObstacleByState.reset();
    disableNavAgentByState.reset();
}
template<>
void Unit::OnStateUpdate<UnitBehaviourTree::Chasing>()
{
    if (!referenceBlockPendingOrder.BeingReferenced() && !pendingTargetUnit.expired())
    {
        currentTargetUnit = pendingTargetUnit;
        pendingTargetUnit.reset();
    }
}
template<>
void Unit::OnStateEngage<UnitBehaviourTree::Attack>()
{
    onStateEngage[UnitBehaviourTree::Attack]();
    PlayAnimation(UnitAnimType::Idle);
    defaultAnimationType = UnitAnimType::Idle;
    enableNavObstacleByState = referenceEnableNavObstacle.Acquire();
    disableNavAgentByState = referenceDisableNavAgent.Acquire();
}
template<>
void Unit::OnStateExit<UnitBehaviourTree::Attack>()
{
    DeleteCoroutine(coroutineAttack);
    DeleteCoroutine(coroutineAttackEffect);
    animatorComponent.lock()->GetGI().SetPlaySpeed(1);
    onStateExit[UnitBehaviourTree::Attack]();
    enableNavObstacleByState.reset();
    disableNavAgentByState.reset();
}
template<>
void Unit::OnStateUpdate<UnitBehaviourTree::Attack>()
{
    auto currentTarget = currentTargetUnit.lock();
    if (!currentTarget)
    {
        return;
    }
    else
    {
        if (!currentTarget->IsAlive() || currentTarget->IsInvulenerable())
        {
            currentTargetUnit.reset();
            return;
        }
    }
    SetDesiredRotation(currentTargetUnit.lock()->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition());
    if (!referenceBlockAttack.BeingReferenced())
    {
        coroutineAttack = StartCoroutine(AttackCoroutine(currentTargetUnit));
    }
}
template<>
void Unit::OnStateEngage<UnitBehaviourTree::Move>()
{
    onStateEngage[UnitBehaviourTree::Move]();
    navAgentComponent.lock()->SetSpeed(unitTemplateData->pod.m_unitSpeed);
    StartCoroutine(ShowPath(SingleNavigationField::Instance().GetSmoothPath(GetTransform()->GetWorldPosition() + GetTransform()->GetWorldRotation().Forward() * unitTemplateData->pod.collisionSize, moveDestination)));
    PlayAnimation(UnitAnimType::Move, Animation::PlayFlag_::Blending | Animation::PlayFlag_::Repeat);
}
template<>
void Unit::OnStateUpdate<UnitBehaviourTree::Move>()
{
    static constexpr float epsilon = 0.1f;
    if ((moveDestination - GetTransform()->GetWorldPosition()).MagnitudeSqr() < epsilon)
    {
        OrderAttackMove(moveDestination);
    }
    navAgentComponent.lock()->MoveTo(moveDestination);
    SetDesiredRotation(GetTransform()->GetWorldPosition() - lastPosition);
}
template<>
void Unit::OnStateEngage<UnitBehaviourTree::Skill>()
{
    onStateEngage[UnitBehaviourTree::Skill]();
    currentOrderType = UnitOrderType::Skill;
}
template<>
void Unit::OnStateExit<UnitBehaviourTree::SkillOnGoing>()
{
    onStateExit[UnitBehaviourTree::SkillOnGoing]();
    if (!coroutineSkill.expired())
    {
        onGoingSkill->OnInterruption();
        DeleteCoroutine(coroutineSkill);
    }
    onGoingSkill.reset();
    OrderAttackMove();
}
template<>
void Unit::OnStateEngage<UnitBehaviourTree::SkillCasting>()
{
    assert(pendingSkill.get() != nullptr);
    SetDesiredRotation(pendingSkill.get()->targetPos - GetTransform()->GetWorldPosition());
    onGoingSkill = std::move(pendingSkill);
    coroutineSkill = StartCoroutine(onGoingSkill.get()->operator()());
}
template<>
void Unit::OnStateEngage<UnitBehaviourTree::Tactic>()
{
    onStateEngage[UnitBehaviourTree::Tactic]();
    enableNavObstacleByState = referenceEnableNavObstacle.Acquire();
    disableNavAgentByState = referenceDisableNavAgent.Acquire();
}
template<>
void Unit::OnStateExit<UnitBehaviourTree::Tactic>()
{
    onStateExit[UnitBehaviourTree::Tactic]();
    enableNavObstacleByState.reset();
    disableNavAgentByState.reset();
}

template<>
void Unit::OnStateEngage<UnitBehaviourTree::Stop>()
{
    onStateEngage[UnitBehaviourTree::Stop]();
    PlayAnimation(UnitAnimType::Idle, Animation::PlayFlag_::Blending | Animation::PlayFlag_::Repeat);
    enableNavObstacleByState = referenceEnableNavObstacle.Acquire();
    disableNavAgentByState = referenceDisableNavAgent.Acquire();
}
template<>
void Unit::OnStateExit<UnitBehaviourTree::Stop>()
{
    onStateExit[UnitBehaviourTree::Stop]();
    enableNavObstacleByState.reset();
    disableNavAgentByState.reset();
}

Unit::~Unit()
{
    if (!unitStatusUI.expired())
    {
        Scene::getCurrentScene()->DestroyGameObject(unitStatusUI.lock()->GetGameObject());
    }
    if (!navAgentComponent.expired())
    {
        Scene::getCurrentScene()->DestroyGameObject(navAgentComponent.lock()->GetGameObject());
    }
}

void Unit::OnPause()
{
    isPaused = true;
    localBuffTimeScale = FLT_MIN * 10000;
    if (!IsPlayerUnit())
    {
        localTimeScale = FLT_MIN * 10000;
        animatorComponent.lock()->Pause();

        for (auto& each : GetGameObject()->GetChildren())
        {
            if (auto ptr = each->GetComponent<graphics::ParticleRenderer>())
            {
                ptr->Pause();
            }
        }
    }
}

void Unit::OnResume()
{
    isPaused = false;
    localBuffTimeScale = 1.0f;
    if (!IsPlayerUnit())
    {
        localTimeScale = 1.0f;
        animatorComponent.lock()->Resume();

        for (auto& each : GetGameObject()->GetChildren())
        {
            if (auto ptr = each->GetComponent<graphics::ParticleRenderer>())
            {
                ptr->Resume();
            }
        }
    }
}

bool Unit::IsPlayerUnit() const
{
    return unitTemplateData->pod.playerUnitType.enumValue != PlayerCharacterType::None;
}
bool Unit::IsInvulenerable() const
{
    return referenceInvulnerable.BeingReferenced();
}
bool Unit::IsAlive() const
{
    return isAlive;
}

bool Unit::IsPreempted() const
{
    return (*unitBehaviourTree.GetActiveNodes().rbegin())->GetNodeKey() == UnitBehaviourTree::Knockback;
}

bool Unit::IsTacTicReady() const
{
    for (auto each : unitBehaviourTree.GetActiveNodes())
    {
        if (each->GetNodeKey() == UnitBehaviourTree::Tactic)
        {
            return true;
        }
    }

    return false;
}

float Unit::GetCritChance()
{
    return unitTemplateData->pod.m_critChance + adderCritChance;
}

float Unit::GetCritMultiplier()
{
    return unitTemplateData->pod.m_critMultiplier;
}

int Unit::GetArmor()
{
    return unitTemplateData->pod.m_armor;
}

float Unit::GetEvasionChance()
{
    return unitTemplateData->pod.m_evasionChance;
}

float Unit::GetCritResistance()
{
    return unitTemplateData->pod.m_critResistance;
}

std::string Unit::GetFBXName() const
{
    if (!skinnedMeshGameObject)
    {
        return std::string();
    }

    return skinnedMeshGameObject->getName();
}
Vector3d Unit::GetRandomPositionInsideCapsuleCollider()
{
    const auto& pod = GetUnitTemplateData().pod;
    auto unitPos = GetTransform()->GetWorldPosition();
    return unitPos + Vector3d{ math::Random::GetRandomFloat(pod.collisionSize * 0.3f), pod.collisionSize + math::Random::GetRandomFloat(0 , pod.collisionHeight), math::Random::GetRandomFloat(pod.collisionSize * 0.3f) };
}
void Unit::EraseBuff(UnitBuffType buffType)
{
    if (buffs.find(buffType) != buffs.end())
        buffs.find(buffType)->second->OnEnd();
    buffs.erase(buffType);
}
void Unit::Damaged(std::weak_ptr<Unit> opponentUnit, float opponentDmg, DamageType damageType, Transform* projectileTransform)
{
    switch (damageType)
    {
    case DamageType::Attack:
        // 치명타의 경우 치명타 배율은 이미 opponentDmg에 반영된 것으로 친다.
    case DamageType::AttackCrit:
        if (math::Random::ByRandomChance(GetEvasionChance()))
        {
            // 공격 빗나감
            opponentDmg = 0;
            return;
        }

        if (damageType == DamageType::AttackCrit)
        {
            opponentDmg *= 1 - GetCritResistance();
        }
        opponentDmg *= 1 - GetArmor() * 0.01f;
        opponentUnit.lock()->onAttackHit(GetWeakPtr<Unit>());
        break;
    case DamageType::Skill:
        break;
    default:
        break;
    }

    coroutineDamagedEffect = StartCoroutine(DamagedEffectCoroutine(opponentUnit, projectileTransform));
    coroutineDamagedEffect.lock()->PushDestroyCallBack([this]()
        {
            if (!damagedEffectQueue.empty())
            {
                if (!damagedEffectQueue.front().expired())
                {
                    FBXPool::Instance().Return(damagedEffectQueue.front());
                    damagedEffectQueue.pop();
                }
            }
        });

    onDamagedFromUnit(opponentUnit);
    Damaged(opponentDmg);
}

void Unit::Damaged(float dmg)
{
    SetCurrentHp(currentHitPoint -= dmg);
    onDamaged();
}

yunutyEngine::coroutine::Coroutine Unit::DamagedEffectCoroutine(std::weak_ptr<Unit> opponent, Transform* projectileTransform)
{
    if (projectileTransform == nullptr)
    {
        co_return;
    }

    auto damagedVFX = wanderResources::GetVFX(opponent.lock()->unitTemplateData->pod.skinnedFBXName, UnitAnimType::Damaged);
    co_await std::suspend_always{};
    auto vfxAnimator = damagedVFX.lock()->AcquireVFXAnimator();
    vfxAnimator.lock()->SetAutoActiveFalse();
    vfxAnimator.lock()->Init();
    vfxAnimator.lock()->Play();
    //direction *= -1;
    //damagedVFX.lock()->GetGameObject()->GetTransform()->SetWorldPosition(startPos + direction * 2);
    damagedVFX.lock()->GetGameObject()->GetTransform()->SetWorldPosition(projectileTransform->GetWorldPosition());
    auto temp = projectileTransform->GetLocalRotation();
    auto euler = temp.Euler();
    euler.y += -180;
    damagedVFX.lock()->GetGameObject()->GetTransform()->SetWorldRotation(Quaternion{ euler });
    //damagedVFX.lock()->GetGameObject()->GetTransform()->SetWorldRotation(direction);
    //damagedVFX.lock()->GetGameObject()->GetTransform()->SetWorldScale(GetTransform()->GetWorldScale());

    damagedEffectQueue.push(damagedVFX);

    auto relativePos = projectileTransform->GetWorldPosition() - GetTransform()->GetWorldPosition();
    auto prevRot = QuaternionToEastAngle(GetTransform()->GetWorldRotation());
    
    while (!vfxAnimator.lock()->IsDone())
    {
        damagedVFX.lock()->GetGameObject()->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + relativePos);
        //damagedVFX.lock()->GetGameObject()->GetTransform()->SetWorldRotation(damagedVFX.lock()->GetGameObject()->GetTransform()->GetWorldRotation() * Quaternion(Vector3d{ 0, prevRot - QuaternionToEastAngle(GetTransform()->GetWorldRotation()), 0}));
        //prevRot = QuaternionToEastAngle(GetTransform()->GetWorldRotation());
        co_await std::suspend_always{};
    }

    FBXPool::Instance().Return(damagedVFX);

    co_return;
}


void Unit::Heal(float healingPoint)
{
    // 최대 체력이면 x
    SetCurrentHp(currentHitPoint += healingPoint);
    if (currentHitPoint >= unitTemplateData->pod.max_Health)
        SetCurrentHp(unitTemplateData->pod.max_Health);

    coroutineHealEffect = StartCoroutine(HealEffectCoroutine());
    coroutineHealEffect.lock()->PushDestroyCallBack([this]()
        {
            FBXPool::Instance().Return(healVFX);
        });
}

yunutyEngine::coroutine::Coroutine Unit::HealEffectCoroutine()
{
    healVFX = FBXPool::Instance().Borrow("VFX_Buff_Healing");
    co_await std::suspend_always{};
    auto vfxAnimator = healVFX.lock()->AcquireVFXAnimator();
    vfxAnimator.lock()->SetAutoActiveFalse();
    vfxAnimator.lock()->Init();
    vfxAnimator.lock()->Play();

    healVFX.lock()->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
    healVFX.lock()->GetTransform()->SetWorldRotation(GetTransform()->GetWorldRotation());

    while (!vfxAnimator.lock()->IsDone())
    {
        co_await std::suspend_always{};
        healVFX.lock()->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
        healVFX.lock()->GetTransform()->SetWorldRotation(GetTransform()->GetWorldRotation());
    }

    co_return;
}

void Unit::SetCurrentHp(float p_newHp)
{
    currentHitPoint = std::fmax(0, p_newHp);
    if (p_newHp <= 0 && isAlive)
    {
        liveCountLeft--;
        SetIsAlive(false);
    }
    if (!unitStatusUI.expired())
    {
        unitStatusUI.lock()->GetLocalUIsByEnumID().at(UIEnumID::StatusBar_HP_Fill)->adjuster->SetTargetFloat(1 - currentHitPoint / unitTemplateData->pod.max_Health);
        if (unitStatusUI.lock()->GetLocalUIsByEnumID().contains(UIEnumID::StatusBar_HP_Number_Current))
        {
            unitStatusUI.lock()->GetLocalUIsByEnumID().at(UIEnumID::StatusBar_HP_Number_Current)->SetNumber(currentHitPoint);
        }
    }
    if (!unitStatusPortraitUI.expired())
    {
        unitStatusPortraitUI.lock()->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_HP_Fill)->adjuster->SetTargetFloat(currentHitPoint / unitTemplateData->pod.max_Health);
        unitStatusPortraitUI.lock()->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_PortraitBloodOverlay)->adjuster->SetTargetFloat(1 - currentHitPoint / unitTemplateData->pod.max_Health);
        unitStatusPortraitUI.lock()->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_HP_Number_Current)->SetNumber(currentHitPoint);
        unitStatusPortraitUI.lock()->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_HP_Number_Max)->SetNumber(unitTemplateData->pod.max_Health);
    }
    if (!unitStatusPortraitUI2.expired())
    {
        unitStatusPortraitUI2.lock()->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_HP_Fill)->adjuster->SetTargetFloat(currentHitPoint / unitTemplateData->pod.max_Health);
        unitStatusPortraitUI2.lock()->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_PortraitBloodOverlay)->adjuster->SetTargetFloat(1 - currentHitPoint / unitTemplateData->pod.max_Health);
        unitStatusPortraitUI2.lock()->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_HP_Number_Current)->SetNumber(currentHitPoint);
        unitStatusPortraitUI2.lock()->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_HP_Number_Max)->SetNumber(unitTemplateData->pod.max_Health);
    }

}

float Unit::GetUnitCurrentHp() const
{
    return currentHitPoint;
}

float Unit::GetUnitMaxHp() const
{
    return unitTemplateData->pod.max_Health;
}

void Unit::KnockBack(Vector3d targetPosition, float knockBackDuration)
{
    if (IsAlive())
    {
        DeleteCoroutine(coroutineKnockBack);
        coroutineKnockBack = StartCoroutine(KnockbackCoroutine(referenceParalysis.Acquire(), targetPosition, knockBackDuration));
    }
}

void Unit::KnockBackRelativeVector(Vector3d relativeVector, float knockBackDuration)
{
    if (IsAlive())
    {
        DeleteCoroutine(coroutineKnockBack);
        coroutineKnockBack = StartCoroutine(KnockbackCoroutine(referenceParalysis.Acquire(), relativeVector, knockBackDuration, true));
    }
}

void Unit::Paralyze(float paralyzeDuration)
{
    StartCoroutine(referenceParalysis.AcquireForSecondsCoroutine(paralyzeDuration));
    auto paralCoroutine = StartCoroutine(ParalyzeEffectCoroutine(paralyzeDuration));
    paralCoroutine.lock()->PushDestroyCallBack([this]()
        {
            FBXPool::Instance().Return(paralysisVFX);
        });
}

yunutyEngine::coroutine::Coroutine Unit::ParalyzeEffectCoroutine(float paralyzeDuration)
{
    paralysisVFX = FBXPool::Instance().Borrow("VFX_Monster_HitCC");
    co_await std::suspend_always{};

    auto paralysisEffectAnimator = paralysisVFX.lock()->AcquireVFXAnimator();
    paralysisEffectAnimator.lock()->SetAutoActiveFalse();
    paralysisEffectAnimator.lock()->Init();
    paralysisEffectAnimator.lock()->Play();
    paralysisEffectAnimator.lock()->SetLoop(true);

    paralysisVFX.lock()->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
    paralysisVFX.lock()->GetTransform()->SetWorldRotation(GetTransform()->GetWorldRotation());
    paralysisVFX.lock()->GetTransform()->SetWorldScale(GetTransform()->GetWorldScale());

    coroutine::ForSeconds forSeconds = paralyzeDuration;

    while (forSeconds.Tick())
    {
        co_await std::suspend_always{};
        paralysisVFX.lock()->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
        paralysisVFX.lock()->GetTransform()->SetWorldRotation(GetTransform()->GetWorldRotation());
    }

    co_return;
}

yunutyEngine::coroutine::Coroutine Unit::KnockbackCoroutine(std::shared_ptr<Reference::Guard> paralysisGuard, Vector3d targetPosition, float knockBackDuration, bool relative)
{
    coroutine::ForSeconds forSeconds{ knockBackDuration };
    const auto& constant = GlobalConstant::GetSingletonInstance().pod;
    constant.gravitySpeed;
    // y = vy0 * t - 0.5 * g * t^2
    // vy0 = (0 + 0.5 * g * t^2) / t
    float vy0 = 0.5 * constant.gravitySpeed * knockBackDuration;
    float y;
    Vector3d startPos = GetTransform()->GetWorldPosition();

    if (relative)
    {
        targetPosition = startPos + targetPosition;
    }

    navAgentComponent.lock()->Relocate(targetPosition);

    while (forSeconds.Tick())
    {
        bool isNavAgentActive = navAgentComponent.lock()->GetActive();
        navAgentComponent.lock()->MoveTo(targetPosition + Vector3d::one * 0.0001);
        y = vy0 * forSeconds.Elapsed() - 0.5 * constant.gravitySpeed * forSeconds.Elapsed() * forSeconds.Elapsed();
        Vector3d pos = Vector3d::Lerp(startPos, navAgentComponent.lock()->GetTransform()->GetWorldPosition(), forSeconds.ElapsedNormalized());
        pos.y = y;
        GetTransform()->SetWorldPosition(pos);
        co_await std::suspend_always{};
    }
    co_return;
}
void Unit::PlayAnimation(UnitAnimType animType, Animation::PlayFlag playFlag)
{
    if (playingBattleAnim)
    {
        switch (animType)
        {
        case UnitAnimType::Idle: animType = UnitAnimType::BattleIdle; break;
        case UnitAnimType::Move: animType = UnitAnimType::BattleMove; break;
        }
    }
    auto anim = wanderResources::GetAnimation(unitTemplateData->pod.skinnedFBXName, animType);

    if (animatorComponent.lock()->GetGI().GetCurrentAnimation() == nullptr
        || animatorComponent.lock()->GetGI().GetCurrentAnimation() == anim
        || !(playFlag & Animation::PlayFlag_::Blending))
    {
        animatorComponent.lock()->Play(anim);
    }
    else
    {
        animatorComponent.lock()->ChangeAnimation(anim, GlobalConstant::GetSingletonInstance().pod.defaultAnimBlendTime, 1);
    }

    if (playFlag & Animation::PlayFlag_::Repeat)
    {
        blendWithDefaultAnimTrigger = true;
        defaultAnimationType = animType;
        //defaultAnimation = anim;
    }
}
void Unit::BlendWithDefaultAnimation()
{
    blendWithDefaultAnimTrigger = true;
}
void Unit::SetDefaultAnimation(UnitAnimType animType)
{
    defaultAnimationType = animType;
}
float normalizeAngle(float angle) {
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;
    return angle;
}
void Unit::SetDesiredRotation(const Vector3d& facingDirection)
{
    static constexpr float epsilon = 1.0f;
    if (facingDirection.MagnitudeSqr() < epsilon * Time::GetDeltaTime() * localTimeScale * Time::GetDeltaTime() * localTimeScale)
        return;
    desiredRotation = std::atan2(facingDirection.z, facingDirection.x) * math::Rad2Deg;
}
std::weak_ptr<coroutine::Coroutine> Unit::SetRotation(const Quaternion& targetRotation, float rotatingTime)
{
    return SetRotation(targetRotation * -Vector3d::forward, rotatingTime);
}
std::weak_ptr<coroutine::Coroutine> Unit::SetRotation(const Vector3d& facingDirection, float rotatingTime)
{
    desiredRotation = std::atan2(facingDirection.z, facingDirection.x) * math::Rad2Deg;
    return SetRotation(desiredRotation, rotatingTime);
}
std::weak_ptr<coroutine::Coroutine> Unit::SetRotation(float facingAngle, float rotatingTime)
{
    return StartCoroutine(SettingRotation(facingAngle, rotatingTime));
}
coroutine::Coroutine Unit::SettingRotation(float facingAngle, float rotatingTime)
{
    auto pauseGuard = referencePause.Acquire();
    desiredRotation = facingAngle;
    if (rotatingTime != 0)
    {
        currentRotationSpeed = std::fabs(getDeltaAngle(desiredRotation - currentRotation)) / rotatingTime;
    }
    else
    {
        currentRotationSpeed = 1000000000;
    }

    wanderUtils::UnitCoroutine::ForSecondsFromUnit waitRotating{ GetWeakPtr<Unit>(), rotatingTime };

    while (waitRotating.Tick())
    {
        co_await std::suspend_always();
    }
    co_await std::suspend_always();

    currentRotationSpeed = unitTemplateData->pod.rotationSpeed;
}
void Unit::SetIsAlive(bool isAlive)
{
    this->isAlive = isAlive;
}
void Unit::UpdateRotation()
{
    currentRotation = normalizeAngle(currentRotation);
    desiredRotation = normalizeAngle(desiredRotation);
    float difference = desiredRotation - currentRotation;
    difference = getDeltaAngle(difference);

    if (std::fabs(difference) < currentRotationSpeed * Time::GetDeltaTime() * localTimeScale) {
        currentRotation = desiredRotation;
    }
    else if (difference > 0) {
        currentRotation = normalizeAngle(currentRotation + currentRotationSpeed * Time::GetDeltaTime() * localTimeScale);
    }
    else {
        currentRotation = normalizeAngle(currentRotation - currentRotationSpeed * Time::GetDeltaTime() * localTimeScale);
    }
    GetTransform()->SetWorldRotation(Vector3d(0, -currentRotation + 90, 0));
}
void Unit::OnContentsStop()
{
    ReturnToPool();
}
void Unit::OnEnable()
{
    navAgentComponent.lock()->GetGameObject()->SetSelfActive(true);
}
void Unit::OnDisable()
{
    navAgentComponent.lock()->GetGameObject()->SetSelfActive(false);
}
void Unit::Start()
{
}
void Unit::Relocate(const Vector3d& pos)
{
    navAgentComponent.lock()->Relocate(pos);
    //OrderHold();
}
void Unit::OrderMove(Vector3d position)
{
    static constexpr float epsilon = 0.1f;
    if (DistanceSquare(position) < epsilon)
    {
        return;
    }
    pendingOrderType = UnitOrderType::Move;
    moveDestination = position;
    //moveDestination = position;
}
void Unit::OrderAttackMove(Vector3d position)
{
    pendingOrderType = UnitOrderType::AttackMove;
    attackMoveDestination = position;
}
void Unit::OrderAttackMove()
{
    OrderAttackMove(GetTransform()->GetWorldPosition());
}
void Unit::OrderAttack(std::weak_ptr<Unit> opponent)
{
    pendingOrderType = UnitOrderType::AttackUnit;
    pendingTargetUnit = opponent;
}
void Unit::OrderHold()
{
    pendingOrderType = UnitOrderType::Hold;
}
Vector3d Unit::FromTo(std::weak_ptr<Unit> a, std::weak_ptr<Unit> b)
{
    return FromTo(a.lock().get(), b.lock().get());
}
Vector3d Unit::FromTo(Unit* a, Unit* b)
{
    return b->GetTransform()->GetWorldPosition() - a->GetTransform()->GetWorldPosition();
}
float Unit::Distance(std::weak_ptr<Unit> a, std::weak_ptr<Unit> b)
{
    return Distance(a.lock().get(), b.lock().get());
}
float Unit::DistanceSquare(std::weak_ptr<Unit> a, std::weak_ptr<Unit> b)
{
    return DistanceSquare(a.lock().get(), b.lock().get());
}
float Unit::Distance(Unit* a, Unit* b)
{
    return sqrtf(DistanceSquare(a, b));
}
float Unit::DistanceSquare(Unit* a, Unit* b)
{
    return (a->GetTransform()->GetWorldPosition() - b->GetTransform()->GetWorldPosition()).MagnitudeSqr();
}
float Unit::Distance(Unit* a, const Vector3d& worldPos)
{
    return sqrtf(DistanceSquare(a, worldPos));
}
float Unit::DistanceSquare(Unit* a, const Vector3d& worldPos)
{
    return (a->GetTransform()->GetWorldPosition() - worldPos).MagnitudeSqr();
}
float Unit::QuaternionToEastAngle(const Quaternion& rotation)
{
    auto&& forward = rotation.Forward();
    return 180 + std::atan2f(forward.z, forward.x) * math::Rad2Deg;
}
Quaternion Unit::EastAngleToQuaternion(const float rotation)
{
    return Quaternion();
}
bool Unit::GetPauseAll()
{
    return pauseAll;
}
void Unit::SetPauseAll(bool pause)
{
    pauseAll = pause;
}
float Unit::GetDesiredRotation()
{
    return desiredRotation;
}
float Unit::Distance(const Vector3d& worldPos)
{
    return Unit::Distance(this, worldPos);
}
float Unit::DistanceSquare(const Vector3d& worldPos)
{
    return Unit::DistanceSquare(this, worldPos);
}
// 유닛과 관련된 객체들을 모조리 생성
void Unit::Init(const application::editor::Unit_TemplateData* unitTemplateData)
{
    this->unitTemplateData = unitTemplateData;
    attackRange = GetGameObject()->AddGameObject()->AddComponentAsWeakPtr<UnitAcquisitionSphereCollider>();
    acquisitionRange = GetGameObject()->AddGameObject()->AddComponentAsWeakPtr<UnitAcquisitionSphereCollider>();
    attackRange.lock()->owner = GetWeakPtr<Unit>();
    acquisitionRange.lock()->owner = GetWeakPtr<Unit>();
    navAgentComponent = Scene::getCurrentScene()->AddGameObject()->AddComponentAsWeakPtr<NavigationAgent>();
    navObstacle = navAgentComponent.lock()->GetGameObject()->AddComponentAsWeakPtr<NavigationObstacle>();
    navObstacle.lock()->SetObstacleType(NavigationObstacle::ObstacleType::Cylinder);
    teamIndex = unitTemplateData->pod.playerUnitType.enumValue == PlayerCharacterType::None ? 2 : PlayerController::playerTeamIndex;
    skinnedMeshGameObject = yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX(unitTemplateData->pod.skinnedFBXName);
    burnEffect = skinnedMeshGameObject->AddComponentAsWeakPtr<BurnEffect>();
    animatorComponent = skinnedMeshGameObject->GetComponentWeakPtr<graphics::Animator>();
    skinnedMeshGameObject->SetParent(GetGameObject());
    skinnedMeshGameObject->GetTransform()->SetLocalPosition(Vector3d::zero);
    skinnedMeshGameObject->GetTransform()->SetLocalRotation(Quaternion{ {0,180,0} });
    skinnedMeshGameObject->GetTransform()->SetLocalScale(Vector3d::one * unitTemplateData->pod.unit_scale);
    //wanderResources::PushAnimations(animatorComponent.lock().get(), unitTemplateData->pod.skinnedFBXName);
    unitCollider = GetGameObject()->AddGameObject()->AddComponentAsWeakPtr<UnitCapsuleCollider>();
    unitCollider.lock()->owner = GetWeakPtr<Unit>();
    auto rigidBody = unitCollider.lock()->GetGameObject()->AddComponentAsWeakPtr<physics::RigidBody>();
    rigidBody.lock()->SetAsKinematic(true);
    InitBehaviorTree();
    // 애니메이션에 이벤트 삽입
    auto& ptm = particle::ParticleTool_Manager::GetSingletonInstance();
    const yunuGI::IResourceManager* resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
    std::wstring fbxNameW = yutility::GetWString(unitTemplateData->pod.skinnedFBXName);
    //fbxNameW.assign(pod.templateData->pod.skinnedFBXName.begin(), pod.templateData->pod.skinnedFBXName.end());
    AttachDebugMesh(navAgentComponent.lock()->GetGameObject(), DebugMeshType::Sphere)->GetTransform()->SetLocalScale(Vector3d::one * unitTemplateData->pod.collisionSize);

    /// Particle Setting
    for (auto& eachPI : ptm.GetChildrenParticleInstanceList(unitTemplateData->pod.skinnedFBXName))
    {
        auto pObj = GetGameObject()->AddGameObject();
        auto sptr = eachPI.lock();
        pObj->GetTransform()->SetLocalPosition(Vector3d(sptr->offsetPos.x, sptr->offsetPos.y, -sptr->offsetPos.z));
        pObj->GetTransform()->SetLocalRotation(Quaternion(Vector3d(0, 180, 0)) * sptr->rotation);
        pObj->GetTransform()->SetLocalScale(sptr->scale);
        pObj->setName(sptr->name);
        auto pr = pObj->AddComponent<graphics::ParticleRenderer>();
        pr->SetParticleShape((yunutyEngine::graphics::ParticleShape)sptr->particleData.shape);
        pr->SetParticleMode((yunutyEngine::graphics::ParticleMode)sptr->particleData.particleMode);
        pr->SetLoop(sptr->particleData.isLoop);
        pr->SetDuration(sptr->particleData.duration);
        pr->SetLifeTime(sptr->particleData.lifeTime);
        pr->SetSpeed(sptr->particleData.speed);
        pr->SetStartScale(sptr->particleData.startScale);
        pr->SetEndScale(sptr->particleData.endScale);
        pr->SetMaxParticle(sptr->particleData.maxParticle);
        pr->SetPlayAwake(sptr->particleData.playAwake);
        pr->SetRadius(sptr->particleData.radius);
        pr->SetAngle(sptr->particleData.angle);

        pr->SetRateOverTime(sptr->particleData.rateOverTime);

        pr->SetBurstsCount(sptr->particleData.burstsCount);
        pr->SetInterval(sptr->particleData.interval);

        std::wstring texturePath;
        texturePath.assign(sptr->particleData.texturePath.begin(), sptr->particleData.texturePath.end());
        auto texturePtr = resourceManager->GetTexture(texturePath);
        if (texturePtr)
        {
            pr->SetTexture(texturePtr);
        }

        pObj->SetSelfActive(false);
    }

    /// Animation Event Setting
    auto& list = resourceManager->GetFBXAnimationList(fbxNameW);
    for (auto& each : list)
    {
        std::string aniName;
        aniName.assign(each->GetName().begin(), each->GetName().end());

        for (auto& eventWeak : ptm.GetAnimationEventList(ptm.GetMatchingIAnimation(unitTemplateData->pod.skinnedFBXName, aniName)))
        {
            auto event = eventWeak.lock();
            auto type = event->GetType();
            switch (type)
            {
            case application::AnimationEventType::GameObject_ActivateEvent:
            {
                auto ptr = static_cast<GameObject_ActivateEvent*>(event.get());
                GameObject* particle = nullptr;
                for (auto& child : GetGameObject()->GetChildren())
                {
                    if (child->getName() == ptr->objName)
                    {
                        particle = child;
                        break;
                    }
                }

                animatorComponent.lock()->PushAnimationWithFunc(each, event->frame, [=]()
                    {
                        particle->SetSelfActive(true);
                        auto ptr = particle->GetComponent<graphics::ParticleRenderer>();
                        ptr->Play();
                    });

                break;
            }
            case application::AnimationEventType::GameObject_DisabledEvent:
            {
                auto ptr = static_cast<GameObject_DisabledEvent*>(event.get());
                GameObject* particle = nullptr;
                for (auto& child : GetGameObject()->GetChildren())
                {
                    if (child->getName() == ptr->objName)
                    {
                        particle = child;
                        break;
                    }
                }

                animatorComponent.lock()->PushAnimationWithFunc(each, event->frame, [=]()
                    {
                        particle->SetSelfActive(false);
                    });

                break;
            }
            case application::AnimationEventType::GameObject_TransformEditEvent:
            {
                auto ptr = static_cast<GameObject_TransformEditEvent*>(event.get());
                GameObject* particle = nullptr;
                for (auto& child : GetGameObject()->GetChildren())
                {
                    if (child->getName() == ptr->objName)
                    {
                        particle = child;
                        break;
                    }
                }

                for (int i = 0; i < each->GetTotalFrame(); i++)
                {
                    animatorComponent.lock()->PushAnimationWithFunc(each, i, [=]()
                        {
                            auto& aem = AnimationEventManager::GetSingletonInstance();
                            auto target = aem.GetLerpPoint(ptr->editData, i);
                            auto pos = target->GetLocalPosition();
                            particle->GetTransform()->SetLocalPosition(Vector3d(pos.x, pos.y, -pos.z));
                            particle->GetTransform()->SetLocalRotation(Quaternion(Vector3d(0, 180, 0)) * target->GetLocalRotation());
                            particle->GetTransform()->SetLocalScale(target->GetLocalScale());
                        });
                }

                break;
            }
            case application::AnimationEventType::Sound_PlayOnceEvent:
            {
                auto ptr = static_cast<Sound_PlayOnceEvent*>(event.get());
                animatorComponent.lock()->PushAnimationWithFunc(each, event->frame, [=]()
                    {
                        yunutyEngine::SoundSystem::PlaySoundfile3D(ptr->rscPath, animatorComponent.lock()->GetGameObject()->GetTransform()->GetWorldPosition());
                    });
                break;
            }
            case application::AnimationEventType::GameObject_AwakeEvent:
            {
                auto ptr = static_cast<GameObject_AwakeEvent*>(event.get());
                GameObject* particle = nullptr;
                for (auto& child : GetGameObject()->GetChildren())
                {
                    if (child->getName() == ptr->objName)
                    {
                        particle = child;
                        break;
                    }
                }

                animatorComponent.lock()->PushAnimationWithFunc(each, event->frame, [=]()
                    {
                        auto ptr = particle->GetComponent<graphics::ParticleRenderer>();
                        ptr->Reset();
                    });

                break;
            }
            default:
                break;
            }
        }
    }
}
void Unit::Summon(const application::editor::UnitData* unitData)
{
    this->unitData = unitData;
    unitData->inGameUnit = GetWeakPtr<Unit>();

    GetTransform()->SetWorldPosition(Vector3d{ unitData->pod.position });
    navAgentComponent.lock()->SetActive(true);
    navObstacle.lock()->SetActive(true);
    navAgentComponent.lock()->GetTransform()->SetWorldPosition(Vector3d{ unitData->pod.position });
    navAgentComponent.lock()->AssignToNavigationField(&SingleNavigationField::Instance());
    navObstacle.lock()->AssignToNavigationField(&SingleNavigationField::Instance());

    ResetCallbacks();
    Reset();

    onAttack = unitData->onAttack;
    onAttackHit = unitData->onAttackHit;
    onDamaged = unitData->onDamaged;
    onCreated = unitData->onCreated;
    onRotationFinish = unitData->onRotationFinish;
    onStateEngage = unitData->onStateEngage;
    onStateExit = unitData->onStateExit;

    Summon(unitData->GetUnitTemplateData());

    Quaternion quat{ unitData->pod.rotation.w,unitData->pod.rotation.x,unitData->pod.rotation.y ,unitData->pod.rotation.z };
    auto forward = quat.Forward();
    desiredRotation = currentRotation = QuaternionToEastAngle(quat);
    //desiredRotation = currentRotation = 180 + std::atan2f(forward.z, forward.x) * math::Rad2Deg;
    coroutineBirth = StartCoroutine(BirthCoroutine());
}
void Unit::Summon(application::editor::Unit_TemplateData* td, const Vector3d& position, float rotation, bool instant)
{
    this->unitData = nullptr;

    ResetCallbacks();
    Reset();
    Summon(td);

    GetTransform()->SetWorldPosition(Vector3d{ position });
    navAgentComponent.lock()->GetTransform()->SetWorldPosition(Vector3d{ position });
    navAgentComponent.lock()->AssignToNavigationField(&SingleNavigationField::Instance());
    navObstacle.lock()->AssignToNavigationField(&SingleNavigationField::Instance());

    GetTransform()->SetWorldRotation(Vector3d{ 0,90 - rotation,0 });
    desiredRotation = currentRotation = rotation;

    if (instant)
    {
        onCreated();
    }
    else
    {
        coroutineBirth = StartCoroutine(BirthCoroutine());
    }
}
void Unit::Summon(application::editor::Unit_TemplateData* td, const Vector3d& position, const Quaternion& rotation, bool instant)
{
    auto finalRot = QuaternionToEastAngle(rotation);
    Summon(td, position, finalRot, instant);
}
void Unit::AddPassiveSkill(std::shared_ptr<PassiveSkill> skill)
{
    passiveSkill = skill;
    passiveSkill->owner = GetWeakPtr<Unit>();
    passiveSkill->Init(GetWeakPtr<Unit>());
}
// 패시브 스킬을 추가하며 유닛의 여러 콜백 함수에 영향을 미친다.
void Unit::Summon(application::editor::Unit_TemplateData* templateData)
{
    skinnedMeshGameObject->GetTransform()->SetLocalScale(Vector3d::one * unitTemplateData->pod.unit_scale);
    switch (unitTemplateData->pod.playerUnitType.enumValue)
    {
    case PlayerCharacterType::Robin:
        unitStatusPortraitUI = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Robin)->GetWeakPtr<UIElement>();
        unitStatusPortraitUI2 = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Robin_Left)->GetWeakPtr<UIElement>();
        AddPassiveSkill(std::static_pointer_cast<PassiveSkill>(std::make_shared<PassiveRobinBleed>()));
        break;
    case PlayerCharacterType::Ursula:
        unitStatusPortraitUI = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Ursula)->GetWeakPtr<UIElement>();
        unitStatusPortraitUI2 = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Ursula_Left)->GetWeakPtr<UIElement>();
        AddPassiveSkill(std::static_pointer_cast<PassiveSkill>(std::make_shared<PassiveUrsula>()));
        break;
    case PlayerCharacterType::Hansel:
        unitStatusPortraitUI = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Hansel)->GetWeakPtr<UIElement>();
        unitStatusPortraitUI2 = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Hansel_Left)->GetWeakPtr<UIElement>();
        AddPassiveSkill(std::static_pointer_cast<PassiveSkill>(std::make_shared<PassiveHanselHeal>()));
        break;
    default:
        break;
    }
    switch (unitTemplateData->pod.unitStatusBar.enumValue)
    {
    case UnitStatusBarType::PLAYER:
        unitStatusUI = UIManager::Instance().DuplicateUIElement(UIManager::Instance().GetUIElementByEnum(UIEnumID::StatusBar_Heroes));
        break;
    case UnitStatusBarType::ENEMY:
        unitStatusUI = UIManager::Instance().DuplicateUIElement(UIManager::Instance().GetUIElementByEnum(UIEnumID::StatusBar_MeleeEnemy));
        break;
    }

    unitCollider.lock()->SetRadius(unitTemplateData->pod.collisionSize);
    unitCollider.lock()->SetHalfHeight(unitTemplateData->pod.collisionHeight * 0.5f);
    unitCollider.lock()->GetTransform()->SetLocalRotation(Vector3d{ 0,0,90 });
    unitCollider.lock()->GetTransform()->SetLocalPosition(Vector3d::up * unitTemplateData->pod.collisionHeight * 0.5f);

    attackRange.lock()->SetRadius(unitTemplateData->pod.m_atkRadius);
    attackRange.lock()->SetColor(yunuGI::Color::red());
    acquisitionRange.lock()->SetRadius(unitTemplateData->pod.m_idRadius);
    acquisitionRange.lock()->SetColor(yunuGI::Color::gray());
    // 공격 범위
    //AttachDebugMesh(attackRange.lock()->GetGameObject()->AddGameObject(), DebugMeshType::Sphere)->GetTransform()->SetLocalScale(Vector3d{ 1,0.25,1 } *2 * unitTemplateData->pod.m_atkRadius);
    // 인식 범위
    //AttachDebugMesh(acquisitionRange.lock()->GetGameObject()->AddGameObject(), DebugMeshType::Sphere)->GetTransform()->SetLocalScale(Vector3d{ 1,0.25,1 } *2 * unitTemplateData->pod.m_idRadius);
    // 혹여나 플레이어 캐릭터라면 플레이어로 등록, 플레이어가 아니면 그냥 무시된다.
    switch (templateData->pod.unitControllerType.enumValue)
    {
    case UnitControllerType::PLAYER:
    {
        PlayerController::Instance().RegisterUnit(GetWeakPtr<Unit>());
        controllers.push_back(&PlayerController::Instance());
        break;
    }
    case UnitControllerType::ANGRY_MOB:
    {
        EnemyAggroController::Instance().RegisterUnit(GetWeakPtr<Unit>());
        controllers.push_back(&EnemyAggroController::Instance());
        break;
    }
    case UnitControllerType::MELEE_ELITE:
    {
        EnemyAggroController::Instance().RegisterUnit(GetWeakPtr<Unit>());
        controllers.push_back(&EnemyAggroController::Instance());
        MeleeEliteController::Instance().RegisterUnit(GetWeakPtr<Unit>());
        controllers.push_back(&MeleeEliteController::Instance());
        break;
    }
    case UnitControllerType::RANGED_ELITE:
    {
        EnemyAggroController::Instance().RegisterUnit(GetWeakPtr<Unit>());
        controllers.push_back(&EnemyAggroController::Instance());
        RangedEliteController::Instance().RegisterUnit(GetWeakPtr<Unit>());
        controllers.push_back(&EnemyAggroController::Instance());
        break;
    }
    case UnitControllerType::HEART_QUEEN:
    {
        BossController::Instance().RegisterUnit(GetWeakPtr<Unit>());
        controllers.push_back(&BossController::Instance());
        break;
    }
    }
    currentRotationSpeed = unitTemplateData->pod.rotationSpeed;
    navAgentComponent.lock()->SetRadius(unitTemplateData->pod.collisionSize);
    navAgentComponent.lock()->SetSpeed(unitTemplateData->pod.m_unitSpeed);
    navObstacle.lock()->SetRadiusAndHeight(unitTemplateData->pod.collisionSize, 100);
    SetCurrentHp(unitTemplateData->pod.max_Health);
}
void Unit::Reset()
{
    SetIsAlive(true);
    SetCurrentHp(unitTemplateData->pod.max_Health);
    DeleteCoroutine(coroutineKnockBack);
    DeleteCoroutine(coroutineAttack);
    if (!coroutineSkill.expired())
    {
        onGoingSkill->OnInterruption();
        DeleteCoroutine(coroutineSkill);
        onGoingSkill.reset();
    }
    DeleteCoroutine(coroutineRevival);
    DeleteCoroutine(coroutineDeath);
    ClearCoroutines();
    passiveSkill.reset();
    buffs.clear();
    liveCountLeft = unitTemplateData->pod.liveCount;
    currentTargetUnit.reset();
    if (!burnEffect.expired())
    {
        burnEffect.lock()->Reset();
    }
    currentOrderType = UnitOrderType::AttackMove;
    pendingOrderType = UnitOrderType::AttackMove;
    attackMoveDestination = moveDestination = GetGameObject()->GetTransform()->GetWorldPosition();
    //pendingAttackMoveDestination = attackMoveDestination;
    //pendingMoveDestination = moveDestination;

    isPaused = false;
    localTimeScale = 1.0f;

    ResetSharedRef();
}
void Unit::InitBehaviorTree()
{
    // 이 행동 트리에 대한 설계문서는 Document/프로그래밍 폴더 내부의 파일 "InWanderLand Behaviour tree.drawio"입니다.
    unitBehaviourTree[UnitBehaviourTree::Death].enteringCondtion = [this]()
        {
            return !referenceBlockDeath.BeingReferenced() && !isAlive && liveCountLeft < 0;
        };
    unitBehaviourTree[UnitBehaviourTree::Death].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Death>();
        };
    unitBehaviourTree[UnitBehaviourTree::Death].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Death>();
        };
    unitBehaviourTree[UnitBehaviourTree::Paralysis].enteringCondtion = [this]()
        {
            return referenceParalysis.BeingReferenced();
        };
    unitBehaviourTree[UnitBehaviourTree::Paralysis].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Paralysis>();
        };
    unitBehaviourTree[UnitBehaviourTree::Paralysis].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Paralysis>();
        };
    unitBehaviourTree[UnitBehaviourTree::Paralysis][UnitBehaviourTree::Knockback].enteringCondtion = [this]()
        {
            return !coroutineKnockBack.expired();
        };
    unitBehaviourTree[UnitBehaviourTree::Paralysis][UnitBehaviourTree::Knockback].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Knockback>();
        };
    unitBehaviourTree[UnitBehaviourTree::Paralysis][UnitBehaviourTree::Knockback].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Knockback>();
        };
    unitBehaviourTree[UnitBehaviourTree::Paralysis][UnitBehaviourTree::Stun].enteringCondtion = [this]()
        {
            return true;
        };
    unitBehaviourTree[UnitBehaviourTree::Paralysis][UnitBehaviourTree::Stun].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Stun>();
        };
    unitBehaviourTree[UnitBehaviourTree::Paralysis][UnitBehaviourTree::Stun].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Stun>();
        };
    unitBehaviourTree[UnitBehaviourTree::Pause].enteringCondtion = [this]()
        {
            return referencePause.BeingReferenced() || pauseAll;
        };
    unitBehaviourTree[UnitBehaviourTree::Pause].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Pause>();
        };
    unitBehaviourTree[UnitBehaviourTree::Pause].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Pause>();
        };
    unitBehaviourTree[UnitBehaviourTree::Reviving].enteringCondtion = [this]()
        {
            return !isAlive;
        };
    unitBehaviourTree[UnitBehaviourTree::Reviving].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Reviving>();
        };
    unitBehaviourTree[UnitBehaviourTree::Reviving].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Reviving>();
        };
    unitBehaviourTree[UnitBehaviourTree::Skill].enteringCondtion = [this]()
        {
            return !coroutineSkill.expired() || (CanProcessOrder<UnitOrderType::Skill>() && pendingSkill.get());
        };
    unitBehaviourTree[UnitBehaviourTree::Skill].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Skill>();
        };
    unitBehaviourTree[UnitBehaviourTree::Skill].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Skill>();
        };
    unitBehaviourTree[UnitBehaviourTree::Skill][UnitBehaviourTree::SkillOnGoing].enteringCondtion = [this]()
        {
            return !coroutineSkill.expired();
        };
    unitBehaviourTree[UnitBehaviourTree::Skill][UnitBehaviourTree::SkillOnGoing].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::SkillOnGoing>();
        };
    unitBehaviourTree[UnitBehaviourTree::Skill][UnitBehaviourTree::SkillOnGoing].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::SkillOnGoing>();
        };
    unitBehaviourTree[UnitBehaviourTree::Skill][UnitBehaviourTree::SkillCasting].enteringCondtion = [this]()
        {
            return DistanceTo(pendingSkill.get()->targetPos) < pendingSkill.get()->GetCastRange();
        };
    unitBehaviourTree[UnitBehaviourTree::Skill][UnitBehaviourTree::SkillCasting].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::SkillCasting>();
        };
    unitBehaviourTree[UnitBehaviourTree::Skill][UnitBehaviourTree::SkillCasting].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::SkillCasting>();
        };
    unitBehaviourTree[UnitBehaviourTree::Skill][UnitBehaviourTree::Move].enteringCondtion = [this]()
        {
            return true;
        };
    unitBehaviourTree[UnitBehaviourTree::Skill][UnitBehaviourTree::Move].onEnter = [this]()
        {
            moveDestination = pendingSkill.get()->targetPos;
            attackMoveDestination = moveDestination;
            OnStateEngage<UnitBehaviourTree::Move>();
        };
    unitBehaviourTree[UnitBehaviourTree::Skill][UnitBehaviourTree::Move].onUpdate = [this]()
        {
            OnStateUpdate<UnitBehaviourTree::Move>();
        };
    unitBehaviourTree[UnitBehaviourTree::Skill][UnitBehaviourTree::Move].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Move>();
        };
    unitBehaviourTree[UnitBehaviourTree::Tactic].enteringCondtion = [this]()
        {
            return referenceTactic.BeingReferenced();
        };
    unitBehaviourTree[UnitBehaviourTree::Tactic].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Tactic>();
        };
    unitBehaviourTree[UnitBehaviourTree::Tactic].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Tactic>();
        };
    unitBehaviourTree[UnitBehaviourTree::Chasing].enteringCondtion = [this]()
        {
            return ((!pendingTargetUnit.expired() && pendingTargetUnit.lock()->IsAlive()) ||
                (!currentTargetUnit.expired() && currentTargetUnit.lock()->IsAlive())) &&
                CanProcessOrder<UnitOrderType::AttackUnit>();
        };
    unitBehaviourTree[UnitBehaviourTree::Chasing].onEnter = [this]()
        {
            currentOrderType = pendingOrderType;
            OnStateEngage<UnitBehaviourTree::Chasing>();
        };
    unitBehaviourTree[UnitBehaviourTree::Chasing].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Chasing>();
        };
    unitBehaviourTree[UnitBehaviourTree::Chasing].onUpdate = [this]()
        {
            OnStateUpdate<UnitBehaviourTree::Chasing>();
        };
    unitBehaviourTree[UnitBehaviourTree::Chasing][UnitBehaviourTree::Attack].enteringCondtion = [this]()
        {
            return attackRange.lock()->GetEnemies().contains(currentTargetUnit.lock().get()) || !coroutineAttack.expired();
        };
    unitBehaviourTree[UnitBehaviourTree::Chasing][UnitBehaviourTree::Attack].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Attack>();
        };
    unitBehaviourTree[UnitBehaviourTree::Chasing][UnitBehaviourTree::Attack].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Attack>();
        };
    unitBehaviourTree[UnitBehaviourTree::Chasing][UnitBehaviourTree::Attack].onUpdate = [this]()
        {
            OnStateUpdate<UnitBehaviourTree::Attack>();
        };
    unitBehaviourTree[UnitBehaviourTree::Chasing][UnitBehaviourTree::Move].enteringCondtion = [this]()
        {
            return true;
        };
    unitBehaviourTree[UnitBehaviourTree::Chasing][UnitBehaviourTree::Move].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Move>();
        };
    unitBehaviourTree[UnitBehaviourTree::Chasing][UnitBehaviourTree::Move].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Move>();
        };
    unitBehaviourTree[UnitBehaviourTree::Chasing][UnitBehaviourTree::Move].onUpdate = [this]()
        {
            moveDestination = GetAttackPosition(GetAttackTarget());
            OnStateUpdate<UnitBehaviourTree::Move>();
        };
    unitBehaviourTree[UnitBehaviourTree::Hold].enteringCondtion = [this]()
        {
            return CanProcessOrder<UnitOrderType::Hold>();
        };
    unitBehaviourTree[UnitBehaviourTree::Hold].onEnter = [this]()
        {
            currentOrderType = UnitOrderType::Hold;
            OnStateEngage<UnitBehaviourTree::Hold>();
        };
    unitBehaviourTree[UnitBehaviourTree::Hold].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Hold>();
        };
    unitBehaviourTree[UnitBehaviourTree::Hold][UnitBehaviourTree::Attack].enteringCondtion = [this]()
        {
            return !attackRange.lock()->GetEnemies().empty() || !coroutineAttack.expired();
        };
    unitBehaviourTree[UnitBehaviourTree::Hold][UnitBehaviourTree::Attack].onEnter = [this]()
        {
            currentTargetUnit = GetClosestEnemy();
            OnStateEngage<UnitBehaviourTree::Attack>();
        };
    unitBehaviourTree[UnitBehaviourTree::Hold][UnitBehaviourTree::Attack].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Attack>();
        };
    unitBehaviourTree[UnitBehaviourTree::Hold][UnitBehaviourTree::Attack].onUpdate = [this]()
        {
            OnStateUpdate<UnitBehaviourTree::Attack>();
        };
    unitBehaviourTree[UnitBehaviourTree::Hold][UnitBehaviourTree::Stop].enteringCondtion = [this]()
        {
            return true;
        };
    unitBehaviourTree[UnitBehaviourTree::Hold][UnitBehaviourTree::Stop].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Stop>();
        };
    unitBehaviourTree[UnitBehaviourTree::Hold][UnitBehaviourTree::Stop].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Stop>();
        };
    unitBehaviourTree[UnitBehaviourTree::Move].enteringCondtion = [this]()
        {
            return CanProcessOrder<UnitOrderType::Move>();
        };
    unitBehaviourTree[UnitBehaviourTree::Move].onEnter = [this]()
        {
            currentOrderType = pendingOrderType;
            OnStateEngage<UnitBehaviourTree::Move>();
        };
    unitBehaviourTree[UnitBehaviourTree::Move].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Move>();
        };
    unitBehaviourTree[UnitBehaviourTree::Move].onUpdate = [this]()
        {
            OnStateUpdate<UnitBehaviourTree::Move>();
        };

    unitBehaviourTree[UnitBehaviourTree::AttackMove].enteringCondtion = [this]()
        {
            //return CanProceedOrder<UnitOrderType::AttackMove>();
            return true;
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove].onEnter = [this]()
        {
            //currentOrderType = pendingOrderType;
            currentOrderType = UnitOrderType::AttackMove;
            OnStateEngage<UnitBehaviourTree::AttackMove>();
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::AttackMove>();
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove][UnitBehaviourTree::Attack].enteringCondtion = [this]()
        {
            return !attackRange.lock()->GetEnemies().empty() || !coroutineAttack.expired();
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove][UnitBehaviourTree::Attack].onEnter = [this]()
        {
            currentTargetUnit = GetClosestEnemy();
            OnStateEngage<UnitBehaviourTree::Attack>();
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove][UnitBehaviourTree::Attack].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Attack>();
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove][UnitBehaviourTree::Attack].onUpdate = [this]()
        {
            OnStateUpdate<UnitBehaviourTree::Attack>();
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove][UnitBehaviourTree::Move].enteringCondtion = [this]()
        {
            constexpr float epsilon = 0.1f;
            auto distance = (attackMoveDestination - GetTransform()->GetWorldPosition()).MagnitudeSqr();
            return !acquisitionRange.lock()->GetEnemies().empty() || distance > epsilon;
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove][UnitBehaviourTree::Move].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Move>();
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove][UnitBehaviourTree::Move].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Move>();
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove][UnitBehaviourTree::Move].onUpdate = [this]()
        {
            currentTargetUnit = GetClosestEnemy();
            moveDestination = currentTargetUnit.expired() ? attackMoveDestination : GetAttackPosition(currentTargetUnit);
            OnStateUpdate<UnitBehaviourTree::Move>();
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove][UnitBehaviourTree::Stop].enteringCondtion = [this]()
        {
            return true;
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove][UnitBehaviourTree::Stop].onEnter = [this]()
        {
            OnStateEngage<UnitBehaviourTree::Stop>();
        };
    unitBehaviourTree[UnitBehaviourTree::AttackMove][UnitBehaviourTree::Stop].onExit = [this]()
        {
            OnStateExit<UnitBehaviourTree::Stop>();
        };
}
Vector3d Unit::GetAttackPosition(std::weak_ptr<Unit> opponent)
{
    auto delta = opponent.lock()->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition();
    return opponent.lock()->GetTransform()->GetWorldPosition() - opponent.lock()->unitTemplateData->pod.collisionSize * delta.Normalized();
}
yunutyEngine::coroutine::Coroutine Unit::RevivalCoroutine()
{
    float birthAnimDuration = wanderResources::GetAnimation(unitTemplateData->pod.skinnedFBXName, UnitAnimType::Birth)->GetDuration();
    PlayAnimation(UnitAnimType::Death);
    SetDefaultAnimation(UnitAnimType::None);
    co_yield coroutine::WaitForSeconds(unitTemplateData->pod.revivalDuration - birthAnimDuration);
    PlayAnimation(UnitAnimType::Birth);
    co_yield coroutine::WaitForSeconds(birthAnimDuration);
    SetCurrentHp(unitTemplateData->pod.max_Health);
    SetIsAlive(true);
    ApplyBuff(UnitBuffInvulenerability{ unitTemplateData->pod.revivalInvulnerableDuration });
    co_return;
}
yunutyEngine::coroutine::Coroutine Unit::BirthCoroutine()
{
    if (unitTemplateData->pod.birthTime <= 0)
    {
        onCreated();
        co_return;
    }

    while (!IsPlayerUnit() && isPaused)
    {
        co_await std::suspend_always();
    }

    const auto& gc = GlobalConstant::GetSingletonInstance().pod;
    auto pauseGuard = referencePause.Acquire();
    auto invulnerableGuard = referenceInvulnerable.Acquire();
    //animatorComponent.lock()->GetGameObject()->SetSelfActive(false);
    //co_yield coroutine::WaitForSeconds{ math::Random::GetRandomFloat(0, gc.unitBirthTimeOffsetNoise) };
    //animatorComponent.lock()->GetGameObject()->SetSelfActive(true);
    float animSpeed = wanderResources::GetAnimation(unitTemplateData->pod.skinnedFBXName, UnitAnimType::Birth)->GetDuration() / unitTemplateData->pod.birthTime;
    PlayAnimation(UnitAnimType::Birth, Animation::PlayFlag_::None);
    //animatorComponent.lock()->GetGI().SetPlaySpeed(animSpeed);
    burnEffect.lock()->SetDuration(unitTemplateData->pod.birthTime);
    burnEffect.lock()->SetEdgeColor({ unitTemplateData->pod.birthBurnEdgeColor.x,unitTemplateData->pod.birthBurnEdgeColor.y,unitTemplateData->pod.birthBurnEdgeColor.z });
    burnEffect.lock()->SetEdgeThickness(unitTemplateData->pod.birthBurnEdgeThickness);
    burnEffect.lock()->Appear();

    wanderUtils::UnitCoroutine::ForSecondsFromUnit forSeconds{ GetWeakPtr<Unit>(), unitTemplateData->pod.birthTime };

    while (forSeconds.Tick())
    {
        co_await std::suspend_always();
    }

    animatorComponent.lock()->GetGI().SetPlaySpeed(1);
    onCreated();
    co_return;
}
yunutyEngine::coroutine::Coroutine Unit::DeathCoroutine()
{
    if (unitTemplateData->pod.deathBurnTime <= 0)
    {
        ReturnToPool();
        co_return;
    }
    float animSpeed = wanderResources::GetAnimation(unitTemplateData->pod.skinnedFBXName, UnitAnimType::Death)->GetDuration() / unitTemplateData->pod.deathBurnTime;
    PlayAnimation(UnitAnimType::Death);
    //animatorComponent.lock()->GetGI().SetPlaySpeed(animSpeed);
    burnEffect.lock()->SetDuration(unitTemplateData->pod.deathBurnTime);
    burnEffect.lock()->SetEdgeColor({ unitTemplateData->pod.deathBurnEdgeColor.x,unitTemplateData->pod.deathBurnEdgeColor.y,unitTemplateData->pod.deathBurnEdgeColor.z });
    burnEffect.lock()->SetEdgeThickness(unitTemplateData->pod.deathBurnEdgeThickness);
    burnEffect.lock()->Disappear();
    co_yield coroutine::WaitForSeconds(unitTemplateData->pod.deathBurnTime);
    ReturnToPool();
    co_return;
}

yunutyEngine::coroutine::Coroutine Unit::AttackCoroutine(std::weak_ptr<Unit> opponent)
{
    auto blockAttack = referenceBlockAttack.Acquire();
    defaultAnimationType = UnitAnimType::Idle;
    // 공격 애니메이션이 자연스럽게 맞물리기까지 필요한 최소시간
    float animMinimumTime = unitTemplateData->pod.m_attackPreDelay + unitTemplateData->pod.m_attackPostDelay;
    // 현재 공격주기
    float finalAttackCooltime = unitTemplateData->pod.m_atkCooltime / (1 + adderAttackSpeed);
    float attackDelayMultiplier = 1;
    // 공격 애니메이션 재생시간이 공격주기보다 더 길다면, 애니메이션 재생속도를 더 빠르게 해줘야 한다.
    if (animMinimumTime > finalAttackCooltime)
    {
        attackDelayMultiplier = finalAttackCooltime / animMinimumTime;
        animatorComponent.lock()->GetGI().SetPlaySpeed(animMinimumTime / finalAttackCooltime);
    }
    PlayAnimation(UnitAnimType::Attack);

    switch (unitTemplateData->pod.attackType.enumValue)
    {
    case UnitAttackType::MELEE:
    {
        if (!coroutineAttackEffect.expired())
            FBXPool::Instance().Return(attackVFX);

        coroutineAttackEffect = StartCoroutine(MeleeAttackEffectCoroutine(opponent));
        coroutineAttackEffect.lock()->PushDestroyCallBack([this]()
            {
                FBXPool::Instance().Return(attackVFX);
            });
        break;
    }
    }

    float playSpeed = animatorComponent.lock()->GetGI().GetPlaySpeed();

    co_yield coroutine::WaitForSeconds(unitTemplateData->pod.m_attackPreDelay * attackDelayMultiplier);
    onAttack(opponent);
    playSpeed = animatorComponent.lock()->GetGI().GetPlaySpeed();
    float dmg = unitTemplateData->pod.m_autoAttackDamage + adderAttackDamage;
    DamageType damageType = DamageType::Attack;
    if (math::Random::ByRandomChance(GetCritChance()))
    {
        damageType = DamageType::AttackCrit;
    }
    switch (unitTemplateData->pod.attackType.enumValue)
    {
    case UnitAttackType::MELEE:
    {
        if (!referenceBlindness.BeingReferenced())
        {
            opponent.lock()->Damaged(GetWeakPtr<Unit>(), dmg, damageType);
        }
        if (!coroutineAttackEffect.expired())
        {
            attackVFX.lock()->GetGameObject()->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
            attackVFX.lock()->GetGameObject()->GetTransform()->SetWorldRotation(GetTransform()->GetWorldRotation());
        }
        break;
    }
    case UnitAttackType::MISSILE:
    {
        //auto projectile = ProjectileSelector::SingleInstance().RequestProjectile(GetWeakPtr<Unit>(), opponent.lock(), static_cast<ProjectileType::Enum>(unitTemplateData->pod.projectileType.enumValue));
        auto projectile = ProjectilePool::SingleInstance().Borrow(GetWeakPtr<Unit>(), opponent.lock());
        projectile.lock()->damage = dmg;
        projectile.lock()->SetDamageType(damageType);
        break;
    }
    }
    StartCoroutine(referenceBlockAttack.AcquireForSecondsCoroutine(finalAttackCooltime - unitTemplateData->pod.m_attackPreDelay * attackDelayMultiplier));
    auto blockCommand = referenceBlockPendingOrder.Acquire();
    co_yield coroutine::WaitForSeconds(unitTemplateData->pod.m_attackPostDelay * attackDelayMultiplier);
    playSpeed = animatorComponent.lock()->GetGI().GetPlaySpeed();
    blockCommand.reset();
    animatorComponent.lock()->GetGI().SetPlaySpeed(1);
    co_return;
}
yunutyEngine::coroutine::Coroutine Unit::MeleeAttackEffectCoroutine(std::weak_ptr<Unit> opponent)
{
    /// VFX 실행
    attackVFX = wanderResources::GetVFX(unitTemplateData->pod.skinnedFBXName, UnitAnimType::Attack);

    co_await std::suspend_always{};

    auto vfxAnimator = attackVFX.lock()->AcquireVFXAnimator();
    vfxAnimator.lock()->SetAutoActiveFalse();
    vfxAnimator.lock()->Init();
    vfxAnimator.lock()->Play();
    Vector3d startPos = GetTransform()->GetWorldPosition();
    Vector3d deltaPos = opponent.lock()->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition();
    Vector3d direction = deltaPos.Normalized();
    attackVFX.lock()->GetGameObject()->GetTransform()->SetWorldPosition(startPos);
    attackVFX.lock()->GetGameObject()->GetTransform()->SetWorldRotation(GetTransform()->GetWorldRotation());
    attackVFX.lock()->GetGameObject()->GetTransform()->SetWorldScale(GetTransform()->GetWorldScale());

    while (!vfxAnimator.lock()->IsDone())
    {
        co_await std::suspend_always{};
    }

    co_return;
}
float Unit::DistanceTo(const Vector3d& target)
{
    return (GetTransform()->GetWorldPosition() - target).Magnitude();
}
void Unit::ReturnToPool()
{
    if (!unitStatusUI.expired())
    {
        Scene::getCurrentScene()->DestroyGameObject(unitStatusUI.lock()->GetGameObject());
        unitStatusUI.reset();
    }
    navAgentComponent.lock()->SetActive(false);
    navObstacle.lock()->SetActive(false);
    unitStatusPortraitUI.reset();
    unitStatusPortraitUI2.reset();
    Reset();
    UnitPool::SingleInstance().Return(GetWeakPtr<Unit>());
}
void Unit::ResetSharedRef()
{
    enableNavObstacleByState.reset();
    blockFollowingNavAgentByState.reset();
    disableNavAgentByState.reset();
    invulnerabilityByState.reset();
}
void Unit::ResetCallbacks()
{
    onAttack.Clear();
    onAttackHit.Clear();
    onDamaged.Clear();
    onCreated.Clear();
    onRotationFinish.Clear();
    for (auto& each : onStateEngage)
    {
        each.Clear();
    }
    for (auto& each : onStateExit)
    {
        each.Clear();
    }
}
const editor::Unit_TemplateData& Unit::GetUnitTemplateData()const
{
    return *unitTemplateData;
};
int Unit::GetTeamIndex() const
{
    return teamIndex;
}
std::weak_ptr<Unit> Unit::GetAttackTarget() const
{
    if (currentTargetUnit.expired())
        return pendingTargetUnit;
    return currentTargetUnit;
}
