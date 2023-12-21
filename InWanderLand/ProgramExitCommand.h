/// 2023. 10. 20 �����
/// ���ø����̼� ���Ḧ ���� Ŀ�ǵ�

#pragma once

#include "Command.h"

namespace application
{
	class Application;
}

namespace application
{
	namespace editor
	{
		class ProgramExitCommand
			: public Command
		{
		public:
			ProgramExitCommand();

			virtual void Execute() override;
			
		private:
			Application& app;
		};
	}
}

