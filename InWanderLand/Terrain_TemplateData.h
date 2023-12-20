/// 2023. 11. 23 �����
/// TemplateData �� ��üȭ�� Ŭ����
/// ����

#pragma once

#include "TemplateData.h"

namespace application
{
	namespace editor
	{
		class Terrain_TemplateData
			:public TemplateData
		{
			friend class Terrain;
			friend class TemplateDataManager;

		public:

		protected:
			virtual bool PreEncoding(json& data) const override;
			virtual bool PostEncoding(json& data) const override;
			virtual bool PreDecoding(const json& data) override;
			virtual bool PostDecoding(const json& data) override;

		private:
			Terrain_TemplateData();
			Terrain_TemplateData(const Terrain_TemplateData& prototype);
			Terrain_TemplateData& operator=(const Terrain_TemplateData& prototype);
		};
	}
}

