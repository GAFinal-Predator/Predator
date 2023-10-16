#pragma once
/// <summary>
/// Dotween�� �̿��ϸ� ��ġ�� ������ �׷������ ���� ����.
/// ���� delay�� �̿��� n�� �ڿ� �����ų ���� �ֵ��� �ϱ� (����Ʈ �Ű�����)
/// �̸� �̿��� position, scale, rotation ��� ����.
/// ���ϴ� ���� : (GameObject) dotWeen->AddComponent<Dotween>;
/// dotWeen->DoMove( ��� ������Ʈ, endValue, duration, delay, Ease )...
/// �Ű������� �ʹ� �ʹ� ����
/// ��������� Ease�� ����, delay�� ����Ʈ��� ġ��...
/// �ϴ� delay = 0, Ease : Linear�� ������ ��������
/// �ڱ� �ڽ��� ��� �������ָ鼭 object�� ���� �ٲ��ش�...?
/// </summary>
#include <iostream>
#include <map>
#include "Easing.h"
#include "YunutyEngine.h"

using namespace yunutyEngine;

using std::vector;

class DotweenTimer;

class Dotween : public Component
{
public:
	Dotween();
	~Dotween();

	void Awake();
	void Update();

	/// ù �Ű������� GameObject�� �ٲ��� ����...!
	Dotween& DOMove(Vector3d endPosition, double p_duration);
	Dotween& DOScale(Vector3d endScale, double p_duration);
	Dotween& DORotate(Vector3d endRotation, double p_duration);
	Dotween& DOQRotate(Vector3d axis, double angle, double p_duration);

	// DOLookAtForBoss�� Y���� ������� �ʰ� ���� ��. (���Ʒ� ��� X), DOLookAt�� Y����� ������.
	Dotween& DOLookAt(Transform* lookTransform, double duration, bool isYaxisInclude);
	
	// �Ʒ��� �Լ����� ���� Dotween�� ���� �Լ���...
	Dotween& DOCustom(double p_duration);
	Dotween& DONothing(double p_duration);

	// strength : �ึ�� �̵��ϴ� ��.			vibrato : ��鸲�� ����
	Dotween& DOShakePosition(double p_duration, double strength = 2.0f, int vibrato = 2);

	Dotween& SetDelay(double p_delay);
	Dotween& IsRepeat(bool p_repeat);
	Dotween& SetEase(easing_functions p_ease);
	Dotween& OnComplete(std::function<void()> expirationFunc);

	Dotween& OnUpdate(std::function<void()> updateFunc);

	Dotween& InitDotweenTimer();		// �ش� dotweenTimer�� �ʱ�ȭ���ִ� �Լ�

	std::map<DotweenTimer*, bool> m_dotweenTimerMap;

	// DO �Լ��� ȣ��� ������ function�� �־��ֱ� ���� �ӽ� ��ü.
	// ������ ���Ե� ��ü delete �� ���̱� ������ ���� delete ���� �ʴ´�.
	DotweenTimer* tempTimer;

	// ������ 0~360���� �ƴ� �� 0~360���� �������ִ� �Լ�. 
	double AdjustRotation(double& rot);

	void clearDotweenTimerMap();

private:
	Vector3d randPos;
	Vector3d randRange;

	double previousAngle;

};

