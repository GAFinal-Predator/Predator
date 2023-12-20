/// 2023. 11. 23 �����
/// InstanceData �� ��üȭ�� Ŭ����
/// ��Ĺ�

#pragma once

#include "InstanceData.h"

namespace application
{
	namespace editor
	{
		struct POD_Ornaments_InstanceData
		{
			TO_JSON(POD_Ornaments_InstanceData);
			FROM_JSON(POD_Ornaments_InstanceData);
		};

		class Ornaments_InstanceData
			:public InstanceData
		{
			friend class Ornaments;

		public:
			// ���ø����κ��� �ʱ�ȭ�Ǵ� �����͵��� �ϰ������� ó���ϴ� �Լ�
			virtual bool EnterDataFromTemplate(const TemplateData* templateData) override;

			POD_Ornaments_InstanceData pod = POD_Ornaments_InstanceData();

		protected:
			virtual bool PreEncoding(json& data) const override;
			virtual bool PostEncoding(json& data) const override;
			virtual bool PreDecoding(const json& data) override;
			virtual bool PostDecoding(const json& data) override;
		};
	}
}

