#pragma once

#include "YunutyEngine.h"

using namespace yunutyEngine;
// �ӽ÷� Ư�� ������ Ŀ���� �۾����� ����Ʈ�� �����ϴ� ���
class DebugBeacon : public yunutyEngine::Component
{
public:
    Vector3d maxScale;
    float elapsed{ 0 };
    float duration{ 1 };
    const math::Curve popCurve{[](double t) {return 1 - 4 * (0.5 - t) * (0.5 - t); }};
    static DebugBeacon* PlaceBeacon(Vector3d position, yunuGI::Color color = yunuGI::Color::red(), Vector3d scale = Vector3d::one, float duration = 0.3)
    {
        auto gameObject = Scene::getCurrentScene()->AddGameObject();
        auto debugBeacon = gameObject->AddComponent<DebugBeacon>();
        auto staticMesh = gameObject->AddComponent<yunutyEngine::graphics::StaticMeshRenderer>();
        debugBeacon->debugSphere = staticMesh;
        debugBeacon->maxScale = scale;
        debugBeacon->duration = duration;
        staticMesh->GetGI().SetMesh(yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager()->GetMesh(L"Sphere"));
        staticMesh->GetGI().GetMaterial()->SetColor(color);
        // ���̾�
        //staticMesh->GetGI().GetMaterial()->SetPixelShader(L"DebugPS.cso");
        // �ָ���
        auto shaderList = yunutyEngine::graphics::Renderer::SingleInstance().GetResourceManager()->GetShaderList();
        for (int i = 0; i < shaderList.size(); ++i)
        {
            if (shaderList[i]->GetName() == L"DefaultPS.cso")
            {
                staticMesh->GetGI().GetMaterial()->SetPixelShader(shaderList[i]);
            }
        }
        
        //staticMesh->GetGI().SetColor(0, color);
        //staticMesh->GetGI().SetShader(0, L"Forward");
        //staticMesh->GetGI().SetMaterialName(0, L"Forward");
        gameObject->GetTransform()->scale = Vector3d(0, 0, 0);
        gameObject->GetTransform()->position = position;
        staticMesh->Update();
        return debugBeacon;
    }
    virtual void Update() override
    {
        elapsed += Time::GetDeltaTimeUnscaled();
        if (elapsed > duration)
            elapsed = duration;

        GetTransform()->scale = maxScale * popCurve.Evaluate(elapsed/duration);
        if (elapsed >= duration)
            Scene::getCurrentScene()->DestroyGameObject(GetGameObject());
    }
private:
    graphics::StaticMeshRenderer* debugSphere;
};
