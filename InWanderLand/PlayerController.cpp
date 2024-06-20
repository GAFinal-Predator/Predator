#include "InWanderLand.h"
#include "PlayerController.h"
#include "InputManager.h"
#include "RTSCam.h"
#include "Unit.h"
#include "PlayerUnit.h"
#include "SkillPreviewSystem.h"
#include "UIManager.h"
#include "SkillList.h"
#include "VFXAnimator.h"
#include "TacticModeSystem.h"
#include "UnitMoveCommand.h"
#include "UnitAttackCommand.h"
#include "UnitSkillCommand.h"
#include "EnqueErrorType.h"

const std::unordered_map<UIEnumID, SkillUpgradeType::Enum> PlayerController::skillUpgradeByUI
{
    {UIEnumID::SkillUpgradeButtonRobin00,SkillUpgradeType::ROBIN_PASSIVE_LONGER},
    {UIEnumID::SkillUpgradeButtonRobin11,SkillUpgradeType::ROBIN_Q_RANGE},
    {UIEnumID::SkillUpgradeButtonRobin12,SkillUpgradeType::ROBIN_Q_DAMAGE},
    {UIEnumID::SkillUpgradeButtonRobin21,SkillUpgradeType::ROBIN_W_DAMAGE},
    {UIEnumID::SkillUpgradeButtonRobin22,SkillUpgradeType::ROBIN_W_RADIUS},
    {UIEnumID::SkillUpgradeButtonUrsula00,SkillUpgradeType::URSULA_PASSIVE_ENHANCE},
    {UIEnumID::SkillUpgradeButtonUrsula11,SkillUpgradeType::URSULA_Q_DAMAGE},
    {UIEnumID::SkillUpgradeButtonUrsula12,SkillUpgradeType::URSULA_Q_RANGE},
    {UIEnumID::SkillUpgradeButtonUrsula21,SkillUpgradeType::URSULA_W_DAMAGE},
    {UIEnumID::SkillUpgradeButtonUrsula22,SkillUpgradeType::URSULA_W_RADIUS},
    {UIEnumID::SkillUpgradeButtonHansel00,SkillUpgradeType::HANSEL_PASSIVE_ENHANCE},
    {UIEnumID::SkillUpgradeButtonHansel11,SkillUpgradeType::HANSEL_Q_DAMAGE},
    {UIEnumID::SkillUpgradeButtonHansel12,SkillUpgradeType::HANSEL_Q_RANGE},
    {UIEnumID::SkillUpgradeButtonHansel21,SkillUpgradeType::HANSEL_W_RANGE},
    {UIEnumID::SkillUpgradeButtonHansel22,SkillUpgradeType::HANSEL_W_MORE_HITS},
};
void PlayerController::RegisterUnit(std::weak_ptr<Unit> unit)
{
    auto playerType = unit.lock()->GetUnitTemplateData().pod.playerUnitType.enumValue;
    if (playerType == PlayerCharacterType::None)
        return;

    characters[unit.lock()->GetUnitTemplateData().pod.playerUnitType.enumValue] = unit;
    if (!characters[PlayerCharacterType::Robin].expired() &&
        !characters[PlayerCharacterType::Ursula].expired() &&
        !characters[PlayerCharacterType::Hansel].expired())
    {
        SetCameraOffset();
        SelectPlayerUnit(PlayerCharacterType::Robin);
    }
    unit.lock()->onStateEngage[UnitBehaviourTree::Death].AddCallback([this, unit]() { UnSelectSkill(unit); });
    unit.lock()->onStateEngage[UnitBehaviourTree::Death].AddCallback(std::bind(&PlayerController::OnPlayerChracterDead, this, unit));
    unit.lock()->onStateEngage[UnitBehaviourTree::Paralysis].AddCallback([this, unit]() { UnSelectSkill(unit); });
    unit.lock()->onStateEngage[UnitBehaviourTree::SkillOnGoing].AddCallback(std::bind_front(static_cast<void(PlayerController::*)(std::weak_ptr<Unit>)>(&PlayerController::SetCooltime), this, unit));

	unit.lock()->OnStateEngageCallback()[UnitBehaviourTree::Keywords::Knockback].AddCallback([=]() {
		TacticModeSystem::Instance().InterruptedCommand(unit.lock().get());
		});

	unit.lock()->OnStateEngageCallback()[UnitBehaviourTree::Keywords::Death].AddCallback([=]() {
		TacticModeSystem::Instance().InterruptedCommand(unit.lock().get());
		});
}

void PlayerController::SetSkillUpgradeTarget(UIEnumID skillUpgradeUITarget)
{
    this->skillUpgradeUITarget = skillUpgradeUITarget;
}
bool PlayerController::IsSkillUpgraded(SkillUpgradeType::Enum id)
{
    return skillUpgraded[id];
}
bool PlayerController::IsSkillUpgraded(UIEnumID skillUpgradeUITarget)
{
    return skillUpgraded.at(skillUpgradeByUI.at(skillUpgradeUITarget));
}
void PlayerController::UpgradeSkill()
{
    SetSkillPoints(skillPointsLeft - 1);
    static constexpr float gray = 0.3f;
    UIManager::Instance().GetUIElementByEnum(skillUpgradeUITarget)->imageComponent.lock()->GetGI().SetColor({ gray,gray,gray,1 });
    skillUpgraded[skillUpgradeByUI.at(skillUpgradeUITarget)] = true;
}
void PlayerController::SetSkillPoints(int points)
{
    skillPointsLeft = points;
    UIManager::Instance().GetUIElementByEnum(UIEnumID::SkillPoint_Number)->SetNumber(skillPointsLeft);
}
int PlayerController::GetSkillPoints()
{
    return skillPointsLeft;
}
void PlayerController::IncrementSkillPoint()
{
    SetSkillPoints(skillPointsLeft + 1);
}

void PlayerController::LockCamInRegion(const application::editor::RegionData* camLockRegion)
{
    this->camLockRegion = camLockRegion;
}

void PlayerController::UnlockCamFromRegion()
{
    this->camLockRegion = nullptr;
}

bool PlayerController::CanUnitSelectSkill(std::weak_ptr<Unit> unit)
{
    return (!unit.lock()->referenceParalysis.BeingReferenced() && unit.lock()->IsAlive());
}

void PlayerController::Start()
{
}
void PlayerController::OnContentsPlay()
{
    SetActive(true);
    cursorUnitDetector = Scene::getCurrentScene()->AddGameObject()->AddComponentAsWeakPtr<UnitAcquisitionBoxCollider>();
    AttachDebugMesh(cursorUnitDetector.lock()->GetGameObject(), DebugMeshType::Cube, yunuGI::Color::white());
    skillCooltimeNumberUI[SkillType::ROBIN_Q] = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Robin)->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Skill_Use_Q_Cooltime_Number);
    skillCooltimeNumberUI[SkillType::ROBIN_W] = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Robin)->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Skill_Use_W_Cooltime_Number);
    skillCooltimeNumberUI[SkillType::URSULA_Q] = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Ursula)->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Skill_Use_Q_Cooltime_Number);
    skillCooltimeNumberUI[SkillType::URSULA_W] = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Ursula)->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Skill_Use_W_Cooltime_Number);
    skillCooltimeNumberUI[SkillType::HANSEL_Q] = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Hansel)->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Skill_Use_Q_Cooltime_Number);
    skillCooltimeNumberUI[SkillType::HANSEL_W] = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Hansel)->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Skill_Use_W_Cooltime_Number);
    skillCooltimeMaskUI[SkillType::ROBIN_Q] = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Robin)->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Skill_Use_Q_Overlay);
    skillCooltimeMaskUI[SkillType::ROBIN_W] = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Robin)->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Skill_Use_W_Overlay);
    skillCooltimeMaskUI[SkillType::URSULA_Q] = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Ursula)->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Skill_Use_Q_Overlay);
    skillCooltimeMaskUI[SkillType::URSULA_W] = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Ursula)->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Skill_Use_W_Overlay);
    skillCooltimeMaskUI[SkillType::HANSEL_Q] = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Hansel)->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Skill_Use_Q_Overlay);
    skillCooltimeMaskUI[SkillType::HANSEL_W] = UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Hansel)->GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Skill_Use_W_Overlay);
    SetSkillPoints(0);
    SetManaFull();
    SetState(State::Peace);
    InitUnitMouseInteractionEffects();
}
void PlayerController::OnContentsStop()
{
    stateRequestedByAction = State::None;
    SetState(State::Peace);
    SetActive(false);
    UnlockCamFromRegion();
    Unit::SetPauseAll(false);
    Scene::getCurrentScene()->DestroyGameObject(cursorUnitDetector.lock()->GetGameObject());
}

void PlayerController::Update()
{
    cursorUnitDetector.lock()->GetGameObject()->GetTransform()->SetWorldPosition(GetWorldCursorPosition());
    HandleInput();
    HandleState();
    HandleCamera();
    HandleSkillPreview();
    HandleByState();
    HandleSkillCooltime();
    HandleManaRegen();
    HandleMouseHover();
    HandleUnitPickingCollider();
#ifdef EDITOR
    static yunutyEngine::graphics::UIText* text_State{ nullptr };
    if (text_State == nullptr)
    {
        text_State = Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::graphics::UIText>();
        text_State->GetGI().SetFontSize(30);
        text_State->GetGI().SetColor(yunuGI::Color{ 1,0,1,1 });
        text_State->GetTransform()->SetLocalScale(Vector3d{ 1200,500,0 });
        text_State->GetTransform()->SetLocalPosition(Vector3d{ 0,260,0 });
    }
    if (!selectedDebugCharacter.expired())
    {
        wstringstream wsstream;
        wsstream << L"unitState : ";
        auto& activeStates = selectedDebugCharacter.lock()->GetBehaviourTree().GetActiveNodes();
        for (const auto& each : activeStates)
        {
            wsstream << wstring(L"[") + yutility::GetWString(POD_Enum<UnitBehaviourTree::Keywords>::GetEnumNameMap().at(each->GetNodeKey())) + wstring(L"]");
        }
        wsstream << L"\nacq : " << selectedDebugCharacter.lock()->acquisitionRange.lock()->GetEnemies().size();
        wsstream << L"\ncurrent pos : " << selectedDebugCharacter.lock()->GetTransform()->GetWorldPosition();
        wsstream << L"\nattack target pos : " << selectedDebugCharacter.lock()->attackMoveDestination;
        if (!cursorUnitDetector.lock()->GetUnits().empty())
        {
            wsstream << L"\nhovering unit : " << yutility::GetWString(GetUnitOnCursor()->name);
        }

        text_State->GetGI().SetText(wsstream.str());
    }
#endif
}

void PlayerController::HandleByState()
{
    switch (state)
    {
    case State::Peace:
    {
        const auto& gc = GlobalConstant::GetSingletonInstance().pod;
        Vector3d forward = { std::cos(selectedCharacter.lock()->desiredRotation * math::Deg2Rad) , 0, std::sin(selectedCharacter.lock()->desiredRotation * math::Deg2Rad) };
        Vector3d right = { std::cos((selectedCharacter.lock()->desiredRotation - 90) * math::Deg2Rad) , 0, std::sin((selectedCharacter.lock()->desiredRotation - 90) * math::Deg2Rad) };
        peaceFollowingDestination[0] = selectedCharacter.lock()->GetTransform()->GetWorldPosition();
        peaceFollowingDestination[0] += forward * gc.peaceFollowingZOffest;
        peaceFollowingDestination[0] += right * gc.peaceFollowingXOffest;
        peaceFollowingDestination[1] = selectedCharacter.lock()->GetTransform()->GetWorldPosition();
        peaceFollowingDestination[1] += forward * gc.peaceFollowingZOffest * 1.2f;
        peaceFollowingDestination[1] -= right * gc.peaceFollowingXOffest * 0.8f;

        if (std::max(peaceFollowingUnits[0].lock()->DistanceSquare(peaceFollowingDestination[0]),
            peaceFollowingUnits[1].lock()->DistanceSquare(peaceFollowingDestination[1])) >
            std::max(peaceFollowingUnits[0].lock()->DistanceSquare(peaceFollowingDestination[1]),
                peaceFollowingUnits[1].lock()->DistanceSquare(peaceFollowingDestination[0])))
        {
            std::swap(peaceFollowingDestination[0], peaceFollowingDestination[1]);
        }
        peaceFollowingUnits[0].lock()->OrderMove(peaceFollowingDestination[0]);
        peaceFollowingUnits[1].lock()->OrderMove(peaceFollowingDestination[1]);
        break;
    }
    }
}

void PlayerController::HandleInput()
{
    if (state == State::Cinematic) return;

    if (Input::isKeyPushed(KeyCode::Space) && ((state == State::Battle) || state == State::Tactic))
    {
        if ((TacticModeSystem::Instance().IsCoolTime() == false) && (TacticModeSystem::Instance().IsExecuting() == false))
        {
            TacticModeSystem::Instance().EngageTacticSystem();
        }
        else if ((TacticModeSystem::Instance().IsOperation() == true) && (TacticModeSystem::Instance().IsExecuting() == false))
        {
            TacticModeSystem::Instance().ExecuteCommands();
        }
    }

    // 전술모드의 마지막 명령을 지우는 키
    if (Input::isKeyDown(KeyCode::Control) && Input::isKeyPushed(KeyCode::Z) && TacticModeSystem::Instance().IsOperation() && !TacticModeSystem::Instance().IsExecuting())
    {
        TacticModeSystem::Instance().PopCommand();
    }

    if (Input::isKeyPushed(KeyCode::ESC) && TacticModeSystem::Instance().IsOperation() && !TacticModeSystem::Instance().IsExecuting())
    {
        TacticModeSystem::Instance().ClearCommand();
    }

    if ((TacticModeSystem::Instance().IsExecuting() == false))
    {
        if (Input::isKeyPushed(KeyCode::Q))
        {
            switch (selectedCharacterType)
            {
            case PlayerCharacterType::Robin: SelectSkill(SkillType::ROBIN_Q); break;
            case PlayerCharacterType::Ursula: SelectSkill(SkillType::URSULA_Q); break;
            case PlayerCharacterType::Hansel: SelectSkill(SkillType::HANSEL_Q); break;
            }
        }
        if (Input::isKeyPushed(KeyCode::W))
        {
            switch (selectedCharacterType)
            {
            case PlayerCharacterType::Robin: SelectSkill(SkillType::ROBIN_W); break;
            case PlayerCharacterType::Ursula: SelectSkill(SkillType::URSULA_W); break;
            case PlayerCharacterType::Hansel: SelectSkill(SkillType::HANSEL_W); break;
            }
        }
        if (Input::isKeyPushed(KeyCode::NUM_1))
        {
            SelectPlayerUnit(PlayerCharacterType::Robin);
        }
        if (Input::isKeyPushed(KeyCode::NUM_2))
        {
            SelectPlayerUnit(PlayerCharacterType::Ursula);
        }
        if (Input::isKeyPushed(KeyCode::NUM_3))
        {
            SelectPlayerUnit(PlayerCharacterType::Hansel);
        }
        if (Input::isKeyPushed(KeyCode::A))
        {
            OrderAttackMove(GetWorldCursorPosition());
        }
        if (Input::isKeyPushed(KeyCode::MouseLeftClick) && !UIManager::Instance().IsMouseOnButton())
        {
            OnLeftClick();
        }
        if (Input::isKeyPushed(KeyCode::MouseRightClick))
        {
            OnRightClick();
        }
    }
}

void PlayerController::HandleState()
{
    ///
// 액션쪽에서 활성화한 스테이트가 있는지 판단하는 부분
    //this->isStateAction = false;
    /*if (this->isStateAction)
    {
        return;
    }*/
    //
    if (application::CinematicManager::Instance().IsCinematicMode())
    {
        SetState(State::Cinematic);
        return;
    }
    if ((TacticModeSystem::Instance().IsOperation() == true) || (TacticModeSystem::Instance().IsExecuting() == true))
    {
        this->SetState(State::Tactic);
        return;
    }

    /// Battle 과 Peace 는 Request 요청을 우선시합니다.
    switch (stateRequestedByAction)
    {
    case PlayerController::State::Battle:
    {
        this->SetState(State::Battle);
        return;
    }
    case PlayerController::State::Peace:
    {
        this->SetState(State::Peace);
        return;
    }
    default:
        break;
    }

    if (!PlaytimeWave::GetCurrentOperatingWave().expired())
    {
        if (PlaytimeWave::GetCurrentOperatingWave().lock()->IsRemainEnemyAndWave())
        {
            this->SetState(State::Battle);
        }
        else
        {
            this->SetState(State::Peace);
        }
    }
    else
    {
        this->SetState(State::Peace);
    }

    //if (BossController::Instance().GetBoss().lock())
    //{
    //	if (BossController::Instance().GetBoss().lock()->IsAlive())
    //	{
    //		this->SetState(State::Battle);
    //	}
    //	else
    //	{
    //		this->SetState(State::Peace);
    //	}
    //}
}

void PlayerController::HandleCamera()
{
    static constexpr float tacticZoomoutDistanceFactor = 1.2f;
    // 영웅이 선택되어 있고, 카메라가 선택된 영웅을 따라가는 경우 targetPos는 영웅의 위치로 설정됩니다.
    Vector3d targetPos;
    if (!selectedCharacter.expired())
    {
        Vector3d selectedCharPos = selectedCharacter.lock()->GetTransform()->GetWorldPosition();
        targetPos = selectedCharPos + camOffset;
    }
    // 카메라가 지역 제한에 걸렸을 경우, targetPos를 지역 안으로 정의합니다.
    if (camLockRegion)
    {
        targetPos.x = std::clamp(targetPos.x, camLockRegion->pod.x - camLockRegion->pod.width * 0.5, camLockRegion->pod.x + camLockRegion->pod.width * 0.5);
        targetPos.z = std::clamp(targetPos.z, camLockRegion->pod.z - camLockRegion->pod.height * 0.5, camLockRegion->pod.z + camLockRegion->pod.height * 0.5);
    }
    RTSCam::Instance().SetIdealPosition(targetPos);
    RTSCam::Instance().SetIdealRotation(camRotation);
}

void PlayerController::HandleSkillPreview()
{
    if (state != State::Tactic)
    {
        switch (selectedSkill)
        {
        case SkillType::ROBIN_Q:
            SkillPreviewSystem::Instance().ShowRobinQSkill(characters[PlayerCharacterType::Robin].lock()->GetTransform()->GetWorldPosition());
            SkillPreviewSystem::Instance().ShowSkillMaxRange(SkillPreviewSystem::UnitType::Robin, characters[PlayerCharacterType::Robin].lock()->GetTransform()->GetWorldPosition(), RobinChargeSkill::GetMaxDistance());
            break;
        case SkillType::URSULA_Q:
        {
            UrsulaBlindSkill::UpdatePosition(characters[PlayerCharacterType::Ursula].lock()->GetTransform()->GetWorldPosition(), GetWorldCursorPosition());
            auto pos1 = UrsulaBlindSkill::GetSkillObjectPos_Left(GetWorldCursorPosition());
            auto pos2 = UrsulaBlindSkill::GetSkillObjectPos_Right(GetWorldCursorPosition());
            auto pos3 = UrsulaBlindSkill::GetSkillObjectPos_Top(GetWorldCursorPosition());
            SkillPreviewSystem::Instance().ShowUrsulaQSkill(pos1, pos2, pos3, Vector3d::one * UrsulaBlindSkill::pod.skillRadius);
            SkillPreviewSystem::Instance().ShowSkillMaxRange(SkillPreviewSystem::UnitType::Ursula, characters[PlayerCharacterType::Ursula].lock()->GetTransform()->GetWorldPosition(), UrsulaBlindSkill::GetSkillRange());
            break;
        }
        case SkillType::URSULA_W:
            SkillPreviewSystem::Instance().ShowUrsulaWSkill(GetWorldCursorPosition(), UrsulaParalysisSkill::GetSkillRadius());
            SkillPreviewSystem::Instance().ShowSkillMaxRange(SkillPreviewSystem::UnitType::Ursula, characters[PlayerCharacterType::Ursula].lock()->GetTransform()->GetWorldPosition(), UrsulaParalysisSkill::pod.skillRange);
            break;
        case SkillType::HANSEL_Q:
            SkillPreviewSystem::Instance().ShowHanselQSkill(GetWorldCursorPosition(), HanselChargeSkill::pod.skillRadius);
            SkillPreviewSystem::Instance().ShowSkillMaxRange(SkillPreviewSystem::UnitType::Hansel, characters[PlayerCharacterType::Hansel].lock()->GetTransform()->GetWorldPosition(), HanselChargeSkill::GetMaxRange());
            break;
        case SkillType::HANSEL_W:
            SkillPreviewSystem::Instance().ShowHanselWSkill(characters[PlayerCharacterType::Hansel].lock()->GetTransform()->GetWorldPosition());
            SkillPreviewSystem::Instance().ShowSkillMaxRange(SkillPreviewSystem::UnitType::Hansel, characters[PlayerCharacterType::Hansel].lock()->GetTransform()->GetWorldPosition(), HanselProjectileSkill::GetMaxRange());
            break;
        }
    }
    else
    {
        if (selectedCharacter.lock()->IsTacTicReady())
        {
            TacticModeSystem::Instance().ShowSkillPreviewInTacticMode(selectedSkill);
        }
    }

    // 임시 이동 경로 보여주는 부분
    if ((state == State::Tactic) && (selectedSkill == SkillType::NONE))
    {
        if (selectedCharacter.lock()->IsTacTicReady())
        {
            TacticModeSystem::Instance().ShowTemporaryRouteInTacticMode(this->selectedCharacterType);
        }
    }
}

void PlayerController::HandleSkillCooltime()
{
    if (state == State::Tactic)
    {
        return;
    }

    for (int skillType = SkillType::ROBIN_Q; skillType <= SkillType::HANSEL_W; skillType++)
    {
        SetCooltime((SkillType::Enum)skillType, skillCooltimeLeft[skillType] - Time::GetDeltaTime());
    }
}

void PlayerController::HandleManaRegen()
{
    if (state != State::Tactic)
    {
        SetMana(mana + GlobalConstant::GetSingletonInstance().pod.manaRegen * Time::GetDeltaTime());
    }
}

void PlayerController::HandleMouseHover()
{
    if (!cursorUnitDetector.lock()->GetUnits().empty())
    {
        ApplyHoverEffect(GetUnitOnCursor()->GetWeakPtr<Unit>());
    }
    else
    {
        DisableHoverEffect();
    }
}
// 카메라의 near plane으로부터 far plane까지 뻗는 직선의 형태로
// 셀렉션 박스의 트랜스폼을 변경합니다.
// screenSpacePos는 x,y좌표가 -0.5에서 0.5 사이의 값을 가지는 정규화된 스크린좌표입니다.
// 화면의 중심이 {0,0} 상단이 0.5, 하단이 -0.5인 좌단이 -0.5, 우단이 0.5입니다.
void PlayerController::HandleUnitPickingCollider()
{
    auto cam = graphics::Camera::GetMainCamera();

    Input::getMouseScreenPositionNormalizedZeroCenter();
    auto nearPoint = cam->GetTransform()->GetWorldPosition() - cam->GetTransform()->GetWorldRotation().Forward() * cam->GetGI().GetNear();
    auto farPoint = cam->GetProjectedPoint(Input::getMouseScreenPositionNormalizedZeroCenter(), 2000);
    auto transform = cursorUnitDetector.lock()->GetTransform();
    transform->SetLocalScale({ 0.1,0.1, (nearPoint - farPoint).Magnitude() });
    transform->SetWorldPosition((nearPoint + farPoint) / 2.0);
    transform->SetWorldRotation(Quaternion::MakeWithForwardUp(farPoint - nearPoint, cam->GetTransform()->GetWorldRotation().Up()));
}

void PlayerController::SelectPlayerUnit(PlayerCharacterType::Enum charType)
{
    UnSelectSkill();
    if (charType == selectedCharacterType || charType == PlayerCharacterType::None)
    {
        return;
    }
    selectedCharacterType = charType;
    selectedCharacter = characters[charType];
    selectedDebugCharacter = characters[charType];
    ApplySelectEffect(characters[charType]);
    switch (selectedCharacterType)
    {
    case PlayerCharacterType::Robin:
        peaceFollowingUnits[0] = characters[PlayerCharacterType::Ursula];
        peaceFollowingUnits[1] = characters[PlayerCharacterType::Hansel];
        UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Robin)->
            GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Portrait)->button->InvokeInternalButtonClickEvent();
        break;
    case PlayerCharacterType::Ursula:
        peaceFollowingUnits[0] = characters[PlayerCharacterType::Robin];
        peaceFollowingUnits[1] = characters[PlayerCharacterType::Hansel];
        UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Ursula)->
            GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Portrait)->button->InvokeInternalButtonClickEvent();
        break;
    case PlayerCharacterType::Hansel:
        peaceFollowingUnits[0] = characters[PlayerCharacterType::Robin];
        peaceFollowingUnits[1] = characters[PlayerCharacterType::Ursula];
        UIManager::Instance().GetUIElementByEnum(UIEnumID::CharInfo_Hansel)->
            GetLocalUIsByEnumID().at(UIEnumID::CharInfo_Portrait)->button->InvokeInternalButtonClickEvent();
    default:
        break;
    }
}

void PlayerController::OnLeftClick()
{
    if (selectedSkill == SkillType::NONE)
    {
        if (!cursorUnitDetector.lock()->GetUnits().empty())
        {
            SelectUnit(GetUnitOnCursor()->GetWeakPtr<Unit>());
        }
    }
    else
    {
        ActivateSkill(selectedSkill, GetWorldCursorPosition());
        if (state != State::Tactic)
        {
            selectedSkill = SkillType::NONE;
        }
    }
}

void PlayerController::OnPlayerChracterDead(std::weak_ptr<Unit> unit)
{
    bool alldead = true;
    for (auto& each : characters)
    {
        if (each.lock()->IsAlive())
        {
            alldead = false;
            break;
        }
    }
    if (alldead)
    {
        OnPlayerChracterAllDead();
    }
}

void PlayerController::OnPlayerChracterAllDead()
{
    UIManager::Instance().GetUIElementByEnum(UIEnumID::DefeatPage)->EnableElement();
}

void PlayerController::OnRightClick()
{
    if (selectedSkill == SkillType::NONE)
    {
        if (state != State::Tactic)
        {
            if (!cursorUnitDetector.lock()->GetUnits().empty() && GetUnitOnCursor()->teamIndex != playerTeamIndex)
            {
                OrderAttack(GetUnitOnCursor()->GetWeakPtr<Unit>());
            }
            else
            {
                SFXManager::PlaySoundfile("sounds/Etc/Character move click.mp3");
                OrderMove(GetWorldCursorPosition());
            }
        }
        else
        {
            SkillPreviewSystem::Instance().HideTemporaryRoute();
            if (!cursorUnitDetector.lock()->GetUnits().empty() && GetUnitOnCursor()->teamIndex != playerTeamIndex)
            {
                // Attack
                // 걸어가서 공격을 하게 될 수 있음
                EnqueErrorType errorType = EnqueErrorType::NONE;
                std::vector<Vector3d> path;
                path = TacticModeSystem::Instance().GetPathInTacticMode(selectedCharacterType, GetUnitOnCursor());
                this->ModifyPathForAttack(path);
                if (!path.empty())
                {
                    // 이동을 해야한다면
                    // 이동 명령
                    errorType = TacticModeSystem::Instance().EnqueueCommand(std::make_shared<UnitMoveCommand>(characters[selectedCharacterType].lock().get()
                        , path.back()
                        , path
                        , true
                    ));

                    // 공격 명령
                    if (errorType == EnqueErrorType::Success)
                    {
                        errorType = TacticModeSystem::Instance().EnqueueCommand(std::make_shared<UnitAttackCommand>(
                            characters[selectedCharacterType].lock().get()
                            , path.back()
                            , GetUnitOnCursor()
                            , true
                            , path.back() - path[path.size() - 2]));

                        // 이동 명령은 Enque됐지만 공격명령이 Enque되지 않았을 경우 이동 명령까지 지운다
                        if (errorType != EnqueErrorType::Success)
                        {
                            TacticModeSystem::Instance().PopCommand();
                        }
                    }
                }
                else
                {
                    // 이동없이 공격이 가능하다면 공격 명령
                    EnqueErrorType errorType = EnqueErrorType::NONE;
                    errorType = TacticModeSystem::Instance().EnqueueCommand(std::make_shared<UnitAttackCommand>(
                        characters[selectedCharacterType].lock().get()
                        , Vector3d::zero
                        , GetUnitOnCursor()
                        , false
                        , GetUnitOnCursor()->GetGameObject()->GetTransform()->GetWorldPosition() -
                        characters[selectedCharacterType].lock().get()->GetGameObject()->GetTransform()->GetWorldPosition()));
                    // 에러 타입에 따른 UI활성화
                }
            }
            else
            {
                // Move
                std::vector<Vector3d> path;
                EnqueErrorType errorType = EnqueErrorType::NONE;
                path = TacticModeSystem::Instance().GetPathInTacticMode(selectedCharacterType);

                errorType = TacticModeSystem::Instance().EnqueueCommand(std::make_shared<UnitMoveCommand>(characters[selectedCharacterType].lock().get()
                    , GetWorldCursorPosition(),
                    path, false));
                // 에러 타입에 따른 UI활성화
            }
        }
    }
    else if (selectedSkill != SkillType::NONE)
    {
        UnSelectSkill();
    }
}

void PlayerController::SelectUnit(std::weak_ptr<Unit> unit)
{
    SelectPlayerUnit(static_cast<PlayerCharacterType::Enum>(unit.lock()->GetUnitTemplateData().pod.playerUnitType.enumValue));
    selectedDebugCharacter = unit;
}

void PlayerController::OrderMove(Vector3d position)
{
    selectedCharacter.lock()->OrderMove(position);
    UIManager::Instance().SummonMoveToFeedback(GetWorldCursorPosition());
}

void PlayerController::OrderAttackMove(Vector3d position)
{
    selectedCharacter.lock()->OrderAttackMove(position);
    UIManager::Instance().SummonMoveToFeedback(GetWorldCursorPosition());
}

void PlayerController::OrderAttack(std::weak_ptr<Unit> unit)
{
    ApplyTargetedEffect(unit);
    selectedCharacter.lock()->OrderAttack(unit);
}

void PlayerController::OrderInteraction(std::weak_ptr<IInteractableComponent> interactable)
{
    selectedCharacter.lock()->OrderMove(interactable.lock()->GetTransform()->GetWorldPosition());
}

void PlayerController::ActivateSkill(SkillType::Enum skillType, Vector3d pos)
{
    if (skillCooltimeLeft[skillType] > 0) return;
    if (state == State::Cinematic) return;
    switch (skillType)
    {
    case SkillType::ROBIN_Q:
    case SkillType::HANSEL_Q:
        static constexpr float epsilon = 0.01f;
        Vector3d deltaDistance = (pos - SingleNavigationField::Instance().GetClosestPointOnField(pos));
        deltaDistance.y = 0;
        if (deltaDistance.MagnitudeSqr() > epsilon)
        {
            UIManager::Instance().GetUIElementByEnum(UIEnumID::ErrorPopup_CantLand)->EnableElement();
            UnSelectSkill();
            return;
        }
        break;
    }
    if (state != State::Tactic)
    {
        // 전술 모드가 아니라면 기존 로직 수행
        SetMana(mana - RequiredManaForSkill(skillType));
        onSkillActivate[skillType]();
        switch (skillType)
        {
        case SkillType::ROBIN_Q:
            selectedCharacter.lock()->OrderSkill(RobinChargeSkill{  }, pos);
            break;
        case SkillType::ROBIN_W:
            selectedCharacter.lock()->OrderSkill(RobinTauntSkill{  }, pos);
            break;
        case SkillType::URSULA_Q:
            selectedCharacter.lock()->OrderSkill(UrsulaBlindSkill{  }, pos);
            break;
        case SkillType::URSULA_W:
            selectedCharacter.lock()->OrderSkill(UrsulaParalysisSkill{  }, pos);
            break;
        case SkillType::HANSEL_Q:
            selectedCharacter.lock()->OrderSkill(HanselChargeSkill{}, pos);
            break;
        case SkillType::HANSEL_W:
            selectedCharacter.lock()->OrderSkill(HanselProjectileSkill{}, pos);
            break;
        }
        UnSelectSkill();
        // 스킬 프리뷰를 비활성화시킨다.
    }
    else
    {
        SkillPreviewSystem::Instance().HideSkillMaxRange();
        // 전술모드일 때 
        // Skill
        // 걸어가서 스킬을 쓰게될 수 있음
        EnqueErrorType errorType = EnqueErrorType::NONE;
        std::vector<Vector3d> path;
        path = TacticModeSystem::Instance().GetPathInTacticMode(selectedCharacterType);
        this->ModifyPathForSkill(path, skillType);
        if (!path.empty())
        {

            // 이동을 해야한다면
            // 이동 명령
            errorType = TacticModeSystem::Instance().EnqueueCommand(std::make_shared<UnitMoveCommand>(characters[selectedCharacterType].lock().get()
                , path.back()
                , path
                , false
            ));

            // 스킬 명령
            if (errorType == EnqueErrorType::Success)
            {
                errorType = TacticModeSystem::Instance().EnqueueCommand(std::make_shared<UnitSkillCommand>(characters[selectedCharacterType].lock().get()
                    , GetWorldCursorPosition()
                    , skillType
                ));

                // 이동 명령은 Enque됐지만 공격명령이 Enque되지 않았을 경우 이동 명령까지 지운다
                if (errorType != EnqueErrorType::Success)
                {
                    TacticModeSystem::Instance().PopCommand();
                }
                if (errorType == EnqueErrorType::Success)
                {
                    selectedSkill = SkillType::NONE;
                }
            }
        }
        else
        {
            // 이동없이 스킬 사용이 가능하다면 스킬 명령

            EnqueErrorType errorType = EnqueErrorType::NONE;
            errorType = TacticModeSystem::Instance().EnqueueCommand(std::make_shared<UnitSkillCommand>(characters[selectedCharacterType].lock().get()
                , GetWorldCursorPosition()
                , skillType
            ));
            if (errorType == EnqueErrorType::Success)
            {
                selectedSkill = SkillType::NONE;
            }

            // 에러 타입에 따른 UI활성화
        }
    }
}

void PlayerController::SelectSkill(SkillType::Enum skillType)
{
    if ((TacticModeSystem::Instance().CanSelectSkill(skillType) == false) && (TacticModeSystem::Instance().IsOperation()))
    {
        return;
    }
    if (blockSkillSelection[skillType] == true)
    {
        return;
    }

    UnSelectSkill();
    SkillPreviewSystem::Instance().HideTemporaryRoute();
    if (skillCooltimeLeft[skillType] > 0)
    {
        UIManager::Instance().GetUIElementByEnum(UIEnumID::ErrorPopup_Cooltime)->EnableElement();
        return;
    }
    if (mana < RequiredManaForSkill(skillType))
    {
        UIManager::Instance().GetUIElementByEnum(UIEnumID::ErrorPopup_NoMana)->EnableElement();
        return;
    }
    // 이 부분은 협의가 필요함
    if ((state == State::Peace) || (state == State::Cinematic)) return;
    onSkillSelect[skillType]();
    switch (skillType)
    {
    case SkillType::ROBIN_Q: case SkillType::ROBIN_W: SelectPlayerUnit(PlayerCharacterType::Robin); break;
    case SkillType::URSULA_Q: case SkillType::URSULA_W: SelectPlayerUnit(PlayerCharacterType::Ursula); break;
    case SkillType::HANSEL_Q: case SkillType::HANSEL_W: SelectPlayerUnit(PlayerCharacterType::Hansel); break;
    }

    // 즉발은 그냥 써버리고 나머지는 준비상태로 만든다.
    switch (skillType)
    {
    case SkillType::ROBIN_W:
    {
        ActivateSkill(SkillType::ROBIN_W, characters[PlayerCharacterType::Robin].lock()->GetTransform()->GetWorldPosition());
    }
    break;
    default:
        selectedSkill = skillType;
        // 스킬 프리뷰를 활성화시킨다.
        break;
    }
}
void PlayerController::SetState(State::Enum newState)
{
    if (newState == state)
        return;
    switch (state)
    {
    case PlayerController::State::Tactic:
        UIManager::Instance().GetUIElementByEnum(UIEnumID::TacticModeIngameUI)->DisableElement();
        UIManager::Instance().GetUIElementByEnum(UIEnumID::Ingame_Bottom_Layout)->EnableElement();
        break;
    }
    state = newState;
    switch (state)
    {
    case State::Peace:
    case State::Cinematic:
        UnSelectSkill();
        for (auto& each : characters)
        {
            each.lock()->playingBattleAnim = false;
        }
        break;
    case State::Battle:
        for (auto& each : characters)
        {
            each.lock()->playingBattleAnim = true;
        }
        break;
    case State::Tactic:
    {
        UIManager::Instance().GetUIElementByEnum(UIEnumID::TacticModeIngameUI)->EnableElement();
        UIManager::Instance().GetUIElementByEnum(UIEnumID::Ingame_Bottom_Layout)->DisableElement();
        UnSelectSkill();
    }
    break;
    }
}
std::array<float, (int)PlayerCharacterType::Num> PlayerController::GetAggroProportions() const
{
    std::array<float, (int)PlayerCharacterType::Num> proportions;
    float sum = 0;
    sum = proportions[PlayerCharacterType::Robin] = characters.at(PlayerCharacterType::Robin).lock()->GetUnitTemplateData().pod.playerAggroRatio;
    sum += proportions[PlayerCharacterType::Ursula] = characters.at(PlayerCharacterType::Ursula).lock()->GetUnitTemplateData().pod.playerAggroRatio;
    sum += proportions[PlayerCharacterType::Hansel] = characters.at(PlayerCharacterType::Hansel).lock()->GetUnitTemplateData().pod.playerAggroRatio;
    proportions[PlayerCharacterType::Robin] /= sum;
    proportions[PlayerCharacterType::Ursula] /= sum;
    proportions[PlayerCharacterType::Hansel] /= sum;
    return proportions;
}
// 필요한 것들을 다 초기화한다.
void PlayerController::Reset()
{
    UIManager::Instance().GetUIElementByEnum(UIEnumID::Ingame_Combo_Number)->DisableElement();
    UIManager::Instance().GetUIElementByEnum(UIEnumID::Ingame_Combo_Text)->DisableElement();
    for (auto& each : onSkillActivate) each.Clear();
    for (auto& each : onSkillSelect) each.Clear();
    for (auto& each : blockSkillSelection) each = false;
    if (cursorUnitDetector.expired())
        cursorUnitDetector = Scene::getCurrentScene()->AddGameObject()->AddComponentAsWeakPtr<UnitAcquisitionBoxCollider>();
    std::for_each(skillUpgradeByUI.begin(), skillUpgradeByUI.end(), [&](auto& pair) {
        auto& [ui, upgrade] = pair;
        UIManager::Instance().GetUIElementByEnum(ui)->imageComponent.lock()->GetGI().SetColor({ 1,1,1,1 });
        skillUpgraded[upgrade] = false;
        });
    skillPointsLeft = 0;
}

// 현재 카메라의 위치에 따라 카메라의 플레이어 기준 오프셋 위치와 회전각을 결정합니다.
void PlayerController::SetCameraOffset()
{
    if (characters[PlayerCharacterType::Robin].expired())
        return;
    auto camPos = graphics::Camera::GetMainCamera()->GetTransform()->GetWorldPosition();
    camOffset = camPos - characters[PlayerCharacterType::Robin].lock()->GetTransform()->GetWorldPosition();
    camRotation = graphics::Camera::GetMainCamera()->GetTransform()->GetWorldRotation();
}

void PlayerController::SetComboObjectives(const std::array<int, 3>& targetCombos)
{
    comboObjective = targetCombos;
    comboAchieved.fill(false);
    for (int i = 0; i < 3; i++)
    {
        comboAchieved[i] = false;
        UIManager::Instance().GetUIElementByEnum(UIManager::comboNumbers[i])->SetNumber(comboObjective[i]);
        UIManager::Instance().GetUIElementByEnum(UIManager::comboNumbers[i + 3])->SetNumber(comboObjective[i]);
        UIManager::Instance().GetUIElementByEnum(UIManager::comboFinishedImgs[i])->DisableElement();
        UIManager::Instance().GetUIElementByEnum(UIManager::comboUnFinishedImgs[i])->EnableElement();
        UIManager::Instance().GetUIElementByEnum(UIManager::comboCheckImgs[i])->DisableElement();
    }
}

void PlayerController::AddCombo()
{
    currentCombo++;
    UIManager::Instance().GetUIElementByEnum(UIEnumID::Ingame_Combo_Number)->EnableElement();
    UIManager::Instance().GetUIElementByEnum(UIEnumID::Ingame_Combo_Text)->EnableElement();
    UIManager::Instance().GetUIElementByEnum(UIEnumID::Ingame_Combo_Number)->SetNumber(currentCombo);
    for (auto i = 0; i < 3; i++)
    {
        if (!comboAchieved[i] && comboObjective[i] > 0 && currentCombo >= comboObjective[i])
        {
            comboAchieved[i] = true;
            IncrementSkillPoint();
            UIManager::Instance().GetUIElementByEnum(UIManager::comboCheckImgs[i])->EnableElement();
            UIManager::Instance().GetUIElementByEnum(UIManager::comboFinishedImgs[i])->EnableElement();
            UIManager::Instance().GetUIElementByEnum(UIManager::comboUnFinishedImgs[i])->DisableElement();
        }
    }
}

void PlayerController::UnSelectSkill()
{
    if (blockSkillCancel == true)
    {
        return;
    }
    switch (selectedSkill)
    {
    case SkillType::ROBIN_Q: SkillPreviewSystem::Instance().HideRobinQSkill(); break;
    case SkillType::URSULA_Q: SkillPreviewSystem::Instance().HideUrsulaQSkill(); break;
    case SkillType::URSULA_W: SkillPreviewSystem::Instance().HideUrsulaWSkill(); break;
    case SkillType::HANSEL_Q: SkillPreviewSystem::Instance().HideHanselQSkill(); break;
    case SkillType::HANSEL_W: SkillPreviewSystem::Instance().HideHanselWSkill(); break;
    }
    if (selectedSkill != SkillType::NONE)
        SkillPreviewSystem::Instance().HideSkillMaxRange();
    selectedSkill = SkillType::NONE;
}

void PlayerController::UnSelectSkill(std::weak_ptr<Unit> unit)
{
    auto playerType = unit.lock()->unitTemplateData->pod.playerUnitType.enumValue;
    switch (playerType)
    {
    case PlayerCharacterType::Robin:
        if (selectedSkill == SkillType::ROBIN_Q || selectedSkill == SkillType::ROBIN_W)
        {
            UnSelectSkill();
        }
        break;
    case PlayerCharacterType::Ursula:
        if (selectedSkill == SkillType::URSULA_Q || selectedSkill == SkillType::URSULA_W)
        {
            UnSelectSkill();
        }
        break;
    case PlayerCharacterType::Hansel:
        if (selectedSkill == SkillType::HANSEL_Q || selectedSkill == SkillType::HANSEL_W)
        {
            UnSelectSkill();
        }
        break;
    }
}

// xz평면에 사영된 마우스 위치를 반환한다.
Vector3d PlayerController::GetWorldCursorPosition()
{
    auto mainCam = yunutyEngine::graphics::Camera::GetMainCamera();
    auto distToXZPlane = abs(mainCam->GetTransform()->GetWorldPosition().y);
    auto projectedPoint = mainCam->GetProjectedPoint(Input::getMouseScreenPositionNormalizedZeroCenter(), distToXZPlane, Vector3d(0, 1, 0));
    return projectedPoint;
}

void PlayerController::ResetCombo()
{
}

void PlayerController::SetManaFull()
{
    SetMana(GlobalConstant::GetSingletonInstance().pod.maxMana);
}
void PlayerController::SetMana(float mana)
{
    const auto& gc = GlobalConstant::GetSingletonInstance().pod;
    this->mana = std::fmin(gc.maxMana, mana);
    UIManager::Instance().GetUIElementByEnum(UIEnumID::ManaFill)->adjuster->SetTargetFloat(1 - mana / gc.maxMana);
    UIManager::Instance().GetUIElementByEnum(UIEnumID::Mana_Text_MaxMP)->SetNumber(gc.maxMana);
    UIManager::Instance().GetUIElementByEnum(UIEnumID::Mana_Text_CurrentMP)->SetNumber(mana);
}

Unit* PlayerController::GetUnitOnCursor()
{
    if (cursorUnitDetector.lock()->GetUnits().empty()) return nullptr;

    auto mainCamPos = graphics::Camera::GetMainCamera()->GetTransform()->GetWorldPosition();
    return *std::min_element(cursorUnitDetector.lock()->GetUnits().begin(), cursorUnitDetector.lock()->GetUnits().end(),
        [&mainCamPos](Unit* a, Unit* b) {
            return a->DistanceSquare(mainCamPos) < b->DistanceSquare(mainCamPos);
        });
}

void PlayerController::OnPause()
{

}

void PlayerController::OnResume()
{
}

void PlayerController::RequestStateFromAction(State::Enum newState)
{
    this->stateRequestedByAction = newState;
}

float PlayerController::GetMana()
{
    return this->mana;
}

void PlayerController::SetCooltime(SkillType::Enum skillType, float cooltime)
{
    skillCooltimeLeft[skillType] = std::fmax(0.0f, cooltime);
    skillCooltimeNumberUI[skillType]->SetNumber(cooltime);
    skillCooltimeMaskUI[skillType]->adjuster->SetTargetFloat(skillCooltimeLeft[skillType] / GetCooltimeForSkill(skillType));
}

void PlayerController::SetCooltime(std::weak_ptr<Unit> unit)
{
    SetCooltime(unit.lock()->onGoingSkill->GetSkillType(), GetCooltimeForSkill(unit.lock()->onGoingSkill->GetSkillType()));
}

float PlayerController::GetCooltimeForSkill(SkillType::Enum skillType)
{
    switch (skillType)
    {
    case SkillType::ROBIN_Q: return skillType, RobinChargeSkill::pod.coolTime;
    case SkillType::ROBIN_W: return RobinTauntSkill::pod.coolTime;
    case SkillType::URSULA_Q: return UrsulaBlindSkill::pod.skillCoolTime;
    case SkillType::URSULA_W: return UrsulaParalysisSkill::pod.skillCoolTime;
    case SkillType::HANSEL_Q: return HanselChargeSkill::pod.coolTime;
    case SkillType::HANSEL_W: return HanselProjectileSkill::pod.coolTime;
    }
}

float PlayerController::RequiredManaForSkill(SkillType::Enum skillType)
{
    switch (skillType)
    {
    case SkillType::ROBIN_Q: return RobinChargeSkill::pod.cost;
    case SkillType::ROBIN_W: return RobinTauntSkill::pod.skillCost;
    case SkillType::URSULA_Q: return UrsulaBlindSkill::pod.skillCost;
    case SkillType::URSULA_W: return UrsulaParalysisSkill::pod.skillCost;
    case SkillType::HANSEL_Q: return HanselChargeSkill::pod.skillCost;
    case SkillType::HANSEL_W: return HanselProjectileSkill::pod.skillCost;
    }
}

void PlayerController::ApplyHoverEffect(std::weak_ptr<Unit> unit)
{
    if (unit.lock()->GetTeamIndex() == playerTeamIndex)
    {
        enemyHoverEffect->SetSelfActive(false);
        allyHoverEffect->SetSelfActive(true);
        allyHoverEffect->GetTransform()->SetLocalPosition(unit.lock()->GetGameObject()->GetTransform()->GetWorldPosition());
        allyHoverEffectAnimator->Resume();

        switch (unit.lock()->unitTemplateData->pod.playerUnitType.enumValue)
        {
        case PlayerCharacterType::Robin:
            allyHoverEffectRenderer->GetGI().GetMaterial()->SetColor(wanderResources::unitColor::ROBIN_COLOR);
            break;
        case PlayerCharacterType::Ursula:
            allyHoverEffectRenderer->GetGI().GetMaterial()->SetColor(wanderResources::unitColor::URSULA_COLOR);
            break;
        case PlayerCharacterType::Hansel:
            allyHoverEffectRenderer->GetGI().GetMaterial()->SetColor(wanderResources::unitColor::HANSEL_COLOR);
            break;
        }
    }
    else
    {
        enemyHoverEffect->SetSelfActive(true);
        allyHoverEffect->SetSelfActive(false);
        enemyHoverEffect->GetTransform()->SetLocalPosition(unit.lock()->GetGameObject()->GetTransform()->GetWorldPosition());
        enemyHoverEffectAnimator->Resume();
    }
}

void PlayerController::DisableHoverEffect()
{
    if (!mouseInteractionEffectInitalized) return;
    enemyHoverEffect->SetSelfActive(false);
    allyHoverEffect->SetSelfActive(false);
}

void PlayerController::ApplySelectEffect(std::weak_ptr<Unit> unit)
{
    if (!mouseInteractionEffectInitalized) return;
    allySelectedEffect->SetSelfActive(true);
    allySelectedEffect->SetParent(unit.lock()->GetGameObject());
    switch (selectedCharacterType)
    {
    case PlayerCharacterType::Robin:
        allySelectedEffectRenderer->GetGI().GetMaterial()->SetColor(wanderResources::unitColor::ROBIN_COLOR);
        break;
    case PlayerCharacterType::Ursula:
        allySelectedEffectRenderer->GetGI().GetMaterial()->SetColor(wanderResources::unitColor::URSULA_COLOR);
        break;
    case PlayerCharacterType::Hansel:
        allySelectedEffectRenderer->GetGI().GetMaterial()->SetColor(wanderResources::unitColor::HANSEL_COLOR);
        break;
    }

}

void PlayerController::ApplyTargetedEffect(std::weak_ptr<Unit> unit)
{
    if (!mouseInteractionEffectInitalized) return;

    enemyTargetedEffect[selectedCharacterType]->SetSelfActive(true);
    enemyTargetedEffectAnimator[selectedCharacterType]->Play();
    enemyTargetedEffect[selectedCharacterType]->SetParent(unit.lock()->GetGameObject());
}

void PlayerController::InitUnitMouseInteractionEffects()
{
    const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
    if (mouseInteractionEffectInitalized) return;
    mouseInteractionEffectInitalized = true;
    {
        allySelectedEffect = Scene::getCurrentScene()->AddGameObjectFromFBX("VFX_CharacterRange");
        allySelectedEffectRenderer = allySelectedEffect->GetChildren()[0]->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>();
        allySelectedEffectRenderer->GetGI().GetMaterial()->SetPixelShader(_resourceManager->GetShader(L"VFX_PS.cso"));
        allySelectedEffect->SetSelfActive(false);
    }
    {
        int i = 0;
        for (auto& each : enemyTargetedEffect)
        {
            each = Scene::getCurrentScene()->AddGameObjectFromFBX("VFX_AttackSelected");
            each->GetChildren()[0]->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(wanderResources::unitColor::ENEMY_COLOR);
            each->SetSelfActive(false);
            enemyTargetedEffectAnimator[i] = each->AddComponent<VFXAnimator>();
            enemyTargetedEffectAnimator[i]->Init();
            ++i;
        }
    }
    {
        allyHoverEffect = Scene::getCurrentScene()->AddGameObjectFromFBX("VFX_CharacterSelected");
        allyHoverEffect->SetSelfActive(false);
        allyHoverEffectRenderer = allyHoverEffect->GetChildren()[0]->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>();
        allyHoverEffectRenderer->GetGI().GetMaterial()->SetPixelShader(_resourceManager->GetShader(L"VFX_PS.cso"));

        allyHoverEffectAnimator = allyHoverEffect->AddComponent<VFXAnimator>();
        allyHoverEffectAnimator->Init();
        allyHoverEffectAnimator->SetLoop(true);
    }
    {
        enemyHoverEffect = Scene::getCurrentScene()->AddGameObjectFromFBX("VFX_CharacterSelected");
        enemyHoverEffect->SetSelfActive(false);
        enemyHoverEffectRenderer = enemyHoverEffect->GetChildren()[0]->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>();
        enemyHoverEffectRenderer->GetGI().GetMaterial()->SetPixelShader(_resourceManager->GetShader(L"VFX_PS.cso"));
        enemyHoverEffectRenderer->GetGI().GetMaterial()->SetColor(wanderResources::unitColor::ENEMY_COLOR);

        enemyHoverEffectAnimator = enemyHoverEffect->AddComponent<VFXAnimator>();
        enemyHoverEffectAnimator->Init();
        enemyHoverEffectAnimator->SetLoop(true);
    }
}

std::vector<yunutyEngine::Vector3d>& PlayerController::ModifyPathForAttack(std::vector<Vector3d>& path)
{
    bool isModify = false;
    auto lastElement = path.back();

    float attackRange = selectedCharacter.lock()->GetUnitTemplateData().pod.m_atkRadius;

    for (auto it = path.rbegin(); it != path.rend(); ++it)
    {
        if (it != path.rbegin())
        {
            auto tempVec = lastElement - *it;
            float distance = tempVec.Magnitude();
            if (attackRange < distance - 0.001f)
            {
                isModify = true;
                // 조건을 만족하는 요소를 찾으면 그 위치의 다음 요소부터 제거
                path.erase((it.base() + 1), path.end());
                break;
            }
        }
    }
    if (!isModify)
    {
        path.clear();
    }
    return path;
}

std::vector<yunutyEngine::Vector3d>& PlayerController::ModifyPathForSkill(std::vector<Vector3d>& path, SkillType::Enum skillType)
{
    bool isModify = false;
    auto lastElement = path.back();

    float skillRange;

    switch (skillType)
    {
    case SkillType::ROBIN_Q:
        skillRange = RobinChargeSkill::GetMaxDistance();
        break;
    case SkillType::ROBIN_W:
        break;
    case SkillType::URSULA_Q:
        skillRange = UrsulaBlindSkill::GetSkillRange();
        break;
    case SkillType::URSULA_W:
        skillRange = UrsulaParalysisSkill::pod.skillRange;
        break;
    case SkillType::HANSEL_Q:
        skillRange = HanselChargeSkill::GetMaxRange();
        break;
    case SkillType::HANSEL_W:
        skillRange = HanselProjectileSkill::GetMaxRange();
        break;
    }

    if ((skillType != SkillType::ROBIN_W))
    {
        for (auto it = path.rbegin(); it != path.rend(); ++it)
        {
            if (it != path.rbegin())
            {
                auto tempVec = lastElement - *it;
                float distance = tempVec.Magnitude();
                if (skillRange < distance - 0.001f)
                {
                    isModify = true;
                    // 조건을 만족하는 요소를 찾으면 그 위치의 다음 요소부터 제거
                    path.erase((it.base() + 1), path.end());
                    break;
                }
            }
        }
    }

    if ((!isModify) || (skillType == SkillType::ROBIN_W))
    {
        path.clear();
    }
    return path;
}
