/// 2023. 10. 04 김상준
/// 에디터에서 사용할 패널들의 공통 요소를 모아둔 추상 클래스

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
