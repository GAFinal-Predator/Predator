#include "GameManager.h"
#include "PlayerController.h"
#include "SkillPreviewSystem.h"
#include "Unit.h"
#include "RTSCam.h"
#include "ContentsLayer.h"
#include "Application.h"
#include "InputManager.h"
#include "PlayTimeWave.h"

void GameManager::Start()
{
	application::contents::ContentsLayer* contentsLayer = dynamic_cast<application::contents::ContentsLayer*>(application::Application::GetInstance().GetContentsLayer());
	contentsLayer->RegisterToEditorComponentVector(this);

	EndBattle();
}

void GameManager::Update()
{
	if (isComboTimerActivated)
	{
		m_comboElapsed += Time::GetDeltaTime();
		
		if (m_comboElapsed >= m_comboResistDuration)
		{
			isComboTimerActivated = false;
			m_comboElapsed = 0.0f;
			currentCombo = 0;
			/// member로 세팅해놨던 comboNumber UIImage Component에게 현재 콤보 수를 넘겨준다.
		}
	}
}

void GameManager::EngageBattle()
{
	isBattleModeOn = true;

	/// 플레이어 유닛들을 WaveEngage 상태로 전환 시키는 로직
	PlayerController::SingleInstance().SetCurrentPlayerSerialNumber(Unit::UnitType::Warrior);
	rtscam->SetTarget(PlayerController::SingleInstance().GetPlayerMap().find(Unit::UnitType::Warrior)->second->GetGameObject());
}

void GameManager::EndBattle()
{
	isBattleModeOn = false;

	PlayerController::SingleInstance().SetCurrentPlayerSerialNumber(Unit::UnitType::AllPlayers);
	rtscam->SetTarget(PlayerController::SingleInstance().GetPlayerMap().find(Unit::UnitType::Warrior)->second->GetGameObject());
	SkillPreviewSystem::Instance().ActivateSkillPreview(false);
}

void GameManager::EngageCinematic()
{
	InputManager::Instance().SetInputManagerActive(false);
	SkillPreviewSystem::Instance().ActivateSkillPreview(false);
}

void GameManager::EndCinematic()
{
	InputManager::Instance().SetInputManagerActive(true);
	SkillPreviewSystem::Instance().ActivateSkillPreview(false);
}

void GameManager::Reset()
{
	isBattleModeOn = false;
	PlayerController::SingleInstance().SetCurrentPlayerSerialNumber(Unit::UnitType::AllPlayers);
	rtscam->SetTarget(PlayerController::SingleInstance().GetPlayerMap().find(Unit::UnitType::Warrior)->second->GetGameObject());
}

bool GameManager::IsBattleSystemOperating() const
{
	return isBattleModeOn;
}

void GameManager::AddCombo()
{
	isComboTimerActivated = true;
	m_comboElapsed = 0.0f;
	currentCombo++;
	/// member로 세팅해놨던 comboNumber UIImage Component에게 현재 콤보 수를 넘겨준다.
}

void GameManager::ResetCombo()
{
	currentCombo = 0;
}

void GameManager::ReportWaveStartStateEnd(Unit* p_unit)
{
	m_waveEnterCheckMap.find(p_unit->GetUnitType())->second = true;

	bool tempCheck = true;

	for (auto each : m_waveEnterCheckMap)
	{
		if (each.second == false)
		{
			tempCheck = false;
			break;
		}
	}

	if (tempCheck)
	{
		/// 모든 플레이어 유닛이 wave진입 완료
		waveEngageMotionActivate = true;
		isPlayerEnteredWaveRegion = false;
		waveEngageMotionEnd = false;
		m_waveEnterCheckMap.clear();
		EngageBattle();
	}
}

void GameManager::ReportWaveMotionEnd(Unit* p_unit)
{
	m_waveEnterMotionCheckMap.insert({ Unit::UnitType::Warrior, false });
	m_waveEnterMotionCheckMap.insert({ Unit::UnitType::Magician, false });
	m_waveEnterMotionCheckMap.insert({ Unit::UnitType::Healer, false });

	m_waveEnterMotionCheckMap.find(p_unit->GetUnitType())->second = true;

	bool tempCheck = true;

	for (auto each : m_waveEnterMotionCheckMap)
	{
		if (each.second == false)
		{
			tempCheck = false;
			break;
		}
	}

	if (tempCheck)
	{
		/// 모든 플레이어 유닛이 wave Motion 완료
		waveEngageMotionEnd = true;
		m_waveEnterMotionCheckMap.clear();
	}
}

void GameManager::ReportPlayerEnteredWaveRegion(PlaytimeWave* p_wave)
{
	waveEngageMotionActivate = false;
	isPlayerEnteredWaveRegion = true;
	currentActivatingWave = p_wave;
	m_waveEnterCheckMap.insert({ Unit::UnitType::Warrior, false});
	m_waveEnterCheckMap.insert({ Unit::UnitType::Magician, false});
	m_waveEnterCheckMap.insert({ Unit::UnitType::Healer, false });

	yunutyEngine::Transform* warriorTransform = PlayerController::SingleInstance().GetPlayerMap().find(Unit::UnitType::Warrior)->second->GetTransform();
	Vector3d leftPosition = warriorTransform->GetWorldPosition() + warriorTransform->GetWorldRotation().Right() * -3.0f;
	Vector3d rightPosition = warriorTransform->GetWorldPosition() + warriorTransform->GetWorldRotation().Right() * 3.0f;

	/// 유닛들을 warrior 기준 양쪽으로 이동시키자
	PlayerController::SingleInstance().GetPlayerMap().find(Unit::UnitType::Warrior)->second->SetWaveStartPosition(warriorTransform->GetWorldPosition());
	PlayerController::SingleInstance().GetPlayerMap().find(Unit::UnitType::Magician)->second->SetWaveStartPosition(leftPosition);
	PlayerController::SingleInstance().GetPlayerMap().find(Unit::UnitType::Healer)->second->SetWaveStartPosition(rightPosition);

	PlayerController::SingleInstance().GetPlayerMap().find(Unit::UnitType::Warrior)->second->m_currentBelongingWavePosition = p_wave->GetTransform()->GetWorldPosition();
	PlayerController::SingleInstance().GetPlayerMap().find(Unit::UnitType::Magician)->second->m_currentBelongingWavePosition = p_wave->GetTransform()->GetWorldPosition();
	PlayerController::SingleInstance().GetPlayerMap().find(Unit::UnitType::Healer)->second->m_currentBelongingWavePosition = p_wave->GetTransform()->GetWorldPosition();
}

bool GameManager::IsPlayerJustEnteredWaveRegion() const
{
	return isPlayerEnteredWaveRegion;
}

bool GameManager::IsReadyToWaveEngageMotion() const
{
	return waveEngageMotionActivate;
}

bool GameManager::IsWaveEngageMotionEnd() const
{
	return waveEngageMotionEnd;
}
