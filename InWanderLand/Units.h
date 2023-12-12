/// 2023. 11. 23 �����
/// EditableData �� ��üȭ�� Ŭ����
/// ����

#pragma once

#include "IEditableData.h"
#include "Units_InstanceData.h"
#include "Units_TemplateData.h"

#include <memory>
#include <string>

namespace Application
{
	namespace Editor
	{
		class Units
			: public IEditableData
		{
			friend class InstanceManager;

		public:
			virtual std::shared_ptr<IEditableData> Clone() const override;

			Units_InstanceData instanceData;
			std::shared_ptr<Units_TemplateData> templateData;

		protected:
			virtual bool PreEncoding(json& data) const override;
			virtual bool PostEncoding(json& data) const override;
			virtual bool PreDecoding(const json& data) override;
			virtual bool PostDecoding(const json& data) override;

		private:
			Units(const std::string& name);
			Units(const Units& prototype);
			Units& operator=(const Units& prototype);
		};
	}
}
