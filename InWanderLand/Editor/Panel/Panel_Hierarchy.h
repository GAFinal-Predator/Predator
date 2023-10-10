/// 2023. 10. 04 �����
/// ���� ������ ���� ������ Ȯ���ϴ� �г�

#pragma once

#include "EditorPanel.h"

namespace Editor
{
	class HierarchyPanel
		: public Panel
	{
	public:
		static HierarchyPanel& GetInstance();

		virtual ~HierarchyPanel();

		virtual void Initialize() override;
		virtual void Update(float ts) override;
		virtual void DrawPanel() override;
		virtual void Finalize() override;

	protected:
		static HierarchyPanel* instance;

		HierarchyPanel();
		HierarchyPanel(const HierarchyPanel& copy) = delete;
		HierarchyPanel& operator=(const HierarchyPanel& copy) = delete;
	};
}
