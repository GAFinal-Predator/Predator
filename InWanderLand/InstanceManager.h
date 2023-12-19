/// 2023. 11. 23 �����
/// ���� Instance �� �Ǿ� �����Ǵ� EditableData �� ���� Manager Ŭ����
/// EditableData �� InstanceMaanger �� ���ؼ��� ������ �� ����

#pragma once

#include "Storable.h"
#include "IEditableData.h"
#include "Identifiable.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>


namespace Application
{
	namespace Editor
	{
		class TemplateData;
	}
}

namespace Application
{
	namespace Editor
	{
		class InstanceManager
			: public Storable
		{
		public:
			static InstanceManager& GetInstance();

			virtual ~InstanceManager();

			// �̹� ������� TemplateData �κ��� Instance �� ������
			IEditableData* CreateInstance(const std::string& dataName);
			IEditableData* GetInstance(const UUID& uuid) const;
			void Clear();

		protected:
			virtual bool PreEncoding(json& data) const override;
			virtual bool PostEncoding(json& data) const override;
			virtual bool PreDecoding(const json& data) override;
			virtual bool PostDecoding(const json& data) override;

		private:
			static std::unique_ptr<InstanceManager> instance;

			InstanceManager();
			InstanceManager(const InstanceManager&) = delete;
			InstanceManager& operator=(const InstanceManager&) = delete;

			// TemplateData �� �����Ǳ� ���� �� Ʋ�� �켱 �ۼ���
			IEditableData* CreateEmptyInstance(const IEditableData::DataType& type);

			std::unordered_map<const UUID, std::unique_ptr<IEditableData>> list;
			std::unordered_map<const UUID, TemplateData*> tdMap;
			// CreateEmptyInstance Only
			std::unordered_set<IEditableData*> listBeforeMatching;
			IEditableData* mould;
		};
	}
}


