/// 2023. 10. 19 �����
/// Ŀ�ǵ���� �ϰ������� ó���ϱ� ���� ������ Ŭ����

#pragma once

#include "Command.h"

#include <memory>
#include <queue>
#include <deque>
#include <stack>
#include <concepts>

namespace application
{
	namespace editor
	{
		class CommandManager
		{
		public:
			static CommandManager& GetInstance();

			virtual ~CommandManager();
			
			// commandQueue �� Ŀ�ǵ带 �߰�
			template<class T, class... Types> requires (std::is_base_of_v<Command, T> && !std::is_base_of_v<UndoableCommand, T>)
			void AddQueue(const std::shared_ptr<T>& command)
			{
				commandQueue.emplace(command);
			}

			// Undo / Redo ������ ���� ����� ���� �����ϱ� ���� �Լ�
			/// AddQueue �Լ� ���ο��� dynamic_cast �� ���ؼ� ������ ���� ������,
			/// UndoableCommand �� ���ؼ� ��� Command �� dynamic_cast �� �ϴ� ������ ��ȿ�����̶� �Ǵ�,
			/// ó�� ���� �������� UndoableCommand �� ���� ������ �� �ֵ��� ó����
			template<class T> requires std::derived_from<T, UndoableCommand>
			void AddQueue(const std::shared_ptr<T>& command)
			{
				commandQueue.emplace(command);
				undoQueue.emplace_back(command);

				ResizeBuffer();
			}

			void ExecuteCommands();											// commandQueue �� ������ ��� ������

			void UndoCommand();												// ���������� �����ߴ� Ŀ�ǵ带 ����ϰ� �ǵ���
			void RedoCommand();												// ���� ����ߴ� Ŀ�ǵ带 �ٽ� ������

		private:
			static std::unique_ptr<CommandManager> instance;

			CommandManager();
			CommandManager(const CommandManager& app) = delete;
			CommandManager& operator=(const CommandManager& app) = delete;

			void ResizeBuffer();											// undoQueue �� redoStack ����� ��ģ bufferSize �� ��ĥ �� �����ϴ� �Լ�

			int bufferSize = 50;
			std::queue<std::shared_ptr<Command>> commandQueue;
			std::deque<std::shared_ptr<UndoableCommand>> undoQueue;
			std::stack<std::shared_ptr<UndoableCommand>> redoStack;
		};
	}
}
