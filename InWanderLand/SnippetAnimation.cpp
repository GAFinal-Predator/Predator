#ifdef GEN_TESTS
#include "CppUnitTest.h"
#include "ContentsLayer.h"
#include "YunutyEngine.h"
#include "DelayedTestFunctions.h"
#include "Application.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// �� �Լ��� ������ �⺻ �ʱ�ȭ �Լ��� �������̵��մϴ�.
void SnippetInitializerAnimation()
{
    yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());

    auto rsrcManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();

    rsrcManager->LoadFile("FBX/Boss");
    auto skinnedMesh = yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX("Boss");
    auto animator = skinnedMesh->GetComponent<yunutyEngine::graphics::Animator>();

    auto& animList = rsrcManager->GetAnimationList();
    yunuGI::IAnimation* anim = nullptr;
    for (auto each : animList)
    {
        if (each->GetName() == L"root|000.Idle")
        {
            anim = each;
        }
    }
    anim->SetLoop(true);
    animator->GetGI().PushAnimation(anim);
    animator->GetGI().Play(anim);

    auto directionalLight = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<graphics::DirectionalLight>();
    directionalLight->GetTransform()->SetWorldRotation(Quaternion({ 100,10,0 }));

    auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::graphics::Camera>();

    camObj->GetTransform()->SetWorldPosition({ 0,5,-15 });

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
        TEST_METHOD(SnippetAnimation)
        {
            Application::Application& client = Application::Application::CreateApplication(0, 0);
            Application::Contents::ContentsLayer::AssignTestInitializer(SnippetInitializerAnimation);
            client.Initialize();
            client.Run();
            client.Finalize();
        }
    };
}
#endif