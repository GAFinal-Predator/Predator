#include "Panel_SceneView.h"

#include "imgui.h"

namespace Editor
{
	SceneViewPanel* SceneViewPanel::instance = nullptr;

	SceneViewPanel& SceneViewPanel::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new SceneViewPanel();
		}

		return *instance;
	}

	SceneViewPanel::~SceneViewPanel()
	{
		delete instance;
	}

	void SceneViewPanel::Initialize()
	{

	}

	void SceneViewPanel::Update(float ts)
	{

	}

	void SceneViewPanel::DrawPanel()
	{
		ImGui::Begin("SceneView");
		
		/// ImGui ���� ���� ���� ������Ʈ
		isMouseOver = ImGui::IsWindowHovered();
		isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
		
		/// ���� �гο� �׸��� ����


		ImGui::End();
	}
	
	void SceneViewPanel::Finalize()
	{

	}	

	SceneViewPanel::SceneViewPanel()
	{

	}
}
