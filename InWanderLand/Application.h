/// 2023.10. 11 �����
/// Entry Point �� �Ǵ� main �Լ��� ����ȭ��Ű��, �۾� ������ �����Ͽ� ����ϱ� ����
/// ���μ����� ����ü ������ �ϴ� Ŭ���� �ۼ�

#pragma once

#include "Layer.h"
#include "CommandManager.h"

#include <vector>
#include <memory>

namespace Application
{
	class Application
	{
	public:
		static Application& CreateApplication(int argc, char** argv);
		static Application& GetInstance();

		~Application();

		/// �ʼ� ���
		void Initialize();		// �ʱ�ȭ
		void Run();				// ����(����)
		void Finalize();		// ������

		/// ��� ���� ���
		void TurnOff();			// Run ������ ��, ������ Ż����

	private:
		enum class LayerList
		{
			EditorLayer	= 0,
			ContentsLayer,
		};

		static std::unique_ptr<Application> instance;

		Application(int argc, char** argv);
		Application(const Application& app) = delete;
		Application& operator=(const Application& app) = delete;

		bool isRunning = false;
		std::vector<Layer*> layers;

#ifdef _DEBUG
		Editor::CommandManager& cm = Editor::CommandManager::GetInstance();
#endif
	};
}
