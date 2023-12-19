#include "CppUnitTest.h"
#include "ContentsLayer.h"
#include "YunutyEngine.h"
#include "DelayedTestFunctions.h"
#include "Application.h"
#include "RTSCam.h"
#include "YunutyEngine.h"
#include "DelayedTestFunctions.h"
#include "ShakyCam.h"
#include "DebugTilePlane.h"
#include "DebugBeacon.h"
#include "DebugMeshes.h"
#include "YunutyEngine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void CreateNavPlane(Vector3f botleft, Vector3f topright, std::vector<Vector3f>& worldVertices, std::vector<int>& worldFaces)
{
    int startingIdx = worldVertices.size();
    worldVertices.push_back({ botleft.x,0,topright.z });
    worldVertices.push_back({ botleft.x,0,botleft.z });
    worldVertices.push_back({ topright.x,0,botleft.z });
    worldVertices.push_back({ topright.x,0,topright.z });

    worldFaces.push_back(startingIdx + 2);
    worldFaces.push_back(startingIdx + 1);
    worldFaces.push_back(startingIdx + 0);
    worldFaces.push_back(startingIdx + 3);
    worldFaces.push_back(startingIdx + 2);
    worldFaces.push_back(startingIdx + 0);

    auto tilePlane = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<DebugTilePlane>();
    auto size = topright - botleft;
    tilePlane->GetTransform()->SetWorldPosition((botleft + topright) / 2.0);
    tilePlane->width = size.x;
    tilePlane->height = size.z;
    tilePlane->SetTiles();
}

NavigationAgent* CreateAgent(NavigationField* navField)
{
    auto agent = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::NavigationAgent>();
    agent->SetSpeed(2);
    agent->SetRadius(0.5);
    agent->AssignToNavigationField(navField);
    auto staticMesh = agent->GetGameObject()->AddGameObject()->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
    staticMesh->GetGI().SetMesh(graphics::Renderer::SingleInstance().GetResourceManager()->GetMesh(L"Capsule"));
    staticMesh->GetGI().GetMaterial()->SetColor({ 0.75,0.75,0.75,0 });
    staticMesh->GetTransform()->position = Vector3d{ 0,0.5,0 };
    return agent;
}
// �� �Լ��� ������ �⺻ �ʱ�ȭ �Լ��� �������̵��մϴ�.
void TestCaseNavigationInit()
{
    yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());

    const float corridorRadius = 3;
    std::vector<Vector3f> worldVertices { };
    std::vector<int> worldFaces { };

    auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
    auto rtsCam = camObj->AddComponent<RTSCam>();
    rtsCam->GetTransform()->position = Vector3d(3, 10, 3);

    CreateNavPlane({ -2,0,-8 }, { 2,0,8 }, worldVertices, worldFaces);
    CreateNavPlane({ -8,0,-2 }, { 8,0,2 }, worldVertices, worldFaces);
    CreateNavPlane({ -8,0,-8 }, { -6,0,8 }, worldVertices, worldFaces);
    CreateNavPlane({ 6,0,-8 }, { 8,0,8 }, worldVertices, worldFaces);
    CreateNavPlane({ -8,0,6 }, { 8,0,8 }, worldVertices, worldFaces);
    CreateNavPlane({ -2,0,-8 }, { 2,0,8 }, worldVertices, worldFaces);
    auto navField = Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::NavigationField>();
    navField->BuildField(worldVertices, worldFaces);
    auto agent = CreateAgent(navField);
    auto delayedTestFunctions = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<DelayedTestFunctions>();

    auto directionalLight = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<graphics::DirectionalLight>();
    directionalLight->GetTransform()->SetWorldRotation(Quaternion({ 100,10,0 }));

    rtsCam->groundRightClickCallback = [=](Vector3d position)
    {
        agent->MoveTo(position);
    };
    const Vector3d moveDestination{ 6.5,0,6.5 };
    agent->MoveTo(moveDestination);
    agent->SetSpeed(10);
    agent->SetAcceleration(10000000);
    // delayedTestFunctions�� 2�� �� �����ų �ݹ� �Լ��� ����մϴ�. �� �ݹ��Լ��� ���� ���� �����忡�� ȣ��˴ϴ�.
    delayedTestFunctions->todoList.push_back({ 2,[=]() {
        // ���� ���� �����忡�� ���� �����忡�� Ư�� ������ ������Ű�� �ʹٸ� �Ʒ��� AddMainLoopTodo �Լ��� ����մϴ�.
        Application::Application::GetInstance().AddMainLoopTodo([=]() {
            // Assert �Լ����� �׽�Ʈ ���̽��� ���� ���� ���θ� �Ǵ��ϴµ��� ���Դϴ�.
            // Assert�� ������ ���� �����忡�� ����Ǿ�� �մϴ�.
            Assert::IsTrue((agent->GetTransform()->GetWorldPosition() - moveDestination).MagnitudeSqr() < 0.3,L"navigation agent couldn't move to a specific location!");
            agent->Relocate(Vector3d{ -10,0,0 });
                });
            } });
    delayedTestFunctions->todoList.push_back({ 2.2,[=]() {
        // ���� ���� �����忡�� ���� �����忡�� Ư�� ������ ������Ű�� �ʹٸ� �Ʒ��� AddMainLoopTodo �Լ��� ����մϴ�.
        Application::Application::GetInstance().AddMainLoopTodo([=]() {
            Assert::IsTrue((agent->GetTransform()->GetWorldPosition() - Vector3d{-7.5,0,0}).MagnitudeSqr() < 1,L"navigation agent's ""Relocate"" method didn't really relocate the agent!");
            // �� ���� ���̶�� ���α׷��� �����մϴ�. 
            Application::Application::GetInstance().TurnOff();
                });
            } });
    yunutyEngine::YunutyCycle::SingleInstance().Play();
}

namespace InWanderLand
{
    TEST_CLASS(InWanderLand)
    {
    public:
        TEST_METHOD(TestCaseNavigation)
        {
            Application::Application& client = Application::Application::CreateApplication(0, 0);
            Application::Contents::ContentsLayer::AssignTestInitializer(TestCaseNavigationInit);
            client.Initialize();
            client.Run();
            client.Finalize();
        }
    };
}