#include "Panel_MiniMap.h"

#include "imgui.h"

namespace application
{
	namespace editor
	{
		MiniMapPanel::MiniMapPanel()
		{

		}

		MiniMapPanel::~MiniMapPanel()
		{

		}

		void MiniMapPanel::Initialize()
		{

		}

		void MiniMapPanel::Update(float ts)
		{

		}

		void MiniMapPanel::GUIProgress()
		{
			ImGui::Begin("MiniMap");

			/// ImGui ���� ���� ���� ������Ʈ
			isMouseOver = ImGui::IsWindowHovered();
			isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			/// ���� �гο� �׸��� ����


			ImGui::End();
		}

		void MiniMapPanel::Finalize()
		{

		}
	}
}
