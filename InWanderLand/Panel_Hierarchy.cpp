#include "InWanderLand.h"
#include "Panel_Hierarchy.h"

#include "imgui.h"

namespace application
{
	namespace editor
	{	
		HierarchyPanel::HierarchyPanel()
		{

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
			ImGui::Begin("Hierarchy", 0, ImGuiWindowFlags_NoBringToFrontOnFocus);

			/// ImGui ���� ���� ���� ������Ʈ
			isMouseOver = ImGui::IsWindowHovered();
			isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			/// ���� �гο� �׸��� ����


			ImGui::End();
		}

		void HierarchyPanel::Finalize()
		{

		}
	}
}
