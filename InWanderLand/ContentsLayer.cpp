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
#include "TestUtilGraphicsTestCam.h"
#include "MapFileManager.h"
#include "InstanceManager.h"
#include "SingleNavigationField.h"
#include "TestUtilRTSTestCam.h"
#include "MagicianProductor.h"

#include <algorithm>
#include <string>
#ifdef GEN_TESTS
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

#include <d3d11.h>

std::function<void()> application::contents::ContentsLayer::testInitializer;

bool contentsInputControl = true;

/// 그래픽스 테스트용
void GraphicsTest()
{
	auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	camObj->AddComponent<tests::GraphicsTestCam>();

	const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();

	_resourceManager->LoadFile("Texture/Brick_Albedo.jpg");
	_resourceManager->LoadFile("Texture/Brick_Normal.jpg");

	yunuGI::IMesh* mesh = _resourceManager->GetMesh(L"Cube");
	yunuGI::IMesh* mesh2 = _resourceManager->GetMesh(L"Sphere");
	yunuGI::IMesh* mesh3 = _resourceManager->GetMesh(L"Rectangle");
	yunuGI::ITexture* tex = _resourceManager->GetTexture(L"Texture/Brick_Albedo.jpg");
	yunuGI::ITexture* tex2 = _resourceManager->GetTexture(L"Texture/Brick_Normal.jpg");

	auto& animationList = _resourceManager->GetAnimationList();
	yunuGI::IAnimation* animation;

	for (auto& i : animationList)
	{
		if (i->GetName() == L"Ani_Monster2_Walk")
		{
			i->SetLoop(true);
			animation = i;
		}
	}

	auto& shaderList = _resourceManager->GetShaderList();
	yunuGI::IShader* shader;
	for (auto& i : shaderList)
	{
		if (i->GetName() == L"DebugPS.cso")
		{
			shader = i;
		}
	}

	{
		auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		obj->GetTransform()->SetLocalPosition(Vector3d{ -47.56,3.67,44.81 });
		auto light = obj->AddComponent<yunutyEngine::graphics::PointLight>();
		yunuGI::Color color{ 0,0,1.f,1 };
		light->GetGI().SetLightDiffuseColor(color);
		light->GetGI().SetRange(10);
	}

	{
		auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		obj->GetTransform()->SetLocalPosition(Vector3d{ -47.56,3.67,45.32 });
		auto light = obj->AddComponent<yunutyEngine::graphics::PointLight>();
		yunuGI::Color color{ 1.f,0,0,1 };
		light->GetGI().SetLightDiffuseColor(color);
		light->GetGI().SetRange(10);
	}

	{
		auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		obj->GetTransform()->SetLocalScale(Vector3d{ 100,1,100 });
		auto renderer = obj->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		renderer->GetGI().SetMesh(mesh);
	}
	{
		auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		obj->GetTransform()->SetLocalScale(Vector3d{ 100,100,1 });
		obj->GetTransform()->SetLocalPosition(Vector3d{ 0,50,50 });
		auto renderer = obj->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		renderer->GetGI().SetMesh(mesh);
	}
	{
		auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		obj->GetTransform()->SetLocalScale(Vector3d{ 1,100,100 });
		obj->GetTransform()->SetLocalPosition(Vector3d{ -50,50,0 });
		auto renderer = obj->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		renderer->GetGI().SetMesh(mesh);
	}

	{
		auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		obj->GetTransform()->SetLocalPosition(Vector3d{ -48.67,3.67,44.81 });
		auto renderer = obj->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		renderer->GetGI().SetMesh(mesh);
	}
	{
		auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		obj->GetTransform()->SetLocalPosition(Vector3d{ -47.26,3.67,47.83 });
		auto renderer = obj->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		renderer->GetGI().SetMesh(mesh);
	}
	{
		auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		obj->GetTransform()->SetLocalPosition(Vector3d{ -48.08,5.32,46.24 });
		auto renderer = obj->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		renderer->GetGI().SetMesh(mesh);
	}

	{
		auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		obj->GetTransform()->SetLocalScale(Vector3d{ 1,100,100 });
		obj->GetTransform()->SetLocalPosition(Vector3d{ 48.08,5.32,46.24 });
		auto renderer = obj->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		renderer->GetGI().SetMesh(mesh);
	}

	{
		auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX("Monster2");
		auto animator = obj->GetComponent<yunutyEngine::graphics::Animator>();
		animator->GetGI().PushAnimation(animation);
		animator->GetGI().Play(animation);
		obj->GetTransform()->SetLocalPosition(Vector3d{ -47.55, 0.5f,42.53 });
		obj->GetTransform()->SetLocalScale(Vector3d{ 0.01,0.01,0.01});
		obj->GetTransform()->SetLocalRotation(Quaternion{ Vector3d{ 0,0,0} });
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//{
	//	auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	//	obj->GetTransform()->SetLocalPosition(Vector3d{ 0,0,0 });
	//	auto renderer = obj->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
	//	renderer->GetGI().SetMesh(mesh);
	//	renderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::ALBEDO, tex);
	//	renderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::NORMAL, tex2);
	//}
}

void application::contents::ContentsLayer::SetInputControl(bool control)
{
	contentsInputControl = control;
}

bool application::contents::ContentsLayer::GetInputControl()
{
	return contentsInputControl;
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

	//auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	//camObj->GetTransform()->SetLocalPosition(Vector3d(0, 20, -10));
	//camObj->GetTransform()->SetLocalRotation( Quaternion(Vector3d(60, 0, 0)));
	//auto roamingCam = camObj->AddComponent<RTSCam>();
	//roamingCam->SetCameraMain();

	const yunuGI::IResourceManager* resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();

	//resourceManager->LoadFile("FBXMaterial.scres");

	//resourceManager->LoadFile("LeavesVS.cso");
	//resourceManager->LoadFile("LeavesPS.cso");

	resourceManager->LoadFile("FBX/Monster1");
	resourceManager->LoadFile("FBX/Monster2");
	//resourceManager->LoadFile("FBX/Stone");
	//resourceManager->LoadFile("FBX/SM_Bush_001");
	//resourceManager->LoadFile("FBX/SM_Bush_002");
	//resourceManager->LoadFile("FBX/SM_Trunk_001");
	//resourceManager->LoadFile("FBX/SM_CastleWall");
	//resourceManager->LoadFile("FBX/SM_CastleWall_Door");
	//resourceManager->LoadFile("FBX/SM_CastleWall_Pillar");
	//resourceManager->LoadFile("FBX/SM_Chair");
	//resourceManager->LoadFile("FBX/SM_Cuptower");
	//resourceManager->LoadFile("FBX/SM_Fork");
	//resourceManager->LoadFile("FBX/SM_GuideBook");
	//resourceManager->LoadFile("FBX/SM_Hat01");
	//resourceManager->LoadFile("FBX/SM_Hat02");
	//resourceManager->LoadFile("FBX/SM_SmallBush_001");
	//resourceManager->LoadFile("FBX/SM_Stone_001");
	//resourceManager->LoadFile("FBX/SM_Stone_002");
	//resourceManager->LoadFile("FBX/SM_Stump");
	//resourceManager->LoadFile("FBX/SM_Temple_Book_etc");
	//resourceManager->LoadFile("FBX/SM_Temple_Books");
	//resourceManager->LoadFile("FBX/SM_Temple_Floor");
	//resourceManager->LoadFile("FBX/SM_Temple_Pillar");
	//resourceManager->LoadFile("FBX/SM_Temple_Pillar_Broken");
	//resourceManager->LoadFile("FBX/SM_Temple_Rabbit");
	//resourceManager->LoadFile("FBX/SM_Temple_Stairs");
	//resourceManager->LoadFile("FBX/SM_Temple_Welcome");
	//resourceManager->LoadFile("FBX/SM_Trunk_001");
	//resourceManager->LoadFile("FBX/SM_Stage1_Floor");
	//resourceManager->LoadFile("FBX/Spear");

#ifndef EDITOR
#ifdef GRAPHICS_TEST
	{
		yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());
		//yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());
		yunutyEngine::Collider2D::SetIsOnXYPlane(false);
		auto directionalLight = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		directionalLight->GetTransform()->SetLocalRotation(Quaternion{ Vector3d{50,-30,0} });
		directionalLight->GetTransform()->SetLocalPosition(Vector3d{ 0,0,-20 });
		auto light = directionalLight->AddComponent<yunutyEngine::graphics::DirectionalLight>();
		//auto color = yunuGI::Color{ 0.831,0.722,0.569,1.f };
		auto color = yunuGI::Color{ 0.2,0.2,0.2,1.f };
		light->GetGI().SetLightDiffuseColor(color);
		//directionalLight->GetTransform()->SetLocalPosition(Vector3d{ 0,0,-10 });
		//directionalLight->GetTransform()->rotation = Quaternion{ Vector3d{0, 45, 0} };
		//auto test = directionalLight->AddComponent<TestComponent2>();
		//test->gameObject = directionalLight;

		editor::MapFileManager::GetSingletonInstance().LoadStaticOrnaments("TestOrnaments.punreal");
	}
	GraphicsTest();
#else
	{
		yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());
		auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		auto camComp = camObj->AddComponent<RTSCam>();
		camObj->GetTransform()->SetLocalPosition({ 0,20,0 });
		auto directionalLight = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		auto light = directionalLight->AddComponent<yunutyEngine::graphics::DirectionalLight>();
		auto color = yunuGI::Color{ 0.831,0.722,0.569,1.f };
		light->GetGI().SetLightDiffuseColor(color);
		directionalLight->GetTransform()->SetLocalPosition(Vector3d{ 0,0,-10 });
		yunutyEngine::NavigationAgent* agent = nullptr;
		{
			agent = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::NavigationAgent>();
			agent->GetTransform()->SetLocalPosition(Vector3d{ 0,0,20 });
			agent->SetSpeed(5);
			agent->SetRadius(0.5);
			agent->AssignToNavigationField(&SingleNavigationField::Instance());
			auto staticMesh = agent->GetGameObject()->AddGameObject()->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
			staticMesh->GetGI().SetMesh(graphics::Renderer::SingleInstance().GetResourceManager()->GetMesh(L"Capsule"));
			staticMesh->GetGI().GetMaterial()->SetColor({ 0.75,0.75,0.75,1 });
			staticMesh->GetTransform()->SetLocalPosition(Vector3d{ 0,0.5,0 });
		}

		editor::MapFileManager::GetSingletonInstance().LoadMapFile("TestMap.pmap");
		editor::InstanceManager::GetSingletonInstance().ApplyInstancesAsPlaytimeObjects();

	}
#endif
#endif // ! EDITOR
}

void application::contents::ContentsLayer::Update(float ts)
{
	std::cout << Time::GetFPS() << std::endl;
}

void application::contents::ContentsLayer::GUIProgress()
{

}

void application::contents::ContentsLayer::Finalize()
{

}

void application::contents::ContentsLayer::PlayContents()
{
	editor::InstanceManager::GetSingletonInstance().ApplyInstancesAsPlaytimeObjects();
}

void application::contents::ContentsLayer::PauseContents()
{
	Time::SetTimeScale(0);
}

void application::contents::ContentsLayer::ResumeContents()
{
	Time::SetTimeScale(1);
}

void application::contents::ContentsLayer::StopContents()
{
	Time::SetTimeScale(1);
	ClearPlaytimeObject();
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

void application::contents::ContentsLayer::ClearPlaytimeObject()
{
	
}
