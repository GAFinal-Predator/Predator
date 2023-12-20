/// 2023. 12. 19 김상준
/// Singleton 패턴으로 사용할 클래스의 경우 해당 클래스를 통해
/// 반복되는 부분을 줄일 수 있도록 함

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
			static T& GetInstance()
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