#ifdef GEN_TESTS
#include "CppUnitTest.h"
#include "ContentsLayer.h"
#include "YunutyEngine.h"
#include "DelayedTestFunctions.h"
#include "Application.h"
#include "TestComponent2.h"
#include "RTSCam.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// �� �Լ��� ������ �⺻ �ʱ�ȭ �Լ��� �������̵��մϴ�.
void SnippetAnimationChangeInterpolationInit()
{
	yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());

	{
		auto directionalLight = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		directionalLight->AddComponent<yunutyEngine::graphics::DirectionalLight>();
		directionalLight->GetTransform()->rotation = Quaternion{ Vector3d{90,0,45} };
	}

	auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();

	auto rtsCam = camObj->AddComponent<RTSCam>();

	const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();

	_resourceManager->LoadFile("FBX/Boss");

	for (int i = 0; i < 1000; ++i)
	{
		{
			auto object = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
			auto tempX = static_cast<float>(rand() % 100);
			auto tempZ = static_cast<float>(rand() % 100);
			object->GetTransform()->position = Vector3d{ tempX,0,tempZ };
			auto renderer = object->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
			auto meshList = _resourceManager->GetMeshList();
			for (auto& i : meshList)
			{
				if (i->GetName() == L"Sphere")
				{
					renderer->GetGI().SetMesh(i);
				}
			}
		}
	}

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
		TEST_METHOD(SnippetAnimationChangeInterpolation)
		{
			application::Application& client = application::Application::CreateApplication(0, 0);
			application::Contents::ContentsLayer::AssignTestInitializer(SnippetAnimationChangeInterpolationInit);
			client.Initialize();
			client.Run();
			client.Finalize();
		}
	};
}
#endif