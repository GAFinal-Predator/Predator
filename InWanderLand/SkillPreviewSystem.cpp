#include "SkillPreviewSystem.h"

#include "DirectXMath.h"

#define OFFSET 1.414
#define AXIS Vector3d{-1,0,0}
#define Y_OFFSET 0.1
#define VERTEX_OFFSET 0.1


void SkillPreviewSystem::ObjectInitializer(graphics::StaticMeshRenderer* comp)
{
	const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
	yunuGI::IShader* vs = _resourceManager->GetShader(L"TextureVS.cso");
	yunuGI::IShader* ps = _resourceManager->GetShader(L"GuideLinePS.cso");
	yunuGI::ITexture* move = _resourceManager->GetTexture(L"Texture/move.png");
	comp->GetGI().GetMaterial()->SetVertexShader(vs);
	comp->GetGI().GetMaterial()->SetPixelShader(ps);
	comp->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, move);
}

void SkillPreviewSystem::Init()
{
	const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
	yunuGI::IShader* vs = _resourceManager->GetShader(L"TextureVS.cso");
	yunuGI::IShader* ps = _resourceManager->GetShader(L"GuideLinePS.cso");
	yunuGI::ITexture* quad = _resourceManager->GetTexture(L"Texture/quad.png");
	yunuGI::ITexture* triangle = _resourceManager->GetTexture(L"Texture/triangle.png");
	yunuGI::ITexture* circle = _resourceManager->GetTexture(L"Texture/circle.png");
	yunuGI::ITexture* move = _resourceManager->GetTexture(L"Texture/move.png");
	yunuGI::IMesh* quadMesh = _resourceManager->GetMesh(L"Rectangle");

	for (int i = 0; i < 10; ++i)
	{
		this->Borrow();
	}

#pragma region TemporaryRouteMeshRenderer
	{
		this->temporaryRouteMeshRendererObj = Scene::getCurrentScene()->AddGameObject();
		this->temporaryRouteMeshRenderer = this->temporaryRouteMeshRendererObj->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		this->temporaryRouteMeshRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
		this->temporaryRouteMeshRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
		this->temporaryRouteMeshRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, move);
	}
#pragma endregion

#pragma region RobinQSkillPreview
	{
		this->robinQSkillPreviewObj = Scene::getCurrentScene()->AddGameObject();
		auto arrowPreviewObj = Scene::getCurrentScene()->AddGameObjectFromFBX("Guideline");
		arrowPreviewObj->SetParent(this->robinQSkillPreviewObj);
		auto arrowRenderer = arrowPreviewObj->GetChildren()[0]->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		arrowRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
		arrowRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
		arrowRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, triangle);
		arrowRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1,0,0,1 });

		auto arrowBody = Scene::getCurrentScene()->AddGameObjectFromFBX("Guideline");
		arrowBody->SetParent(this->robinQSkillPreviewObj);
		auto bodyRenderer = arrowBody->GetChildren()[0]->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		bodyRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
		bodyRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
		bodyRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, quad);
		bodyRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1,0,0,1 });

		this->robinQSkillPreviewObj->SetSelfActive(false);
	}
#pragma endregion 


#pragma region UrsulaQSkillPreview
	{
		this->ursulaQSkillPreviewObj = Scene::getCurrentScene()->AddGameObject();
		{
			auto arrowPreviewObj = Scene::getCurrentScene()->AddGameObjectFromFBX("Guideline");
			arrowPreviewObj->SetParent(this->ursulaQSkillPreviewObj);
			auto arrowRenderer = arrowPreviewObj->GetChildren()[0]->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>();
			arrowRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
			arrowRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
			arrowRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, triangle);
			arrowRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0.545,0,1,1 });
		}
		{
			auto arrowBody = Scene::getCurrentScene()->AddGameObjectFromFBX("Guideline");
			arrowBody->SetParent(this->ursulaQSkillPreviewObj);
			auto bodyRenderer = arrowBody->GetChildren()[0]->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>();
			bodyRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
			bodyRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
			bodyRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, quad);
			bodyRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0.545,0,1,1 });
		}

		{
			auto circleOne = Scene::getCurrentScene()->AddGameObject();
			circleOne->GetTransform()->SetLocalRotation(Quaternion{ Vector3d{90,0,0} });
			circleOne->SetParent(this->ursulaQSkillPreviewObj);
			auto circleOneRenderer = circleOne->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
			circleOneRenderer->GetGI().SetMesh(quadMesh);
			circleOneRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
			circleOneRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
			circleOneRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, circle);
			circleOneRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0.545,0,1,1 });
		}
		{
			auto circleTwo = Scene::getCurrentScene()->AddGameObject();
			circleTwo->GetTransform()->SetLocalRotation(Quaternion{ Vector3d{90,0,0} });
			circleTwo->SetParent(this->ursulaQSkillPreviewObj);
			auto circleTwoRenderer = circleTwo->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
			circleTwoRenderer->GetGI().SetMesh(quadMesh);
			circleTwoRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
			circleTwoRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
			circleTwoRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, circle);
			circleTwoRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0.545,0,1,1 });
		}
		{
			auto circleThree = Scene::getCurrentScene()->AddGameObject();
			circleThree->GetTransform()->SetLocalRotation(Quaternion{ Vector3d{90,0,0} });
			circleThree->SetParent(this->ursulaQSkillPreviewObj);
			auto circleThreeRenderer = circleThree->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
			circleThreeRenderer->GetGI().SetMesh(quadMesh);
			circleThreeRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
			circleThreeRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
			circleThreeRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, circle);
			circleThreeRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0.545,0,1,1 });
		}
		this->ursulaQSkillPreviewObj->SetSelfActive(false);
	}
#pragma endregion

#pragma region UrsulaWSkill
	{
		this->ursulaWSkillPreviewObj = Scene::getCurrentScene()->AddGameObject();
		{
			auto circleOne = Scene::getCurrentScene()->AddGameObject();
			circleOne->GetTransform()->SetLocalRotation(Quaternion{ Vector3d{90,0,0} });
			circleOne->SetParent(this->ursulaWSkillPreviewObj);
			auto circleOneRenderer = circleOne->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
			circleOneRenderer->GetGI().SetMesh(quadMesh);
			circleOneRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
			circleOneRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
			circleOneRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, circle);
			circleOneRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0.545,0,1,1 });
		}
		this->ursulaWSkillPreviewObj->SetSelfActive(false);
	}
#pragma endregion

#pragma region HanselQSkill
	{
		this->hanselQSkillPreviewObj = Scene::getCurrentScene()->AddGameObject();
		{
			auto arrowPreviewObj = Scene::getCurrentScene()->AddGameObjectFromFBX("Guideline");
			arrowPreviewObj->SetParent(this->hanselQSkillPreviewObj);
			auto arrowRenderer = arrowPreviewObj->GetChildren()[0]->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>();
			arrowRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
			arrowRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
			arrowRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, triangle);
			arrowRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1,0.5,0,1 });
		}
		{
			auto arrowBody = Scene::getCurrentScene()->AddGameObjectFromFBX("Guideline");
			arrowBody->SetParent(this->hanselQSkillPreviewObj);
			auto bodyRenderer = arrowBody->GetChildren()[0]->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>();
			bodyRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
			bodyRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
			bodyRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, quad);
			bodyRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1,0.5,0,1 });
		}

		{
			auto circleOne = Scene::getCurrentScene()->AddGameObject();
			circleOne->GetTransform()->SetLocalRotation(Quaternion{ Vector3d{90,0,0} });
			circleOne->SetParent(this->hanselQSkillPreviewObj);
			auto circleOneRenderer = circleOne->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
			circleOneRenderer->GetGI().SetMesh(quadMesh);
			circleOneRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
			circleOneRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
			circleOneRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, circle);
			circleOneRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1,0.5,0,1 });
		}
		this->hanselQSkillPreviewObj->SetSelfActive(false);
	}
#pragma endregion

#pragma region HanselWSkillPreview
	{
		this->hanselWSkillPreviewObj = Scene::getCurrentScene()->AddGameObject();
		auto arrowPreviewObj = Scene::getCurrentScene()->AddGameObjectFromFBX("Guideline");
		arrowPreviewObj->SetParent(this->hanselWSkillPreviewObj);
		auto arrowRenderer = arrowPreviewObj->GetChildren()[0]->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		arrowRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
		arrowRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
		arrowRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, triangle);
		arrowRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1,0.5,0,1 });

		auto arrowBody = Scene::getCurrentScene()->AddGameObjectFromFBX("Guideline");
		arrowBody->SetParent(this->hanselWSkillPreviewObj);
		auto bodyRenderer = arrowBody->GetChildren()[0]->GetComponent<yunutyEngine::graphics::StaticMeshRenderer>();
		bodyRenderer->GetGI().GetMaterial()->SetVertexShader(vs);
		bodyRenderer->GetGI().GetMaterial()->SetPixelShader(ps);
		bodyRenderer->GetGI().GetMaterial()->SetTexture(yunuGI::Texture_Type::Temp0, quad);
		bodyRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1,0.5,0,1 });

		this->hanselWSkillPreviewObj->SetSelfActive(false);
	}
#pragma endregion 
}

void SkillPreviewSystem::ShowRobinQSkill(const Vector3d& objectPos, float maxDistance)
{
	if (this->robinQSkillPreviewObj->GetActive() == false)
	{
		this->robinQSkillPreviewObj->SetSelfActive(true);
	}

	// 매개변수로 들어온 objectPos로부터 사거리가 표시됩니다.
	// maxDistance로 들어온 값까지 늘어납니다.

	// this->robinQSkillPreviewObj의 위치를 objectPos로
	auto offsetPos = objectPos;
	offsetPos.y += Y_OFFSET;
	this->robinQSkillPreviewObj->GetTransform()->SetLocalPosition(offsetPos);

	auto distToXZPlane = abs(camObj->GetTransform()->GetWorldPosition().y);
	auto centeredPosition = Input::getMouseScreenPositionNormalizedZeroCenter();

	Vector3d mouseWorldPos = camObj->GetComponent<yunutyEngine::graphics::Camera>()->GetProjectedPoint(centeredPosition, distToXZPlane, Vector3d(0, 1, 0));
	Vector3d mouseVector = mouseWorldPos - objectPos;
	mouseWorldPos.y = 0.f;
	auto normalizedPos = mouseVector.Normalize(mouseVector);


	auto basicAxis = AXIS;

	float dotValue = (Vector3d::Dot(basicAxis, normalizedPos));

	float angle = acos(dotValue);
	angle = angle * (180 / yunutyEngine::math::PI);

	if (mouseWorldPos.z - objectPos.z < 0)
	{
		angle = 360.0f - angle;
	}

	this->robinQSkillPreviewObj->GetChildren()[static_cast<int>(RobinQSkillInfo::ArrowHead)]->GetTransform()->SetLocalRotation(Quaternion{ Vector3f{0.f,angle,0.f} });
	this->robinQSkillPreviewObj->GetChildren()[static_cast<int>(RobinQSkillInfo::ArrowBody)]->GetTransform()->SetLocalRotation(Quaternion{ Vector3f{0.f,angle,0.f} });

	float distance = sqrt(pow(objectPos.x - mouseWorldPos.x, 2) + pow(objectPos.y - mouseWorldPos.y, 2) + pow(objectPos.z - mouseWorldPos.z, 2));

	if (distance > OFFSET)
	{
		if (distance >= maxDistance)
		{
			distance = maxDistance;
		}
		this->robinQSkillPreviewObj->GetChildren()[static_cast<int>(RobinQSkillInfo::ArrowBody)]->GetTransform()->SetLocalScale(Vector3d{ (distance - OFFSET) / 2 ,1.0,1.0 });

		this->robinQSkillPreviewObj->GetChildren()[static_cast<int>(RobinQSkillInfo::ArrowHead)]->GetTransform()->SetLocalPosition(normalizedPos * (distance - OFFSET));
	}
}

void SkillPreviewSystem::HideRobinQSkill()
{
	if (this->robinQSkillPreviewObj->GetActive() == true)
	{
		this->robinQSkillPreviewObj->SetSelfActive(false);
	}
}

void SkillPreviewSystem::ShowUrsulaQSkill(const Vector3d& objectPos, float maxDistance, const Vector3d& circleOnePos, const Vector3d& circleTwoPos, const Vector3d& circleThreePos, Vector3d circleRadius)
{
	circleRadius.x *= 2;
	circleRadius.x *= 2;
	circleRadius.x *= 2;

	if (this->ursulaQSkillPreviewObj->GetActive() == false)
	{
		this->ursulaQSkillPreviewObj->SetSelfActive(true);
	}

	// 매개변수로 들어온 objectPos로부터 사거리가 표시됩니다.
	// maxDistance로 들어온 값까지 늘어납니다.

	// this->robinQSkillPreviewObj의 위치를 objectPos로
	auto offsetPos = objectPos;
	offsetPos.y += Y_OFFSET;
	this->ursulaQSkillPreviewObj->GetTransform()->SetLocalPosition(offsetPos);

	// objectPos로 부터 마우스 위치까지의 거리
	auto distToXZPlane = abs(camObj->GetTransform()->GetWorldPosition().y);
	auto centeredPosition = Input::getMouseScreenPositionNormalizedZeroCenter();

	Vector3d mouseWorldPos = camObj->GetComponent<yunutyEngine::graphics::Camera>()->GetProjectedPoint(centeredPosition, distToXZPlane, Vector3d(0, 1, 0));
	Vector3d mouseVector = mouseWorldPos - objectPos;
	mouseWorldPos.y = 0.f;
	auto normalizedPos = mouseVector.Normalize(mouseVector);

	auto basicAxis = AXIS;

	float dotValue = (Vector3d::Dot(basicAxis, normalizedPos));

	float angle = acos(dotValue);
	angle = angle * (180 / yunutyEngine::math::PI);

	if (mouseWorldPos.z - objectPos.z < 0)
	{
		angle = 360.0f - angle;
	}

	this->ursulaQSkillPreviewObj->GetChildren()[static_cast<int>(UrsulaQSkillInfo::ArrowHead)]->GetTransform()->SetLocalRotation(Quaternion{ Vector3f{0.f,angle,0.f} });
	this->ursulaQSkillPreviewObj->GetChildren()[static_cast<int>(UrsulaQSkillInfo::ArrowBody)]->GetTransform()->SetLocalRotation(Quaternion{ Vector3f{0.f,angle,0.f} });

	float distance = sqrt(pow(objectPos.x - mouseWorldPos.x, 2) + pow(objectPos.y - mouseWorldPos.y, 2) + pow(objectPos.z - mouseWorldPos.z, 2));

	if (distance > OFFSET)
	{
		if (distance >= maxDistance)
		{
			distance = maxDistance;
		}
		this->ursulaQSkillPreviewObj->GetChildren()[static_cast<int>(UrsulaQSkillInfo::ArrowBody)]->GetTransform()->SetLocalScale(Vector3d{ (distance - OFFSET) / 2 ,1.0,1.0 });

		this->ursulaQSkillPreviewObj->GetChildren()[static_cast<int>(UrsulaQSkillInfo::ArrowHead)]->GetTransform()->SetLocalPosition(normalizedPos * (distance - OFFSET));
	}

	// 원 3개 출력
	this->ursulaQSkillPreviewObj->GetChildren()[static_cast<int>(UrsulaQSkillInfo::CircleOne)]->GetTransform()->SetLocalPosition(circleOnePos);
	this->ursulaQSkillPreviewObj->GetChildren()[static_cast<int>(UrsulaQSkillInfo::CircleOne)]->GetTransform()->SetLocalScale(Vector3d{ circleRadius.x,1.0,circleRadius.x });

	this->ursulaQSkillPreviewObj->GetChildren()[static_cast<int>(UrsulaQSkillInfo::CircleTwo)]->GetTransform()->SetLocalPosition(circleTwoPos);
	this->ursulaQSkillPreviewObj->GetChildren()[static_cast<int>(UrsulaQSkillInfo::CircleTwo)]->GetTransform()->SetLocalScale(Vector3d{ circleRadius.y,1.0,circleRadius.y });

	this->ursulaQSkillPreviewObj->GetChildren()[static_cast<int>(UrsulaQSkillInfo::CircleThree)]->GetTransform()->SetLocalPosition(circleThreePos);
	this->ursulaQSkillPreviewObj->GetChildren()[static_cast<int>(UrsulaQSkillInfo::CircleThree)]->GetTransform()->SetLocalScale(Vector3d{ circleRadius.z,1.0,circleRadius.z });
}

void SkillPreviewSystem::HideUrsulaQSkill()
{
	if (this->ursulaQSkillPreviewObj->GetActive() == true)
	{
		this->ursulaQSkillPreviewObj->SetSelfActive(false);
	}
}

void SkillPreviewSystem::ShowUrsulaWSkill(const Vector3d& objectPos, float maxDistance, float circleRadius)
{
	circleRadius *= 2;
	if (this->ursulaWSkillPreviewObj->GetActive() == false)
	{
		this->ursulaWSkillPreviewObj->SetSelfActive(true);
	}

	auto offsetPos = objectPos;
	offsetPos.y += Y_OFFSET;
	this->ursulaWSkillPreviewObj->GetTransform()->SetLocalPosition(offsetPos);

	auto distToXZPlane = abs(camObj->GetTransform()->GetWorldPosition().y);
	auto centeredPosition = Input::getMouseScreenPositionNormalizedZeroCenter();

	Vector3d mouseWorldPos = camObj->GetComponent<yunutyEngine::graphics::Camera>()->GetProjectedPoint(centeredPosition, distToXZPlane, Vector3d(0, 1, 0));
	mouseWorldPos.y = Y_OFFSET;

	float distance = sqrt(pow(objectPos.x - mouseWorldPos.x, 2) + pow(objectPos.y - mouseWorldPos.y, 2) + pow(objectPos.z - mouseWorldPos.z, 2));
	auto direction = mouseWorldPos - objectPos;
	direction = direction.Normalize(direction);

	if (distance >= maxDistance)
	{
		distance = maxDistance;
	}

	ursulaWSkillPreviewObj->GetChildren()[static_cast<int>(UrsulaWSkillInfo::CircleOne)]->GetTransform()->SetLocalPosition(direction * distance);
}

void SkillPreviewSystem::HideUrsulaWSkill()
{
	if (this->ursulaWSkillPreviewObj->GetActive() == true)
	{
		this->ursulaWSkillPreviewObj->SetSelfActive(false);
	}
}

void SkillPreviewSystem::ShowHanselQSkill(const Vector3d& objectPos, float maxDistance, float circleRadius)
{
	circleRadius *= 2;

	if (this->hanselQSkillPreviewObj->GetActive() == false)
	{
		this->hanselQSkillPreviewObj->SetSelfActive(true);
	}

	// 매개변수로 들어온 objectPos로부터 사거리가 표시됩니다.
	// maxDistance로 들어온 값까지 늘어납니다.

	// this->robinQSkillPreviewObj의 위치를 objectPos로
	auto offsetPos = objectPos;
	offsetPos.y += Y_OFFSET;
	this->hanselQSkillPreviewObj->GetTransform()->SetLocalPosition(offsetPos);

	// objectPos로 부터 마우스 위치까지의 거리
	auto distToXZPlane = abs(camObj->GetTransform()->GetWorldPosition().y);
	auto centeredPosition = Input::getMouseScreenPositionNormalizedZeroCenter();

	Vector3d mouseWorldPos = camObj->GetComponent<yunutyEngine::graphics::Camera>()->GetProjectedPoint(centeredPosition, distToXZPlane, Vector3d(0, 1, 0));
	Vector3d mouseVector = mouseWorldPos - objectPos;
	mouseWorldPos.y = 0.f;
	auto normalizedPos = mouseVector.Normalize(mouseVector);

	auto basicAxis = AXIS;

	float dotValue = (Vector3d::Dot(basicAxis, normalizedPos));

	float angle = acos(dotValue);
	angle = angle * (180 / yunutyEngine::math::PI);

	if (mouseWorldPos.z - objectPos.z < 0)
	{
		angle = 360.0f - angle;
	}

	this->hanselQSkillPreviewObj->GetChildren()[static_cast<int>(HanselQSkillInfo::ArrowHead)]->GetTransform()->SetLocalRotation(Quaternion{ Vector3f{0.f,angle,0.f} });
	this->hanselQSkillPreviewObj->GetChildren()[static_cast<int>(HanselQSkillInfo::ArrowBody)]->GetTransform()->SetLocalRotation(Quaternion{ Vector3f{0.f,angle,0.f} });

	float distance = sqrt(pow(objectPos.x - mouseWorldPos.x, 2) + pow(objectPos.y - mouseWorldPos.y, 2) + pow(objectPos.z - mouseWorldPos.z, 2));
	auto direction = mouseWorldPos - objectPos;
	direction = direction.Normalize(direction);


	if (distance > OFFSET)
	{
		if (distance >= maxDistance)
		{
			distance = maxDistance;
		}
		this->hanselQSkillPreviewObj->GetChildren()[static_cast<int>(HanselQSkillInfo::ArrowBody)]->GetTransform()->SetLocalScale(Vector3d{ (distance - OFFSET) / 2 ,1.0,1.0 });

		this->hanselQSkillPreviewObj->GetChildren()[static_cast<int>(HanselQSkillInfo::ArrowHead)]->GetTransform()->SetLocalPosition(normalizedPos * (distance - OFFSET));

		// 원 출력
		this->hanselQSkillPreviewObj->GetChildren()[static_cast<int>(HanselQSkillInfo::CircleOne)]->GetTransform()->SetLocalPosition(distance * direction);
	}
	else
	{
		// 원 출력
		this->hanselQSkillPreviewObj->GetChildren()[static_cast<int>(HanselQSkillInfo::CircleOne)]->GetTransform()->SetLocalPosition(OFFSET * direction);
	}
	this->hanselQSkillPreviewObj->GetChildren()[static_cast<int>(HanselQSkillInfo::CircleOne)]->GetTransform()->SetLocalScale(Vector3d{ circleRadius,1.0,circleRadius });


}

void SkillPreviewSystem::HideHanselQSkill()
{
	if (this->hanselQSkillPreviewObj->GetActive() == true)
	{
		this->hanselQSkillPreviewObj->SetSelfActive(false);
	}
}

void SkillPreviewSystem::ShowHanselWSkill(const Vector3d& objectPos, float maxDistance)
{
	if (this->hanselWSkillPreviewObj->GetActive() == false)
	{
		this->hanselWSkillPreviewObj->SetSelfActive(true);
	}

	// 매개변수로 들어온 objectPos로부터 사거리가 표시됩니다.
	// maxDistance로 들어온 값까지 늘어납니다.

	// this->robinQSkillPreviewObj의 위치를 objectPos로
	auto offsetPos = objectPos;
	offsetPos.y += Y_OFFSET;
	this->hanselWSkillPreviewObj->GetTransform()->SetLocalPosition(offsetPos);

	auto distToXZPlane = abs(camObj->GetTransform()->GetWorldPosition().y);
	auto centeredPosition = Input::getMouseScreenPositionNormalizedZeroCenter();

	Vector3d mouseWorldPos = camObj->GetComponent<yunutyEngine::graphics::Camera>()->GetProjectedPoint(centeredPosition, distToXZPlane, Vector3d(0, 1, 0));
	Vector3d mouseVector = mouseWorldPos - objectPos;
	mouseWorldPos.y = 0.f;
	auto normalizedPos = mouseVector.Normalize(mouseVector);


	auto basicAxis = AXIS;

	float dotValue = (Vector3d::Dot(basicAxis, normalizedPos));

	float angle = acos(dotValue);
	angle = angle * (180 / yunutyEngine::math::PI);

	if (mouseWorldPos.z - objectPos.z < 0)
	{
		angle = 360.0f - angle;
	}

	this->hanselWSkillPreviewObj->GetChildren()[static_cast<int>(HanselWSkillInfo::ArrowHead)]->GetTransform()->SetLocalRotation(Quaternion{ Vector3f{0.f,angle,0.f} });
	this->hanselWSkillPreviewObj->GetChildren()[static_cast<int>(HanselWSkillInfo::ArrowBody)]->GetTransform()->SetLocalRotation(Quaternion{ Vector3f{0.f,angle,0.f} });

	float distance = sqrt(pow(objectPos.x - mouseWorldPos.x, 2) + pow(objectPos.y - mouseWorldPos.y, 2) + pow(objectPos.z - mouseWorldPos.z, 2));

	if (distance > OFFSET)
	{
		if (distance >= maxDistance)
		{
			distance = maxDistance;
		}
		this->hanselWSkillPreviewObj->GetChildren()[static_cast<int>(HanselWSkillInfo::ArrowBody)]->GetTransform()->SetLocalScale(Vector3d{ (distance - OFFSET) / 2 ,1.0,1.0 });

		this->hanselWSkillPreviewObj->GetChildren()[static_cast<int>(HanselWSkillInfo::ArrowHead)]->GetTransform()->SetLocalPosition(normalizedPos * (distance - OFFSET));
	}
}

void SkillPreviewSystem::HideHanselWSkill()
{
	if (this->hanselWSkillPreviewObj->GetActive() == true)
	{
		this->hanselWSkillPreviewObj->SetSelfActive(false);
	}
}

void SkillPreviewSystem::ShowTemporaryRoute(UnitType unitType, std::vector<Vector3d>& vertexList)
{
	const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
	// 버텍스가 1개만 들어오는 일은 없을 것으로 예상 혹시모르니 return
	if (vertexList.size() < 2)
	{
		return;
	}
	// 이 함수는 계속 불릴 함수이기 때문에 이미 메쉬가 생성된 것이 있다면 삭제해줘야함
	if (temporaryRouteMesh)
	{
		_resourceManager->DeleteMesh(temporaryRouteMesh);
		temporaryRouteMesh = nullptr;
	}
	temporaryRouteMesh = CreateRouteMesh(vertexList);

	this->temporaryRouteMeshRenderer->GetGI().SetMesh(temporaryRouteMesh);
	switch (unitType)
	{
		case SkillPreviewSystem::UnitType::Robin:
		{
			this->temporaryRouteMeshRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{1,0,0,1});
		}
			break;
		case SkillPreviewSystem::UnitType::Ursula:
		{
			this->temporaryRouteMeshRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0.545,0,1,1 });
		}
			break;
		case SkillPreviewSystem::UnitType::Hansel:
		{
			this->temporaryRouteMeshRenderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1,0.5,0,1 });
		}
			break;
		default: 
			{}
			break;
	}
	
}

yunuGI::IMesh* SkillPreviewSystem::ShowRoute(UnitType unitType, std::vector<Vector3d>& vertexList)
{
	auto mesh = CreateRouteMesh(vertexList);
	auto renderer = this->Borrow();
	renderer->GetGI().SetMesh(mesh);

	this->rendererMap.insert({ mesh, renderer });

	switch (unitType)
	{
		case SkillPreviewSystem::UnitType::Robin:
		{
			renderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1,0,0,0.3 });
		}
		break;
		case SkillPreviewSystem::UnitType::Ursula:
		{
			renderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 0.545,0,1,0.3 });
		}
		break;
		case SkillPreviewSystem::UnitType::Hansel:
		{
			renderer->GetGI().GetMaterial()->SetColor(yunuGI::Color{ 1,0.5,0,0.3 });
		}
		break;
		default:
		{}
		break;
	}

	return mesh;
}

void SkillPreviewSystem::DeleteRouteMesh(yunuGI::IMesh* mesh)
{
	// 생성된 메쉬를 지우며 renderer가 붙은 게임 오브젝트를 Pool에 반납
	auto iter = this->rendererMap.find(mesh);
	iter->second->GetGI().SetMesh(nullptr);
	this->Return(iter->second);

	this->rendererMap.erase(iter);

	const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();
	_resourceManager->DeleteMesh(mesh);
}

yunuGI::IMesh* SkillPreviewSystem::CreateRouteMesh(std::vector<Vector3d>& vertexList)
{
	const yunuGI::IResourceManager* _resourceManager = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager();

	unsigned int vertexCount = vertexList.size();
	unsigned int meshVertexCount = vertexCount * 2;

	Vector3d direction = Vector3d{ 0,0,0 };
	Vector3d prevDirection = Vector3d{ 0,0,0 };
	std::vector<yunuGI::Vector3> posVec;
	std::vector<unsigned int> idxVec;
	std::vector<yunuGI::Vector2> uvVec;
	std::vector<yunuGI::Vector3> normalVec;


	for (int i = 0; i < vertexCount - 1; i++)
	{
		if (i == 0)
		{
			// 처음 버텍스 일 때
			direction = vertexList[i + 1] - vertexList[i];
			direction.y = 0;
			Vector3d crossVector = Vector3d{ -direction.z,0,direction.x };
			crossVector = crossVector.Normalize(crossVector);

			yunuGI::Vector3 vertexPos = vertexList[i];
			yunuGI::Vector3 vertexPos2 = vertexList[i];
			yunuGI::Vector3 vertexPos3 = vertexList[i + 1];
			yunuGI::Vector3 vertexPos4 = vertexList[i + 1];
			vertexPos += (crossVector * VERTEX_OFFSET);
			vertexPos.y += Y_OFFSET;

			vertexPos2 += (-crossVector * VERTEX_OFFSET);
			vertexPos2.y += Y_OFFSET;

			vertexPos3 += (crossVector * VERTEX_OFFSET);
			vertexPos3.y += Y_OFFSET;

			vertexPos4 += (-crossVector * VERTEX_OFFSET);
			vertexPos4.y += Y_OFFSET;

			posVec.push_back(vertexPos);
			posVec.push_back(vertexPos2);
			posVec.push_back(vertexPos3);
			posVec.push_back(vertexPos4);
			normalVec.emplace_back(yunuGI::Vector3{ 0,1,0 });
			normalVec.emplace_back(yunuGI::Vector3{ 0,1,0 });
			normalVec.emplace_back(yunuGI::Vector3{ 0,1,0 });
			normalVec.emplace_back(yunuGI::Vector3{ 0,1,0 });
			idxVec.emplace_back(i * 2);
			idxVec.emplace_back((i * 2) + 2);
			idxVec.emplace_back((i * 2) + 1);
			idxVec.emplace_back((i * 2) + 2);
			idxVec.emplace_back((i * 2) + 3);
			idxVec.emplace_back((i * 2) + 1);
		}
		else
		{
			// 중간 버텍스 일 때
			// 우선 끝 두 점을 구하고 인덱스 버퍼를 갱신한다.
			direction = vertexList[i + 1] - vertexList[i];
			direction = direction.Normalize(direction);
			direction.y = 0;
			Vector3d crossVector = Vector3d{ -direction.z,0,direction.x };
			crossVector = crossVector.Normalize(crossVector);

			yunuGI::Vector3 vertexPos = vertexList[i + 1];
			yunuGI::Vector3 vertexPos2 = vertexList[i + 1];

			vertexPos += (crossVector * VERTEX_OFFSET);
			vertexPos.y += Y_OFFSET;

			vertexPos2 += (-crossVector * VERTEX_OFFSET);
			vertexPos2.y += Y_OFFSET;

			posVec.push_back(vertexPos);
			posVec.push_back(vertexPos2);
			normalVec.emplace_back(yunuGI::Vector3{ 0,1,0 });
			normalVec.emplace_back(yunuGI::Vector3{ 0,1,0 });

			idxVec.emplace_back(i * 2);
			idxVec.emplace_back((i * 2) + 2);
			idxVec.emplace_back((i * 2) + 1);
			idxVec.emplace_back((i * 2) + 2);
			idxVec.emplace_back((i * 2) + 3);
			idxVec.emplace_back((i * 2) + 1);

			// posVec의 마직막 요소 2개의 포지션을 현재 방향에 맞게 수정
			prevDirection = vertexList[i] - vertexList[i - 1];
			prevDirection = prevDirection.Normalize(prevDirection);

			float dotValue = Vector3d::Dot(direction, prevDirection);

			float angle = acos(dotValue);
			angle /= 2;
			//angle = angle * (180 / yunutyEngine::math::PI);

			auto& prevPos = posVec[posVec.size() - 4];
			auto& prevPos2 = posVec[posVec.size() - 3];
			auto center = (prevPos + prevPos2) * 0.5;
			prevPos -= center;
			prevPos2 -= center;

			float cosTheta = cos(angle);
			float sinTheta = sin(angle);

			float v1x = prevPos.x;
			float v1z = prevPos.z;
			prevPos.x = v1x * cosTheta + v1z * sinTheta;
			prevPos.z = -v1x * sinTheta + v1z * cosTheta;

			float v2x = prevPos2.x;
			float v2z = prevPos2.z;
			prevPos2.x = v2x * cosTheta + v2z * sinTheta;
			prevPos2.z = -v2x * sinTheta + v2z * cosTheta;

			prevPos += center;
			prevPos2 += center;
		}
	}

	uvVec.resize(posVec.size());

	for (int i = 0; i < vertexList.size(); i += 2)
	{
		// uv setting용 반복문
		float outDistance = sqrt(pow(posVec[i].x - posVec[i + 2].x, 2) + pow(posVec[i].z - posVec[i + 2].z, 2));
		float inDistance = sqrt(pow(posVec[i + 1].x - posVec[i + 3].x, 2) + pow(posVec[i + 1].z - posVec[i + 3].z, 2));

		if (i == 0)
		{
			uvVec[i] = yunuGI::Vector2{ 0,0 };
			uvVec[i + 2] = yunuGI::Vector2{ outDistance,0 };

			uvVec[i + 1] = yunuGI::Vector2{ 0,1 };
			uvVec[i + 3] = yunuGI::Vector2{ inDistance,1 };
		}
		else
		{
			uvVec[i * 2] = uvVec[(i * 2) - 2] + outDistance;
			uvVec[i * 2].y = 0;

			uvVec[(i * 2) + 1] = uvVec[(i * 2) - 1] + inDistance;
			uvVec[(i * 2) + 1].y = 1;
		}
	}

	for (auto& each : posVec)
	{
		each.y = Y_OFFSET;
	}

	std::wstring meshName = L"RouteMesh_";
	meshName += std::to_wstring(routeMeshID);
	auto tempMesh = _resourceManager->CreateMesh(meshName, posVec, idxVec, normalVec, uvVec);
	routeMeshID++;
	return tempMesh;
}
