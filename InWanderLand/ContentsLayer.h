/// 2023. 10. 11 �����
/// Contents ������ ���� ���̾�

#pragma once

#include "Layer.h"

namespace Application
{
	namespace Contents
	{
		class ContentsLayer
			: public Layer
		{
		public:
			virtual void Initialize() override;
			//virtual void EventProgress() override;
			virtual void Update(float ts) override;
			virtual void GUIProgress() override;
			virtual void Finalize() override;
		};
	}
}
