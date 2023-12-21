#ifdef GEN_TESTS
#include "CppUnitTest.h"
#include "ContentsLayer.h"
#include "YunutyEngine.h"
#include "DelayedTestFunctions.h"
#include "Application.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// �� �Լ��� ������ �⺻ �ʱ�ȭ �Լ��� �������̵��մϴ�.
void SnippetInitializerTemplate()
{
    yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());

    auto rsrcManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
    auto debugStaticMesh = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
    debugStaticMesh->GetGI().SetMesh(yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager()->GetMesh(L"Cube"));
    debugStaticMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color::blue());
    auto& shaderList = rsrcManager->GetShaderList();
    yunuGI::IShader* shader = nullptr;
    for (auto each : shaderList)
    {
        if (each->GetName() == L"DebugPS.cso")
        {
            shader = each;
        }
    }
    debugStaticMesh->GetGI().GetMaterial()->SetPixelShader(shader);

    auto directionalLight = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<graphics::DirectionalLight>();
    directionalLight->GetTransform()->SetWorldRotation(Quaternion({ 100,10,0 }));

    auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::graphics::Camera>();

    camObj->GetTransform()->SetWorldPosition({ 0,0,-5 });

    yunutyEngine::YunutyCycle::SingleInstance().Play();
}

namespace snippets
{
    TEST_CLASS(InWanderLand)
    {
    public:
        // �׽�Ʈ �Լ��� �̸��� Snippet���� �����ϴ� �׽�Ʈ���� ������ ���� ���� �Ǵܿ� ������ �ʽ��ϴ�.
        // RunTests.bat�� �����ص� �� �׽�Ʈ���� ������� ������, Jenkins������ �� �׽�Ʈ���� ������� �ʽ��ϴ�.
        // �� �׽�Ʈ���� ���� �����ڰ� ���� ��Ʈ�� ����Ʈ�� �޸��Ͽ� �׽�Ʈ�� �����ϰ� ���� ���� �뵵�θ� ���˴ϴ�.
        TEST_METHOD(SnippetTemplate)
        {
            Application::Application& client = Application::Application::CreateApplication(0, 0);
            Application::Contents::ContentsLayer::AssignTestInitializer(SnippetInitializerTemplate);
            client.Initialize();
            client.Run();
            client.Finalize();
        }
    };
}
#endif