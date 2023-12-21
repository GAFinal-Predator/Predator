/// 2023. 11. 21 �����
/// UUID �� ����Ͽ� ������ �� ����� ��ӹ��� Ŭ����

#pragma once

#include "UUID.h"

#include <string>

namespace application
{
	namespace editor
	{
		class UUIDManager;
	}
}

namespace application
{
	namespace editor
	{
		std::string UUID_To_String(UUID uuid);
		UUID String_To_UUID(std::string str);

		class Identifiable
		{
		public:
			virtual ~Identifiable();

			Identifiable();
			Identifiable(const UUID& id);

			UUID GetUUID() const;

			// SetUUID �� ���� UUID �� �ٲپ�߸� UUIDManager ���� ��������� ����ǹǷ� ����
			void SetUUID(const UUID& id);

		protected:
			UUID id;

		private:
			static UUIDManager& uuidManager;
		};
	}
}
