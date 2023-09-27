#include "RenderSystem.h"

#include <DirectXMath.h>
#include "SimpleMath.h"
using namespace DirectX::PackedVector;

#include "NailEngine.h"
#include "Struct.h"
#include "ConstantBuffer.h"

#include "ResourceManager.h"
#include "Material.h"
#include "Mesh.h"

LazyObjects<RenderSystem> RenderSystem::Instance;

void RenderSystem::RenderObject()
{
	MatrixBuffer matrixBuffer;
	matrixBuffer.WTM = DirectX::XMMatrixIdentity();

	DirectX::SimpleMath::Vector3 pos = {0.f,0.f,2.f};
	DirectX::SimpleMath::Vector3 target = {0.f,0.f,0.f};
	DirectX::SimpleMath::Vector3 up = {0.f,1.f,0.f};

	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&pos),
		DirectX::XMLoadFloat3(&target),
		DirectX::XMLoadFloat3(&up)
	);

	float aspectRatio = 1280 / 800; // ȭ�� ���� ���� ����
	float fieldOfView = DirectX::XMConvertToRadians(45.0f); // �þ߰� (45��)
	float nearClip = 1.0f; // ����� Ŭ���� ���
	float farClip = 1000.0f; // ���� Ŭ���� ���

	// Projection Matrix�� �����մϴ�.
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearClip, farClip);

	matrixBuffer.VTM = viewMatrix;
	matrixBuffer.PTM = projectionMatrix;

	NailEngine::Instance.Get().GetConstantBuffer(0)->PushGraphicsData(&matrixBuffer, sizeof(MatrixBuffer));
	
	ResourceManager::Instance.Get().GetMaterial(L"DefaultMaterial")->PushGraphicsData();
	ResourceManager::Instance.Get().GetMesh(L"Cube")->Render();
}
