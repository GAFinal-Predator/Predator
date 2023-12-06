#include "Panel_SceneView.h"

#include "imgui.h"

namespace Application
{
	namespace Editor
	{
		std::unique_ptr<SceneViewPanel> SceneViewPanel::instance = nullptr;

		SceneViewPanel& SceneViewPanel::GetInstance()
		{
			if (instance == nullptr)
			{
				instance = std::unique_ptr<SceneViewPanel>(new SceneViewPanel());
			}

			return *instance;
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
			ImGui::Begin("SceneView");

			/// ImGui ���� ���� ���� ������Ʈ
			isMouseOver = ImGui::IsWindowHovered();
			isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			/// ���� �гο� �׸��� ����


			ImGui::End();
		}

		void SceneViewPanel::Finalize()
		{

		}

		SceneViewPanel::SceneViewPanel()
		{

		}
	}
}
