#include "Panel_Inspector.h"

#include "imgui.h"

namespace application
{
	namespace editor
	{
		std::unique_ptr<InspectorPanel> InspectorPanel::instance = nullptr;

		InspectorPanel& InspectorPanel::GetInstance()
		{
			if (instance == nullptr)
			{
				instance = std::unique_ptr<InspectorPanel>(new InspectorPanel());
			}

			return *instance;
		}

		InspectorPanel::~InspectorPanel()
		{

		}

		void InspectorPanel::Initialize()
		{

		}

		void InspectorPanel::Update(float ts)
		{

		}

		void InspectorPanel::GUIProgress()
		{
			ImGui::Begin("Inspector");

			/// ImGui ���� ���� ���� ������Ʈ
			isMouseOver = ImGui::IsWindowHovered();
			isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			/// ���� �гο� �׸��� ����


			ImGui::End();
		}

		void InspectorPanel::Finalize()
		{

		}

		InspectorPanel::InspectorPanel()
		{

		}
	}
}
