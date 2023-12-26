#pragma once
#include "YunutyEngine.h"
using namespace yunutyEngine;
// RTS ��θ� XZ��鿡 �����ϱ� ���� ������Ʈ
// �� ������Ʈ�� ���Ե� XZ ����� �׸���� ������, �׸����� �������� ��� ��带 ������ �� �ְ� �Ѵ�.
// ������ ��γ��鳢�� �̷�� �ٰ����� RTS ���ֵ��� ���� �� �ִ� ��ΰ� �ȴ�.
class PathField : public Component
{
public:
    struct Node
    {
        bool passable{ false };
        graphics::StaticMeshRenderer* debugMesh{nullptr};
    };
    // ����,���η� ��� ���̺��� �����.
    void Initialize(int xSize, int ySize, float nodeWidth)
    {
        this->NodesXSize = xSize;
        this->NodesYSize = ySize;
        this->nodeWidth = nodeWidth;
        this->initializedWorldPos = GetTransform()->GetWorldPosition();
        nodeTable.resize(xSize, std::vector<Node>(ySize));
    }
    Node* GetNode(const Vector3d& position)
    {
        Vector2i nodeIndex = NodePositionToIndex(position);
        if (nodeIndex.x < 0 || nodeIndex.y < 0 || nodeIndex.x >= NodesXSize || nodeIndex.y >= NodesYSize)
            return nullptr;
        return &nodeTable[nodeIndex.x][nodeIndex.y];
    }
    Vector3d GetNearestNodePosition(const Vector3d& position)
    {
        auto nodeIndex = NodePositionToIndex(position);
        return NodePosition(nodeIndex.x, nodeIndex.y);
    }
    // ��带 ǥ���� ���� ���̴� ����� �޽�
    static graphics::StaticMeshRenderer* CreateNodeDebugMesh()
    {
        auto nodeHoverDebugMesh = Scene::getCurrentScene()->AddGameObject()->AddComponent<graphics::StaticMeshRenderer>();
        nodeHoverDebugMesh->GetTransform()->scale = Vector3d{ 0.2,1,0.2 };
        nodeHoverDebugMesh->GetGI().LoadMesh("Cylinder");
        nodeHoverDebugMesh->GetGI().GetMaterial()->SetColor(yunuGI::Color::white());
        nodeHoverDebugMesh->GetGI().GetMaterial()->SetPixelShader(L"DefaultPS.cso");

        return nodeHoverDebugMesh;
    }
private:
    std::vector<std::vector<Node>> nodeTable;
    float nodeWidth;
    // ��� �ʵ尡 �ʱ�ȭ�Ǿ��� �� �ʵ��� ���� ��ġ
    Vector3d initializedWorldPos;
    int NodesXSize{ 0 };
    int NodesYSize{ 0 };
    Vector3d NodePosition(int x, int y)
    {
        return initializedWorldPos + Vector3d{(x - NodesXSize / 2)* nodeWidth, 0, (y - NodesYSize / 2)* nodeWidth};
    }
    Vector2i NodePositionToIndex(Vector3d position)
    {
        Vector3d relativePos = (position - initializedWorldPos) / nodeWidth;
        return Vector2i{ static_cast<int>(relativePos.x + NodesXSize / 2 + 0.5),static_cast<int>(relativePos.z + NodesYSize / 2 + 0.5) };
    }
};
