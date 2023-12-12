/// 2023. 12. 08 �����
/// �������� ���� �� ���¸� �����ϴ� Ŀ�ǵ�

#pragma once

#include "Command.h"

namespace Application
{
	namespace Editor
	{
		class MapFileManager;
	}
}

namespace Application
{
	namespace Editor
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

