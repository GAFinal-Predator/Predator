/// 2023. 10. 04 �����
/// ������ Ư�� ��ü�� ���Ͽ� ���� ���� ������ Ȯ���� �� �ִ� �г�

#pragma once

#include "EditorPanel.h"

namespace Editor
{
	class InspectorPanel
		: public Panel
	{
	public:
		virtual ~InspectorPanel();

		virtual void Initialize() override;
		virtual void Update(float ts) override;
		virtual void DrawPanel() override;
		virtual void Finalize() override;
		
		static InspectorPanel& GetInstance();

	protected:
		InspectorPanel();
		InspectorPanel(const InspectorPanel& copy) = delete;
		InspectorPanel& operator=(const InspectorPanel& copy) = delete;

		static InspectorPanel* instance;
	};
}
