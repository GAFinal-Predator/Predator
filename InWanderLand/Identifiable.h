/// 2023. 11. 21 �����
/// UUID �� ����Ͽ� ������ �� ����� ��ӹ��� Ŭ����

#pragma once

#include "UUID.h"

#include <string>

namespace Application
{
	namespace Editor
	{
		std::string UUID_To_String(UUID uuid);
		UUID String_From_UUID(std::string str);

		class Identifiable
		{
		public:
			Identifiable();
			Identifiable(const UUID& id);

			UUID GetUUID();
			void SetUUID(const UUID& id);

		protected:
			UUID id;
		};
	}
}
