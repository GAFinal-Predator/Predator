#include "Player.h"
#include "Dotween.h"

Player::Player()
{

}

void Player::Start()
{
	unitType = "Player";
	m_speed = 2.0f;
}

void Player::Update()
{

}

//void Player::IdentifyAction(Vector3d endPosition, float distance)
//{
//	//GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 1, 1, 0 });
//	GetGameObject()->GetComponent<Dotween>()->DOMove(endPosition, distance / m_speed).OnComplete([=]()
//		{
//			AttackAction(endPosition, distance);
//		});
//}

//void Player::AttackAction(Vector3d endPosition, float distance)
//{
//	// ���� ��Ÿ� ���� ������ ��� ȣ��Ǵ� �Լ�.
//	GetGameObject()->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>()->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1, 1, 1, 0 });
//}

