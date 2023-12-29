/// 2023. 10. 04 �����
/// �����Ϳ��� ����� �гε��� ���� ��Ҹ� ��Ƶ� �߻� Ŭ����

#pragma once

#include "CommandManager.h"

#include <memory>

namespace application
{
	namespace editor
	{
		class Panel
		{
		public:
			virtual ~Panel() = default;

			virtual void Initialize() = 0;
			//virtual void EventProgress(Events& e) = 0;
			virtual void Update(float ts) = 0;
			virtual void GUIProgress() = 0;
			virtual void Finalize() = 0;

			inline bool IsPanelMouseOver() { return isMouseOver; }
			inline bool IsPanelFocused() { return isFocused; }

		protected:
			bool isMouseOver = false;
			bool isFocused = false;

			CommandManager& commandManager = CommandManager::GetSingletonInstance();
		};
	}
}
