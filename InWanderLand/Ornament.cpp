#include "Ornament.h"

#include "InstanceManager.h"
#include "TemplateDataManager.h"

namespace application
{
	namespace editor
	{
		TemplateDataManager& Ornament::templateDataManager = TemplateDataManager::GetSingletonInstance();

		bool Ornament::EnterDataFromTemplate()
		{
			// ���ø����κ��� �ʱ�ȭ�Ǵ� �����͵� ó�� ����	

			return true;
		}

		ITemplateData* Ornament::GetTemplateData()
		{
			return pod.templateData;
		}

		bool Ornament::SetTemplateData(const std::string& dataName)
		{
			auto ptr = templateDataManager.GetTemplateData(dataName);
			if (ptr == nullptr)
			{
				return false;
			}

			pod.templateData = static_cast<Ornament_TemplateData*>(ptr);

			return true;
		}

		IEditableData* Ornament::Clone() const
		{
			auto& imanager = InstanceManager::GetSingletonInstance();
			auto instance = imanager.CreateInstance(pod.templateData->GetDataKey());

			if (instance != nullptr)
			{
				static_cast<Ornament*>(instance)->pod = pod;
			}

			return instance;
		}

		bool Ornament::PreEncoding(json& data) const
		{
			FieldPreEncoding<boost::pfr::tuple_size_v<POD_Ornament>>(pod, data["POD"]);

			return true;
		}

		bool Ornament::PostEncoding(json& data) const
		{
			FieldPostEncoding<boost::pfr::tuple_size_v<POD_Ornament>>(pod, data["POD"]);

			return true;
		}

		bool Ornament::PreDecoding(const json& data)
		{
			FieldPreDecoding<boost::pfr::tuple_size_v<POD_Ornament>>(pod, data["POD"]);

			return true;
		}

		bool Ornament::PostDecoding(const json& data)
		{
			FieldPostDecoding<boost::pfr::tuple_size_v<POD_Ornament>>(pod, data["POD"]);

			return true;
		}

		Ornament::Ornament()
			: pod()
		{

		}

		Ornament::Ornament(const std::string& name)
			: pod()
		{
			pod.templateData = static_cast<Ornament_TemplateData*>(templateDataManager.GetTemplateData(name));
			EnterDataFromTemplate();
		}

		Ornament::Ornament(const Ornament& prototype)
			: pod(prototype.pod)
		{

		}

		Ornament& Ornament::operator=(const Ornament& prototype)
		{
			IEditableData::operator=(prototype);
			pod = prototype.pod;
			return *this;
		}
	}
}
