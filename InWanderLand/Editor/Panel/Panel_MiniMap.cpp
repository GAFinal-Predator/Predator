#include "Panel_MiniMap.h"

#include "imgui.h"

namespace Editor
{
	MiniMapPanel* MiniMapPanel::instance = nullptr;

	MiniMapPanel& MiniMapPanel::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new MiniMapPanel();
		}

		return *instance;
	}

	MiniMapPanel::~MiniMapPanel()
	{
		delete instance;
	}

	void MiniMapPanel::Initialize()
	{

	}

	void MiniMapPanel::Update(float ts)
	{

	}

	void MiniMapPanel::DrawPanel()
	{
		ImGui::Begin("MiniMap");
		
		/// ImGui ���� ���� ���� ������Ʈ
		isMouseOver = ImGui::IsWindowHovered();
		isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
		
		/// ���� �гο� �׸��� ����


		ImGui::End();
	}
	
	void MiniMapPanel::Finalize()
	{

	}	

	MiniMapPanel::MiniMapPanel()
	{

	}
}
