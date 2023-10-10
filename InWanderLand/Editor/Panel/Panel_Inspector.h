/// 2023. 10. 04 김상준
/// 선택한 특정 객체에 대하여 상세한 편집 내용을 확인할 수 있는 패널

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
