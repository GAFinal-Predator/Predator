/// 2023. 10. 04 �����
/// ������ Ư�� ��ü�� ���Ͽ� ���� ���� ������ Ȯ���� �� �ִ� �г�

#pragma once

#include "Singleton.h"
#include "EditorPanel.h"

#include <memory>

namespace application
{
	namespace editor
	{
		class InspectorPanel
			: public Panel, public Singleton<InspectorPanel>
		{
		public:
			InspectorPanel();
			virtual ~InspectorPanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;
		};
	}
}
