#include "Panel_Inspector.h"

#include "imgui.h"

namespace application
{
	namespace editor
	{
		InspectorPanel::InspectorPanel()
		{

		}

		InspectorPanel::~InspectorPanel()
		{

		}

		void InspectorPanel::Initialize()
		{

		}

		void InspectorPanel::Update(float ts)
		{

		}

		void InspectorPanel::GUIProgress()
		{
			ImGui::Begin("Inspector", 0, ImGuiWindowFlags_NoBringToFrontOnFocus);

			/// ImGui ���� ���� ���� ������Ʈ
			isMouseOver = ImGui::IsWindowHovered();
			isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			/// ���� �гο� �׸��� ����


			ImGui::End();
		}

		void InspectorPanel::Finalize()
		{

		}
	}
}
