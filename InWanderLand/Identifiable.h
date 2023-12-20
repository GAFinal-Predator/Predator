/// 2023. 11. 21 김상준
/// UUID 를 사용하여 관리가 될 대상이 상속받을 클래스

#pragma once

#include "UUID.h"

#include <string>

namespace application
{
	namespace editor
	{
		std::string UUID_To_String(UUID uuid);
		UUID String_To_UUID(std::string str);

		class Identifiable
		{
		public:
			Identifiable();
			Identifiable(const UUID& id);

			UUID GetUUID() const;
			void SetUUID(const UUID& id);

		protected:
			UUID id;
		};
	}
}
