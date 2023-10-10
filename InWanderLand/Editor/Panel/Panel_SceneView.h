/// 2023. 10. 04 �����
/// �����Ϳ��� ������ ���� ������ Ȯ���� �� �ִ� View �г�

#pragma once

#include "EditorPanel.h"

namespace Editor
{
	class SceneViewPanel
		: public Panel
	{
	public:
		virtual ~SceneViewPanel();

		virtual void Initialize() override;
		virtual void Update(float ts) override;
		virtual void DrawPanel() override;
		virtual void Finalize() override;
		
		static SceneViewPanel& GetInstance();

	protected:
		SceneViewPanel();
		SceneViewPanel(const SceneViewPanel& copy) = delete;
		SceneViewPanel& operator=(const SceneViewPanel& copy) = delete;

		static SceneViewPanel* instance;
	};
}
