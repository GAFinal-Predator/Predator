/// 2023. 11. 01 �����
/// Hierarchy ���� ������ ��� ���� ������ ������ �� ���� �����ִ� �г�

#pragma once

#include "EditorPanel.h"

#include <memory>

namespace application
{
	namespace editor
	{
		class PreviewPanel
			: public Panel
		{
		public:
			static PreviewPanel& GetInstance();

			virtual ~PreviewPanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;

		protected:
			static std::unique_ptr<PreviewPanel> instance;

			PreviewPanel();
			PreviewPanel(const PreviewPanel& copy) = delete;
			PreviewPanel& operator=(const PreviewPanel& copy) = delete;
		};
	}
}

