#include "CppUnitTest.h"
#include "ContentsLayer.h"
#include "YunutyEngine.h"
#include "Application.h"
#include "RTSCam.h"
#include "DelayedTestFunctions.h"
#include "ShakyCam.h"
#include "DebugTilePlane.h"
#include "DebugBeacon.h"
#include "DebugMeshes.h"
#include "ProjectileSystem.h"
#include "PlayerController.h"
#include "UnitFactory.h"
#include "InputManager.h"
#include "WarriorProduction.h"
#include "HealerProduction.h"
#include "MagicianProduction.h"
#include "MeleeEnemyProduction.h"
#include "UnitProductionOrder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void CreateNavigationPlane2(Vector3f botleft, Vector3f topright, std::vector<Vector3f>& worldVertices, std::vector<int>& worldFaces)
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

// �� �Լ��� ������ �⺻ �ʱ�ȭ �Լ��� �������̵��մϴ�.
void TestCaseGroupControlInit()
{
	yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());
	yunutyEngine::Collider2D::SetIsOnXYPlane(false);

	//auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	//camObj->GetTransform()->position = Vector3d(0, 0, -5);
	//auto roamingCam = camObj->AddComponent<RoamingCam>();
	auto sphereMesh = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager()->GetMesh(L"Sphere");
	auto mouseCursorObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	auto mouseCursorMesh = mouseCursorObject->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
	mouseCursorMesh->GetGI().SetMesh(sphereMesh);
	mouseCursorMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0, 0, 0, 0 });

	auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();

	auto rtsCam = camObj->AddComponent<RTSCam>();
	rtsCam->SetMouseCursorObject(mouseCursorObject);
	std::vector<Vector3f> worldVertices{ };
	std::vector<int> worldFaces{ };

	camObj->GetTransform()->SetWorldPosition(Vector3d(0, 30, 0));

	// ��ã�� �ʵ� ����
	CreateNavigationPlane2({ -2,0,-8 }, { 2,0,8 }, worldVertices, worldFaces);
	CreateNavigationPlane2({ -8,0,-2 }, { 8,0,2 }, worldVertices, worldFaces);
	CreateNavigationPlane2({ -8,0,-8 }, { -6,0,8 }, worldVertices, worldFaces);
	CreateNavigationPlane2({ 6,0,-8 }, { 8,0,8 }, worldVertices, worldFaces);
	CreateNavigationPlane2({ -8,0,6 }, { 8,0,8 }, worldVertices, worldFaces);
	CreateNavigationPlane2({ -2,0,-8 }, { 2,0,8 }, worldVertices, worldFaces);
	auto navField = Scene::getCurrentScene()->AddGameObject()->AddComponent<yunutyEngine::NavigationField>();
	navField->BuildField(worldVertices, worldFaces);

	/// ProjectileSystem SetUp
	ProjectileSystem::GetInstance()->SetUp();

	/// PlayerController SetUp
	PlayerController::GetInstance()->SetMovingSystemComponent(rtsCam);

	/// UnitFactory
	unique_ptr<UnitFactory> unitfactory = make_unique<UnitFactory>();

	/// FBX File Load
	auto rsrcManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
	rsrcManager->LoadFile("FBX/Player");
	rsrcManager->LoadFile("FBX/Boss");

	auto warriorProductor = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<WarriorProduction>();
	warriorProductor->SetUnitData(yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX("Boss"));
	auto player1GameObject = warriorProductor->CreateUnitWithOrder();
	unitfactory->OrderCreateUnit(player1GameObject, navField, Vector3d(-7.0f, 0.0f, 0.0f));

	auto magicianProductor = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<MagicianProduction>();
	magicianProductor->SetUnitData(yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX("Boss"));
	auto player2GameObject = magicianProductor->CreateUnitWithOrder();
	unitfactory->OrderCreateUnit(player2GameObject, navField, Vector3d(-7.0f, 0.0f, 7.0f));

	auto healerProductor = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<HealerProduction>();
	healerProductor->SetUnitData(yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX("Boss"));
	auto player3GameObject = healerProductor->CreateUnitWithOrder();
	unitfactory->OrderCreateUnit(player3GameObject, navField, Vector3d(-7.0f, 0.0f, -7.0f));

	auto meleeEnemyProductor1 = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<MeleeEnemyProduction>();
	meleeEnemyProductor1->SetUnitData(yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX("Player"));
	auto enemy1GameObject = meleeEnemyProductor1->CreateUnitWithOrder();
	unitfactory->OrderCreateUnit(enemy1GameObject, navField, Vector3d(7.0f, 0.0f, 7.0f));

	auto meleeEnemyProductor2 = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<MeleeEnemyProduction>();
	meleeEnemyProductor2->SetUnitData(yunutyEngine::Scene::getCurrentScene()->AddGameObjectFromFBX("Player"));
	auto enemy2GameObject = meleeEnemyProductor2->CreateUnitWithOrder();
	unitfactory->OrderCreateUnit(enemy2GameObject, navField, Vector3d(7.0f, 0.0f, 0.0f));


	{
		auto directionalLight = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		directionalLight->AddComponent<yunutyEngine::graphics::DirectionalLight>();
		directionalLight->GetTransform()->SetWorldPosition(Vector3d(0, 50, 0));
		directionalLight->GetTransform()->rotation = Quaternion{ Vector3d{100,0,0} };
	}

	//auto delayFunctionObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	//auto delayFunctionComponent = delayFunctionObject->AddComponent<DelayedTestFunctions>();
	//delayFunctionComponent->todoList.push_back({ 1.0f, [=]()
	//	{
	//		playerGameObject->GetComponent<Unit>()->OrderAttackMove(Vector3d::zero);
	//		playerGameObject2->GetComponent<Unit>()->OrderAttackMove(Vector3d::zero);
	//		playerGameObject3->GetComponent<Unit>()->OrderAttackMove(Vector3d::zero);
	//		playerGameObject4->GetComponent<Unit>()->OrderAttackMove(Vector3d::zero);

	//		enemyGameObject1->GetComponent<Unit>()->OrderAttackMove(Vector3d::zero);
	//		enemyGameObject2->GetComponent<Unit>()->OrderAttackMove(Vector3d::zero);
	//		enemyGameObject3->GetComponent<Unit>()->OrderAttackMove(Vector3d::zero);
	//		enemyGameObject4->GetComponent<Unit>()->OrderAttackMove(Vector3d::zero);
	//	} });

	auto inputManagerObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	inputManagerObject->AddComponent<InputManager>();

	yunutyEngine::YunutyCycle::SingleInstance().Play();
}

namespace InWanderLand
{
	TEST_CLASS(InWanderLand)
	{
	public:
		TEST_METHOD(TestCaseGroupControl)
		{
			Application::Application& client = Application::Application::CreateApplication(0, 0);
			Application::Contents::ContentsLayer::AssignTestInitializer(TestCaseGroupControlInit);
			client.Initialize();
			client.Run();
			client.Finalize();
		}
	};
}