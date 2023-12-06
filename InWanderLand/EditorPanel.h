/// 2023. 10. 04 �����
/// �����Ϳ��� ����� �гε��� ���� ��Ҹ� ��Ƶ� �߻� Ŭ����

#pragma once

#include "CommandManager.h"

#include <memory>

namespace Application
{
	namespace Editor
	{
		class Panel
		{
		public:
			virtual void Initialize() = 0;
			//virtual void EventProgress(Events& e) = 0;
			virtual void Update(float ts) = 0;
			virtual void GUIProgress() = 0;
			virtual void Finalize() = 0;

			bool IsPanelMouseOver() { return isMouseOver; }
			bool IsPanelFocused() { return isFocused; }

		protected:
			bool isMouseOver = false;
			bool isFocused = false;

			CommandManager& commandManager = CommandManager::GetInstance();
		};
	}
}
