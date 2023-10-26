#include "Dotween.h"
#include "Transform.h"
#include "DotweenTimer.h"
#include <cmath>
#include <float.h>
#include "DotweenTimerPool.h"

Dotween::Dotween()
{
}

Dotween::~Dotween()
{
	// vector ���� �����Ҵ��� �޸� ����.


	//delete tempTimer;
}

void Dotween::Awake()
{

}

void Dotween::Update()
{
	for (int i = 0; i < TimerIndex::End; i++)
	{
		if (dotweenTimerArray[i] != nullptr)
		{
			dotweenTimerArray[i]->Update();

			if (dotweenTimerArray[i]->isDone == true)
			{
				DotweenTimerPool::GetInstance()->ReturnDotweenTimer(dotweenTimerArray[i]);
				dotweenTimerArray[i] = nullptr;
			}
		}
	}
}

void Dotween::StopAllDotweenFunction()
{
	for (int i = 0; i < TimerIndex::End; i++)
	{
		if (dotweenTimerArray[i] != nullptr)
		{
			/// isDone�� true�� �ٲ��־� ������ Ÿ�̸� ����.
			dotweenTimerArray[i]->isDone = true;
		}
	}
}

Dotween& Dotween::DOMove(Vector3d endPosition, double p_duration)
{
	// 	GetGameObject()->GetTransform()�� Component�� ���� �� GameObject�� �����Ѵ�.
	DotweenTimer* m_doMovetweenTimer = DotweenTimerPool::GetInstance()->GetDotweenTimer();


	// ���� �� ����
	m_doMovetweenTimer->m_xValue = GetGameObject()->GetTransform()->GetWorldPosition().x;
	m_doMovetweenTimer->m_yValue = GetGameObject()->GetTransform()->GetWorldPosition().y;
	m_doMovetweenTimer->m_zValue = GetGameObject()->GetTransform()->GetWorldPosition().z;

	// ���� �ϴ� �Ÿ�
	double distanceX = endPosition.x - GetGameObject()->GetTransform()->GetWorldPosition().x;
	double distanceY = endPosition.y - GetGameObject()->GetTransform()->GetWorldPosition().y;
	double distanceZ = endPosition.z - GetGameObject()->GetTransform()->GetWorldPosition().z;

	m_doMovetweenTimer->Start();
	m_doMovetweenTimer->duration = p_duration;
	m_doMovetweenTimer->onUpdate = [=]()
	{
		// �ð� ����ȭ : ���� �ð� / ��ü �Ⱓ
		m_doMovetweenTimer->easeTime = (m_doMovetweenTimer->elapsed - m_doMovetweenTimer->delay) / (m_doMovetweenTimer->duration - m_doMovetweenTimer->delay);

		// �����ϴ� �Ÿ��� ����ȭ...
		// ���� �Ÿ� / ��ü �����ϴ� �Ÿ�
		// ���� �Ÿ� = �ð� * �ӷ�

		double easeWeight = getEasingFunction(m_doMovetweenTimer->m_ease)(m_doMovetweenTimer->easeTime);

		// �Ʒ� ���� �ȿ� ������ ���ٸ� �� �տ� ���� ���� - ������
		m_doMovetweenTimer->movingDistanceX = easeWeight * (distanceX);
		m_doMovetweenTimer->movingDistanceY = easeWeight * (distanceY);
		m_doMovetweenTimer->movingDistanceZ = easeWeight * (distanceZ);

		m_doMovetweenTimer->finalXvalue = m_doMovetweenTimer->m_xValue + m_doMovetweenTimer->movingDistanceX;
		m_doMovetweenTimer->finalYvalue = m_doMovetweenTimer->m_yValue + m_doMovetweenTimer->movingDistanceY;
		m_doMovetweenTimer->finalZvalue = m_doMovetweenTimer->m_zValue + m_doMovetweenTimer->movingDistanceZ;

		/// �ε� �Ҽ��� ������ ������ �����ε�... ���� Ŀ������ ������ ���� ���� ����....

		if (m_doMovetweenTimer->easeTime >= 1)
		{
			m_doMovetweenTimer->finalXvalue = endPosition.x;
			m_doMovetweenTimer->finalYvalue = endPosition.y;
			m_doMovetweenTimer->finalZvalue = endPosition.z;
		}


		GetGameObject()->GetTransform()->SetWorldPosition(Vector3d(m_doMovetweenTimer->finalXvalue, m_doMovetweenTimer->finalYvalue, m_doMovetweenTimer->finalZvalue));
	};

	currentTimerIndex = TimerIndex::MoveTimer;
	tempTimer = m_doMovetweenTimer;
	dotweenTimerArray[TimerIndex::MoveTimer] = tempTimer;

	/// �ڱ� �ڽ��� ��ȯ���ش�...�ڿ� SetDelay, SetEase ���� ����
	return *this;
}

// DOScale, DoRotate ���� �Լ��� ���� ����...
// �ᱹ Vector3�� ���� �������ִ� �Ŷ� �ݹ� ���� �� �ִ�
Dotween& Dotween::DOScale(Vector3d endScale, double p_duration)
{
	DotweenTimer* m_doScaletweenTimer = DotweenTimerPool::GetInstance()->GetDotweenTimer();

	m_doScaletweenTimer->m_xValue = GetGameObject()->GetTransform()->GetWorldScale().x;
	m_doScaletweenTimer->m_yValue = GetGameObject()->GetTransform()->GetWorldScale().y;
	m_doScaletweenTimer->m_zValue = GetGameObject()->GetTransform()->GetWorldScale().z;

	double distanceX = endScale.x - GetGameObject()->GetTransform()->GetWorldScale().x;
	double distanceY = endScale.y - GetGameObject()->GetTransform()->GetWorldScale().y;
	double distanceZ = endScale.z - GetGameObject()->GetTransform()->GetWorldScale().z;

	m_doScaletweenTimer->Start();
	m_doScaletweenTimer->duration = p_duration;
	m_doScaletweenTimer->onUpdate = [=]()
	{
		// �Ҽ������� ������ ������ ��Ȯ�� ���� ����� ��� �Ѵ�...
		m_doScaletweenTimer->easeTime = (m_doScaletweenTimer->elapsed - m_doScaletweenTimer->delay) / (m_doScaletweenTimer->duration - m_doScaletweenTimer->delay);

		double easeWeight = getEasingFunction(m_doScaletweenTimer->m_ease)(m_doScaletweenTimer->easeTime);

		// �Ʒ� ���� �ȿ� ������ ���ٸ� �� �տ� ���� ���� - ������
		m_doScaletweenTimer->movingDistanceX = easeWeight * (distanceX);
		m_doScaletweenTimer->movingDistanceY = easeWeight * (distanceY);
		m_doScaletweenTimer->movingDistanceZ = easeWeight * (distanceZ);

		m_doScaletweenTimer->finalXvalue = m_doScaletweenTimer->m_xValue + m_doScaletweenTimer->movingDistanceX;
		m_doScaletweenTimer->finalYvalue = m_doScaletweenTimer->m_yValue + m_doScaletweenTimer->movingDistanceY;
		m_doScaletweenTimer->finalZvalue = m_doScaletweenTimer->m_zValue + m_doScaletweenTimer->movingDistanceZ;

		/// �ε� �Ҽ��� ������ ������ �����ε�... ���� Ŀ������ ������ ���� ���� ����....

		if (m_doScaletweenTimer->easeTime >= 1)
		{
			m_doScaletweenTimer->finalXvalue = endScale.x;
			m_doScaletweenTimer->finalYvalue = endScale.y;
			m_doScaletweenTimer->finalZvalue = endScale.z;
		}

		//GetGameObject()->GetTransform()->SetWorldScale(Vector3D(m_doScaletweenTimer->finalXvalue, m_doScaletweenTimer->finalYvalue, m_doScaletweenTimer->finalZvalue));
	};

	currentTimerIndex = TimerIndex::ScaleTimer;
	tempTimer = m_doScaletweenTimer;
	dotweenTimerArray[TimerIndex::ScaleTimer] = tempTimer;

	return *this;
}


Dotween& Dotween::DORotate(Vector3d endRotation, double p_duration)
{
	/// DORotate ���� �� ������ ��.
	/// 1. ���� 360�� �Ѿ�� 0���� �ʱ�ȭ ���ش�. 
	/// �ϴ� �������?
	DotweenTimer* m_doRotatetweenTimer = DotweenTimerPool::GetInstance()->GetDotweenTimer();

	m_doRotatetweenTimer->m_xValue = GetGameObject()->GetTransform()->GetWorldRotation().x;
	m_doRotatetweenTimer->m_yValue = GetGameObject()->GetTransform()->GetWorldRotation().y;
	m_doRotatetweenTimer->m_zValue = GetGameObject()->GetTransform()->GetWorldRotation().z;

	double distanceX = endRotation.x - GetGameObject()->GetTransform()->GetWorldRotation().x;
	double distanceY = endRotation.y - GetGameObject()->GetTransform()->GetWorldRotation().y;
	double distanceZ = endRotation.z - GetGameObject()->GetTransform()->GetWorldRotation().z;

	m_doRotatetweenTimer->Start();
	m_doRotatetweenTimer->duration = p_duration;
	m_doRotatetweenTimer->onUpdate = [=]()
	{
		m_doRotatetweenTimer->easeTime = (m_doRotatetweenTimer->elapsed - m_doRotatetweenTimer->delay) / (m_doRotatetweenTimer->duration - m_doRotatetweenTimer->delay);

		double easeWeight = getEasingFunction(m_doRotatetweenTimer->m_ease)(m_doRotatetweenTimer->easeTime);

		m_doRotatetweenTimer->movingDistanceX = easeWeight * (distanceX);
		m_doRotatetweenTimer->movingDistanceY = easeWeight * (distanceY);
		m_doRotatetweenTimer->movingDistanceZ = easeWeight * (distanceZ);

		m_doRotatetweenTimer->finalXvalue = m_doRotatetweenTimer->m_xValue + m_doRotatetweenTimer->movingDistanceX;
		m_doRotatetweenTimer->finalYvalue = m_doRotatetweenTimer->m_yValue + m_doRotatetweenTimer->movingDistanceY;
		m_doRotatetweenTimer->finalZvalue = m_doRotatetweenTimer->m_zValue + m_doRotatetweenTimer->movingDistanceZ;


		if (m_doRotatetweenTimer->easeTime >= 1)
		{
			m_doRotatetweenTimer->finalXvalue = endRotation.x;
			m_doRotatetweenTimer->finalYvalue = endRotation.y;
			m_doRotatetweenTimer->finalZvalue = endRotation.z;

			AdjustRotation(m_doRotatetweenTimer->finalXvalue);
			AdjustRotation(m_doRotatetweenTimer->finalYvalue);
			AdjustRotation(m_doRotatetweenTimer->finalZvalue);
		}

		// ���� 360�� �Ѿ�� 0���� �ʱ�ȭ ���ش�.
		// SetRotation ������ �ٲ��ֱ�
		if (m_doRotatetweenTimer->finalXvalue >= 360)
			AdjustRotation(m_doRotatetweenTimer->finalXvalue);

		if (m_doRotatetweenTimer->finalYvalue >= 360)
			AdjustRotation(m_doRotatetweenTimer->finalYvalue);

		if (m_doRotatetweenTimer->finalZvalue >= 360)
			AdjustRotation(m_doRotatetweenTimer->finalZvalue);

		/// �ݴ�� ������ -360 �̸��̶�� -360 ~ 0���� �ٲ��ش�.
		if (m_doRotatetweenTimer->finalXvalue < 0)
			AdjustRotation(m_doRotatetweenTimer->finalXvalue);

		if (m_doRotatetweenTimer->finalYvalue < 0)
			AdjustRotation(m_doRotatetweenTimer->finalYvalue);

		if (m_doRotatetweenTimer->finalZvalue < 0)
			AdjustRotation(m_doRotatetweenTimer->finalZvalue);

		GetGameObject()->GetTransform()->SetWorldRotation(Vector3d(m_doRotatetweenTimer->finalXvalue, m_doRotatetweenTimer->finalYvalue, m_doRotatetweenTimer->finalZvalue));
	};

	currentTimerIndex = TimerIndex::RotateTimer;
	tempTimer = m_doRotatetweenTimer;
	dotweenTimerArray[TimerIndex::RotateTimer] = tempTimer;

	return *this;
}

Dotween& Dotween::DOQRotate(Vector3d axis, double angle, double p_duration)
{
	DotweenTimer* m_doQrotatetweenTimer = DotweenTimerPool::GetInstance()->GetDotweenTimer();

	m_doQrotatetweenTimer->Start();
	m_doQrotatetweenTimer->duration = p_duration;
	m_doQrotatetweenTimer->onUpdate = [=]() 
	{
		// �ᱹ ������ �ϴ°� angle, �׷��� ease�� ������...
		m_doQrotatetweenTimer->easeTime = (m_doQrotatetweenTimer->elapsed - m_doQrotatetweenTimer->delay) / (m_doQrotatetweenTimer->duration - m_doQrotatetweenTimer->delay);
		double easeWeight = getEasingFunction(m_doQrotatetweenTimer->m_ease)(m_doQrotatetweenTimer->easeTime);

		//double anglePerFrame = angle / (p_duration);

		previousAngle = m_doQrotatetweenTimer->movingDistanceX;

		m_doQrotatetweenTimer->movingDistanceX = easeWeight * angle;

		//GetGameObject()->GetTransform()->Rotate(axis, DegreeToRadian(m_doQrotatetweenTimer->movingDistanceX - previousAngle));
	};

	currentTimerIndex = TimerIndex::RotateTimer;
	tempTimer = m_doQrotatetweenTimer;
	dotweenTimerArray[TimerIndex::RotateTimer] = tempTimer;

	return *this;
}

/// <summary>
/// DOLookAt �Լ��� y���� ������� �ʾҴ�. (�� �Ʒ��� ȸ������ �ʴ´�.)
/// </summary>
/// <param name="lookPosition"></param>
/// <returns></returns>
Dotween& Dotween::DOLookAt(Transform* lookTransform, double p_duration, bool isYaxisInclude)
{	
	//DotweenTimer* m_doLookTweenTimer = DotweenTimerPool::GetInstance()->GetDotweenTimer();

	//Vector3d objectFront = GetGameObject()->GetTransform()->GetFront();
	//Vector3d objectUp;
	//Vector3d objectRight = GetGameObject()->GetTransform()->GetRight();
	//Vector3d objectLeft = Vector3d(-1 * objectRight.x, objectRight.y, -1 * objectRight.z);

	//Vector3d tempLookPos = lookTransform->GetWorldPosition();
	//Vector3d tempPos = GetGameObject()->GetTransform()->GetWorldPosition();
	//Vector3d tempUp = GetGameObject()->GetTransform()->GetUp();

	//Vector3d distanceVec = Vector3d(tempLookPos.x - tempPos.x, tempLookPos.y - tempPos.y, tempLookPos.z - tempPos.z);

	//double angle;
	//double sq;
	//double finalAngle;
	//double finalDegree;

	//double dot = DotProduct(GetGameObject()->GetTransform()->GetFront(), GetGameObject()->GetTransform()->GetLocalPosition() - tempLookPos);

	//// ȸ�� ���� ����
	//Vector3D axis = CrossProduct(objectFront, distanceVec);

	//angle = (objectFront.x * distanceVec.x + objectFront.z * distanceVec.z);
	//sq = (sqrt(pow(objectFront.x, 2) + pow(objectFront.z, 2)) *
	//	sqrt(pow(distanceVec.x, 2) + pow(distanceVec.z, 2)));

	//// �� ������ ������ 180�� �̻��̸� 180��, -180 ���� �̶�� -180�� 
	////finalAngle = acos( max( -1.0f, min(1.0f, angle / sq) ) );
	//finalAngle = acos(std::clamp(angle / sq, -1.0, 1.0));			// c++17 �ȴٸ�
	//finalDegree = RadianToDegree(finalAngle);

	//if (axis.y < 0)
	//	finalDegree *= -1;

	//objectUp = GetGameObject()->GetTransform()->GetUp();

	//m_doLookTweenTimer->Start();
	//m_doLookTweenTimer->duration = p_duration;
	//m_doLookTweenTimer->onUpdate = [=]()distanceVector.Normalized()
	//{
	//	double degreePerFrame = finalDegree / (m_doLookTweenTimer->duration);
	//	if (tempPos - tempLookPos != Vector3D(0,0,0) && !isnan(degreePerFrame))
	//		GetGameObject()->GetTransform()->Rotate(objectUp, DegreeToRadian(degreePerFrame * ZeldaEngine::TimeController::GetInstance().GetDeltaTime()));
	//};

	//tempTimer = m_doLookTweenTimer;
	//m_dotweenTimerMap.insert({ tempTimer, m_doLookTweenTimer->isDone });

	return *this;
}


/// <summary>
/// dotweenTimer�� OnUpdate�� ���� �������ְ� ���డ���� �Լ�.
/// �׷����� ��! OnUpdate�� �־�����Ѵ�!
/// </summary>
/// <param name="p_duration"></param>
/// <returns></returns>
Dotween& Dotween::DOCustom(double p_duration)
{
	DotweenTimer* m_doCustomTweenTimer = DotweenTimerPool::GetInstance()->GetDotweenTimer();
	m_doCustomTweenTimer->Start();
	m_doCustomTweenTimer->duration = p_duration;

	currentTimerIndex = TimerIndex::CustomTimer;
	tempTimer = m_doCustomTweenTimer;
	dotweenTimerArray[TimerIndex::CustomTimer] = tempTimer;

	return *this;
}

Dotween& Dotween::DONothing(double p_duration)
{
	DotweenTimer* m_doNothingTweenTimer = DotweenTimerPool::GetInstance()->GetDotweenTimer();
	m_doNothingTweenTimer->Start();
	m_doNothingTweenTimer->duration = p_duration;

	currentTimerIndex = TimerIndex::NothingTimer;
	tempTimer = m_doNothingTweenTimer;
	dotweenTimerArray[TimerIndex::NothingTimer] = tempTimer;

	return *this;

}

Dotween& Dotween::DOShakePosition(double p_duration, double strength /*= 1.5f*/, int vibrato /*= 1.5*/)
{
	DotweenTimer* m_doShaketweenTimer = DotweenTimerPool::GetInstance()->GetDotweenTimer();
	m_doShaketweenTimer->Start();
	m_doShaketweenTimer->duration = p_duration;
	randPos = Vector3d::zero;

	m_doShaketweenTimer->onUpdate = [=]()
	{
		if (Time::GetDeltaTime() != 0)
		{
			GetGameObject()->GetTransform()->SetWorldPosition(GetGameObject()->GetTransform()->GetWorldPosition() - randPos);

			int plusORminus = rand() % 2;

			randRange = Vector3d(rand() % vibrato, rand() % vibrato, 0.0f);

			randRange *= strength;

			switch (plusORminus)
			{
				case 0:
					randPos = randRange;
					break;

				case 1:
					randPos = Vector3d(randRange.x * -1, randRange.y * -1, 0);
					break;
			}

			GetGameObject()->GetTransform()->SetWorldPosition(GetGameObject()->GetTransform()->GetWorldPosition() + randPos);
		}
	};
	
	m_doShaketweenTimer->onExpiration = [=]()
	{
		GetGameObject()->GetTransform()->SetWorldPosition(GetGameObject()->GetTransform()->GetWorldPosition() - randPos);
	};

	currentTimerIndex = TimerIndex::ShakeTimer;
	tempTimer = m_doShaketweenTimer;
	dotweenTimerArray[TimerIndex::ShakeTimer] = tempTimer;

	return *this;
}

Dotween& Dotween::SetDelay(double p_delay)
{
	tempTimer->delay = p_delay;
	tempTimer->duration += p_delay;

	return *this;
}

Dotween& Dotween::IsRepeat(bool p_repeat)
{
	tempTimer->isRepeat = p_repeat;
	return *this;
}

Dotween& Dotween::SetEase(easing_functions p_ease)
{
	tempTimer->m_ease = p_ease;

	return *this;
}

Dotween& Dotween::OnComplete(std::function<void()> expirationFunc)
{
	dotweenTimerArray[currentTimerIndex]->onCompleteFunction = expirationFunc;
	return *this;
}

Dotween& Dotween::OnUpdate(std::function<void()> updateFunc)
{
	tempTimer->onUpdate = updateFunc;
	return *this;
}

double Dotween::AdjustRotation(double& rot)
{
	while (rot > 360 || rot < 0)
	{
		if (rot < 0)
			rot += 360;
		else if (rot > 360)
			rot -= 360;
	}
	return rot;
}

//void Dotween::clearDotweenTimerMap()
//{
//	if (tempTimer != nullptr)
//	{
//		tempTimer->onCompleteFunction = []() {};
//		tempTimer->onExpiration = []() {};
//		tempTimer->onUpdate = []() {};
//	}
//
//	auto it = m_dotweenTimerMap.begin();
//	while (it != m_dotweenTimerMap.end())
//	{
//		it->first->onUpdate = []() {};
//		it->first->onExpiration = []() {};
//		it->first->onCompleteFunction = []() {};
//		it++;
//	}
//}
