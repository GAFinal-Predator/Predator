#include "InWanderLand.h"
#include "ContentsLayer.h"

#include "YunutyEngine.h"
#include "ShakyCam.h"
#include "RTSCam.h"   
#include "DebugTilePlane.h"
#include "DebugBeacon.h"
#include "DebugMeshes.h"
#include "Application.h"
#include "TestComponent2.h"

#include <algorithm>
#include <string>
#ifdef GEN_TESTS
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

#include <d3d11.h>

std::function<void()> application::contents::ContentsLayer::testInitializer;

/// 그래픽스 테스트용
void GraphicsTest()
{
	//const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
	////_resourceManager->LoadFile("FBX/Bush");
	//_resourceManager->LoadFile("FBX/Stone");
	//_resourceManager->LoadFile("Texture/T_LeafBrush.png");
	//_resourceManager->LoadFile("LeavesPS.cso");
	//_resourceManager->LoadFile("LeavesVS.cso");
	//auto& meshList = _resourceManager->GetMeshList();
	//auto& shaderList = _resourceManager->GetShaderList();
	//auto& textureList = _resourceManager->GetTextureList();

	//yunuGI::IMesh* planeMesh = nullptr;
	//yunuGI::IMesh* sphereMesh = nullptr;
	//yunuGI::IMesh* cubeMesh = nullptr;
	//yunuGI::IShader* pshader = nullptr;
	//yunuGI::IShader* vshader = nullptr;
	//yunuGI::ITexture* texture = nullptr;

	//auto material = _resourceManager->CreateMaterial(L"Leaves");

	//for (auto& i : shaderList)
	//{
	//	float tempX = rand() % 1000;
	//	float tempZ = rand() % 1000;
	//	auto object = yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX("Boss");
	//	object->GetTransform()->position = Vector3d{ tempX,0,tempZ };
	//	auto animator = object->GetComponent<yunutyEngine::graphics::Animator>();
	//	for (auto& i : animationList)
	//	{
	//		pshader = i;
	//	}
	//	if (i->GetName() == L"LeavesVS.cso")
	//	{
	//		vshader = i;
	//	}
	//}

	//for (auto& i : meshList)
	//{
	//	if (i->GetName() == L"Rectangle")
	//	{
	//		planeMesh = i;
	//	}
	//	if (i->GetName() == L"Sphere")
	//	{
	//		sphereMesh = i;
	//	}
	//	if (i->GetName() == L"Cube")
	//	{
	//		cubeMesh = i;
	//	}
	//}

	//for (auto& i : textureList)
	//{
	//	if (i->GetName() == L"Texture/T_LeafBrush.png")
	//	{
	//		texture = i;
	//	}
	//}

	//{
	//	auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX("Stone");
	//	obj->GetTransform()->rotation = Quaternion{ Vector3d{90,0,0} };
	//}
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
void application::contents::ContentsLayer::Initialize()
{
	if (ContentsLayer::testInitializer)
	{
		ContentsLayer::testInitializer();
		return;
	}

	yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());
	auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	camObj->GetTransform()->position = Vector3d(0, 20, 0);
	camObj->GetTransform()->rotation = Quaternion(Vector3d(60, 0, 0));
	auto roamingCam = camObj->AddComponent<RTSCam>();
	roamingCam->SetCameraMain();
#ifdef GRAPHICS_TEST
	{
		yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());
		yunutyEngine::Collider2D::SetIsOnXYPlane(false);
		auto directionalLight = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		auto light =directionalLight->AddComponent<yunutyEngine::graphics::DirectionalLight>();
		auto color = yunuGI::Color{ 0.831,0.722,0.569,1.f };
		light->GetGI().SetLightDiffuseColor(color);
		directionalLight->GetTransform()->position = Vector3d{ 0,0,-10 };
		//directionalLight->GetTransform()->rotation = Quaternion{ Vector3d{0, 45, 0} };
		//auto test = directionalLight->AddComponent<TestComponent2>();
		//test->gameObject = directionalLight;
	}
	GraphicsTest();
#else

#endif

	yunutyEngine::YunutyCycle::SingleInstance().Play();
	}

void application::contents::ContentsLayer::Update(float ts)
{
	//std::cout << Time::GetFPS() << std::endl;
}

void application::contents::ContentsLayer::GUIProgress()
{

}

void application::contents::ContentsLayer::Finalize()
{

}

#ifdef GEN_TESTS
void application::contents::ContentsLayer::AssignTestInitializer(std::function<void()> testInitializer)
{
	ContentsLayer::testInitializer = testInitializer;
	YunutyCycle::SingleInstance().onExceptionThrown = [](const std::exception& e) {
		application::Application::GetInstance().AddMainLoopTodo([=]() {
			Assert::Fail(yunutyEngine::yutility::GetWString(e.what()).c_str());
			});
		};
}
#endif
