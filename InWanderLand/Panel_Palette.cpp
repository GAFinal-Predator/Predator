#include "Panel_Palette.h"

#include "imgui.h"

namespace Application
{
	namespace Editor
	{
		std::unique_ptr<PalettePanel> PalettePanel::instance = nullptr;

		PalettePanel& PalettePanel::GetInstance()
		{
			if (instance == nullptr)
			{
				instance = std::unique_ptr<PalettePanel>(new PalettePanel());
			}

			return *instance;
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

		PalettePanel::PalettePanel()
		{

		}
	}
}
