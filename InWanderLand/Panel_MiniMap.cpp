#include "Panel_MiniMap.h"

#include "imgui.h"

namespace application
{
	namespace editor
	{
		std::unique_ptr<MiniMapPanel> MiniMapPanel::instance = nullptr;

		MiniMapPanel& MiniMapPanel::GetInstance()
		{
			if (instance == nullptr)
			{
				instance = std::unique_ptr<MiniMapPanel>(new MiniMapPanel());
			}

			return *instance;
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

		MiniMapPanel::MiniMapPanel()
		{

		}
	}
}
