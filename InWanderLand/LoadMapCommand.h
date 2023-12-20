/// 2023. 12. 08 �����
/// �� ������ �����Ϳ� �ҷ����� Ŀ�ǵ�

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
		class LoadMapCommand
			: public Command
		{
		public:
			LoadMapCommand();

			virtual void Execute() override;

		private:
			MapFileManager& mapFileManager;
		};
	}
}

