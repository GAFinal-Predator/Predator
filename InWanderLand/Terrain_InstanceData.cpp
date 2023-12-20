#include "Terrain_InstanceData.h"

#include "Terrain_TemplateData.h"

namespace application
{
	namespace editor
	{
		bool Terrain_InstanceData::EnterDataFromTemplate(const TemplateData* templateData)
		{
#pragma region Check
			auto ptr = dynamic_cast<Terrain_TemplateData*>(const_cast<TemplateData*>(templateData));
			if (ptr == nullptr)
			{
				return false;
			}
#pragma endregion It was included for review, but it appears that it can be ultimately excluded \
			because it includes a dynamic casting.

			// 템플릿으로부터 초기화되는 데이터들 처리 영역

			return true;
		}

		bool Terrain_InstanceData::PreEncoding(json& data) const
		{
			FieldEncoding<boost::pfr::tuple_size_v<POD_InstanceData>>(InstanceData::pod, data["POD_Base"]);
			FieldEncoding<boost::pfr::tuple_size_v<POD_Terrain_InstanceData>>(pod, data["POD"]);
			return true;
		}

		bool Terrain_InstanceData::PostEncoding(json& data) const
		{
			return true;
		}

		bool Terrain_InstanceData::PreDecoding(const json& data)
		{
			FieldDecoding<boost::pfr::tuple_size_v<POD_InstanceData>>(InstanceData::pod, data["POD_Base"]);
			FieldDecoding<boost::pfr::tuple_size_v<POD_Terrain_InstanceData>>(pod, data["POD"]);
			return true;
		}

		bool Terrain_InstanceData::PostDecoding(const json& data)
		{
			return true;
		}
	}
}
