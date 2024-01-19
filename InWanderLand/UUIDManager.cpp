#include "InWanderLand.h"
#include "UUIDManager.h"

namespace application
{
	namespace editor
	{
		bool UUIDManager::RegisterUUIDWithPointer(const UUID& uuid, Identifiable* pointer)
		{
			if (uuidMap.find(uuid) != uuidMap.end())
			{
				// �̹� ��ϵ� uuid �� ���� ���
				return false;
			}

			uuidMap[uuid] = pointer;
			return true;
		}

		bool UUIDManager::EraseUUID(const UUID& uuid)
		{
			if (uuidMap.erase(uuid) == 0)
			{
				return false;
			}

			return true;
		}

		void UUIDManager::Clear()
		{
			uuidMap.clear();
		}
	}
}
