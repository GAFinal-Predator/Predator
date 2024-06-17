#include "Interactable_ChessRook.h"

#include "InteractableData.h"
#include "DebugMeshes.h"
#include "Unit.h"
#include "TacticModeSystem.h"
#include "ChessBombComponent.h"
#include "YunutyWaitForSeconds.h"
#include "SFXManager.h"
#include "BossSummonChessSkill.h"
#include "PlayerController.h"

void Interactable_ChessRook::Start()
{
	auto ts = GetGameObject()->GetTransform();
	ts->SetWorldPosition(initPos);
	ts->SetWorldRotation(initRotation);
	ts->SetWorldScale(initScale);

	auto rendererObj = GetGameObject()->AddGameObject();
	AttachDebugMesh(rendererObj, DebugMeshType::Cube, yunuGI::Color::green());
	rendererObj->GetTransform()->SetLocalScale(chessBlockUnitLength * Vector3d::one);
	auto boxCollider = GetGameObject()->AddComponent<physics::BoxCollider>();
	boxCollider->SetHalfExtent(chessBlockUnitLength * 0.5 * Vector3d::one);

	for (auto each : GetGameObject()->GetChildren())
	{
		auto renderer = each->GetComponent<graphics::StaticMeshRenderer>();
		if (renderer)
		{
			mesh = each;
			const yunuGI::IResourceManager* resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
			orginTexture = resourceManager->GetTexture(L"FBX/SM_Chess_Rook/SM_Chess_Rook.fbm/T_Chess_Rook_BaseColor.dds");
			flashTexture = resourceManager->GetTexture(L"Texture/Interactable/BombFlash.png");
			break;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (i == 2)
		{
			continue;
		}

		auto bombObj = GetGameObject()->AddGameObject();
		bombObj->AddComponent<ChessBombComponent>()->SetBombTime(delayTime);
		bombObj->GetTransform()->SetWorldScale(chessBlockUnitLength * Vector3d::one);
		Vector3d combVector = (i - 2) * chessBlockUnitLength * GetGameObject()->GetTransform()->GetWorldRotation().Forward().Normalized();
		bombObj->GetTransform()->SetLocalPosition(combVector + Vector3d(0, guideUp_Y, (i - 2) * chessBlockUnitOffset));
		bombObjList.push_back(bombObj);
		bombObj->SetSelfActive(false);

		auto bombObj2 = GetGameObject()->AddGameObject();
		bombObj2->AddComponent<ChessBombComponent>()->SetBombTime(delayTime);
		bombObj2->GetTransform()->SetWorldScale(chessBlockUnitLength * Vector3d::one);
		Vector3d combVector2 = (i - 2) * chessBlockUnitLength * GetGameObject()->GetTransform()->GetWorldRotation().Right().Normalized();
		bombObj2->GetTransform()->SetLocalPosition(combVector2 + Vector3d((i - 2) * chessBlockUnitOffset, guideUp_Y, 0));
		bombObjList.push_back(bombObj2);
		bombObj2->SetSelfActive(false);
	}

	auto oBomobObj = GetGameObject()->AddGameObject();
	oBomobObj->AddComponent<ChessBombComponent>()->SetBombTime(delayTime);
	oBomobObj->GetTransform()->SetWorldScale(chessBlockUnitLength * Vector3d::one);
	oBomobObj->GetTransform()->SetLocalPosition(Vector3d(0, guideUp_Y, 0));
	bombObjList.push_back(oBomobObj);
	oBomobObj->SetSelfActive(false);
}

void Interactable_ChessRook::Update()
{
	if (!unitSet.empty())
	{
		OnInteractableTriggerEnter();
	}

	if (!isPause && triggerOn)
	{
		if (!isInteracting)
		{
			lastCoroutine = StartCoroutine(DoInteraction());
			lastCoroutine.lock()->PushDestroyCallBack([this]() { GetGameObject()->SetSelfActive(false); });
			isInteracting = true;
		}
		else if (lastCoroutine.expired() || lastCoroutine.lock()->Done())
		{
			isInteracting = false;
			OnInteractableTriggerExit();
		}
	}
}

void Interactable_ChessRook::OnTriggerEnter(physics::Collider* collider)
{
	if (Unit* colliderUnitComponent = UnitCollider::AcquireUnit(collider);
		PlayerController::Instance().GetState() == PlayerController::State::Battle &&
		colliderUnitComponent != nullptr &&
		colliderUnitComponent->IsPlayerUnit() &&
		colliderUnitComponent->IsAlive())
	{
		unitSet.insert(colliderUnitComponent);
	}
}

void Interactable_ChessRook::OnTriggerExit(physics::Collider* collider)
{
	if (Unit* colliderUnitComponent = UnitCollider::AcquireUnit(collider);
		colliderUnitComponent != nullptr &&
		colliderUnitComponent->IsPlayerUnit())
	{
		unitSet.erase(colliderUnitComponent);
	}
}

yunutyEngine::coroutine::Coroutine Interactable_ChessRook::DoInteraction()
{
	float localTimer = 0;
	assert(delayTime > 0 && "delayTime must be greater than 0");
	float ratio = localTimer / delayTime;

	for (auto each : bombObjList)
	{
		each->SetSelfActive(true);
	}

	while (ratio < 1)
	{
		while (isPause)
		{
			co_await std::suspend_always();
		}

		ratio = localTimer / delayTime;

		if (ratio > 1)
		{
			ratio = 1;
		}

		if (!TacticModeSystem::Instance().IsOperation())
		{
			auto beforePos = mesh->GetTransform()->GetLocalPosition();
			if (beforePos.x >= 0)
			{
				beforePos.x = -vibeMaxOffset * ratio;
			}
			else
			{
				beforePos.x = vibeMaxOffset * ratio;
			}
			mesh->GetTransform()->SetLocalPosition(beforePos);
		}

		auto renderer = mesh->GetComponent<graphics::StaticMeshRenderer>();
		if ((int)(ratio * 10) % 2 != 0)
		{
			renderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::ALBEDO, flashTexture);
		}
		else
		{
			renderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::ALBEDO, orginTexture);
		}

		co_await std::suspend_always();
		localTimer += yunutyEngine::Time::GetDeltaTime();
	}

	mesh->SetSelfActive(false);

	std::unordered_set<Unit*> target = std::unordered_set<Unit*>();

	for (auto each : bombObjList)
	{
		auto comp = each->GetComponent<ChessBombComponent>();
		for (auto unit : comp->GetUnitsInTrigger())
		{
			target.insert(unit);
		}

		if (target.size() == 3)
		{
			break;
		}
	}

	for (auto each : target)
	{
		each->Damaged(damage);
	}

	SFXManager::PlaySoundfile3D("sounds/trap/Explosion.mp3", GetTransform()->GetWorldPosition());

	if (particleEffectTime == 0)
	{
		co_return;
	}

	co_yield yunutyEngine::coroutine::WaitForSeconds(particleEffectTime, false);
}

void Interactable_ChessRook::SetDataFromEditorData(const application::editor::InteractableData& data)
{
	initPos.x = data.pod.position.x;
	initPos.y = data.pod.position.y;
	initPos.z = data.pod.position.z;
	initRotation.w = data.pod.rotation.w;
	initRotation.x = data.pod.rotation.x;
	initRotation.y = data.pod.rotation.y;
	initRotation.z = data.pod.rotation.z;
	initScale.x = data.pod.scale.x;
	initScale.y = data.pod.scale.y;
	initScale.z = data.pod.scale.z;
	chessBlockUnitLength = application::GlobalConstant::GetSingletonInstance().pod.chessBlockUnitLength;
	chessBlockUnitOffset = application::GlobalConstant::GetSingletonInstance().pod.chessBlockUnitOffset;
	vibeMaxOffset = application::GlobalConstant::GetSingletonInstance().pod.vibeMaxOffset;
	damage = data.pod.templateData->pod.damage;
	delayTime = data.pod.templateData->pod.delayTime;
	particleEffectTime = data.pod.templateData->pod.particleEffectTime;
}

void Interactable_ChessRook::OnPause()
{
	isPause = true;

	for (auto each : bombObjList)
	{
		each->GetComponent<ChessBombComponent>()->OnPause();
	}
}

void Interactable_ChessRook::OnResume()
{
	isPause = false;

	for (auto each : bombObjList)
	{
		each->GetComponent<ChessBombComponent>()->OnResume();
	}
}
