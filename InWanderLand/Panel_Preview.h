/// 2023. 11. 01 �����
/// Hierarchy ���� ������ ��� ���� ������ ������ �� ���� �����ִ� �г�

#pragma once

#include "Singleton.h"
#include "EditorPanel.h"

#include <memory>

namespace application
{
	namespace editor
	{
		class PreviewPanel
			: public Panel, public Singleton<PreviewPanel>
		{
		public:
			PreviewPanel();
			virtual ~PreviewPanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;
		};
	}
}

