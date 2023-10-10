/// 2023. 10. 04 �����
/// �����Ϳ��� ����� �гε��� ���� ��Ҹ� ��Ƶ� �߻� Ŭ����

#pragma once

#include <memory>

namespace Editor
{
	class Panel
	{
	public:
		enum class Panel_List
		{
			HIERARCHY,
			INSPECTOR,
			MINIMAP,
			SCENEVIEW,
			GAMEVIEW
		};

		virtual void Initialize() = 0;
		virtual void Update(float ts) = 0;
		virtual void DrawPanel() = 0;
		virtual void Finalize() = 0;
		//virtual void OnEvent(Events& e) = 0;

		bool IsPanelMouseOver() { return isMouseOver; }
		bool IsPanelFocused() { return isFocused; }

	protected:
		bool isMouseOver = false;
		bool isFocused = false;
	};
}
