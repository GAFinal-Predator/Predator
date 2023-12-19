/// 2023. 11. 14 �����
/// �ν��Ͻ�ȭ �Ǵ� ������ ���� ��� �����͸� ��� �ִ� Ŭ����
/// �⺻���� �����͸� ������ ������, �ش� Ŭ������ ��ӹ޾� �߰����� �������� Ȯ���� �� ����

#pragma once

#include "Identifiable.h"
#include "Storable.h"

#include <memory>
#include <string>

namespace Application
{
	namespace Editor
	{
		struct POD_TemplateData
		{
			TO_JSON(POD_TemplateData);
			FROM_JSON(POD_TemplateData);

			std::string test = "test";
		};

		class TemplateData
			: public Identifiable, public Storable
		{
			friend class TemplateDataManager;

		public:
			virtual ~TemplateData() = default;

			std::string GetDataKey() const;

			// �⺻ ���� ������
			POD_TemplateData pod = POD_TemplateData();
			double testDouble = 10.0;

		protected:
			virtual bool PreEncoding(json& data) const = 0;
			virtual bool PostEncoding(json& data) const = 0;
			virtual bool PreDecoding(const json& data) = 0;
			virtual bool PostDecoding(const json& data) = 0;
		};
	}
}
