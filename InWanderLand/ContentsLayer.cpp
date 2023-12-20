#include "ContentsLayer.h"

#include "YunutyEngine.h"
#include "ShakyCam.h"
#include "RTSCam.h"   
#include "DebugTilePlane.h"
#include "DebugBeacon.h"
#include "DebugMeshes.h"
#include "Application.h"
#ifdef GEN_TESTS
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

#include <d3d11.h>

std::function<void()> Application::Contents::ContentsLayer::testInitializer;

/// �׷��Ƚ� �׽�Ʈ��
void GraphicsTest()
{
    const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();

    //����Ʈ ����Ʈ
    {
        auto object = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
        object->GetTransform()->position = Vector3d{ 0,3,0 };
        auto light = object->AddComponent<yunutyEngine::graphics::PointLight>();
        light->GetGI().SetRange(4);
        yunuGI::Color color{1, 0, 0, 1};
        light->GetGI().SetLightDiffuseColor(color);
    }

}


//void CreateNavPlane(Vector3f botleft, Vector3f topright, std::vector<Vector3f>& worldVertices, std::vector<int>& worldFaces)
//{
//    int startingIdx = worldVertices.size();
//    worldVertices.push_back({ botleft.x,0,topright.z });
//    worldVertices.push_back({ botleft.x,0,botleft.z });
//    worldVertices.push_back({ topright.x,0,botleft.z });
//    worldVertices.push_back({ topright.x,0,topright.z });
//
//    worldFaces.push_back(startingIdx + 2);
//    worldFaces.push_back(startingIdx + 1);
//    worldFaces.push_back(startingIdx + 0);
//    worldFaces.push_back(startingIdx + 3);
//    worldFaces.push_back(startingIdx + 2);
//    worldFaces.push_back(startingIdx + 0);
//
//    auto tilePlane = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<DebugTilePlane>();
//    auto size = topright - botleft;
//    tilePlane->GetTransform()->SetWorldPosition((botleft + topright) / 2.0);
//    tilePlane->width = size.x;
//    tilePlane->height = size.z;
//    tilePlane->SetTiles();
//}
//
//NavigationAgent* CreateAgent(NavigationField* navField)
//{
//    auto agent = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::NavigationAgent>();
//    agent->SetSpeed(2);
//    agent->SetRadius(0.5);
//    agent->AssignToNavigationField(navField);
//    auto staticMesh = agent->GetGameObject()->AddGameObject()->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
//
//    const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
//    auto& meshList = _resourceManager->GetMeshList();
//    for (auto& e : meshList)
//    {
//        if (e->GetName() == L"Capsule")
//        {
//            staticMesh->GetGI().SetMesh(e);
//        }
//    }
//
//    staticMesh->GetGI().GetMaterial()->SetColor({ 0.75,0.75,0.75,0 });
//    staticMesh->GetTransform()->position = Vector3d{ 0,0.5,0 };
//    return agent;
//}
void Application::Contents::ContentsLayer::Initialize()
{
    if (ContentsLayer::testInitializer)
    {
        ContentsLayer::testInitializer();
        return;
    }
    yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());
    yunutyEngine::Collider2D::SetIsOnXYPlane(false);

    //auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
    //camObj->GetTransform()->position = Vector3d(0, 0, -5);
    //auto roamingCam = camObj->AddComponent<RoamingCam>();

    GraphicsTest();

    {
        auto directionalLight = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
        directionalLight->AddComponent<yunutyEngine::graphics::DirectionalLight>();
        directionalLight->GetTransform()->rotation = Quaternion{ Vector3d{90,0,45} };
    }

    auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();

    auto rtsCam = camObj->AddComponent<RTSCam>();
    //rtsCam->GetTransform()->position = Vector3d(0, 10, 0);

    //// ��ã�� �׽�Ʈ
    //{
    //    const float corridorRadius = 3;
    //    std::vector<Vector3f> worldVertices { };
    //    std::vector<int> worldFaces { };

    //    CreateNavPlane({ -2,0,-8 }, { 2,0,8 }, worldVertices, worldFaces);
    //    CreateNavPlane({ -8,0,-2 }, { 8,0,2 }, worldVertices, worldFaces);
    //    CreateNavPlane({ -8,0,-8 }, { -6,0,8 }, worldVertices, worldFaces);
    //    CreateNavPlane({ 6,0,-8 }, { 8,0,8 }, worldVertices, worldFaces);
    //    CreateNavPlane({ -8,0,6 }, { 8,0,8 }, worldVertices, worldFaces);
    //    CreateNavPlane({ -2,0,-8 }, { 2,0,8 }, worldVertices, worldFaces);
    //    auto navField = Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::NavigationField>();
    //    navField->BuildField(worldVertices, worldFaces);
    //    auto agent = CreateAgent(navField);
    //    auto agent2 = CreateAgent(navField);
    //    auto agent3 = CreateAgent(navField);
    //    rtsCam->groundRightClickCallback = [=](Vector3d position) {
    //        agent->MoveTo(position);
    //        agent2->MoveTo(position);
    //        agent3->MoveTo(position);
    //    };
    //}

    yunutyEngine::YunutyCycle::SingleInstance().Play();
}

void Application::Contents::ContentsLayer::Update(float ts)
{

}

void Application::Contents::ContentsLayer::GUIProgress()
{

}

void Application::Contents::ContentsLayer::Finalize()
{

}
#ifdef GEN_TESTS
void Application::Contents::ContentsLayer::AssignTestInitializer(std::function<void()> testInitializer)
{
    ContentsLayer::testInitializer = testInitializer;
    YunutyCycle::SingleInstance().onExceptionThrown = [](const std::exception& e) {
        Application::Application::GetInstance().AddMainLoopTodo([=]() {
            Assert::Fail(yunutyEngine::yutility::GetWString(e.what()).c_str());
            });
    };
}
#endif
