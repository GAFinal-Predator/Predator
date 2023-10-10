#include "Panel_Inspector.h"

#include "imgui.h"

namespace Editor
{
	InspectorPanel* InspectorPanel::instance = nullptr;

	InspectorPanel& InspectorPanel::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new InspectorPanel();
		}

		return *instance;
	}

	InspectorPanel::~InspectorPanel()
	{
		delete instance;
	}

	void InspectorPanel::Initialize()
	{

	}

	void InspectorPanel::Update(float ts)
	{

	}

	void InspectorPanel::DrawPanel()
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
