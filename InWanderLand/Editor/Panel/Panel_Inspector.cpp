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
		
		/// ImGui 관련 내부 변수 업데이트
		isMouseOver = ImGui::IsWindowHovered();
		isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
		
		/// 실제 패널에 그리는 영역


		ImGui::End();
	}
	
	void InspectorPanel::Finalize()
	{

	}	

	InspectorPanel::InspectorPanel()
	{

	}
}
