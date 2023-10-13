#pragma once
#include <vector>
#include "YunutyEngine.h"
// ī�޶���� ���� �� ����Ī �����ִ� Ŭ���� 

using namespace yunutyEngine;
class CamSwitcher : public Component
{
public:
    vector<yunutyEngine::graphics::Camera*> cams;
    int camsIndex{ 0 };
    virtual void Update()override
    {
        if (cams.empty()) 
            return;

        if (Input::isKeyPushed(KeyCode::Tab))
        {
            camsIndex = ++camsIndex % cams.size();
            cams[camsIndex]->SetCameraMain();
        }
    }
};
