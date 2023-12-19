/// 2023. 11. 23 �����
/// InstanceData �� ��üȭ�� Ŭ����
/// ����

#pragma once

#include "InstanceData.h"

namespace Application
{
	namespace Editor
	{
		struct POD_Units_InstanceData
		{
			TO_JSON(POD_Units_InstanceData);
			FROM_JSON(POD_Units_InstanceData);

			unsigned int currentHP;
			std::string testName = "Kiki";
		};

		class Units_InstanceData
			:public InstanceData
		{
			friend class Units;

		public:
			// ���ø����κ��� �ʱ�ȭ�Ǵ� �����͵��� �ϰ������� ó���ϴ� �Լ�
			virtual bool EnterDataFromTemplate(const TemplateData* templateData) override;

			POD_Units_InstanceData pod = POD_Units_InstanceData();
			double testDouble = 10.0;

		protected:
			virtual bool PreEncoding(json& data) const override;
			virtual bool PostEncoding(json& data) const override;
			virtual bool PreDecoding(const json& data) override;
			virtual bool PostDecoding(const json& data) override;
		};
	}
}
