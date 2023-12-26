/// 2023. 10. 04 �����
/// SceneView �г��� ��ü �������� ���� Ȯ���ϰ� �ִ� �κ��� �� ���� �ľ��� �� �ִ�
/// MiniMap �� �������ִ� �г�

#pragma once

#include "Singleton.h"
#include "EditorPanel.h"

#include <memory>

namespace application
{
	namespace editor
	{
		class MiniMapPanel
			: public Panel, public Singleton<MiniMapPanel>
		{
		public:
			MiniMapPanel();
			virtual ~MiniMapPanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;
		};
	}
}
