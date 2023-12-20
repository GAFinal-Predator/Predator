/// 2023. 10. 04 �����
/// ������ Ư�� ��ü�� ���Ͽ� ���� ���� ������ Ȯ���� �� �ִ� �г�

#pragma once

#include "EditorPanel.h"

#include <memory>

namespace application
{
	namespace editor
	{
		class InspectorPanel
			: public Panel
		{
		public:
			static InspectorPanel& GetInstance();

			virtual ~InspectorPanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;

		protected:
			static std::unique_ptr<InspectorPanel> instance;

			InspectorPanel();
			InspectorPanel(const InspectorPanel& copy) = delete;
			InspectorPanel& operator=(const InspectorPanel& copy) = delete;
		};
	}
}
