/// 2023. 11. 23 �����
/// Instance ȭ �Ǵ� �������� ���� �����͵��� ������ Ŭ����
/// �⺻���� �����͸� ������ ������, �ش� Ŭ������ ��ӹ޾� �߰����� �������� Ȯ���� �� ����

#pragma once

#include "Storable.h"

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
		class InstanceData
			: public Storable
		{
		public:
			// ���ø����κ��� �ʱ�ȭ�Ǵ� �����͵��� �ϰ������� ó���ϴ� �Լ�
			virtual bool EnterDataFromTemplate(const std::shared_ptr<TemplateData>& templateData) = 0;	

			// �⺻ ���� ������

		protected:
			virtual bool PreEncoding(json& data) const = 0;
			virtual bool PostEncoding(json& data) const = 0;
			virtual bool PreDecoding(const json& data) = 0;
			virtual bool PostDecoding(const json& data) = 0;
		};
	}
}
