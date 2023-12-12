/// 2023. 11. 23 �����
/// Template Data �� Instance Data �� �����Ǵ� ���� Instance �� �������̽�
/// InstanceManager �� ���ؼ� ������

#pragma once

#include "Identifiable.h"
#include "Storable.h"

#include <memory>

namespace Application
{
	namespace Editor
	{
		class IEditableData
			: public Identifiable, public Storable
		{
			friend class InstanceManager;

		public:
			enum class DataType
			{
				None,			// ���ܿ�
				Terrain,
				Units,
				Ornaments
			};

		public:
			virtual std::shared_ptr<IEditableData> Clone() const = 0;

		protected:
			virtual bool PreEncoding(json& data) const = 0;
			virtual bool PostEncoding(json& data) const = 0;
			virtual bool PreDecoding(const json& data) = 0;
			virtual bool PostDecoding(const json& data) = 0;
		};
	}
}

