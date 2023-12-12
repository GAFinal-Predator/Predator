/// 2023. 11. 23 �����
/// TemplateData �� ��üȭ�� Ŭ����
/// ��Ĺ�

#pragma once

#include "TemplateData.h"

namespace Application
{
	namespace Editor
	{
		class Ornaments_TemplateData
			:public TemplateData
		{
			friend class Ornaments;
			friend class TemplateDataManager;

		public:
			virtual std::shared_ptr<TemplateData> Clone() const override;

		protected:
			virtual bool PreEncoding(json& data) const override;
			virtual bool PostEncoding(json& data) const override;
			virtual bool PreDecoding(const json& data) override;
			virtual bool PostDecoding(const json& data) override;

		private:
			Ornaments_TemplateData();
			Ornaments_TemplateData(const Ornaments_TemplateData& prototype);
			Ornaments_TemplateData& operator=(const Ornaments_TemplateData& prototype);
		};
	}
}

