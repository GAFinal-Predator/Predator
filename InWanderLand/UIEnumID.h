#pragma once
#include "PodStructs.h"

enum class UIEnumID
{
    // 아무것도 아님.
    None = 0,
    // 로빈, 우르술라, 헨젤의 초상화, 초상화 위의 이름 태그, 초상화를 가리는 적색 부상 오버레이, 체력바, 체력을 표시하는 텍스트 UI
    CharInfo_Robin,
    CharInfo_Ursula,
    CharInfo_Hansel,
    CharInfo_Robin_Left,
    CharInfo_Ursula_Left,
    CharInfo_Hansel_Left,
    CharInfo_Portrait,
    CharInfo_PortraitBloodOverlay,
    CharInfo_NameTag,
    CharInfo_HP_Fill,
    CharInfo_HP_Number_Current,
    CharInfo_HP_Number_Max,
    CharInfo_Buff_Bleeding,
    CharInfo_Buff_Blinding,
    CharInfo_Buff_Paralysis,
    CharInfo_Buff_KnockBack,
    CharInfo_Buff_Taunted,
    CharInfo_Buff_UrsulaSelf,
    CharInfo_Buff_HanselBuff,
    CharInfo_Buff_HanselDebuff,
    CharInfo_Skill_Use_Q,
    CharInfo_Skill_Use_Q_Overlay,
    CharInfo_Skill_Use_Q_Cooltime_Number,
    CharInfo_Skill_Use_W,
    CharInfo_Skill_Use_W_Overlay,
    CharInfo_Skill_Use_W_Cooltime_Number,
    // 전술모드 토글버튼
    Toggle_TacticMode,
    // 전술모드 토글버튼 흑백 오버레이
    Toggle_TacticMode_Overlay,
    Toggle_TacticMode_Cooltime_Number,
    // 화면 하단 마나 게이지
    ManaBar1,
    // 전술 모드 마나 게이지
    ManaBar2,
    // 현재 마나량을 나타내는 텍스트
    Mana_Text_CurrentMP,
    Mana_Text_CurrentMP_Tactic,
    // 최대 마나량을 나타내는 텍스트
    Mana_Text_MaxMP,
    Mana_Text_MaxMP_Tactic,
    // 사운드가 켜진 상태에서 표시할 UI버튼
    Sound_On,
    // 사운드가 꺼진 상태에서 표시할 UI버튼
    Sound_Off,
    // 음악이 켜진 상태에서 표시할 UI버튼
    Music_On,
    // 음악이 꺼진 상태에서 표시할 UI버튼
    Music_Off,
    // 인게임에서 사용되는 캐릭터 상태창, 전술모드 진입 버튼을 포함하는 하단 레이아웃
    Ingame_Bottom_Layout,
    // 인게임에서 사용되는 메뉴버튼
    Ingame_MenuButton,
    // 콤보 횟수를 표시함
    Ingame_Combo_Number,
    // "Combo"라는 문자를 표시함
    Ingame_Combo_Text,
    Ingame_Combo_DescriptionTitleImg,
    // 어떤 콤보를 달성해야 하는지 설명하는 텍스트, 몇 콤보를 달성해야 하는지에 대한 표시, 콤보 달성을 표시하는 체크박스의 V자 문양
    Ingame_Combo_Description1,
    Ingame_Combo_DescriptionImageUnfinished1,
    Ingame_Combo_DescriptionImageFinished1,
    Ingame_Combo_TargetNumUnfinished1,
    Ingame_Combo_TargetNumFinished1,
    Ingame_Combo_Check1,
    Ingame_Combo_Description2,
    Ingame_Combo_DescriptionImageUnFinished2,
    Ingame_Combo_DescriptionImageFinished2,
    Ingame_Combo_TargetNumUnfinished2,
    Ingame_Combo_TargetNumFinished2,
    Ingame_Combo_Check2,
    Ingame_Combo_Description3,
    Ingame_Combo_DescriptionImageUnFinished3,
    Ingame_Combo_DescriptionImageFinished3,
    Ingame_Combo_TargetNumUnfinished3,
    Ingame_Combo_TargetNumFinished3,
    Ingame_Combo_Check3,
    Ingame_Vinetting,
    BlackMask_RightToLeft,
    BlackMask_TopToBottom,
    BlackMask_LeftToRight,
    BlackMask_BottomToTop,
    // 게임을 시작할 때, 메인화면으로 돌아갈 때에 사용되는 블랙마스크, 단순 연출용이 아니라 실제 게임로드, 종료를 위한 기능이 있다.
    BlackMask_GameLoad,
    BlackMask_GameEnd,
    LetterBox_Top,
    LetterBox_Bottom,
    // 인게임 하단에서 클릭할 시 스킬트리 메뉴를 여는 메뉴버튼
    InGame_SkiltreeMenu_Active,
    InGame_SkiltreeMenu_InActive,
    SkillPoint_Number,
    SkillUpgradeButtonRobin00,
    SkillUpgradeButtonRobin11,
    SkillUpgradeButtonRobin12,
    SkillUpgradeButtonRobin21,
    SkillUpgradeButtonRobin22,
    SkillUpgradeButtonUrsula00,
    SkillUpgradeButtonUrsula11,
    SkillUpgradeButtonUrsula12,
    SkillUpgradeButtonUrsula21,
    SkillUpgradeButtonUrsula22,
    SkillUpgradeButtonHansel00,
    SkillUpgradeButtonHansel11,
    SkillUpgradeButtonHansel12,
    SkillUpgradeButtonHansel21,
    SkillUpgradeButtonHansel22,
    PopUpMessage_NotEnoughSP,
    PopUpMessage_RequirementNotMet,
    PopUpMessage_PermissionForUpgrade,
    PopUpMessage_PermissionForUpgradeProceedButton,
    PopUpMessage_WarningForRestart,
    PopUpMessage_WarningForRestart_ProceedButton,
    StatusBar_Elite,
    StatusBar_MeleeEnemy,
    StatusBar_Hero_Robin,
    StatusBar_Hero_Ursula,
    StatusBar_Hero_Hansel,
    StatusBar_HP_Number_Current,
    StatusBar_HP_Number_Max,
    StatusBar_HP_Cells,
    StatusBar_HP_Fill,
    StatusBar_SelectionName,
    TitleRoot,
    Quit_Proceed,
    MouseCursor,
    MouseCursor_Free,
    MouseCursor_OnButton,
    MouseCursor_OnEnemy,
    MouseCursor_OnAlly,
    MoveTargetFeedbackAnimSprites,
    ErrorPopup_NoMana,
    ErrorPopup_Cooltime,
    ErrorPopup_CantLand,
    ErrorPopup_TacticQueueFull,
    VictoryPage,
    DefeatPage,
    TacticModeIngameUI,
    TacticModeCommandIcon1,
    TacticModeCommandIcon2,
    TacticModeCommandIcon3,
    TacticModeCommandIcon4,
    TacticModeCommandIcon5,
    TacticModeCommandIcon6,
    TacticModeCommandIcon7,
    TacticModeCommandIcon8,
    TacticModeCommandIcon9,
    TacticModeCommandIcon10,
    TacticModeCommandIcon11,
    TacticModeCommandIcon12,
};

template<>
const std::unordered_map<int, std::string>& application::POD_Enum<UIEnumID>::GetEnumNameMap()
{
    static std::unordered_map<int, std::string> enumNameMap
    {
        // 아무것도 아님.
        {static_cast<int>((int)UIEnumID::None), "None"},
        // {static_cast<int>((int)UIEnumID::로빈), "로빈"}, 우르술라, 헨젤의 초상화, 초상화 위의 이름 태그, 초상화를 가리는 적색 부상 오버레이, 체력바, 체력을 표시하는 텍스트 UI
        {static_cast<int>((int)UIEnumID::CharInfo_Robin), "CharInfo_Robin"},
        {static_cast<int>((int)UIEnumID::CharInfo_Ursula), "CharInfo_Ursula"},
        {static_cast<int>((int)UIEnumID::CharInfo_Hansel), "CharInfo_Hansel"},
        {static_cast<int>((int)UIEnumID::CharInfo_Robin_Left), "CharInfo_Robin_Left"},
        {static_cast<int>((int)UIEnumID::CharInfo_Ursula_Left), "CharInfo_Ursula_Left"},
        {static_cast<int>((int)UIEnumID::CharInfo_Hansel_Left), "CharInfo_Hansel_Left"},
        {static_cast<int>((int)UIEnumID::CharInfo_Portrait), "CharInfo_Portrait"},
        {static_cast<int>((int)UIEnumID::CharInfo_PortraitBloodOverlay), "CharInfo_PortraitBloodOverlay"},
        {static_cast<int>((int)UIEnumID::CharInfo_NameTag), "CharInfo_NameTag"},
        {static_cast<int>((int)UIEnumID::CharInfo_HP_Fill), "CharInfo_HP_Fill"},
        {static_cast<int>((int)UIEnumID::CharInfo_HP_Number_Current), "CharInfo_HP_Number_Current"},
        {static_cast<int>((int)UIEnumID::CharInfo_HP_Number_Max), "CharInfo_HP_Number_Max"},
        {static_cast<int>((int)UIEnumID::CharInfo_Buff_Bleeding), "CharInfo_Buff_Bleeding"},
        {static_cast<int>((int)UIEnumID::CharInfo_Buff_Blinding), "CharInfo_Buff_Blinding"},
        {static_cast<int>((int)UIEnumID::CharInfo_Buff_Paralysis), "CharInfo_Buff_Paralysis"},
        {static_cast<int>((int)UIEnumID::CharInfo_Buff_KnockBack), "CharInfo_Buff_KnockBack"},
        {static_cast<int>((int)UIEnumID::CharInfo_Buff_Taunted), "CharInfo_Buff_Taunted"},
        {static_cast<int>((int)UIEnumID::CharInfo_Buff_UrsulaSelf), "CharInfo_Buff_UrsulaSelf"},
        {static_cast<int>((int)UIEnumID::CharInfo_Buff_HanselBuff), "CharInfo_Buff_HanselBuff"},
        {static_cast<int>((int)UIEnumID::CharInfo_Buff_HanselDebuff), "CharInfo_Buff_HanselDebuff"},
        {static_cast<int>((int)UIEnumID::CharInfo_Skill_Use_Q), "CharInfo_Skill_Use_Q"},
        {static_cast<int>((int)UIEnumID::CharInfo_Skill_Use_Q_Overlay), "CharInfo_Skill_Use_Q_Overlay"},
        {static_cast<int>((int)UIEnumID::CharInfo_Skill_Use_Q_Cooltime_Number), "CharInfo_Skill_Use_Q_Cooltime_Number"},
        {static_cast<int>((int)UIEnumID::CharInfo_Skill_Use_W), "CharInfo_Skill_Use_W"},
        {static_cast<int>((int)UIEnumID::CharInfo_Skill_Use_W_Overlay), "CharInfo_Skill_Use_W_Overlay"},
        {static_cast<int>((int)UIEnumID::CharInfo_Skill_Use_W_Cooltime_Number), "CharInfo_Skill_Use_W_Cooltime_Number"},
        // 전술모드 토글버튼
        {static_cast<int>((int)UIEnumID::Toggle_TacticMode), "Toggle_TacticMode"},
        // 전술모드 토글버튼 흑백 오버레이
        {static_cast<int>((int)UIEnumID::Toggle_TacticMode_Overlay), "Toggle_TacticMode_Overlay"},
        {static_cast<int>((int)UIEnumID::Toggle_TacticMode_Cooltime_Number), "Toggle_TacticMode_Cooltime_Number"},
        // 마나 게이지
        {static_cast<int>((int)UIEnumID::ManaBar1), "ManaBar1"},
        // 전술 모드 마나 게이지
        {static_cast<int>((int)UIEnumID::ManaBar2), "ManaBar2"},
        // 현재 마나량을 나타내는 텍스트
        {static_cast<int>((int)UIEnumID::Mana_Text_CurrentMP), "Mana_Text_CurrentMP"},
        {static_cast<int>((int)UIEnumID::Mana_Text_CurrentMP_Tactic), "Mana_Text_Tactic"},
        // 최대 마나량을 나타내는 텍스트
        {static_cast<int>((int)UIEnumID::Mana_Text_MaxMP), "Mana_Text_MaxMP"},
        {static_cast<int>((int)UIEnumID::Mana_Text_MaxMP_Tactic), "Mana_Text_MaxMP_Tactic"},
        // 사운드가 켜진 상태에서 표시할 UI버튼
        {static_cast<int>((int)UIEnumID::Sound_On), "Sound_On"},
        // 사운드가 꺼진 상태에서 표시할 UI버튼
        {static_cast<int>((int)UIEnumID::Sound_Off), "Sound_Off"},
        // 음악이 켜진 상태에서 표시할 UI버튼
        {static_cast<int>((int)UIEnumID::Music_On), "Music_On"},
        // 음악이 꺼진 상태에서 표시할 UI버튼
        {static_cast<int>((int)UIEnumID::Music_Off), "Music_Off"},
        // 인게임에서 사용되는 캐릭터 {static_cast<int>((int)UIEnumID::상태창), "상태창"}, 전술모드 진입 버튼을 포함하는 하단 레이아웃
        {static_cast<int>((int)UIEnumID::Ingame_Bottom_Layout), "Ingame_Bottom_Layout"},
        // 인게임에서 사용되는 메뉴버튼
        {static_cast<int>((int)UIEnumID::Ingame_MenuButton), "Ingame_MenuButton"},
        // 콤보 횟수를 표시함
        {static_cast<int>((int)UIEnumID::Ingame_Combo_Number), "Ingame_Combo_Number"},
        // "Combo"라는 문자를 표시함
        {static_cast<int>((int)UIEnumID::Ingame_Combo_Text), "Ingame_Combo_Text"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_DescriptionTitleImg), "Ingame_Combo_DescriptionTitleImg"},
        // 어떤 콤보를 달성해야 하는지 설명하는 {static_cast<int>((int)UIEnumID::텍스트), "텍스트"}, 몇 콤보를 달성해야 하는지에 대한 표시, 콤보 달성을 표시하는 체크박스의 V자 문양
        {static_cast<int>((int)UIEnumID::Ingame_Combo_Description1), "Ingame_Combo_Description1"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_DescriptionImageUnfinished1), "Ingame_Combo_DescriptionImageUnfinished1"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_DescriptionImageFinished1), "Ingame_Combo_DescriptionImageFinished1"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_TargetNumUnfinished1), "Ingame_Combo_TargetNumUnfinished1"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_TargetNumFinished1), "Ingame_Combo_TargetNumFinished1"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_Check1), "Ingame_Combo_Check1"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_Description2), "Ingame_Combo_Description2"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_DescriptionImageUnFinished2), "Ingame_Combo_DescriptionImageUnFinished2"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_DescriptionImageFinished2), "Ingame_Combo_DescriptionImageFinished2"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_TargetNumUnfinished2), "Ingame_Combo_TargetNumUnfinished2"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_TargetNumFinished2), "Ingame_Combo_TargetNumFinished2"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_Check2), "Ingame_Combo_Check2"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_Description3), "Ingame_Combo_Description3"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_DescriptionImageUnFinished3), "Ingame_Combo_DescriptionImageUnFinished3"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_DescriptionImageFinished3), "Ingame_Combo_DescriptionImageFinished3"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_TargetNumUnfinished3), "Ingame_Combo_TargetNumUnfinished3"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_TargetNumFinished3), "Ingame_Combo_TargetNumFinished3"},
        {static_cast<int>((int)UIEnumID::Ingame_Combo_Check3), "Ingame_Combo_Check3"},
        {static_cast<int>((int)UIEnumID::Ingame_Vinetting), "Ingame_Vinetting"},
        {static_cast<int>((int)UIEnumID::BlackMask_RightToLeft), "BlackMask_RightToLeft"},
        {static_cast<int>((int)UIEnumID::BlackMask_TopToBottom), "BlackMask_TopToBottom"},
        {static_cast<int>((int)UIEnumID::BlackMask_LeftToRight), "BlackMask_LeftToRight"},
        {static_cast<int>((int)UIEnumID::BlackMask_BottomToTop), "BlackMask_BottomToTop"},
        // 게임을 시작할 {static_cast<int>((int)UIEnumID::때), "때"}, 메인화면으로 돌아갈 때에 사용되는 블랙마스크, 단순 연출용이 아니라 실제 게임로드, 종료를 위한 기능이 있다.
        {static_cast<int>((int)UIEnumID::BlackMask_GameLoad), "BlackMask_GameLoad"},
        {static_cast<int>((int)UIEnumID::BlackMask_GameEnd), "BlackMask_GameEnd"},
        {static_cast<int>((int)UIEnumID::LetterBox_Top), "LetterBox_Top"},
        {static_cast<int>((int)UIEnumID::LetterBox_Bottom), "LetterBox_Bottom"},
        // 인게임 하단에서 클릭할 시 스킬트리 메뉴를 여는 메뉴버튼
        {static_cast<int>((int)UIEnumID::InGame_SkiltreeMenu_Active), "InGame_SkiltreeMenu_Active"},
        {static_cast<int>((int)UIEnumID::InGame_SkiltreeMenu_InActive), "InGame_SkiltreeMenu_InActive"},
        {static_cast<int>((int)UIEnumID::SkillPoint_Number), "SkillPoint_Number"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonRobin00), "SkillUpgradeButtonRobin00"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonRobin11), "SkillUpgradeButtonRobin11"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonRobin12), "SkillUpgradeButtonRobin12"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonRobin21), "SkillUpgradeButtonRobin21"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonRobin22), "SkillUpgradeButtonRobin22"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonUrsula00), "SkillUpgradeButtonUrsula00"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonUrsula11), "SkillUpgradeButtonUrsula11"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonUrsula12), "SkillUpgradeButtonUrsula12"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonUrsula21), "SkillUpgradeButtonUrsula21"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonUrsula22), "SkillUpgradeButtonUrsula22"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonHansel00), "SkillUpgradeButtonHansel00"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonHansel11), "SkillUpgradeButtonHansel11"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonHansel12), "SkillUpgradeButtonHansel12"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonHansel21), "SkillUpgradeButtonHansel21"},
        {static_cast<int>((int)UIEnumID::SkillUpgradeButtonHansel22), "SkillUpgradeButtonHansel22"},
        {static_cast<int>((int)UIEnumID::PopUpMessage_NotEnoughSP), "PopUpMessage_NotEnoughSP"},
        {static_cast<int>((int)UIEnumID::PopUpMessage_RequirementNotMet), "PopUpMessage_RequirementNotMet"},
        {static_cast<int>((int)UIEnumID::PopUpMessage_PermissionForUpgrade), "PopUpMessage_PermissionForUpgrade"},
        {static_cast<int>((int)UIEnumID::PopUpMessage_PermissionForUpgradeProceedButton), "PopUpMessage_PermissionForUpgradeProceedButton"},
        {static_cast<int>((int)UIEnumID::PopUpMessage_WarningForRestart), "PopUpMessage_WarningForRestart"},
        {static_cast<int>((int)UIEnumID::PopUpMessage_WarningForRestart_ProceedButton), "PopUpMessage_WarningForRestart_ProceedButton"},
        {static_cast<int>((int)UIEnumID::StatusBar_Elite), "StatusBar_Elite"},
        {static_cast<int>((int)UIEnumID::StatusBar_MeleeEnemy), "StatusBar_MeleeEnemy"},
        {static_cast<int>((int)UIEnumID::StatusBar_Hero_Robin), "StatusBar_Hero_Robin"},
        {static_cast<int>((int)UIEnumID::StatusBar_Hero_Ursula), "StatusBar_Hero_Ursula"},
        {static_cast<int>((int)UIEnumID::StatusBar_Hero_Hansel), "StatusBar_Hero_Hansel"},
        {static_cast<int>((int)UIEnumID::StatusBar_HP_Number_Current), "StatusBar_HP_Number_Current"},
        {static_cast<int>((int)UIEnumID::StatusBar_HP_Number_Max), "StatusBar_HP_Number_Max"},
        {static_cast<int>((int)UIEnumID::StatusBar_HP_Cells), "StatusBar_HP_Cells"},
        {static_cast<int>((int)UIEnumID::StatusBar_HP_Fill), "StatusBar_HP_Fill"},
        {static_cast<int>((int)UIEnumID::StatusBar_SelectionName), "StatusBar_SelectionName"},
        {static_cast<int>((int)UIEnumID::TitleRoot), "TitleRoot"},
        {static_cast<int>((int)UIEnumID::Quit_Proceed), "Quit_Proceed"},
        {static_cast<int>((int)UIEnumID::MouseCursor), "MouseCursor"},
        {static_cast<int>((int)UIEnumID::MouseCursor_Free), "MouseCursor_Free"},
        {static_cast<int>((int)UIEnumID::MouseCursor_OnButton), "MouseCursor_OnButton"},
        {static_cast<int>((int)UIEnumID::MouseCursor_OnEnemy), "MouseCursor_OnEnemy"},
        {static_cast<int>((int)UIEnumID::MouseCursor_OnAlly), "MouseCursor_OnAlly"},
        {static_cast<int>((int)UIEnumID::MoveTargetFeedbackAnimSprites), "MoveTargetFeedbackAnimSprites"},
        {static_cast<int>((int)UIEnumID::ErrorPopup_NoMana), "ErrorPopup_NoMana"},
        {static_cast<int>((int)UIEnumID::ErrorPopup_Cooltime), "ErrorPopup_Cooltime"},
        {static_cast<int>((int)UIEnumID::ErrorPopup_CantLand), "ErrorPopup_CantLand"},
        {static_cast<int>((int)UIEnumID::ErrorPopup_TacticQueueFull), "ErrorPopup_TacticQueueFull"},
        {static_cast<int>((int)UIEnumID::VictoryPage), "VictoryPage"},
        {static_cast<int>((int)UIEnumID::DefeatPage), "DefeatPage"},
        {static_cast<int>((int)UIEnumID::TacticModeIngameUI), "TacticModeIngameUI"},
        {static_cast<int>((int)UIEnumID::TacticModeCommandIcon1), "TacticModeCommandIcon1"},
        {static_cast<int>((int)UIEnumID::TacticModeCommandIcon2), "TacticModeCommandIcon2"},
        {static_cast<int>((int)UIEnumID::TacticModeCommandIcon3), "TacticModeCommandIcon3"},
        {static_cast<int>((int)UIEnumID::TacticModeCommandIcon4), "TacticModeCommandIcon4"},
        {static_cast<int>((int)UIEnumID::TacticModeCommandIcon5), "TacticModeCommandIcon5"},
        {static_cast<int>((int)UIEnumID::TacticModeCommandIcon6), "TacticModeCommandIcon6"},
        {static_cast<int>((int)UIEnumID::TacticModeCommandIcon7), "TacticModeCommandIcon7"},
        {static_cast<int>((int)UIEnumID::TacticModeCommandIcon8), "TacticModeCommandIcon8"},
        {static_cast<int>((int)UIEnumID::TacticModeCommandIcon9), "TacticModeCommandIcon9"},
        {static_cast<int>((int)UIEnumID::TacticModeCommandIcon10), "TacticModeCommandIcon10"},
        {static_cast<int>((int)UIEnumID::TacticModeCommandIcon11), "TacticModeCommandIcon11"},
        {static_cast<int>((int)UIEnumID::TacticModeCommandIcon12), "TacticModeCommandIcon12"},
    };
    return enumNameMap;
}
