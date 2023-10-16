#include "CommonUnit.h"
#include "Scene.h"
#include "GameObject.h"
#include "Collider2D.h"
#include "BoxCollider2D.h"
#include "CircleCollider2D.h"
#include "Transform.h"
#include "Input.h"
#include "Curve.h"
#include "YunutyTime.h"
#include "StaticMeshRenderer.h"
#include <string>
using namespace yunutyEngine;

CommonUnit::CommonUnit()
	: m_unitType(UnitType::WARRIOR),
	m_state(State::IDLE),
	lastMouseScreenPos{ 0, 0 },
	m_isPlayer(false),
	m_isDying(false),
	m_isAttackReady(true),
	m_hp(10.0f),
	m_mp(5.0f),
	m_defense(1.0f),
	m_moveSpeed(5.0f),
	m_attackPower(1.0f),
	m_attackSpeed(1.0f),
	m_attackCoolDown(0.7f),
	m_attackRange(2.0f),
	m_detectRange(4.0f),
	m_hitboxWidth(2.0f),
	m_hitboxHeight(2.0f)
{
	attackTimer.duration = 1.0f / m_attackSpeed;
	attackTimer.onTimerExpiration = [this]()
	{
		m_state = State::IDLE;
		attackCoolDownTimer.Start();
	};
	attackCoolDownTimer.duration = m_attackCoolDown;
	attackCoolDownTimer.onTimerExpiration = [this]()
	{
		m_isAttackReady = true;
	};

}

void CommonUnit::Initialize(UnitType unitType)
{
	m_unitType = unitType;
	switch (m_unitType)
	{
		case UnitType::PLAYER:
		{
			m_isPlayer = true;
			m_hp = 200.0f;
			m_mp = 50.0f;
			m_defense = 10.0f;
			m_moveSpeed = 5.0f;
			m_attackPower = 2.0f;
			m_attackSpeed = 1.0f;
			m_attackCoolDown = 0.7f;
			m_attackRange = 1.5f;
			m_detectRange = 4.0f;
			m_hitboxWidth = 1.2f;
			m_hitboxHeight = 2.0f;
		}
		break;
		case UnitType::WARRIOR:
		{
			m_hp = 20.0f;
			m_mp = 10.0f;
			m_defense = 5.0f;
			m_moveSpeed = 5.0f;
			m_attackPower = 2.0f;
			m_attackSpeed = 1.0f;
			m_attackCoolDown = 0.7f;
			m_attackRange = 1.5f;
			m_detectRange = 4.0f;
			m_hitboxWidth = 1.2f;
			m_hitboxHeight = 2.0f;
		}
		break;
		case UnitType::ARCHER:
		{
			m_hp = 14.0f;
			m_mp = 10.0f;
			m_defense = 3.0f;
			m_moveSpeed = 5.0f;
			m_attackPower = 4.0f;
			m_attackSpeed = 1.15f;
			m_attackCoolDown = 0.8f;
			m_attackRange = 5.0f;
			m_detectRange = 6.0f;
			m_hitboxWidth = 1.2f;
			m_hitboxHeight = 2.0f;
		}
		break;
		default:
			break;
	}
	attackTimer.duration = 1.0f / m_attackSpeed;
	attackCoolDownTimer.duration = m_attackCoolDown;

	// Player Hitbox
	auto playerHitbox = GetGameObject()->AddComponent<BoxCollider2D>();
	playerHitbox->SetWidth(m_hitboxWidth);
	playerHitbox->SetHeight(m_hitboxHeight);
	// Player Detection Range
	auto CommonUnitDetectCollider = GetGameObject()->AddComponent<CircleCollider2D>();
	CommonUnitDetectCollider->SetRadius(m_detectRange);
	CommonUnitDetectCollider->SetTag("Detect");
	// Player Attack Range
	auto CommonUnitAttackRange = GetGameObject()->AddComponent<CircleCollider2D>();
	CommonUnitAttackRange->SetRadius(m_attackRange);
	CommonUnitAttackRange->SetTag("Attack");

#if defined(DEBUG) || defined(_DEBUG)
	// Hitbox Debug Cube
	auto childObject = GetGameObject()->AddGameObject();
	auto hixBoxRect = childObject->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
	hixBoxRect->GetGI().LoadMesh("Cube");
	hixBoxRect->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0,1,0,0 });
	hixBoxRect->GetGI().GetMaterial()->SetPixelShader(L"DebugPS.cso");
	childObject->GetTransform()->scale = Vector3d(m_hitboxWidth, m_hitboxHeight, 0);
	// Detection Range Debug Sphere
	auto childObject2 = GetGameObject()->AddGameObject();
	auto detectRangeSphere = childObject2->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
	detectRangeSphere->GetGI().LoadMesh("Sphere");
	detectRangeSphere->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1,0,1,0 });
	detectRangeSphere->GetGI().GetMaterial()->SetPixelShader(L"DebugPS.cso");
	childObject2->GetTransform()->scale = Vector3d(m_detectRange * 2, m_detectRange * 2, 0);
	// Attack Range Debug Sphere
	auto childObject3 = GetGameObject()->AddGameObject();
	auto attackRangeSphere = childObject3->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
	attackRangeSphere->GetGI().LoadMesh("Sphere");
	attackRangeSphere->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1,0,0,0 });
	attackRangeSphere->GetGI().GetMaterial()->SetPixelShader(L"DebugPS.cso");
	childObject3->GetTransform()->scale = Vector3d(m_attackRange * 2, m_attackRange * 2, 0);
#endif
}

void CommonUnit::Idle()
{
	// Idle Animation
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0, 1, 0, 0 });
	if (m_isPlayer)
	{
		if (yunutyEngine::Input::isKeyDown(KeyCode::MouseRightClick))
		{
			// Ctrl + RightClick �Է��� ������ AttackMove ���·� ��ȯ
			if (yunutyEngine::Input::isKeyDown(KeyCode::Control))
			{
				m_state = State::ATTACKMOVE;
				return;
			}
			else // Ctrl ���� RightClick �Է¸� ������ ��
			{
				m_state = State::MOVE;
				return;
			}
		}
	}

	// ���� Ž���Ǿ��ٸ� Chase ���·� ��ȯ
	if (!detectedEnemies.empty() && attackRangeEnemies.empty())
	{
		m_state = State::CHASE;
		targetEnemy = *detectedEnemies.begin();
		return;
	}
	// ���ݹ��� �ȿ� ���� �ִٸ� Attack ���·� ��ȯ
	if (m_isAttackReady && !attackRangeEnemies.empty())
	{
		m_state = State::ATTACK;
		attackTimer.Start();
		return;
	}
}

void CommonUnit::Move()
{
	// Ctrl + RightClick �Է��� ������ AttackMove ���·� ��ȯ
	if (yunutyEngine::Input::isKeyDown(KeyCode::MouseRightClick))
	{
		if (yunutyEngine::Input::isKeyDown(KeyCode::Control))
		{
			m_state = State::ATTACKMOVE;
			return;
		}
	}
	// Move Animation
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 1, 0, 0 });

	// �̵��� �������� ȸ��
	//Vector3d newForward = (GetTransform()->position - lastMouseScreenPos).Normalized();
	//Vector3d newRight = (Vector3d::Cross(GetTransform()->GetWorldRotation().Up(), newForward)).Normalized();
	//GetTransform()->SetWorldRotation(Quaternion::MakeWithAxes(newRight, GetTransform()->GetWorldRotation().Up(), newForward));

	// ���� ���������� ���콺 Ŭ���� ��ġ���� �� ��ġ�� ����.
	Vector2 moveVec = lastMouseScreenPos - GetTransform()->position;
	// ���콺 Ŭ���� ��ġ�� ������ ������ �����δ�.
	moveToDestination(moveVec);
	if (moveVec.Magnitude() <= 0.03)
	{
		m_state = State::IDLE;
	}
}

void CommonUnit::AttackMove()
{
	// Ctrl �Է��� ������ �ʰ� Right Click�� ������ Move ���·� ��ȯ
	if (yunutyEngine::Input::isKeyDown(KeyCode::MouseRightClick))
	{
		if (!yunutyEngine::Input::isKeyDown(KeyCode::Control))
		{
			m_state = State::MOVE;
			return;
		}
	}

	// �̵��ϴ� ���� ���� ������ ������ �����Ѵ�.
	if (!detectedEnemies.empty())
	{
		m_state = State::CHASE;
		return;
	}

	// Move Animation
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 1, 0, 0 });

	// �̵��� �������� ȸ��
	//Vector3d newForward = (GetTransform()->position - lastMouseScreenPos).Normalized();
	//Vector3d newRight = (Vector3d::Cross(GetTransform()->GetWorldRotation().Up(), newForward)).Normalized();
	//GetTransform()->SetWorldRotation(Quaternion::MakeWithAxes(newRight, GetTransform()->GetWorldRotation().Up(), newForward));

	// ���� ���������� ���콺 Ŭ���� ��ġ���� �� ��ġ�� ����.
	Vector2 moveVec = lastMouseScreenPos - GetTransform()->position;
	// ���콺 Ŭ���� ��ġ�� ������ ������ �����δ�.
	moveToDestination(moveVec);
	if (moveVec.Magnitude() <= 0.03)
	{
		m_state = State::IDLE;
	}
}

void CommonUnit::Attack()
{
	CommonUnit* enemy = nullptr;
	// attackRangeEnemies �߿� targetEnemy�� ������ ����
	for (auto it = attackRangeEnemies.begin(); it != attackRangeEnemies.end();)
	{
		if (*it == targetEnemy)
		{
			enemy = *it;
			break;
		}
		else
		{
			++it;
		}
	}
	// ������ attackRangeEnemies.begin()�� ����
	if (enemy == nullptr && !attackRangeEnemies.empty())
		enemy = *attackRangeEnemies.begin();

	// ���� ������ ���� ȸ���Ѵ�.
	//Vector3d newForward = (GetTransform()->position - enemy->GetTransform()->position);
	//Vector3d newRight = (Vector3d::Cross(GetTransform()->GetWorldRotation().Up(), newForward));
	//GetTransform()->SetWorldRotation(Quaternion::MakeWithAxes(newRight, GetTransform()->GetWorldRotation().Up(), newForward));

	// Attack Animation
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 0, 0, 0 });
	// Damage
	if (enemy != nullptr)
	{
		if (m_isAttackReady)
		{
			enemy->SetHP(enemy->GetHP() - m_attackPower);
			m_isAttackReady = false;
		}
	}
}

void CommonUnit::Chase()
{
	// Chase Animation
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 0, 1, 0 });

	if (m_isPlayer)
	{
		if (yunutyEngine::Input::isKeyDown(KeyCode::MouseRightClick))
		{
			// Ctrl + RightClick �Է��� ������ AttackMove ���·� ��ȯ
			if (yunutyEngine::Input::isKeyDown(KeyCode::Control))
			{
				m_state = State::ATTACKMOVE;
				return;
			}
			else // Ctrl ���� RightClick �Է¸� ������ ��
			{
				m_state = State::MOVE;
				return;
			}
		}
	}

	// �����ϴ� ���� ���� �װų� Ž������ ������ ������ IDLE ���·� �ٲ��ش�.
	if (targetEnemy != *detectedEnemies.begin())
	{
		m_state = State::IDLE;
		return;
	}

	// ���� �ִ� ��ġ���� �� ��ġ�� ����.
	Vector3d targetEnemyPos = targetEnemy->GetTransform()->position;
	Vector2 moveVec = targetEnemyPos - GetTransform()->position;

	// ���� �ִ� �������� ȸ��
	//Vector3d newForward = GetTransform()->position - targetEnemyPos;
	//Vector3d newRight = Vector3d::Cross(GetTransform()->GetWorldRotation().Up(), newForward);
	//GetTransform()->SetWorldRotation(Quaternion::MakeWithAxes(newRight, GetTransform()->GetWorldRotation().Up(), newForward));

	// ���� �ִ� ��ġ�� ������ ������ �����δ�.
	moveToDestination(moveVec);
	if (moveVec.Magnitude() <= m_attackRange + targetEnemy->GetHitBoxWidth() / 2.0)
	{
		m_state = State::IDLE;
	}
}

void CommonUnit::Die()
{
	// Die Animation
	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 1, 1, 0 });
	GetGameObject()->SetSelfActive(false);
	//Scene::getCurrentScene()->DestroyGameObject(GetGameObject());
	m_isDying = true;

	// Ž������ �ȿ� �ִ� ������ Ž��List���� ���� ������Ʈ ����
}

void CommonUnit::moveToDestination(Vector3d targetPos)
{
	//GetTransform()->position += targetPos.Normalized() * m_moveCurve.Evaluate(moveTimer.elapsed / moveTimer.duration) * 0.08f;
	GetTransform()->position += targetPos.Normalized() * Time::GetDeltaTime() * m_moveSpeed;
}

void CommonUnit::Update()
{
	if (m_isDying)
		return;

	attackCoolDownTimer.Update();
	if (m_hp <= 0.0f)
	{
		m_state = State::DIE;
	}

	switch (m_state)
	{
		case State::IDLE:
		{
			Idle();
		}
		break;
		case State::MOVE:
		{
			Move();
		}
		break;
		case State::ATTACKMOVE:
		{
			AttackMove();
		}
		break;
		case State::CHASE:
		{
			Chase();
		}
		break;
		case State::ATTACK:
		{
			attackTimer.Update();
			Attack();
		}
		break;
		case State::DIE:
		{
			Die();
		}
		break;
		default:
			break;
	}
}

void CommonUnit::OnCollisionEnter2D(const yunutyEngine::Collision2D& collision)
{
	if (collision.m_Collider->GetTag() == "Detect")
	{
		auto targetObject = collision.m_OtherCollider->GetGameObject();
		if (targetObject->GetComponent<CommonUnit>() != nullptr)
		{
			auto target = targetObject->GetComponent<CommonUnit>();
			if (m_isPlayer)
			{
				if (target->GetIsPlayer() == false)
					detectedEnemies.emplace_back(target);
			}
			else
			{
				if (target->GetIsPlayer() == true)
					detectedEnemies.emplace_back(target);
			}
		}
	}

	if (collision.m_Collider->GetTag() == "Attack")
	{
		auto targetObject = collision.m_OtherCollider->GetGameObject();
		if (targetObject->GetComponent<CommonUnit>() != nullptr)
		{
			auto target = targetObject->GetComponent<CommonUnit>();
			if (m_isPlayer)
			{
				if (target->GetIsPlayer() == false)
					attackRangeEnemies.emplace_back(target);
			}
			else
			{
				if (target->GetIsPlayer() == true)
					attackRangeEnemies.emplace_back(target);
			}
		}
	}
}

void CommonUnit::OnCollisionExit2D(const yunutyEngine::Collision2D& collision)
{
	if (collision.m_Collider->GetTag() == "Detect")
	{
		auto targetObject = collision.m_OtherCollider->GetGameObject();
		if (targetObject->GetComponent<CommonUnit>() != nullptr)
		{
			auto target = targetObject->GetComponent<CommonUnit>();
			for (auto it = detectedEnemies.begin(); it != detectedEnemies.end();)
			{
				if (*it == target)
				{
					detectedEnemies.erase(it);
					break;
				}
				else
				{
					++it;
				}
			}
		}
	}

	if (collision.m_Collider->GetTag() == "Attack")
	{
		auto targetObject = collision.m_OtherCollider->GetGameObject();
		if (targetObject->GetComponent<CommonUnit>() != nullptr)
		{
			auto target = targetObject->GetComponent<CommonUnit>();
			for (auto it = attackRangeEnemies.begin(); it != attackRangeEnemies.end();)
			{
				if (*it == target)
				{
					attackRangeEnemies.erase(it);
					break;
				}
				else
				{
					++it;
				}
			}
		}
	}
}
