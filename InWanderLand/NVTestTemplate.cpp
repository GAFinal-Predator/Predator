#ifdef GEN_TESTS
#include "CppUnitTest.h"
#include "ContentsLayer.h"
#include "YunutyEngine.h"
#include "DelayedTestFunctions.h"
#include "Application.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// �� �Լ��� ������ �⺻ �ʱ�ȭ �Լ��� �������̵��մϴ�.
void NVTestInitializerTemplate()
{
    yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());

    auto debugStaticMesh = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
    debugStaticMesh->GetGI().SetMesh(yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager()->GetMesh(L"Cube"));
    debugStaticMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color::blue());

    auto directionalLight = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<graphics::DirectionalLight>();
    directionalLight->GetTransform()->SetWorldRotation(Quaternion({ 100,10,0 }));

    auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::graphics::Camera>();

    yunutyEngine::YunutyCycle::SingleInstance().Play();
}

namespace nonValidation
{
    TEST_CLASS(InWanderLand)
    {
    public:
        // nonValidation ���ӽ����̽��� ���ϴ� �׽�Ʈ���� ������ ���� ���θ� �Ǵ����� �ʽ��ϴ�.
        // RunTests.bat�� �����ص� �� �׽�Ʈ���� ������� ������, Jenkins������ �� �׽�Ʈ���� ������� �ʽ��ϴ�.
        // �� �׽�Ʈ���� ���� �����ڰ� ���� ��Ʈ�� ����Ʈ�� �޸��Ͽ� �׽�Ʈ�� �����ϰ� ���� ���� �뵵�θ� ���˴ϴ�.
        TEST_METHOD(NonValTestTemplate)
        {
            Application::Application& client = Application::Application::CreateApplication(0, 0);
            Application::Contents::ContentsLayer::AssignTestInitializer(NVTestInitializerTemplate);
            client.Initialize();
            client.Run();
            client.Finalize();
        }
    };
}
#endif