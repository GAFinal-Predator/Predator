/// 2023. 11. 23 �����
/// TemplateData �� ��üȭ�� Ŭ����
/// ����

#pragma once

#include "TemplateData.h"

namespace Application
{
	namespace Editor
	{
		struct POD_Units_TemplateData
		{
			TO_JSON(POD_Units_TemplateData);

			int maxHP = 50;
		};

		class Units_TemplateData
			:public TemplateData
		{
			friend class Units;
			friend class TemplateDataManager;

		public:
			virtual std::shared_ptr<TemplateData> Clone() const override;

			POD_Units_TemplateData pod;

		protected:
			virtual bool PreEncoding(json& data) const override;
			virtual bool PostEncoding(json& data) const override;
			virtual bool PreDecoding(const json& data) override;
			virtual bool PostDecoding(const json& data) override;

		private:
			Units_TemplateData();
			Units_TemplateData(const Units_TemplateData& prototype);
			Units_TemplateData& operator=(const Units_TemplateData& prototype);
		};
	}
}
