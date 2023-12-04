/// 2023. 11. 23 �����
/// ���� Instance �� �Ǿ� �����Ǵ� EditableData �� ���� Manager Ŭ����
/// EditableData �� InstanceMaanger �� ���ؼ��� ������ �� ����

#pragma once

#include "EditableData.h"

#include <string>
#include <memory>
#include <set>

namespace Application
{
	namespace Editor
	{
		class InstanceManager
		{
		public:
			static InstanceManager& GetInstance();

			virtual ~InstanceManager();

			bool CreateInstance(const std::string& dataName);
			bool CloneInstance(const std::shared_ptr<EditableData>& prototype);
			bool Save(json& jsonData);
			bool Load(const json& jsonData);

		private:
			static std::unique_ptr<InstanceManager> instance;

			InstanceManager();
			InstanceManager(const InstanceManager&) = delete;
			InstanceManager& operator=(const InstanceManager&) = delete;

			void Clear();

			std::set<std::shared_ptr<EditableData>> list;
			std::shared_ptr<EditableData> mould;
		};
	}
}


