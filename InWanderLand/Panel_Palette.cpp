#include "Panel_Palette.h"

#include "imgui.h"

namespace application
{
	namespace editor
	{
		PalettePanel::PalettePanel()
		{

		}

		PalettePanel::~PalettePanel()
		{

		}

		void PalettePanel::Initialize()
		{

		}

		void PalettePanel::Update(float ts)
		{

		}

		void PalettePanel::GUIProgress()
		{
			ImGui::Begin("Palette");

			/// ImGui ���� ���� ���� ������Ʈ
			isMouseOver = ImGui::IsWindowHovered();
			isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
			
			/// ���� �гο� �׸��� ����


			ImGui::End();
		}

		void PalettePanel::Finalize()
		{

		}
	}
}
