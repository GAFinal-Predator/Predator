/// 2023. 11. 21 �����
/// UUID �� ����Ͽ� ������ �� ����� ��ӹ��� Ŭ����

#pragma once

#include "UUID.h"

namespace Application
{
	namespace Editor
	{
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
