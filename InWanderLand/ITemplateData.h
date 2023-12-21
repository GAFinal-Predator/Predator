/// 2023. 11. 14 �����
/// �ν��Ͻ�ȭ �Ǵ� ������ ���� ��� �����͸� ��� �ִ� Ŭ����
/// �⺻���� �����͸� ������ ������, �ش� Ŭ������ ��ӹ޾� �߰����� �������� Ȯ���� �� ����

#pragma once

#include "Identifiable.h"
#include "Storable.h"

#include <memory>
#include <string>

namespace application
{
	namespace editor
	{
		class ITemplateData
			: public Identifiable, public Storable
		{
			friend class TemplateDataManager;

		public:
			virtual ~ITemplateData() = default;

			virtual std::string GetDataKey() const = 0;

		protected:
			virtual bool PreEncoding(json& data) const = 0;
			virtual bool PostEncoding(json& data) const = 0;
			virtual bool PreDecoding(const json& data) = 0;
			virtual bool PostDecoding(const json& data) = 0;
		};
	}
}
