#include "UnitProductionOrder.h"
#include "RangeSystem.h"
#include "PlayerController.h"

GameObject* UnitProductionOrder::CreateUnitToOrder()
{
	// ���ҽ� �Ŵ��� ��������
	const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();

	auto capsuleMesh = _resourceManager->GetMesh(L"Capsule");
	auto sphereMesh = _resourceManager->GetMesh(L"Sphere");

	auto shaderList = _resourceManager->GetShaderList();
	yunuGI::IShader* debugShader;
	for (auto e : shaderList)
	{
		if (e->GetShaderInfo().rasterizer == yunuGI::Rasterizer::Wire)
			debugShader = e;
	}

	/// 1. UnitGameObject ����
	auto unitGameObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();

	// 1-1. unitType�� ���� Unit ������Ʈ �߰�
	Unit* m_unitComponent = unitGameObject->AddComponent<Unit>();

	// 1-2. (�ӽ�) StaticMeshRenderer �߰�
	auto unitMesh = unitGameObject->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
	unitMesh->GetGI().SetMesh(capsuleMesh);

	/// 2. RangeSystem Gameobject �߰�
	auto unitRangeSystemObject = yunutyEngine::Scene::getCurrentScene()->AddGameObject();
	unitRangeSystemObject->SetParent(unitGameObject);
	unitRangeSystemObject->GetTransform()->SetWorldRotation(Vector3d(90, 0, 0));

	// 2-1. RangeSystem Component �߰�
	RangeSystem* rangeSystemComponent = unitRangeSystemObject->AddComponent<RangeSystem>();
	rangeSystemComponent->SetOwnerUnitComponent(m_unitComponent);
	// 2-2. RangeSystem Debug Mesh �߰�
	auto rangeSystemMesh = unitRangeSystemObject->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
	rangeSystemMesh->GetGI().SetMesh(sphereMesh);
	rangeSystemMesh->GetGI().GetMaterial()->SetPixelShader(debugShader);

	/// 3. Collider Component �߰�
	auto unitColliderComponent = unitGameObject->AddComponent<CircleCollider2D>();
	//unitColliderComponent->SetRadius(1.0f);

	m_unitComponent->GetGameObject()->setName(m_objectName);
	m_unitComponent->SetUnitType(m_unitType);
	m_unitComponent->SetUnitSide(m_unitSide);

	m_unitComponent->SetUnitHp(m_hp);
	m_unitComponent->SetUnitAp(m_ap);
	unitMesh->GetGI().GetMaterial()->SetColor(m_unitColor);

	m_unitComponent->SetAtkRadius(m_atkRadius);
	m_unitComponent->SetIdRadius(m_idRadius);
	m_unitComponent->SetUnitSpeed(m_unitSpeed);

	rangeSystemComponent->SetRadius(m_idRadius);

	rangeSystemMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0, 0, 1, 0 });		// �� �ٸ��� �ҰŸ� switch-case�ȿ� �ֱ�.

	unitRangeSystemObject->GetTransform()->scale = Vector2d(m_idRadius * 2, m_idRadius * 2);

	if (m_unitSide == Unit::UnitSide::Player)
		SetPlayerRelatedComponents(m_unitComponent);

	return unitGameObject;
}

void UnitProductionOrder::SetPlayerRelatedComponents(Unit* playerUnit)
{
	playerUnit->SetPlayerSerialNumber();
	PlayerController::GetInstance()->AddPlayerUnit(playerUnit);
}

