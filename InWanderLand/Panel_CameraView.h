/// 2023. 10. 04 �����
/// �����Ϳ��� ���õ� ī�޶� ������ View �� Ȯ���� �� �ִ� View �г�

#pragma once

#include "Singleton.h"
#include "EditorPanel.h"

#include <memory>

namespace application
{
	namespace editor
	{
		class CameraViewPanel
			: public Panel, public Singleton<CameraViewPanel>
		{
		public:
			CameraViewPanel();
			virtual ~CameraViewPanel();

			virtual void Initialize() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;
		};
	}
}
