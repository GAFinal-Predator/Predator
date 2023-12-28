#ifdef GEN_TESTS
#include "CppUnitTest.h"
#include "ContentsLayer.h"
#include "YunutyEngine.h"
#include "DelayedTestFunctions.h"
#include "Application.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// �� �Լ��� ������ �⺻ �ʱ�ȭ �Լ��� �������̵��մϴ�.
void TestInitUnitAnimation()
{
    yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());
    auto delayedTestFunctions = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<DelayedTestFunctions>();

    // delayedTestFunctions�� 2�� �� �����ų �ݹ� �Լ��� ����մϴ�. �� �ݹ��Լ��� ���� ���� �����忡�� ȣ��˴ϴ�.
    delayedTestFunctions->todoList.push_back({ 0.3,[]() {
        // ���� ���� �����忡�� ���� �����忡�� Ư�� ������ ������Ű�� �ʹٸ� �Ʒ��� AddMainLoopTodo �Լ��� ����մϴ�.
        application::Application::GetInstance().AddMainLoopTodo([]() {
            // Assert �Լ����� �׽�Ʈ ���̽��� ���� ���� ���θ� �Ǵ��ϴµ��� ���Դϴ�.
            // Assert�� ������ ���� �����忡�� ����Ǿ�� �մϴ�.
            Assert::IsTrue(1 + 1 == 2);

            // �� ���� ���̶�� ���α׷��� �����մϴ�. 
            application::Application::GetInstance().TurnOff();
                });
            } });

    auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::graphics::Camera>();

    yunutyEngine::YunutyCycle::SingleInstance().Play();
}

namespace InWanderLand
{
    TEST_CLASS(InWanderLand)
    {
    public:
        TEST_METHOD(TestUnitAnimation)
        {
            application::Application& client = application::Application::CreateApplication(0, 0);
            application::Contents::ContentsLayer::AssignTestInitializer(TestInitUnitAnimation);
            client.Initialize();
            client.Run();
            client.Finalize();
        }
    };
}
#endif