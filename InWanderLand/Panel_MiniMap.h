/// 2023. 10. 04 �����
/// SceneView �г��� ��ü �������� ���� Ȯ���ϰ� �ִ� �κ��� �� ���� �ľ��� �� �ִ�
/// MiniMap �� �������ִ� �г�

#pragma once

#include "EditorPanel.h"

#include <memory>

namespace application
{
	namespace editor
	{
		class MiniMapPanel
			: public Panel
		{
		public:
			static MiniMapPanel& GetInstance();

			virtual ~MiniMapPanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;

		protected:
			static std::unique_ptr<MiniMapPanel> instance;

			MiniMapPanel();
			MiniMapPanel(const MiniMapPanel& copy) = delete;
			MiniMapPanel& operator=(const MiniMapPanel& copy) = delete;
		};
	}
}
