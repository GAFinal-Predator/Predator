/// 2023. 11. 21 �����
/// ���� ���·� �����ϱ� ���� json �� �ٷ�� Ŭ����
/// �ش� Ŭ������ ��ӹ޾� Pre / Post ������� �������Ͽ� ��üȭ��

#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <boost/pfr.hpp>

using json = nlohmann::json;

namespace Application
{
	namespace Editor
	{	
		template <int N, typename T>
		json FieldEncoding(T& className, json& data)
		{
			if constexpr (N == 0)
			{
				return data;
			}
			else
			{
				data[boost::pfr::get_name<N - 1, T>()] = boost::pfr::get<N - 1>(className);
				FieldEncoding<N - 1, T>(className, data);
			}
			return data;
		}

		class Storable
		{
		public:
			// json ���·� Storable Ŭ������ �����մϴ�.
			json Encoding() const;
			// json ���·κ��� Storable Ŭ������ �ʱ�ȭ�մϴ�.
			bool Decoding(const json& data);

		protected:
			virtual bool PreEncoding(json& data) const = 0;
			virtual bool PostEncoding(json& data) const = 0;
			virtual bool PreDecoding(const json& data) = 0;
			virtual bool PostDecoding(const json& data) = 0;
		};
	}
}

#define TO_JSON(Class) \
operator json() \
{ \
	json data; \
	Class classObj = Class(); \
	Application::Editor::FieldEncoding<boost::pfr::tuple_size_v<Class>>(classObj, data); \
	return data; \
}
