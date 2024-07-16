#include "PlaytimeRegion.h"
#include "Unit.h"
#include "RegionData.h"
#include "SpecialEvent.h"
#include "PlayerController.h"
#include "SingleNavigationField.h"
#include "PlayTimeRegionManager.h"
#include "Application.h"
#include "ContentsLayer.h"

std::weak_ptr<PlaytimeRegion> PlaytimeRegion::playerConstrainingRegion;
PlaytimeRegion::~PlaytimeRegion()
{
    if (regionData->playtimeRegion == this)
    {
        regionData->playtimeRegion = nullptr;
    }
    if (playerConstrainingRegion.lock().get() == this)
    {
        playerConstrainingRegion.reset();
    }
    for (auto each : regionData->GetDisablingOrnaments())
    {
        if (auto instance = each->GetPaletteInstance())
        {
            instance->GetGameObject()->SetSelfActive(true);
        }
    }
}

void PlaytimeRegion::Start()
{
    //application::contents::ContentsLayer* contentsLayer = dynamic_cast<application::contents::ContentsLayer*>(application::Application::GetInstance().GetContentsLayer());
    //contentsLayer->RegisterToEditorComponentVector(this);
}

bool PlaytimeRegion::IsUnitInside(physics::Collider* unitCollider)
{
    return enteredPlayerColliders.contains(unitCollider);
}

//void PlaytimeRegion::SetRegionName(std::wstring p_name)
//{
//	//m_regionName = p_name;
//}

void PlaytimeRegion::OnTriggerEnter(physics::Collider* collider)
{
    // 조건 더 다듬을 것
    if (Unit* colliderUnitComponent = UnitCollider::AcquireUnit(collider);
        colliderUnitComponent != nullptr && colliderUnitComponent->IsPlayerUnit())
    {
        enteredPlayerColliders.insert(collider);
        if (!isOnceActivatedEnter)
        {
            for (auto& each : OnEnter)
            {
                each();
            }
            //switch (static_cast<SpecialEventType>(regionData->pod.specialEvent))
            //{
            //case SpecialEventType::Stage1To2Transition:
            //    PlayTimeRegionManager::Instance().stage1ToStage2Function();
            //    break;
            //}
        }
        // 가려야 하는 장식물들을 가리는 부분
        if (disablingReferences.empty())
        {
            for (auto each : regionData->GetDisablingOrnaments())
            {
                std::transform(regionData->GetDisablingOrnaments().begin(), regionData->GetDisablingOrnaments().end(),
                    std::back_inserter(disablingReferences), [each](auto each) { return each->AcquireDisablingReference(); });
            }
        }

        isOnceActivatedEnter = true;
    }
}
void PlaytimeRegion::OnTriggerExit(physics::Collider* collider)
{
    // 조건 더 다듬을 것
    if (Unit* colliderUnitComponent = UnitCollider::AcquireUnit(collider);
        colliderUnitComponent != nullptr && colliderUnitComponent->IsPlayerUnit())
    {
        enteredPlayerColliders.erase(collider);
        if (enteredPlayerColliders.empty())
        {
            if (!isOnceActivatedLeave)
            {
                for (auto& each : OnLeave)
                {
                    each();
                }
                isOnceActivatedLeave = true;
            }
            disablingReferences.clear();
        }
    }
}

void PlaytimeRegion::ApplyAsObstacle(bool asObstacle)
{
    if (asObstacle)
    {
        if (regionAsNavObstacle == nullptr)
        {
            regionAsNavObstacle = GetGameObject()->AddComponent<NavigationObstacle>();
            regionAsNavObstacle->AssignToNavigationField(&SingleNavigationField::Instance());
            regionAsNavObstacle->SetHalfExtents(Vector3d{ regionData->pod.width, 10, regionData->pod.height });
        }
        regionAsNavObstacle->SetActive(true);
    }
    else
    {
        if (regionAsNavObstacle)
        {
            regionAsNavObstacle->SetActive(false);
        }
    }
}

std::weak_ptr<PlaytimeRegion> PlaytimeRegion::GetPlayerConstrainingRegion()
{
    return playerConstrainingRegion;
}
