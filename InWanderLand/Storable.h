/// 2023. 11. 21 �����
/// ���� ���·� �����ϱ� ���� json �� �ٷ�� Ŭ����
/// �ش� Ŭ������ ��ӹ޾� Pre / Post ������� �������Ͽ� ��üȭ��

#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Application
{
	namespace Editor
	{
		class Storable
		{
		public:
			json Encoding() const;
			bool Decoding(const json& data);

		protected:
			virtual bool PreEncoding(json& data) const = 0;
			virtual bool PostEncoding(json& data) const = 0;
			virtual bool PreDecoding(const json& data) = 0;
			virtual bool PostDecoding(const json& data) = 0;
		};
	}
}

