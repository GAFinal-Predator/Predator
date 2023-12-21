/// 2023. 12. 19 �����
/// Singleton �������� ����� Ŭ������ ��� �ش� Ŭ������ ����
/// �ݺ��Ǵ� �κ��� ���� �� �ֵ��� ��

#pragma once

#include <memory>

namespace application
{
	namespace editor
	{
		template <typename T>
		class Singleton
		{
		public:
			static T& GetSingletonInstance()
			{
				static std::unique_ptr<T> instance = nullptr;
				if (instance == nullptr)
				{
					instance = std::make_unique<T>();
				}
				return *instance;
			}

		protected:
			Singleton()
			{

			}

			~Singleton()
			{

			}

			Singleton(const Singleton&) = delete;
			Singleton& operator=(const Singleton&) = delete;
		};
	}
}