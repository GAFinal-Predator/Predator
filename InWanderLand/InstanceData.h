/// 2023. 11. 23 �����
/// Instance ȭ �Ǵ� �������� ���� �����͵��� ������ Ŭ����
/// �⺻���� �����͸� ������ ������, �ش� Ŭ������ ��ӹ޾� �߰����� �������� Ȯ���� �� ����

#pragma once

#include "Storable.h"

namespace Application
{
	namespace Editor
	{
		class TemplateData;
	}
}

namespace Application
{
	namespace Editor
	{
		struct POD_InstanceData
		{
			TO_JSON(POD_InstanceData);
			FROM_JSON(POD_InstanceData);

			std::string test = "test";
		};

		class InstanceData
			: public Storable
		{
		public:
			virtual ~InstanceData() = default;

			// ���ø����κ��� �ʱ�ȭ�Ǵ� �����͵��� �ϰ������� ó���ϴ� �Լ�
			virtual bool EnterDataFromTemplate(const TemplateData* templateData) = 0;	

			// �⺻ ���� ������
			POD_InstanceData pod = POD_InstanceData();
			int testInt = 10;

		protected:
			virtual bool PreEncoding(json& data) const = 0;
			virtual bool PostEncoding(json& data) const = 0;
			virtual bool PreDecoding(const json& data) = 0;
			virtual bool PostDecoding(const json& data) = 0;
		};
	}
}
