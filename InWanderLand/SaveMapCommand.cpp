#include "SaveMapCommand.h"

#include "MapFileManager.h"

namespace application
{
	namespace editor
	{
		SaveMapCommand::SaveMapCommand()
			: mapFileManager(MapFileManager::GetSingletonInstance())
		{
			
		}

		void SaveMapCommand::Execute()
		{			
			mapFileManager.SaveMapFile("TestMap.pmap");
		}
	}
}
