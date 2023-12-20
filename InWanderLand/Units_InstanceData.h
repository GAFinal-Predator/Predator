/// 2023. 11. 23 김상준
/// InstanceData 의 구체화된 클래스
/// 유닛

#pragma once

#include "InstanceData.h"

namespace application
{
	namespace editor
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
			// 템플릿으로부터 초기화되는 데이터들을 일괄적으로 처리하는 함수
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
