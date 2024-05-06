#include "TacticModeSystem.h"
#include "Unit.h"
#include "InputManager.h"
#include "RTSCam.h"
#include "PlayerController.h"
#include "SkillPreviewSystem.h"
#include "CursorDetector.h"
#include "InWanderLand.h"


void TacticModeSystem::OnEnable()
{
    SetCurrentGauge(m_maxGauge);
}

void TacticModeSystem::Start()
{
    SetCurrentGauge(m_maxGauge);
}

void TacticModeSystem::Update()
{
    if (m_currentGauge < m_maxGauge && !isTacticModeOperating)
    {
        m_gaugeIncreaseElapsed += Time::GetDeltaTime();
        if (m_gaugeIncreaseElapsed >= m_gaugeIncreaseDuration)
        {
            SetCurrentGauge(++m_currentGauge);
            if (m_currentGauge >= m_maxGauge)
            {
                SetCurrentGauge(m_maxGauge);
            }
            m_gaugeIncreaseElapsed = 0.0f;
        }
    }
    if (isCoolTime)
    {
        SetCurrentCoolTimeElapsed(m_engageCoolTimeElapsed + Time::GetDeltaTime());
        if (m_engageCoolTimeElapsed >= m_engageCoolTimeDuration)
        {
            SetCurrentCoolTimeElapsed(m_engageCoolTimeDuration);
            isCoolTime = false;
        }
    }
}

void TacticModeSystem::SetTacticModeRightClickFunction(InputManager::SelectedSerialNumber currentSelectedNum)
{
    if (currentSelectedNum == 0)
        currentSelectedNum = InputManager::One;

    currentSelectedUnit = playerComponentMap.find(static_cast<Unit::UnitType>(currentSelectedNum))->second;
    if (m_currentGauge > 0)
    {
        m_rtsCam->groundRightClickCallback = [=](Vector3d pos)
            {
                if (m_cursorDetector->GetCurrentOnMouseUnit() == nullptr)
                {
                    currentSelectedUnit->PushMoveFunctionToTacticQueue(pos);
                }
                else if (Unit* selectedUnit = m_cursorDetector->GetCurrentOnMouseUnit();
                    selectedUnit->GetUnitSide() == Unit::UnitSide::Enemy)
                {
                    currentSelectedUnit->PushAttackMoveFunctionToTacticQueue(pos, selectedUnit);
                }
                sequenceQueue.push(currentSelectedUnit);
                SkillPreviewSystem::Instance().ActivateSkillPreview(false);
                SetCurrentGauge(--m_currentGauge);
            };
    }
}

void TacticModeSystem::SetLeftClickAddQueueForAttackMove(InputManager::SelectedSerialNumber currentSelectedNum)
{
    currentSelectedUnit = playerComponentMap.find(static_cast<Unit::UnitType>(currentSelectedNum))->second;
    if (m_currentGauge > 0)
    {
        m_rtsCam->groundRightClickCallback = [=](Vector3d pos)
            {
                currentSelectedUnit->PushAttackMoveFunctionToTacticQueue(pos);
                sequenceQueue.push(currentSelectedUnit);
                SkillPreviewSystem::Instance().ActivateSkillPreview(false);
                //m_rtsCam->groundLeftClickCallback = [](Vector3d pos) {};
                //tacticModeGauge--;
                SetTacticModeRightClickFunction(currentSelectedNum);
                SetCurrentGauge(--m_currentGauge);
            };
    }
}

void TacticModeSystem::SetLeftClickAddQueueForSkill(InputManager::SelectedSerialNumber currentSelectedNum, Unit::SkillEnum currentSelectedSkill)
{
    currentSelectedUnit = playerComponentMap.find(static_cast<Unit::UnitType>(currentSelectedNum))->second;

    if (m_currentGauge > 0)
    {
        SkillPreviewSystem::Instance().SetCurrentSelectedPlayerUnit(currentSelectedUnit);
        SkillPreviewSystem::Instance().SetCurrentSkillPreviewType(currentSelectedUnit->GetSkillPreviewType(currentSelectedSkill));
        SkillPreviewSystem::Instance().SetCurrentSelectedSkillNum(currentSelectedSkill);
        SkillPreviewSystem::Instance().ActivateSkillPreview(true);

        m_rtsCam->groundLeftClickCallback = [=](Vector3d pos)
            {
                currentSelectedUnit->PushSkillFunctionToTacticQueue(currentSelectedSkill, pos);
                sequenceQueue.push(currentSelectedUnit);
                SkillPreviewSystem::Instance().ActivateSkillPreview(false);
                //m_rtsCam->groundLeftClickCallback = [](Vector3d pos) {};
                //tacticModeGauge--;
                SetTacticModeRightClickFunction(currentSelectedNum);
                SetCurrentGauge(--m_currentGauge);
            };
    }
}

void TacticModeSystem::RegisterTutorialQuestAction(Unit::UnitType p_targetUnit, OrderType p_orderType)
{

}

void TacticModeSystem::EngageTacticMode()
{
    /// 전술모드 키 입력 시 실행될 로직.
    /// 1. TimeScale을 0으로 설정한다.
    /// 2. PlayerController에서 현재 전술모드 적용 가능한 Player Unit의 정보를 가져온다.
    Time::SetTimeScale(0.0f);
    playerComponentMap = PlayerController::Instance().GetPlayerMap();
    isTacticModeOperating = true;
    m_gaugeIncreaseElapsed = 0.0f;

    vector<GameObject*> unitGameObjects;
    for (auto each : playerComponentMap)
    {
        unitGameObjects.push_back(each.second->GetGameObject());
    }
    m_rtsCam->SetTargets(unitGameObjects);

}

void TacticModeSystem::ExitTacticMode()
{
    Time::SetTimeScale(1.0f);
    m_gaugeIncreaseElapsed = 0.0f;
    isTacticModeOperating = false;

    SetCurrentCoolTimeElapsed(0.0f);
    isCoolTime = true;

    for (auto each : playerComponentMap)
    {
        if (!each.second->IsTacticModeQueueEmpty())
            each.second->SetUnitStateIdle();
    }

    if (!sequenceQueue.empty())
    {
        isTacticOrderPerforming = true;
        m_rtsCam->SetTarget(sequenceQueue.front()->GetGameObject());
        sequenceQueue.front()->PermitTacticAction();
        sequenceQueue.pop();
    }
}

void TacticModeSystem::SetMovingSystemComponent(RTSCam* sys)
{
    m_rtsCam = sys;
}

bool TacticModeSystem::IsOrderingTimingNow() const
{
    return isTacticModeOperating;
}

bool TacticModeSystem::IsUnitsPerformingCommand() const
{
    return isTacticOrderPerforming;
}

bool TacticModeSystem::IsTacticModeCoolTime() const
{
    return isCoolTime;
}
void TacticModeSystem::SetCurrentCoolTimeElapsed(float elapsed)
{
    m_engageCoolTimeElapsed = elapsed;
    UIManager::Instance().GetUIElementByEnum(UIEnumID::Toggle_TacticMode_Overlay)->adjuster->SetTargetFloat(elapsed / m_engageCoolTimeDuration);
    UIManager::Instance().GetUIElementByEnum(UIEnumID::Toggle_TacticMode_Cooltime_Number)->SetNumber(m_engageCoolTimeDuration - elapsed);
}

float TacticModeSystem::GetLeftCoolTime()
{
    if (m_engageCoolTimeElapsed >= m_engageCoolTimeDuration)
        return 0.0f;
    else
        return m_engageCoolTimeDuration - m_engageCoolTimeElapsed;
}

void TacticModeSystem::SetCurrentGauge(int p_gauge)
{
    m_currentGauge = p_gauge;
}

void TacticModeSystem::ReportTacticActionFinished()
{
    /// 전술모드의 수행을 완료한 유닛이 해당 함수를 호출합니다.
    if (sequenceQueue.empty())
    {
        isTacticOrderPerforming = false;
    }
    else
    {
        m_rtsCam->SetTarget(sequenceQueue.front()->GetGameObject());
        sequenceQueue.front()->PermitTacticAction();
        sequenceQueue.pop();
    }
}


