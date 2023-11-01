#include "Panel_Preview.h"

#include "imgui.h"

namespace Application
{
	namespace Editor
	{
		std::unique_ptr<PreviewPanel> PreviewPanel::instance = nullptr;

		PreviewPanel& PreviewPanel::GetInstance()
		{
			if (instance == nullptr)
			{
				instance = std::unique_ptr<PreviewPanel>(new PreviewPanel());
			}

			return *instance;
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

		PreviewPanel::PreviewPanel()
		{

		}
	}
}
