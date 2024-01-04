#include "EditorLayer.h"

#include "MapFileManager.h"
#include "EditorPanel.h"
#include "PanelList.h"
#include "ModuleList.h"

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
			/// Test ȯ���� ���� Initialize ����
			if (EditorLayer::testInitializer)
			{
				EditorLayer::testInitializer();
				return;
			}

			/// ���� �Ŵ��� Ŭ���� �޸� �Ҵ�
			MapFileManager::GetSingletonInstance();

			/// �����Ϳ��� ����� ������ ī�޶�, ���Ӻ� ī�޶� ����

			/// ������ �г� ���� �� �ʱ�ȭ ����
			editorPanelList.resize((int)Panel_List::Size);

			editorPanelList[(int)Panel_List::HIERARCHY] = std::unique_ptr<Panel>(&HierarchyPanel::GetSingletonInstance());
			editorPanelList[(int)Panel_List::INSPECTOR] = std::unique_ptr<Panel>(&InspectorPanel::GetSingletonInstance());
			editorPanelList[(int)Panel_List::PREVIEW] = std::unique_ptr<Panel>(&PreviewPanel::GetSingletonInstance());
			editorPanelList[(int)Panel_List::MINIMAP] = std::unique_ptr<Panel>(&MiniMapPanel::GetSingletonInstance());
			editorPanelList[(int)Panel_List::SCENEVIEW] = std::unique_ptr<Panel>(&SceneViewPanel::GetSingletonInstance());
			editorPanelList[(int)Panel_List::CAMERAVIEW] = std::unique_ptr<Panel>(&CameraViewPanel::GetSingletonInstance());
			editorPanelList[(int)Panel_List::PALETTE] = std::unique_ptr<Panel>(&PalettePanel::GetSingletonInstance());

			/// ������ ��� ���� �� �ʱ�ȭ ����
			editorModuleList.resize((int)Module_List::Size);

			editorModuleList[(int)Module_List::TemplateDataEditor] = std::unique_ptr<EditorModule>(&Module_TemplateDataEditor::GetSingletonInstance());

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
