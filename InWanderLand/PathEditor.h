#pragma once
#include "YunutyEngine.h"
#include "PathField.h"
using namespace yunutyEngine;

// RTS ��θ� ���콺 Ŭ������ ������ �� �ְ� ����� ���� ������ ���
// � ������ ��������� ������� �׷��� �̹����� ǥ���Ѵ�.
class PathEditor : public Component
{
public:
    // ������ ����� �Ǵ� ����ʵ�
    PathField* targetPathField{ nullptr };
    PathEditor()
    {
        nodeHoverDebugMesh = PathField::CreateNodeDebugMesh();
        nodeHoverDebugMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color::green());
        nodeHoverDebugMesh->GetGI().GetMaterial()->SetPixelShader(L"DebugPS.cso");
        nodeHoverDebugMesh->GetGameObject()->SetSelfActive(false);
    }
    // ��� ���� ���콺�� ȣ���ϰ� ������ ��������� ����� ������ ǥ���ϱ� ���� ȣ���� �Լ�
    void PositionHover(const Vector3d& position)
    {
        if (auto node = targetPathField->GetNode(position))
        {
            auto nodePosition = targetPathField->GetNearestNodePosition(position);
            nodeHoverDebugMesh->GetGameObject()->SetSelfActive(true);
            nodeHoverDebugMesh->GetTransform()->SetWorldPosition(nodePosition);
            nodeHoverDebugMesh->Update();
        }
    }
    // ��Ŭ���� �Ͼ�� �� ��������� �����ϱ� ���� ���̴� �Լ�. RTSCam�� ��Ŭ�� �ݹ��Լ��� ���δ�.
    void OnLeftClick(const Vector3d& position)
    {
        if (auto node = targetPathField->GetNode(position))
        {
            auto nodePosition = targetPathField->GetNearestNodePosition(position);
            if (!node->passable)
            {
                node->debugMesh = PathField::CreateNodeDebugMesh();
                node->debugMesh->GetTransform()->SetWorldPosition(nodePosition);
                node->debugMesh->Update();
                node->passable = true;
            }
            else
            {
                Scene::getCurrentScene()->DestroyGameObject(node->debugMesh->GetGameObject());
                node->passable = false;
            }
        }
    }
private:
    // ȣ���� �� ��忡 ǥ���� ����� �޽�
    graphics::StaticMeshRenderer* nodeHoverDebugMesh{};
};
