/// 2023. 12. 19 �����
/// Singleton �������� ����� Ŭ������ ��� �ش� Ŭ������ ����
/// �ݺ��Ǵ� �κ��� ���� �� �ֵ��� ��

#pragma once

#include <memory>

namespace Application
{
	namespace Editor
	{
		template <typename T>
		class Singleton
		{
		public:
			static T& GetInstance()
			{
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

		private:
			static std::unique_ptr<T> instance;
		};

		template <typename T>
		std::unique_ptr<T> Singleton<T>::instance = nullptr;
	}
}