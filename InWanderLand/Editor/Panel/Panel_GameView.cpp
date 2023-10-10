#include "Panel_GameView.h"

#include "imgui.h"

namespace Editor
{
	GameViewPanel* GameViewPanel::instance = nullptr;

	GameViewPanel& GameViewPanel::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new GameViewPanel();
		}

		return *instance;
	}

	GameViewPanel::~GameViewPanel()
	{
		delete instance;
	}

	void GameViewPanel::Initialize()
	{

	}

	void GameViewPanel::Update(float ts)
	{

	}

	void GameViewPanel::DrawPanel()
	{
		ImGui::Begin("GameView");
		
		/// ImGui ���� ���� ���� ������Ʈ
		isMouseOver = ImGui::IsWindowHovered();
		isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
		
		/// ���� �гο� �׸��� ����


		ImGui::End();
	}
	
	void GameViewPanel::Finalize()
	{

	}	

	GameViewPanel::GameViewPanel()
	{

	}
}
