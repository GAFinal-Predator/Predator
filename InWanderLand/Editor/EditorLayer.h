/// 2023. 10. 05 �����
/// �����Ϳ��� ����� ��� ������ ��Ƶ� Ŭ����
/// �ش� ���̾ Release ��忡�� ������� �ʴ� ������
/// Release ��忡���� �����Ϳ� ���� ������ �Ұ��� �ϵ��� ��

#pragma once

#include <vector>

#include "Panel/EditorPanel.h"

namespace Editor
{
	class EditorLayer
	{
	public:
		void Initialize();
		void Update(float ts);
		void Render();
		void Finalize();
		//void OnEvent(Events& e);

	private:
		std::vector<Panel*> editorPanelList;
	};
}
