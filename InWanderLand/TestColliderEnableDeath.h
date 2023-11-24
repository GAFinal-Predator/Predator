#pragma once
#include "RTSCam.h"
#include "DelayedTestFunctions.h"
#include "YunutyEngine.h"
#include "ShakyCam.h"
#include "DebugTilePlane.h"
#include "DebugBeacon.h"
#include "DebugMeshes.h"
#include "YunutyEngine.h"

class TriggerVolumeTest : public Component
{
// PhysX Visual Debugger��� ���α׷��� �����ϰ� �׽�Ʈ �ڵ带 �����ϸ� � ������ �Ͼ���� ���� �ڼ��� ������ �� �ֽ��ϴ�.
    virtual void OnTriggerEnter(physics::Collider* other) override
    {
        DebugBeacon::PlaceBeacon(other->GetTransform()->GetWorldPosition(), yunuGI::Color::blue(), {3,3,3});
    };
    virtual void OnTriggerExit(physics::Collider* other) override
    {
        DebugBeacon::PlaceBeacon(other->GetTransform()->GetWorldPosition(), yunuGI::Color::blue(), {3,3,3});
    };
};
// PhysX Visual Debugger��� ���α׷��� �����ϰ� �׽�Ʈ �ڵ带 �����ϸ� � ������ �Ͼ���� ���� �ڼ��� ������ �� �ֽ��ϴ�.
void TestColliderEnableDeath()
{
    auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
    auto rtsCam = camObj->AddComponent<RTSCam>();
    rtsCam->GetTransform()->position = Vector3d(0, 10, 0);

    auto boxCollider = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
    auto boxColliderComp = boxCollider->AddComponent<yunutyEngine::physics::BoxCollider>();
    auto rigidComp = boxCollider->AddComponent<physics::RigidBody>();
    //rigidComp->SetAsStatic(true);
    boxCollider->GetTransform()->position = Vector3d(0, 10, 0);
    boxCollider->GetTransform()->rotation = Vector3d(100, 10, 35);
    auto boxMesh = boxCollider->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
    boxMesh->GetGI().LoadMesh("Cube");
    boxMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color::black());
    {
        auto boxCollider = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
        auto boxColliderComp = boxCollider->AddComponent<yunutyEngine::physics::BoxCollider>();
        auto rigidComp = boxCollider->AddComponent<physics::RigidBody>();
        //rigidComp->SetAsStatic(true);
        boxCollider->GetTransform()->position = Vector3d(0, 13, 0);
        boxCollider->GetTransform()->rotation = Vector3d(100, 10, 35);
        auto boxMesh = boxCollider->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
        boxCollider->AddComponent<TriggerVolumeTest>();
        boxMesh->GetGI().LoadMesh("Cube");
        boxMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color::black());
    }

    auto planeCollider = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
    auto planeColliderComp = planeCollider->AddComponent<yunutyEngine::physics::BoxCollider>();
    planeColliderComp->SetHalfExtent({ 10,1,10 });
    auto planeRigidComp = planeCollider->AddComponent<physics::RigidBody>();
    planeRigidComp->SetAsStatic(true);
    planeCollider->GetTransform()->position = Vector3d(0, 0, 0);
    auto planeMesh = planeCollider->AddGameObject();
    auto planeMeshComp = planeMesh->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
    planeMeshComp->GetGI().LoadMesh("Cube");
    planeMeshComp->GetGI().GetMaterial()->SetColor(yunuGI::Color::white());
    planeMesh->GetTransform()->scale = Vector3d(10, 1, 10);

    // Ʈ���� �ݶ��̴� ����
    {
        auto planeCollider = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
        auto planeColliderComp = planeCollider->AddComponent<yunutyEngine::physics::BoxCollider>();
        planeColliderComp->SetHalfExtent({ 10,1,10 });
        planeCollider->GetTransform()->position = Vector3d(0, 3, 0);
        auto planeMesh = planeCollider->AddGameObject();
        auto planeMeshComp = planeMesh->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
        planeMeshComp->GetGI().LoadMesh("Cube");
        planeMesh->GetTransform()->scale = Vector3d(10, 1, 10);
    }

    auto delayedFunctions = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<DelayedTestFunctions>();

    // ����� �����Ǵ��� �׽�Ʈ
    delayedFunctions->todoList.push_back({ 2,[=]() {Scene::getCurrentScene()->DestroyGameObject(boxCollider); } });
    // ����� ��Ȱ��ȭ�Ǵ��� �׽�Ʈ
    delayedFunctions->todoList.push_back({ 1,[=]() {boxColliderComp->SetActive(false); } });
    // ����� ��Ȱ��ȭ�Ǵ��� �׽�Ʈ
    delayedFunctions->todoList.push_back({ 3,[=]() {boxColliderComp->SetActive(true); } });
}
