/// 2023. 11. 23 �����
/// InstanceData �� ��üȭ�� Ŭ����
/// ��Ĺ�

#pragma once

#include "InstanceData.h"

namespace Application
{
	namespace Editor
	{
		class Ornaments_InstanceData
			:public InstanceData
		{
			friend class Ornaments;

		public:
			// ���ø����κ��� �ʱ�ȭ�Ǵ� �����͵��� �ϰ������� ó���ϴ� �Լ�
			virtual bool EnterDataFromTemplate(const std::shared_ptr<TemplateData>& templateData) override;

		protected:
			virtual bool PreEncoding(json& data) const override;
			virtual bool PostEncoding(json& data) const override;
			virtual bool PreDecoding(const json& data) override;
			virtual bool PostDecoding(const json& data) override;
		};
	}
}

