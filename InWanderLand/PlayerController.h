#pragma once
#include "YunutyEngine.h"
/// <summary>
/// Unit Component�� ������ Gameobject���ٰ� �ٿ��ָ� ������ ������������ ���ִ� ������Ʈ.
/// </summary>

class RTSCam;

class PlayerController : public Component
{
private:
	RTSCam* m_movingSystemComponent;

public:
	void SetMovingSystemComponent(RTSCam* sys);

	virtual void Start() override;
};

