/// 2023. 10. 04 �����
/// ���� ������ ���� ������ Ȯ���ϴ� �г�

#pragma once

#include "Singleton.h"
#include "EditorPanel.h"

#include <memory>

namespace application
{
	namespace editor
	{
		class HierarchyPanel
			: public Panel, public Singleton<HierarchyPanel>
		{
		public:
			HierarchyPanel();
			virtual ~HierarchyPanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;
		};
	}
}
