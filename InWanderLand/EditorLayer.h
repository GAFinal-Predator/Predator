/// 2023. 10. 05 �����
/// �����Ϳ��� ����� ��� ������ ��Ƶ� Ŭ����
/// �ش� ���̾ Release ��忡�� ������� �ʴ� ������
/// Release ��忡���� �����Ϳ� ���� ������ �Ұ��� �ϵ��� ��

#pragma once

#include <vector>
#include <memory>

#include "Layer.h"
#include "CommandManager.h"
#include "EditorPanel.h"

namespace Application
{
	namespace Editor
	{
		class EditorLayer
			: public Layer
		{
		public:
			virtual void Initialize() override;
			// virtual void EventProgress(Events& e) override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;

		private:
			enum class Panel_List
			{
				HIERARCHY	= 0,
				INSPECTOR,
				MINIMAP,
				SCENEVIEW,
				CAMERAVIEW
			};

			void UI_DrawMenubar();

			CommandManager& cm = CommandManager::GetInstance();
			std::vector<Panel*> editorPanelList;
		};
	}
}
