#include "Panel_Preview.h"

#include "imgui.h"

namespace application
{
	namespace editor
	{
		PreviewPanel::PreviewPanel()
		{

		}

		PreviewPanel::~PreviewPanel()
		{

		}

		void PreviewPanel::Initialize()
		{

		}

		void PreviewPanel::Update(float ts)
		{

		}

		void PreviewPanel::GUIProgress()
		{
			ImGui::Begin("Preview");

			/// ImGui ���� ���� ���� ������Ʈ
			isMouseOver = ImGui::IsWindowHovered();
			isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			/// ���� �гο� �׸��� ����


			ImGui::End();
		}

		void PreviewPanel::Finalize()
		{

		}
	}
}
