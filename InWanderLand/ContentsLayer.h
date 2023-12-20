/// 2023. 10. 11 �����
/// Contents ������ ���� ���̾�

#pragma once

#include "Layer.h"
#include <functional>

namespace application
{
	namespace Contents
	{
		class ContentsLayer
			: public Layer
		{
		private :
			static std::function<void()> testInitializer;
		public:
			virtual void Initialize() override;
			//virtual void EventProgress() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;
			// �׽�Ʈ �ڵ忡�� ���Ƿ� ContentsLayer�� Initialize �ڵ带 �������̵� �ϰ� ������ ���̴� �Լ�
			static void AssignTestInitializer(std::function<void()> testInitializer);
		};
	}
}
