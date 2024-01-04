/// 2023. 12. 19 �����
/// UUID �� ���� �����͸� �����ϰ� ȹ���� �� �ִ� Manager Ŭ����
/// ���� ���� ������ ��ġ�� �ʰ� void* ���·� ĳ�����Ͽ� �����

#pragma once

#include "Singleton.h"
#include "Identifiable.h"

#include <unordered_map>
#include <type_traits>

namespace application
{
	namespace editor
	{
		class UUIDManager
			: public Singleton<UUIDManager>
		{
		public:
			bool RegisterUUIDWithPointer(const UUID& uuid, Identifiable* pointer);

			template <typename T> requires std::is_pointer_v<T>
			T GetPointerFromUUID(const UUID& uuid)
			{
				return static_cast<T>(uuidMap[uuid]);
			}

			bool EraseUUID(const UUID& uuid);
			void Clear();

		private:
			std::unordered_map<const UUID, Identifiable*> uuidMap;
		};
	}
}
