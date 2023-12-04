/// 2023. 11. 14 �����
/// �ν��Ͻ�ȭ �Ǵ� ������ ���� ��� �����͸� ��� �ִ� Ŭ����

#pragma once

#include "Identifiable.h"
#include "Storable.h"

#include <memory>

namespace Application
{
	namespace Editor
	{
		class SharedData
			: public Identifiable, public Storable
		{
		public:
			virtual bool PreEncoding(json& data) const = 0;
			virtual bool PostEncoding(json& data) const = 0;
			virtual bool PreDecoding(const json& data) = 0;
			virtual bool PostDecoding(const json& data) = 0;

			virtual std::shared_ptr<SharedData> Clone() = 0;
		};
	}
}
