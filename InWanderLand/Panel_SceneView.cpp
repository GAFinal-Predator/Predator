#include "Panel_SceneView.h"

#include "imgui.h"

namespace application
{
	namespace editor
	{
		SceneViewPanel::SceneViewPanel()
		{

		}

		SceneViewPanel::~SceneViewPanel()
		{

		}

		void SceneViewPanel::Initialize()
		{

		}

		void SceneViewPanel::Update(float ts)
		{

		}

		void SceneViewPanel::GUIProgress()
		{
			ImGui::Begin("SceneView", 0, ImGuiWindowFlags_NoBringToFrontOnFocus);

			/// ImGui ���� ���� ���� ������Ʈ
			isMouseOver = ImGui::IsWindowHovered();
			isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			/// ���� �гο� �׸��� ����


			ImGui::End();
		}

		void SceneViewPanel::Finalize()
		{

		}
	}
}
