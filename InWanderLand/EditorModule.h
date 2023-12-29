/// 2023. 12. 27 �����
/// Editor ���� ����ϴ� Module ���� ����� ��� ������ ���� �߻�Ŭ����
/// Module �� Panel �� ��ӹ���

#pragma once

#include "EditorPanel.h"

#pragma region Macro Function
#define CheckActivation() \
if (!activation) \
{ \
	return; \
}
#pragma endregion

namespace application
{
	namespace editor
	{
		class EditorModule
			: public Panel
		{
		public:
			virtual ~EditorModule() = default;

			inline bool GetActivation() { return activation; }
			inline void SetActivation(bool activation) { this->activation = activation; }

		protected:
			bool activation = false;
		};
	}
}
