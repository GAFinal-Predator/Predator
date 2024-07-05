#pragma once
#include "YunutyEngine.h"
#include "nlohmann/json.hpp"
#include "Application.h"
#include "ContentsLayer.h"
#include <d3d11.h>
#include "TemplateDataManager.h"
#include "DelegateCallback.h"
#include "SkillPreviewSystem.h"
#include "CinematicManager.h"
#include "RTSCam.h"

// UI
#include "UIElement.h"
#include "UIManager.h"
#include "UIEnumID.h"
#include "UIExportFlag.h"
#include "PopupOnEnable.h"
#include "PopDownOnDisable.h"
#include "UIOffsetTransition.h"
#include "RotatingUI.h"
#include "UIVideoPlayer.h"
#include "UIButton.h"
#include "UIText.h"
#include "UIImage.h"
#include "UIPriorityLayout.h"
#include "SoundPlayingTimer.h"
#include "JsonUIData.h"
#include "FloatFollower.h"
#include "PulsingUI.h"
#include "TimePauseTimer.h"
#include "LinearClippingTimer.h"
#include "ColorTintTimer.h"
#include "PlayMusicTimer.h"
#include "UISpriteAnimation.h"
#include "PlayerPortrait.h"

#include "SkillSystem.h"
#include "GlobalConstant.h"
#include "StatusTimer.h"
#include "YunutyCoroutine.h"
#include "YunutyYieldInstruction.h"
#include "YunutyWaitForSeconds.h"
#include "IInteractableComponent.h"
#include "SkillList.h"
#include "UnitStatusBarType.h"
#include "DamageType.h"
#include "PlayerCharacterType.h"
#include "BurnEffect.h"

#include "InstanceManager.h"
#include "EditorLayer.h"
#include "EditorPopupManager.h"
#include "UnitData.h"
#include "Unit.h"
#include "UnitCollider.h"
#include "UnitCapsuleCollider.h"
#include "UnitPool.h"
#include "UnitBuff.h"
#include "UnitBuffInvulenerability.h"
#include "UnitBuffBleeding.h"
#include "UnitBuffUrsula.h"
#include "UnitBuffHanselBuff.h"
#include "UnitBuffHanselDebuff.h"
#include "UnitBuffBlinded.h"
#include "UnitBuffTaunted.h"
#include "UnitAnimationType.h"
#include "UnitAttackType.h"
#include "UnitAcquisitionSphereCollider.h"
#include "UnitAcquisitionSphereColliderPool.h"
#include "UnitBuff.h"
#include "Projectile.h"
#include "CurveProjectile.h"
#include "CurveHomingProjectile.h"
#include "DirectProjectile.h"
#include "DirectHomingProjectile.h"
#include "ProjectileType.h"
#include "ProjectilePool.h"
#include "ProjectileSelector.h"
#include "FBXPool.h"
#include "PlayerController.h"
#include "EnemyController.h"
#include "EnemyAggroController.h"
#include "RangedEliteController.h"
#include "MeleeEliteController.h"
#include "RangedKitingController.h"
#include "RangedApproachingController.h"
#include "HolderController.h"
#include "BossController.h"
#include "PlaytimeWave.h"
#include "SingleNavigationField.h"
#include "DebugBeacon.h"
#include "WanderUtils.h"
#include "WanderYieldInstrcution.h"
#include "ParticleToolData.h"
#include "ParticleTool_Manager.h"
#include "wanderResources.h"
#include "WanderFBX.h"
#include "SFXManager.h"
#include "ContentsCoroutine.h"
#include "TacticModeSystem.h"
