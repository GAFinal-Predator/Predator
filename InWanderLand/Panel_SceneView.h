/// 2023. 10. 04 �����
/// �����Ϳ��� ������ ���� ������ Ȯ���� �� �ִ� View �г�

#pragma once

#include "EditorPanel.h"

#include <memory>

namespace Application
{
	namespace Editor
	{
		class SceneViewPanel
			: public Panel
		{
		public:
			static SceneViewPanel& GetInstance();

			virtual ~SceneViewPanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;

		protected:
			static std::unique_ptr<SceneViewPanel> instance;

			SceneViewPanel();
			SceneViewPanel(const SceneViewPanel& copy) = delete;
			SceneViewPanel& operator=(const SceneViewPanel& copy) = delete;
		};
	}
}
