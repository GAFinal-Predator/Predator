/// 2023. 11. 23 �����
/// ���� Instance �� �Ǿ� �����Ǵ� IEditableData �� ���� Manager Ŭ����
/// IEditableData �� InstanceMaanger �� ���ؼ��� ������ �� ����

#pragma once

#include "Singleton.h"
#include "Storable.h"
#include "IEditableData.h"
#include "Identifiable.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>


namespace application
{
	namespace editor
	{
		class TemplateDataManager;
		class ITemplateData;
	}
}

namespace application
{
	namespace editor
	{
		class InstanceManager
			: public Storable, public Singleton<InstanceManager>
		{
		public:
			friend class MapFileManager;

			InstanceManager();

			// �̹� ������� ITemplateData �κ��� Instance �� ������
			IEditableData* CreateInstance(const std::string& dataName);
			bool DeleteInstance(const UUID& uuid);
			IEditableData* GetInstance(const UUID& uuid) const;
			void Clear();

		protected:
			virtual bool PreEncoding(json& data) const override;
			virtual bool PostEncoding(json& data) const override;
			virtual bool PreDecoding(const json& data) override;
			virtual bool PostDecoding(const json& data) override;

		private:
			// ITemplateData �� �����Ǳ� ���� �� Ʋ�� �켱 �ۼ���
			IEditableData* CreateEmptyInstance(const IEditableData::DataType& type);

			TemplateDataManager& templateDataManager;
			std::unordered_map<const UUID, std::unique_ptr<IEditableData>> list;
			std::unordered_map<const UUID, ITemplateData*> tdMap;
			// CreateEmptyInstance Only
			std::unordered_set<IEditableData*> listBeforeMatching;
			IEditableData* mould;
		};
	}
}


