/// 2023. 10. 20 �����
/// ���ø����̼� ���Ḧ ���� Ŀ�ǵ�

#pragma once

#include "Command.h"

namespace Application
{
	class Application;
}

namespace Application
{
	namespace Editor
	{
		class ProgramExitCommand
			: public Command
		{
		public:
			ProgramExitCommand(Application& app);

			virtual void Execute() override;
			
		private:
			Application& app;
		};
	}
}

