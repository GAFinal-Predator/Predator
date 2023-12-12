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

			unsigned int currentHP;
		};

		class Units_InstanceData
			:public InstanceData
		{
			friend class Units;

		public:
			// ���ø����κ��� �ʱ�ȭ�Ǵ� �����͵��� �ϰ������� ó���ϴ� �Լ�
			virtual bool EnterDataFromTemplate(const std::shared_ptr<TemplateData>& templateData) override;

			POD_Units_InstanceData pod;

		protected:
			virtual bool PreEncoding(json& data) const override;
			virtual bool PostEncoding(json& data) const override;
			virtual bool PreDecoding(const json& data) override;
			virtual bool PostDecoding(const json& data) override;
		};
	}
}
