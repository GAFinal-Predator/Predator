#pragma once
#include "YunutyEngine.h"
using namespace yunutyEngine;

class RTSTerrain : public yunutyEngine::Component
{
public:
    // walkable Node�� ���� ����� ��췯�� �ﰢ���� �̷� �� �ִٸ� �� �ﰢ�� ����� �ɾ�ٴ� �� �ִ� ��ΰ� �ȴ�.
    class Node
    {
        bool walkable{false};
    };
    // tiles[x][y]�� �ش��ϴ� ��ǥ�� 
    vector<vector<Node>> tiles;
    float tileWidth;
    // RTSTerrain��ü�� ��ġ ���ؿ��� �·� ��� ���� ������ �����ϴ� Ÿ�ϼ��� ����� �ʱ�ȭ��.
    void Initialize(int row,int col, float tileWidth)
    {
        this->tileWidth = tileWidth;
        tiles.resize()
    }
};
