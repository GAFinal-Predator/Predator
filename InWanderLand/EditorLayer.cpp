#include "InWanderLand.h"
#include "EditorLayer.h"

#include "MapFileManager.h"
#include "EditorPanel.h"
#include "PanelList.h"
#include "ModuleList.h"
#include "PaletteManager.h"
#include "Camera.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "MenubarCommands.h"
#include "Application.h"

namespace application
{
	namespace editor
	{
		std::function<void()> EditorLayer::testInitializer;

		void EditorLayer::Initialize()
		{
			/// Test 환경을 위한 Initialize 지원
			if (EditorLayer::testInitializer)
			{
				EditorLayer::testInitializer();
				return;
			}

			/// 각종 매니저 클래스 메모리 할당
			MapFileManager::GetSingletonInstance();
			palette::PaletteManager::GetSingletonInstance();

			/// 에디터 패널 생성 및 초기화 진행
			editorPanelList.resize((int)Panel_List::Size);

			editorPanelList[(int)Panel_List::HIERARCHY] = &HierarchyPanel::GetSingletonInstance();
			editorPanelList[(int)Panel_List::INSPECTOR] = &InspectorPanel::GetSingletonInstance();
			editorPanelList[(int)Panel_List::PREVIEW] = &PreviewPanel::GetSingletonInstance();
			editorPanelList[(int)Panel_List::MINIMAP] = &MiniMapPanel::GetSingletonInstance();
			editorPanelList[(int)Panel_List::SCENEVIEW] = &SceneViewPanel::GetSingletonInstance();
			editorPanelList[(int)Panel_List::CAMERAVIEW] = &CameraViewPanel::GetSingletonInstance();
			editorPanelList[(int)Panel_List::PALETTE] = &PalettePanel::GetSingletonInstance();

			/// 에디터 모듈 생성 및 초기화 진행
			editorModuleList.resize((int)Module_List::Size);

			editorModuleList[(int)Module_List::TemplateDataEditor] = &Module_TemplateDataEditor::GetSingletonInstance();

			for (auto& each : editorPanelList)
			{
				each->Initialize();
			}

			for (auto& each : editorModuleList)
			{
				
				each->Initialize();
			}
		}

		void EditorLayer::Update(float ts)
		{
			editorCamera.Update(ts);

			for (auto& each : editorPanelList)
			{
				each->Update(ts);
			}

			for (auto& each : editorModuleList)
			{

				each->Update(ts);
			}
		}

		void EditorLayer::GUIProgress()
		{
			UI_DrawMenubar();
			
			for (auto& each : editorPanelList)
			{
				each->GUIProgress();
			}

			for (auto& each : editorModuleList)
			{
				each->GUIProgress();
			}
		}

		void EditorLayer::Finalize()
		{
			for (auto& each : editorPanelList)
			{
				each->Finalize();
			}

			for (auto& each : editorModuleList)
			{

				each->Finalize();
			}
		}

		void EditorLayer::OnEvent(EditorEvents& event)
		{
			editorCamera.OnEvent(event);

			for (auto& each : editorPanelList)
			{
				each->OnEvent(event);
			}

			for (auto& each : editorModuleList)
			{

				each->OnEvent(event);
			}
		}

		void EditorLayer::LateInitialize()
		{
			palette::PaletteManager::GetSingletonInstance().Initialize();

			// 카메라 초기화
			editorCamera.Initialize(yunutyEngine::graphics::Camera::GetMainCamera());
		}

		void EditorLayer::AssignTestInitializer(std::function<void()> testInitializer)
		{
			EditorLayer::testInitializer = testInitializer;
		}

		/// private
		void EditorLayer::UI_DrawMenubar()
		{
			ImGui::BeginMenuBar();
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("LoadMap"))
				{
					cm.AddQueue(std::make_shared<LoadMapCommand>());
				}
				if (ImGui::MenuItem("SaveMap", "Ctrl + S"))
				{
					cm.AddQueue(std::make_shared<SaveMapCommand>());
				}
				if (ImGui::MenuItem("Exit"))
				{
					cm.AddQueue(std::make_shared<ProgramExitCommand>());
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Module"))
			{
				bool tde = editorModuleList[(int)Module_List::TemplateDataEditor]->GetActivation();
				if (ImGui::MenuItem("TemplateData Editor", "F2", tde))
				{
					if (tde)
					{
						editorModuleList[(int)Module_List::TemplateDataEditor]->SetActivation(false);
					}
					else
					{
						editorModuleList[(int)Module_List::TemplateDataEditor]->SetActivation(true);
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}
}
