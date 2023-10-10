/// 2023. 10. 04 �����
/// SceneView �г��� ��ü �������� ���� Ȯ���ϰ� �ִ� �κ��� �� ���� �ľ��� �� �ִ�
/// MiniMap �� �������ִ� �г�

#pragma once

#include "EditorPanel.h"

namespace Editor
{
	class MiniMapPanel
		: public Panel
	{
	public:
		virtual ~MiniMapPanel();

		virtual void Initialize() override;
		virtual void Update(float ts) override;
		virtual void DrawPanel() override;
		virtual void Finalize() override;
		
		static MiniMapPanel& GetInstance();

	protected:
		MiniMapPanel();
		MiniMapPanel(const MiniMapPanel& copy) = delete;
		MiniMapPanel& operator=(const MiniMapPanel& copy) = delete;

		static MiniMapPanel* instance;
	};
}
