/// 2023. 12. 08 �����
/// �� ������ �����Ϳ� �ҷ����� Ŀ�ǵ�

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

