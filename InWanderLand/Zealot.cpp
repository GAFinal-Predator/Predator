#include "Zealot.h"
#include "GameObject.h"
#include "Collider2D.h"
#include "Transform.h"
#include "Input.h"
#include "Curve.h"
#include "YunutyTime.h"
#include "Zergling.h"
#include "StaticMeshRenderer.h"
using namespace yunutyEngine;

Zealot::Zealot()
	: m_state(State::IDLE), m_isMove(false), m_isChase(false), m_isAttack(false),
	m_moveSpeed(.2f), m_attackRange(1.5f),
	targetEnemyPos{ 0, 0, 0 }, lastMouseScreenPos{ 0, 0 }
{

}

void Zealot::Idle()
{
	// Idle Animation
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().SetColor(0, yunuGI::Color{ 0, 1, 0, 0 });
	// ������� �̵� ��� �Է��� ������ Move ���·� ��ȯ
	if (yunutyEngine::Input::isKeyDown(KeyCode::MouseLeftClick))
	{
		m_state = State::MOVE;
		m_isMove = true;
		// ���콺 Ŭ���� ��ġ�� �޴´�.
		//lastMouseScreenPos = yunutyEngine::Input::getMouseScreenPosition();
	}
	// ���� �߰� ���̾��ٸ� Chase ���·� ��ȯ
	if (m_isChase)
	{
		m_state = State::CHASE;
	}
	// ���ݹ��� �ȿ� ���� �ִٸ� Attack ���·� ��ȯ
}

void Zealot::Move()
{
	// Move Animation
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().SetColor(0, yunuGI::Color{ 1, 1, 0, 0 });

	if (yunutyEngine::Input::isKeyDown(KeyCode::MouseLeftClick))
	{
		// ���콺 Ŭ���� ��ġ�� �޴´�.
		//lastMouseScreenPos = yunutyEngine::Input::getMouseScreenPosition();
		m_isMove = true;
		//m_isChase = false;

		m_moveCurveElapsed = 0.0f;
		// �̵� ��� ����� �ð� ���. �Ÿ� = �ӷ� * �ð�
		m_moveCurveDuration = (lastMouseScreenPos - GetTransform()->position).Magnitude() / m_moveSpeed;
	}
	if (m_isMove)
	{
		// ���� ���������� ���콺 Ŭ���� ��ġ���� �� ��ġ�� ����.
		Vector2 moveVec = lastMouseScreenPos - GetTransform()->position;
		// ���콺 Ŭ���� ��ġ�� ������ ������ �����δ�.
		moveToDestination(moveVec);
		if (moveVec.Magnitude() <= 0.05)
		{
			m_state = State::IDLE;
			m_isMove = false;
		}
	}
}

void Zealot::Attack()
{
	if (m_isAttack)
	{
		// Attack Animation
		GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().SetColor(0, yunuGI::Color{ 1, 0, 0, 0 });
		// Damage
	}
	// ���ݸ�� ������ IDLE�� ��ȯ����� ��.
}

void Zealot::Chase()
{
	// Chase Animation
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().SetColor(0, yunuGI::Color{ 1, 0, 1, 0 });
	if (m_isChase && !m_isMove)
	{
		// ���� �ִ� ��ġ���� �� ��ġ�� ����.
		Vector2 moveVec = targetEnemyPos - GetTransform()->position;
		// ���� �ִ� ��ġ�� ������ ������ �����δ�.
		moveToDestination(moveVec);
		if (moveVec.Magnitude() <= m_attackRange)
		{
			m_state = State::ATTACK;
			m_isChase = false;
			m_isAttack = true;
		}
	}
	// Chase ���¿� �ִ� ���� ������� �̵� ����� ������ ��
	if (yunutyEngine::Input::isKeyDown(KeyCode::MouseLeftClick))
	{
		// ���콺 Ŭ���� ��ġ�� �޴´�.
		//lastMouseScreenPos = yunutyEngine::Input::getMouseScreenPosition();
		m_state = State::MOVE;
		m_isMove = true;
	}
	// ���� �߰��ϴ� ���� ���� ������ Chase���¿��� ���������� ��.
}

void Zealot::Die()
{
	// Die Animation
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().SetColor(0, yunuGI::Color{ 1, 1, 1, 0 });
}

void Zealot::moveToDestination(Vector3d targetPos)
{
	m_moveCurveElapsed += Time::GetDeltaTime();
	if (m_moveCurveElapsed > m_moveCurveDuration)
		m_moveCurveElapsed = m_moveCurveDuration;
	GetTransform()->position += targetPos.Normalized() * m_moveCurve.Evaluate(m_moveCurveElapsed / m_moveCurveDuration) * m_moveSpeed;
}

void Zealot::Update()
{
	switch (m_state)
	{
		case State::IDLE:
			Idle();
			break;
		case State::MOVE:
			Move();
			break;
		case State::CHASE:
			Chase();
			break;
		case State::ATTACK:
			Attack();
			break;
		case State::DIE:
			Die();
			break;
		default:
			break;
	}
}

void Zealot::OnCollisionEnter2D(const yunutyEngine::Collision2D& collision)
{
	auto targetObject = collision.m_OtherCollider->GetGameObject();
	if (targetObject->GetComponent<Zergling>() != nullptr)
	{
		if (!m_isChase)
		{
			targetEnemyPos = targetObject->GetTransform()->GetWorldPosition();
			//targetEnemyPos.x = targetEnemyPos.x - m_attackRange;
			m_state = State::CHASE;
			m_isChase = true;
			m_isMove = false;
		}
	}
}
