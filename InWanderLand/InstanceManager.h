/// 2023. 11. 23 �����
/// ���� Instance �� �Ǿ� �����Ǵ� EditableData �� ���� Manager Ŭ����
/// EditableData �� InstanceMaanger �� ���ؼ��� ������ �� ����

#pragma once

#include "Storable.h"
#include "IEditableData.h"

#include <string>
#include <memory>
#include <unordered_set>

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

			bool CreateInstance(const std::string& dataName);
			bool CloneInstance(const std::shared_ptr<IEditableData>& prototype);

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

			void Clear();

			std::unordered_set<std::shared_ptr<IEditableData>> list;
			std::shared_ptr<IEditableData> mould;
		};
	}
}


