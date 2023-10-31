/// 2023. 10. 11 �����
/// Application ���� ����� ������ �׷�ȭ�Ͽ� ���� �����ϱ� ����
/// Layer ������ ����� �������̽�

#pragma once

namespace Application
{
	class Layer
	{
	public:
		virtual void Initialize() = 0;
		//virtual void EventProgress(Events& e) = 0;
		virtual void Update(float ts) = 0;
		virtual void GUIProgress() = 0;
		virtual void Finalize() = 0;
	};
}
