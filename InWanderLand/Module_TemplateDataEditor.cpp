#include "Module_TemplateDataEditor.h"

#include "imgui.h"

namespace application
{
	namespace editor
	{
		Module_TemplateDataEditor::Module_TemplateDataEditor()
		{

		}

		Module_TemplateDataEditor::~Module_TemplateDataEditor()
		{

		}

		void Module_TemplateDataEditor::Initialize()
		{

		}

		void Module_TemplateDataEditor::Update(float ts)
		{

		}

		void Module_TemplateDataEditor::GUIProgress()
		{
			CheckActivation();

			ImGui::Begin("TemplateData Editor", &activation);

			/// ImGui ���� ���� ���� ������Ʈ
			isMouseOver = ImGui::IsWindowHovered();
			isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			/// ���� �гο� �׸��� ����


			ImGui::End();
		}

		void Module_TemplateDataEditor::Finalize()
		{

		}
	}
}

