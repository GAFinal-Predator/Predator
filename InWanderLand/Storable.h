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
		json FieldEncoding(T& classInstance, json& data)
		{
			if constexpr (N == 0)
			{
				return data;
			}
			else
			{
				data[boost::pfr::get_name<N - 1, T>()] = boost::pfr::get<N - 1>(classInstance);
				FieldEncoding<N - 1, T>(classInstance, data);
			}
			return data;
		}

		template <int N, typename T>
		const json& FieldDecoding(T& classInstance, const json& data)
		{
			if constexpr (N == 0)
			{
				return data;
			}
			else
			{
				boost::pfr::get<N - 1>(classInstance) = data[boost::pfr::get_name<N - 1, T>()];
				FieldDecoding<N - 1, T>(classInstance, data);
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
	Application::Editor::FieldEncoding<boost::pfr::tuple_size_v<Class>>(*this, data); \
	return data; \
}

#define FROM_JSON(Class) \
Class& operator=(const json& data) \
{ \
	Application::Editor::FieldDecoding<boost::pfr::tuple_size_v<Class>>(*this, data); \
	return *this; \
}
