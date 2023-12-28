/// 2023. 11. 23 �����
/// ITemplateData �� ��üȭ�� Ŭ����
/// ��Ĺ�

#pragma once

#include "ITemplateData.h"

namespace application
{
	namespace editor
	{
		class Ornament_TemplateData;

		struct POD_Ornament_TemplateData
		{
			TO_JSON(POD_Ornament_TemplateData);
			FROM_JSON(POD_Ornament_TemplateData);
		};

		class Ornament_TemplateData
			:public ITemplateData
		{
			friend class OrnamentData;
			friend class TemplateDataManager;

		public:
			virtual std::string GetDataKey() const override;

			POD_Ornament_TemplateData pod;

		protected:
			virtual bool PreEncoding(json& data) const override;
			virtual bool PostEncoding(json& data) const override;
			virtual bool PreDecoding(const json& data) override;
			virtual bool PostDecoding(const json& data) override;

		private:
			Ornament_TemplateData();
			Ornament_TemplateData(const Ornament_TemplateData& prototype);
			Ornament_TemplateData& operator=(const Ornament_TemplateData& prototype);
		};
	}
}

