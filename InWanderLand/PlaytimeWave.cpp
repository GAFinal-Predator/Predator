#include "PlaytimeWave.h"
#include "UnitData.h"
#include "WaveData.h"
#include "UnitProductor.h"
#include "Unit.h"
#include "MeleeEnemyProductor.h"
#include "RangedEnemyProductor.h"
#include "Application.h"
#include "ContentsLayer.h"
#include "MeleeEnemyPool.h"
#include "RangedEnemyPool.h"
#include "ShortcutSystem.h"

PlaytimeWave::~PlaytimeWave()
{
	waveData->playtimeWave = nullptr;
}

void PlaytimeWave::ActivateWave()
{
	isWaveActivated = true;
}
void PlaytimeWave::DeActivateWave()
{
	waveDataIndex = 0;
	this->SetActive(false);
	// 여기엔 wave 종료 시 카메라 락 해제 등의 로직이 들어가야 한다.
}

void PlaytimeWave::Start()
{
	//ActivateWave();
	productorSelector.push_back(&MeleeEnemyProductor::Instance());
	productorSelector.push_back(&RangedEnemyProductor::Instance());
}

void PlaytimeWave::Update()
{
	if (isWaveActivated)
	{
		m_elapsed += Time::GetDeltaTime();
	}
	auto& waveDelays = waveData->pod.waveDelays;
	auto waveSize = waveData->pod.waveSizes[currentSequenceIndex];
	auto sequenceSize = waveData->pod.waveSizes.size();
	// waveDelays는 delay가 오름차순으로 정렬되어 있다.
	// 현재 웨이브에서 아직 소환할 유닛이 더 남았는가?
	if (nextSummonUnitIndex < waveSize)
	{
		// 유닛 소환이 임박했는가?
		while (nextSummonUnitIndex < waveSize && waveDelays[waveDataIndex] < m_elapsed)
		{
			// 유닛을 소환하고 인덱스를 증가시킨다.
			// 유닛 데이터는 아래 값을 사용하면 됨.
			Vector3d pos = { waveData->waveUnitDatasVector[waveDataIndex]->pod.position.x,
				waveData->waveUnitDatasVector[waveDataIndex]->pod.position.y,
				waveData->waveUnitDatasVector[waveDataIndex]->pod.position.z };

			UnitProductor* currentSelectedProductor;
			Unit* unitComponent{ nullptr };

			if (waveData->waveUnitDatasVector[waveDataIndex]->pod.templateData->pod.skinnedFBXName == "SKM_Monster1")
			{
				currentSelectedProductor = &MeleeEnemyProductor::Instance();
				currentSelectedProductor->MappingUnitData(waveData->waveUnitDatasVector[waveDataIndex]->pod.templateData->pod);
				MeleeEnemyPool::SingleInstance().SetStartPosition(pos);
				unitComponent = MeleeEnemyPool::SingleInstance().Borrow()->m_pairUnit;
				unitComponent->GetTransform()->SetWorldPosition(pos);
				unitComponent->GetGameObject()->SetSelfActive(true);
				application::ShortcutSystem::Instance().RegisterObject(2, unitComponent->GetGameObject());

			}
			else if (waveData->waveUnitDatasVector[waveDataIndex]->pod.templateData->pod.skinnedFBXName == "SKM_Monster2")
			{
				currentSelectedProductor = &RangedEnemyProductor::Instance();
				currentSelectedProductor->MappingUnitData(waveData->waveUnitDatasVector[waveDataIndex]->pod.templateData->pod);
				RangedEnemyPool::SingleInstance().SetStartPosition(pos);
				unitComponent = RangedEnemyPool::SingleInstance().Borrow()->m_pairUnit;
				unitComponent->GetTransform()->SetWorldPosition(pos);
				unitComponent->GetGameObject()->SetSelfActive(true);
				application::ShortcutSystem::Instance().RegisterObject(2, unitComponent->GetGameObject());
			}

			//for (auto& e : productorSelector)
			//{
			//	if (e->SelectUnitProductorByFbxName(waveData->waveUnitDatasVector[waveDataIndex]->pod.templateData->pod.skinnedFBXName))
			//	{
			//		currentSelectedProductor = e;
			//		break;
			//	}
			//}

			GameObject* unitObject = unitComponent->GetGameObject();

			m_currentWaveUnitVector.push_back(unitComponent);
			
			nextSummonUnitIndex++;
			waveDataIndex++;
		}
	}
	// 현재 웨이브에서 소환 대상이 되는 유닛들이 다 소환된 경우
	else
	{
		bool isAllUnitTerminated = true;

		for (auto& e : m_currentWaveUnitVector)
		{
			// 한 유닛이라도 살아 있다면 bool값을 false로
			if (!(e->IsUnitDead()))
			{
				isAllUnitTerminated = false;
				break;
			}
		}

		// 현재 등장한 유닛들이 다 죽었는가?
		if (isAllUnitTerminated)
		{
			m_currentWaveUnitVector.clear();
			m_elapsed = 0.0f;
			currentSequenceIndex++;
			nextSummonUnitIndex = 0;
			// 웨이브에서 더 진행할 시퀀스가 없다면 웨이브를 중지하라.
			if (currentSequenceIndex >= sequenceSize)
			{
				DeActivateWave();
			}

		}
	}
}
