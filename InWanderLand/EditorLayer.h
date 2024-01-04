/// 2023. 10. 05 �����
/// �����Ϳ��� ����� ��� ������ ��Ƶ� Ŭ����
/// �ش� ���̾ Release ��忡�� ������� �ʴ� ������
/// Release ��忡���� �����Ϳ� ���� ������ �Ұ��� �ϵ��� ��

#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <memory>

#include "Layer.h"
#include "CommandManager.h"
#include "EditorPanel.h"
#include "EditorModule.h"

namespace application
{
	namespace editor
	{
		class EditorLayer
			: public Layer
		{
		public:
			static void AssignTestInitializer(std::function<void()> testInitializer);

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
				PREVIEW,
				MINIMAP,
				SCENEVIEW,
				CAMERAVIEW,
				PALETTE,

				/// Size�� �ڵ����� �ֱ� ���� ����ϴ� enum
				/// ù enum ���� 0 �̰�, ��� ������ 1�� ���� ������
				Size
			};

			enum class Module_List
			{
				TemplateDataEditor	= 0,

				/// Size�� �ڵ����� �ֱ� ���� ����ϴ� enum
				/// ù enum ���� 0 �̰�, ��� ������ 1�� ���� ������
				Size
			};


			static std::function<void()> testInitializer;

			void UI_DrawMenubar();

			CommandManager& cm = CommandManager::GetSingletonInstance();
			std::vector<std::unique_ptr<Panel>> editorPanelList;
			std::vector<std::unique_ptr<EditorModule>> editorModuleList;
		};
	}
}
