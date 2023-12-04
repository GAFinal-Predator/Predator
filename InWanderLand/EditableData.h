/// 2023. 11. 23 �����
/// Shared Data �� Instance Data �� �����Ǵ� ���� Instance �� �������̽�
/// InstanceManager �� ���ؼ� ������

#pragma once

#include "Identifiable.h"
#include "Storable.h"

#include <memory>

namespace Application
{
	namespace Editor
	{
		class EditableData
			: public Identifiable, public Storable
		{
		public:
			enum class DataType
			{
				Terrain,
				Units,
				Ornaments
			};

		public:
			virtual bool PreEncoding(json& data) const = 0;
			virtual bool PostEncoding(json& data) const = 0;
			virtual bool PreDecoding(const json& data) = 0;
			virtual bool PostDecoding(const json& data) = 0;

			virtual std::shared_ptr<EditableData> Clone() = 0;
		};
	}
}

