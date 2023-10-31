/// 2023. 10. 04 �����
/// �����Ϳ��� ���õ� ī�޶� ������ View �� Ȯ���� �� �ִ� View �г�

#pragma once

#include "EditorPanel.h"

#include <memory>

namespace Application
{
	namespace Editor
	{
		class CameraViewPanel
			: public Panel
		{
		public:
			static CameraViewPanel& GetInstance();

			virtual ~CameraViewPanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;

		protected:
			static std::unique_ptr<CameraViewPanel> instance;

			CameraViewPanel();
			CameraViewPanel(const CameraViewPanel& copy) = delete;
			CameraViewPanel& operator=(const CameraViewPanel& copy) = delete;
		};
	}
}
