/// 2023. 11. 23 �����
/// InstanceData �� ��üȭ�� Ŭ����
/// ����

#pragma once

#include "InstanceData.h"

namespace application
{
	namespace editor
	{
		struct POD_Terrain_InstanceData
		{
			TO_JSON(POD_Terrain_InstanceData);
			FROM_JSON(POD_Terrain_InstanceData);
		};

		class Terrain_InstanceData
			:public InstanceData
		{
			friend class Terrain;

		public:
			// ���ø����κ��� �ʱ�ȭ�Ǵ� �����͵��� �ϰ������� ó���ϴ� �Լ�
			virtual bool EnterDataFromTemplate(const TemplateData* templateData) override;

			POD_Terrain_InstanceData pod = POD_Terrain_InstanceData();

		protected:
			virtual bool PreEncoding(json& data) const override;
			virtual bool PostEncoding(json& data) const override;
			virtual bool PreDecoding(const json& data) override;
			virtual bool PostDecoding(const json& data) override;
		};
	}
}

