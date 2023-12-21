/// 2023. 10. 18 �����
/// �����Ϳ��� Ư���� ��ɿ� ���� �ϰ� ó���� �� �ִ� ������ Ž���ϴ� ��
/// ����� ȣ���ϴ� ȣ���ڿ� �̸� �����ϴ� ��ü�� �и��Ͽ� ����ϴ� ������
/// ��� ����(Command Pattern)�� �߰��Ͽ� �̸� ����ϱ���Ͽ� ���� Ŭ����
/// 
/// Command Ŭ������ '�������̽�'��, �ش� Ŭ������ ��ӹ޾� ��üȭ��
/// ���� Command Ŭ�������� ����� ó���ϵ��� ��

#pragma once

#include <memory>

namespace application
{
	namespace editor
	{
		class Command
		{
		public:
			virtual void Execute() = 0;	// �ش� Ŀ�ǵ带 ������
		};
		
		/// Undo / Redo ������ Command �� �����Ͽ� ó���� �� �ֵ��� �����ϱ� ����
		/// ���� �� �ܰ� �߻�ȭ�� ����
		class UndoableCommand 
			: public Command
		{
		public:
			virtual void Execute() = 0;
			virtual void Undo() = 0;	// �ش� Ŀ�ǵ� ������ ���� ���·� �ǵ���
		};

		/// Ư���� ���� ��û ���� �ܼ��� ó���� �� �ִ� ��ɿ� ���ؼ� ó���ϱ� ����
		/// �ܼ��� ������ Ŀ�ǵ� ���ø� �ۼ�
		template <typename Receiver>
		class SimpleCommands
			: public Command
		{
		public:
			using Action = void (Receiver::*)();

			SimpleCommands(Receiver* receiver, Action action)
				: receiver(receiver), action(action)
			{

			}

			virtual void Execute()
			{
				(receiver->*action)();
			}

		private:
			Action action;		// Execute �ÿ� ����� �ܼ��� �Լ�
			Receiver* receiver;	// Ŀ�ǵ��� ����� �� ������
		};
	}
}


