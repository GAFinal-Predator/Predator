#include "InWanderLand.h"
#ifdef GEN_TESTS
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
#include "DebuggingMeshPool.h"
#include "PlayerController.h"
#include "UnitFactory.h"
#include "InputManager.h"
#include "StatusTimerPool.h"
#include "WarriorProductor.h"
#include "HealerProductor.h"
#include "MagicianProductor.h"
#include "MeleeEnemyProductor.h"
#include "UnitProductor.h"
#include "TacticModeSystem.h"
#include "SingleNavigationField.h"
#include "SkillPreviewSystem.h"
#include "UIButton.h"
#include "UIManager.h"
#include "SingletonUpdate.h"
#include "UIPanel.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void CreateNavigationPlanes(Vector3f botleft, Vector3f topright, std::vector<Vector3f>& worldVertices, std::vector<int>& worldFaces)
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

// 이 함수는 게임의 기본 초기화 함수를 오버라이드합니다.
void SnippetSkillSystemInit()
{
	yunutyEngine::Scene::LoadScene(new yunutyEngine::Scene());
	yunutyEngine::Collider2D::SetIsOnXYPlane(false);

	auto rsrcMgr = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();

	auto sphereMesh = rsrcMgr->GetMesh(L"Sphere");
	auto mouseCursorObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	auto mouseCursorMesh = mouseCursorObject->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
	mouseCursorMesh->GetGI().SetMesh(sphereMesh);
	mouseCursorMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0, 0, 0, 1 });

	auto skillPreviewCubeMeshObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	AttachDebugMesh(skillPreviewCubeMeshObject, DebugMeshType::Cube)->GetGI().SetMaterial(0, GetColoredDebugMaterial(yunuGI::Color::red(), false));
	SkillPreviewSystem::SingleInstance().SetPathPreviewObject(skillPreviewCubeMeshObject);

	auto skillPreviewSphereMeshObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	AttachDebugMesh(skillPreviewSphereMeshObject, DebugMeshType::Sphere)->GetGI().SetMaterial(0, GetColoredDebugMaterial(yunuGI::Color::red(), false));
	SkillPreviewSystem::SingleInstance().SetRangePreviewObject(skillPreviewSphereMeshObject);

	auto camObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();

	auto rtsCam = camObj->AddComponent<RTSCam>();
	rtsCam->groundHoveringClickCallback = [=](Vector3d pos)
	{
		mouseCursorObject->GetTransform()->SetWorldPosition(pos);
		SkillPreviewSystem::SingleInstance().SetCurrentMousPosition(pos);
	};
	std::vector<Vector3f> worldVertices{ };
	std::vector<int> worldFaces{ };

	camObj->GetTransform()->SetWorldPosition(Vector3d(0, 30, 0));

	CreateNavigationPlanes({ -30,0,-30 }, { 30,0,30 }, worldVertices, worldFaces);

	//CreateNavigationPlanes({ -1000,0,-1000 }, { 1000,0,1000 }, worldVertices, worldFaces);
	auto fieldObject = Scene::getCurrentScene()->AddGameObject();

	SingleNavigationField::Instance().BuildField(worldVertices, worldFaces);

	/// PlayerController SetUp
	PlayerController::GetInstance()->SetMovingSystemComponent(rtsCam);
	TacticModeSystem::SingleInstance().SetMovingSystemComponent(rtsCam);

	WarriorProductor::Instance().CreateUnit(Vector3d(-7.0f, 0.0f, 0.0f));;
	MagicianProductor::Instance().CreateUnit(Vector3d(-7.0f, 0.0f, -7.0f));;
	HealerProductor::Instance().CreateUnit(Vector3d(-7.0f, 0.0f, 7.0f));
	MeleeEnemyProductor::Instance().CreateUnit(Vector3d(7.0f, 0.0f, 7.0f));
	MeleeEnemyProductor::Instance().CreateUnit(Vector3d(7.0f, 0.0f, -7.0f));

	/// UIButton Test
	rsrcMgr->LoadFile("Texture/zoro.jpg");
	rsrcMgr->LoadFile("Texture/zoro_highLighted.jpg");
	rsrcMgr->LoadFile("Texture/zoro_Clicked.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Menu.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/TacticMode.jpg");

	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Robin_ParentUI.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Ursula_ParentUI.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Gretel_ParentUI.jpg");

	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Robin_Portrait.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Ursula_Portrait.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Gretel_Portrait.jpg");

	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Robin_Skill1_Ikon.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Ursula_Skill1_Ikon.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Gretel_Skill1_Ikon.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Robin_Skill2_Ikon.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Ursula_Skill2_Ikon.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Gretel_Skill2_Ikon.jpg");

	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/CloseButton.jpg");

	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/menu_window.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/GameManual_Button.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/GameManual.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Sound_Window.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Sound_Button.jpg");
	rsrcMgr->LoadFile("Texture/UI/InGameUITemp/Restart_Button.jpg");


#pragma region Menu UI
	auto menuPanel = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<UIPanel>();

#pragma region MenuButton
	auto menuUIObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	auto menuUIImageComponent = menuUIObject->AddComponent<yunutyEngine::graphics::UIImage>();
	auto menuUIButtonComponent = menuUIObject->AddComponent<UIButton>();
	menuUIButtonComponent->SetImageComponent(menuUIImageComponent);
	menuUIButtonComponent->SetIdleImage(rsrcMgr->GetTexture(L"Texture/UI/InGameUITemp/Menu.jpg"));
	menuUIButtonComponent->SetLayer(100);
	menuUIButtonComponent->SetButtonClickFunction([=]()
		{
			menuPanel->SetPanelActive(true);
		});
	menuUIObject->GetTransform()->SetLocalPosition({ 30, 1000, 0 });
#pragma endregion

	const int menuWindowLayer = 10;
#pragma region Menu Window
	auto menuWindowUIObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	auto menuWindowImageComponent = menuWindowUIObject->AddComponent<yunutyEngine::graphics::UIImage>();
	menuWindowImageComponent->GetGI().SetImage(rsrcMgr->GetTexture(L"Texture/UI/InGameUITemp/menu_window.jpg"));
	menuWindowImageComponent->GetGI().SetLayer(menuWindowLayer);
	menuWindowUIObject->SetSelfActive(false);
	menuWindowUIObject->GetTransform()->SetWorldPosition({ 900, 540, 0 });
	menuPanel->SetWindowImage(menuWindowImageComponent);
#pragma endregion

#pragma region Game Manual Panel
	auto gameManualPanel = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<UIPanel>();
	auto gameManualWindowObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	auto gameManualWindowImageComponent = gameManualWindowObject->AddComponent<yunutyEngine::graphics::UIImage>();
	gameManualWindowImageComponent->GetGI().SetImage(rsrcMgr->GetTexture(L"Texture/UI/InGameUITemp/GameManual.jpg"));
	gameManualWindowImageComponent->GetGI().SetLayer(menuWindowLayer + 2);
	gameManualWindowObject->SetSelfActive(false);
	gameManualWindowObject->GetTransform()->SetWorldPosition({ 900, 540, 0 });
	gameManualPanel->SetWindowImage(gameManualWindowImageComponent);
#pragma endregion

#pragma region Game Manual Button
	{
		auto gameManualButtonObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		auto gameManualImageComponent = gameManualButtonObject->AddComponent<yunutyEngine::graphics::UIImage>();
		auto gameManualButtonComponent = gameManualButtonObject->AddComponent<UIButton>();
		gameManualButtonComponent->SetImageComponent(gameManualImageComponent);
		gameManualButtonComponent->SetIdleImage(rsrcMgr->GetTexture(L"Texture/UI/InGameUITemp/GameManual_Button.jpg"));
		gameManualButtonComponent->SetLayer(menuWindowLayer + 1);
		gameManualButtonComponent->SetButtonClickFunction([=]()
			{
				gameManualPanel->SetPanelActive(true);
			});
		gameManualButtonObject->GetTransform()->SetLocalPosition({ 1060, 623, 0 });
		gameManualButtonObject->SetSelfActive(false);
		menuPanel->SetUIButtonComponent(gameManualButtonComponent);
	}
#pragma endregion

#pragma region Sound Panel
	auto soundPanel = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<UIPanel>();
	auto soundWindowObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	auto soundWindowImageComponent = soundWindowObject->AddComponent<yunutyEngine::graphics::UIImage>();
	soundWindowImageComponent->GetGI().SetImage(rsrcMgr->GetTexture(L"Texture/UI/InGameUITemp/Sound_Window.jpg"));
	soundWindowImageComponent->GetGI().SetLayer(menuWindowLayer + 2);
	soundWindowObject->SetSelfActive(false);
	soundWindowObject->GetTransform()->SetWorldPosition({ 900, 540, 0 });
	soundPanel->SetWindowImage(soundWindowImageComponent);
#pragma endregion

#pragma region Sound Button
	{
		auto soundButtonObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		auto soundButtonImageComponent = soundButtonObject->AddComponent<yunutyEngine::graphics::UIImage>();
		auto soundButtonComponent = soundButtonObject->AddComponent<UIButton>();
		soundButtonComponent->SetImageComponent(soundButtonImageComponent);
		soundButtonComponent->SetIdleImage(rsrcMgr->GetTexture(L"Texture/UI/InGameUITemp/Sound_Button.jpg"));
		soundButtonComponent->SetLayer(menuWindowLayer + 4);
		soundButtonComponent->SetButtonClickFunction([=]()
			{
				soundPanel->SetPanelActive(true);
			});
		soundButtonObject->GetTransform()->SetLocalPosition({ 1060, 693, 0 });
		soundButtonObject->SetSelfActive(false);
		menuPanel->SetUIButtonComponent(soundButtonComponent);
	}
#pragma endregion

#pragma region Restart Button
	{
		auto restartButtonObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		auto restartButtonImageComponent = restartButtonObject->AddComponent<yunutyEngine::graphics::UIImage>();
		auto restartButtonComponent = restartButtonObject->AddComponent<UIButton>();
		restartButtonComponent->SetImageComponent(restartButtonImageComponent);
		restartButtonComponent->SetIdleImage(rsrcMgr->GetTexture(L"Texture/UI/InGameUITemp/Restart_Button.jpg"));
		restartButtonComponent->SetLayer(menuWindowLayer + 3);
		restartButtonComponent->SetButtonClickFunction([=]()
			{
				/// 게임을 재시작하는 로직
			});
		restartButtonObject->GetTransform()->SetLocalPosition({ 1060, 763, 0 });
		restartButtonObject->SetSelfActive(false);
		menuPanel->SetUIButtonComponent(restartButtonComponent);
	}
#pragma endregion


	menuPanel->SetCloseButtonActive(true);
	gameManualPanel->SetCloseButtonActive(true);
	soundPanel->SetCloseButtonActive(true);
#pragma endregion

	auto singletonUpdateComponent = yunutyEngine::Scene::getCurrentScene()->AddGameObject()->AddComponent<SingletonUpdate>();

	/// UItext Test
	//auto enemy1TextObj = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	//auto enemy1UITextCom = enemy1TextObj->AddComponent<graphics::UIText>();
	//enemy1TextObj->SetParent(player1GameObject);
	//enemy1TextObj->GetTransform()->SetLocalScale(Vector3d(1000, 1000, 0));
	//enemy1UITextCom->GetGI().SetFontSize(76);
	//enemy1UITextCom->GetGI().SetText(L"Test");
	{
		auto directionalLight = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
		directionalLight->AddComponent<yunutyEngine::graphics::DirectionalLight>();
		directionalLight->GetTransform()->SetWorldPosition(Vector3d(0, 100, 0));
		directionalLight->GetTransform()->SetLocalRotation( Quaternion{ Vector3d{120,0,0} });
	}
}

namespace snippets
{
	TEST_CLASS(SnippetSkillSystemClass)
	{
	public:
		TEST_METHOD(SnippetSkillSystem)
		{
			application::Application& client = application::Application::CreateApplication(0, 0);
			application::contents::ContentsLayer::AssignTestInitializer(SnippetSkillSystemInit);
			client.Initialize();
			client.Run();
			client.Finalize();
		}
	};
}
#endif
