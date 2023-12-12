#include "SaveMapCommand.h"

#include "MapFileManager.h"

namespace Application
{
	namespace Editor
	{
		SaveMapCommand::SaveMapCommand()
			: mapFileManager(MapFileManager::GetInstance())
		{
			
		}

		void SaveMapCommand::Execute()
		{
			mapFileManager.SaveMapFile("TestMap");
		}
	}
}
