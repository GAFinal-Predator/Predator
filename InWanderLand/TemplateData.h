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

			std::string name = "None";
		};

		class TemplateData
			: public Identifiable, public Storable
		{
		public:
			virtual std::shared_ptr<TemplateData> Clone() const = 0;

			// �⺻ ���� ������
			POD_TemplateData pod = POD_TemplateData();

		protected:
			virtual bool PreEncoding(json& data) const = 0;
			virtual bool PostEncoding(json& data) const = 0;
			virtual bool PreDecoding(const json& data) = 0;
			virtual bool PostDecoding(const json& data) = 0;
		};
	}
}
