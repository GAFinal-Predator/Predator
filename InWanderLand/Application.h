/// 2023.10. 11 �����
/// Entry Point �� �Ǵ� main �Լ��� ����ȭ��Ű��, �۾� ������ �����Ͽ� ����ϱ� ����
/// ���μ����� ����ü ������ �ϴ� Ŭ���� �ۼ�

#pragma once

#include "Layer.h"
#include "CommandManager.h"

#include <vector>
#include <memory>
#include <functional>
#include <mutex>

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
        // ���� ������ ������ ������ ���� ������ �Լ��� ����մϴ�. ��ϵ� ������ �ѹ� ������ ������ϴ�.
        // AddMainLoopTodo �Լ��� ���� ������ ������ ���ؽ� ���� �̴ϴ�.
        // AddMainLoopTodo �Լ��� �Ű������� ��ϵ� ���� todo�� ����� �� ���� ���� ������ ���ؽ� ���� �̴ϴ�.
        void AddMainLoopTodo(std::function<void()> todo);

    private:
        std::mutex loopTodoRegistrationMutex;
        // AddMainLoopTodo�� ��ϵ� �ֹ߼� �ݹ� �Լ����Դϴ�.
        // �� ������ ����� �� �� �����̳ʿ� ���� ���۵��� ����ִٸ� ��� �����ϰ� ������ ���ϴ�.
        // �� ��Ͽ� ��� �Լ����� ����Ǵ� ���� ���� ���� ������� ������ �����մϴ�.
        std::vector<std::function<void()>> loopRegistrations;

        enum class LayerList
        {
            EditorLayer = 0,
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
