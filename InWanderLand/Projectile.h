#pragma once
#include "YunutyEngine.h"
/// <summary>
/// ������ �߻�ü�� �ٿ��� ������Ʈ.
/// �����Ӱ� �ٸ� ���ְ� �浹 ���� ����� �������� �����ȴ�.
/// </summary>

class Projectile : public Component
{
private:
	float m_speed = 2.0f;
	string ownerType;

public:
	void SetOwnerType(string type);

public:
	virtual void Start() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
};

