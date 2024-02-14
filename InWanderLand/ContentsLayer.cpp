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

/// 그래픽스 테스트용
void GraphicsTest()
{
    auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
    camObj->AddComponent<tests::GraphicsTestCam>();

    const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();

	_resourceManager->LoadFile("CastleWallPS.cso");

	yunuGI::IShader* shader;
	yunuGI::IShader* shader2;
	auto& shaderList = _resourceManager->GetShaderList();
	for (auto& i : shaderList)
	{
		if (i->GetName() == L"CastleWallPS.cso")
		{
			shader = i;
		}

		if (i->GetName() == L"Debug_AlphaPS.cso")
		{
			shader2 = i;
		}
	}

	_resourceManager->LoadFile("FBX/SM_CastleWall");
	_resourceManager->LoadFile("Texture/T_Brick_Dirt_BaseColor.png");
	_resourceManager->LoadFile("Texture/T_Brick_Dirt_Normal.png");
	yunuGI::ITexture* texture;
	yunuGI::ITexture* texture2;
	auto& textureList = _resourceManager->GetTextureList();
	for (auto& i : textureList)
	{
		if (i->GetName() == L"Texture/T_Brick_Dirt_BaseColor.png")
		{
			texture = i;
		}

		if (i->GetName() == L"Texture/T_Brick_Dirt_Normal.png")
		{
			texture2 = i;
		}
	}
	{
		auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX("SM_CastleWall");
		auto& childVec = obj->GetChildren();
		childVec[0]->GetComponent<graphics::StaticMeshRenderer>()->GetGI().GetMaterial(1)->SetPixelShader(shader);
		childVec[0]->GetComponent<graphics::StaticMeshRenderer>()->GetGI().GetMaterial(1)->SetTexture(yunuGI::Texture_Type::Temp0, texture);
		childVec[0]->GetComponent<graphics::StaticMeshRenderer>()->GetGI().GetMaterial(1)->SetTexture(yunuGI::Texture_Type::Temp1, texture2);
		obj->GetTransform()->SetLocalPosition(Vector3d{ 0,0,10 });
	}

	{
		auto obj = yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX("SM_CastleWall");
		auto& childVec = obj->GetChildren();
	
		childVec[0]->GetComponent<graphics::StaticMeshRenderer>()->GetGI().GetMaterial(0)->SetPixelShader(shader2);
		childVec[0]->GetComponent<graphics::StaticMeshRenderer>()->GetGI().GetMaterial(0)->SetColor(yunuGI::Color{ 0,1,1,0.2 });
	
		childVec[0]->GetComponent<graphics::StaticMeshRenderer>()->GetGI().GetMaterial(1)->SetPixelShader(shader2);
		childVec[0]->GetComponent<graphics::StaticMeshRenderer>()->GetGI().GetMaterial(1)->SetColor(yunuGI::Color{ 0,1,1,0.2 });
	
		childVec[0]->GetComponent<graphics::StaticMeshRenderer>()->GetGI().GetMaterial(2)->SetPixelShader(shader2);
		childVec[0]->GetComponent<graphics::StaticMeshRenderer>()->GetGI().GetMaterial(2)->SetColor(yunuGI::Color{ 0,1,1,0.2 });
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
    //resourceManager->LoadFile("FBX/Monster1");
	///resourceManager->LoadFile("FBX/SM_Bush_001");
	///resourceManager->LoadFile("FBX/SM_Bush_002");
	///resourceManager->LoadFile("FBX/SM_CastleWall");
	///resourceManager->LoadFile("FBX/SM_CastleWall_Door");
	///resourceManager->LoadFile("FBX/SM_CastleWall_Pillar");
	///resourceManager->LoadFile("FBX/SM_Chair");
	///resourceManager->LoadFile("FBX/SM_Cuptower");
	///resourceManager->LoadFile("FBX/SM_Fork");
	///resourceManager->LoadFile("FBX/SM_GuideBook");
	///resourceManager->LoadFile("FBX/SM_Hat01");
	///resourceManager->LoadFile("FBX/SM_Hat02");
	///resourceManager->LoadFile("FBX/SM_SmallBush_001");
	///resourceManager->LoadFile("FBX/SM_Stone_001");
	///resourceManager->LoadFile("FBX/SM_Stone_002");
	///resourceManager->LoadFile("FBX/SM_Stump");
	///resourceManager->LoadFile("FBX/SM_Temple_Book_etc");
	///resourceManager->LoadFile("FBX/SM_Temple_Books");
	///resourceManager->LoadFile("FBX/SM_Temple_Floor");
	///resourceManager->LoadFile("FBX/SM_Temple_Pillar");
	///resourceManager->LoadFile("FBX/SM_Temple_Pillar_Broken");
	///resourceManager->LoadFile("FBX/SM_Temple_Rabbit");
	///resourceManager->LoadFile("FBX/SM_Temple_Stairs");
	///resourceManager->LoadFile("FBX/SM_Temple_Welcome");
	///resourceManager->LoadFile("FBX/SM_Trunk_001");
    //resourceManager->LoadFile("FBX/Spear");

#ifndef EDITOR
#ifdef GRAPHICS_TEST
	{
		yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());
		//yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());
		yunutyEngine::Collider2D::SetIsOnXYPlane(false);
		auto directionalLight = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		//directionalLight->GetTransform()->SetLocalRotation(Quaternion{ Vector3d{0,180,0} });
		auto light = directionalLight->AddComponent<yunutyEngine::graphics::DirectionalLight>();
		//auto color = yunuGI::Color{ 0.831,0.722,0.569,1.f };
		auto color = yunuGI::Color{ 1,1,1,1.f };
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
        camObj->GetTransform()->SetLocalPosition({ 0,10,0 });
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

        {
            MagicianProductor::Instance().m_startPosition = Vector3d{ 4,0,4 };
            auto player1GameObject = MagicianProductor::Instance().CreateUnitWithOrder();
        }

        {
            MagicianProductor::Instance().m_startPosition = Vector3d{ 2,0,4 };
            auto player1GameObject = MagicianProductor::Instance().CreateUnitWithOrder();
        }
        {
            MagicianProductor::Instance().m_startPosition = Vector3d{ 4,0,6 };
            auto player1GameObject = MagicianProductor::Instance().CreateUnitWithOrder();
        }
        {
            MagicianProductor::Instance().m_startPosition = Vector3d{ 1,0,4 };
            auto player1GameObject = MagicianProductor::Instance().CreateUnitWithOrder();
        }
        camComp->groundRightClickCallback = [=](Vector3d pos)
        {
            agent->MoveTo(pos);
        };
    }
#endif

#endif // ! EDITOR
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
