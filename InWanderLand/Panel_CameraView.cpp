#include "Panel_CameraView.h"

#include "imgui.h"

namespace application
{
	namespace editor
	{
		CameraViewPanel::CameraViewPanel()
		{

		}

		CameraViewPanel::~CameraViewPanel()
		{

		}

		void CameraViewPanel::Initialize()
		{

		}

		void CameraViewPanel::Update(float ts)
		{

		}

		void CameraViewPanel::GUIProgress()
		{
			ImGui::Begin("CameraView");

			/// ImGui ���� ���� ���� ������Ʈ
			isMouseOver = ImGui::IsWindowHovered();
			isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			/// ���� �гο� �׸��� ����


			ImGui::End();
		}

		void CameraViewPanel::Finalize()
		{

		}
	}
}
