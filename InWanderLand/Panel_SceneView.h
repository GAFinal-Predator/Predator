/// 2023. 10. 04 �����
/// �����Ϳ��� ������ ���� ������ Ȯ���� �� �ִ� View �г�

#pragma once

#include "Singleton.h"
#include "EditorPanel.h"

namespace application
{
	namespace editor
	{
		class SceneViewPanel
			: public Panel, public Singleton<SceneViewPanel>
		{
		public:
			SceneViewPanel();
			virtual ~SceneViewPanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;
		};
	}
}
