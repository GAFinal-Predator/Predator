#include "Panel_Hierarchy.h"

#include "imgui.h"

namespace Editor
{
	HierarchyPanel* HierarchyPanel::instance = nullptr;

	HierarchyPanel& HierarchyPanel::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new HierarchyPanel();
		}

		return *instance;
	}

	HierarchyPanel::~HierarchyPanel()
	{
		delete instance;
	}

	void HierarchyPanel::Initialize()
	{

	}

	void HierarchyPanel::Update(float ts)
	{

	}

	void HierarchyPanel::DrawPanel()
	{
		ImGui::Begin("Hierarchy");
		
		/// ImGui ���� ���� ���� ������Ʈ
		isMouseOver = ImGui::IsWindowHovered();
		isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
		
		/// ���� �гο� �׸��� ����


		ImGui::End();
	}
	
	void HierarchyPanel::Finalize()
	{

	}	

	HierarchyPanel::HierarchyPanel()
	{

	}
}
