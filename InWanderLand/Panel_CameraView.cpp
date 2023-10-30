#include "Panel_CameraView.h"

#include "imgui.h"

namespace Application
{
	namespace Editor
	{
		std::unique_ptr<CameraViewPanel> CameraViewPanel::instance = nullptr;

		CameraViewPanel& CameraViewPanel::GetInstance()
		{
			if (instance == nullptr)
			{
				instance = std::unique_ptr<CameraViewPanel>(new CameraViewPanel());
			}

			return *instance;
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

		CameraViewPanel::CameraViewPanel()
		{

		}
	}
}
