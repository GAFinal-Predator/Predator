/// 2023. 12. 08 �����
/// �������� ���� �� ���¸� �����ϴ� Ŀ�ǵ�

#pragma once

#include "Command.h"

namespace application
{
	namespace editor
	{
		class MapFileManager;
	}
}

namespace application
{
	namespace editor
	{
		class SaveMapCommand
			: public Command
		{
		public:
			SaveMapCommand();

			virtual void Execute() override;

		private:
			MapFileManager& mapFileManager;
		};
	}
}

