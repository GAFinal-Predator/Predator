#include "Panel_Hierarchy.h"

#include "imgui.h"

namespace Application
{
	namespace Editor
	{
		std::unique_ptr<HierarchyPanel> HierarchyPanel::instance = nullptr;

		HierarchyPanel& HierarchyPanel::GetInstance()
		{
			if (instance == nullptr)
			{
				instance = std::unique_ptr<HierarchyPanel>(new HierarchyPanel());
			}

			return *instance;
		}

		HierarchyPanel::~HierarchyPanel()
		{
		
		}

		void HierarchyPanel::Initialize()
		{

		}

		void HierarchyPanel::Update(float ts)
		{

		}

		void HierarchyPanel::GUIProgress()
		{
			ImGui::Begin("Hierarchy");

			/// ImGui ���� ���� ���� ������Ʈ
			isMouseOver = ImGui::IsWindowHovered();
			isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			/// ���� �гο� �׸��� ����


			ImGui::End();
		}

		void HierarchyPanel::Finalize()
		{

		}

		HierarchyPanel::HierarchyPanel()
		{

		}
	}
}
