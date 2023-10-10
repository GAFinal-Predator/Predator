/// 2023. 10. 04 �����
/// ���ӿ��� ����ϴ� MainCamera �������� Ȯ���� �� �ִ� ȭ���� �����ִ� �г�

#pragma once

#include "EditorPanel.h"

namespace Editor
{
	class GameViewPanel
		: public Panel
	{
	public:
		virtual ~GameViewPanel();

		virtual void Initialize() override;
		virtual void Update(float ts) override;
		virtual void DrawPanel() override;
		virtual void Finalize() override;
		
		static GameViewPanel& GetInstance();

	protected:
		GameViewPanel();
		GameViewPanel(const GameViewPanel& copy) = delete;
		GameViewPanel& operator=(const GameViewPanel& copy) = delete;

		static GameViewPanel* instance;
	};
}
