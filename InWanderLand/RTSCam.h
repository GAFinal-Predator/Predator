#pragma once
#include "YunutyEngine.h"
using namespace yunutyEngine;

class RTSCam :public yunutyEngine::graphics::Camera
{
protected:
    bool roamingMode = true;
    void Update()
    {
        if (Input::isKeyPushed(KeyCode::Tab))
            roamingMode = !roamingMode;

        if (!roamingMode)
        {
            float cameraSpeed = 10.0f;
            Camera::Update();
            Vector3d deltaDirection = Vector3d::zero;

            if (yunutyEngine::Input::isKeyDown(KeyCode::LShift))
                cameraSpeed = 20.0f;

            auto lastMousePos = yunutyEngine::Input::getMouseScreenPositionNormalized();
            if (yunutyEngine::Input::isKeyDown(KeyCode::A) || lastMousePos.x < 0)
                deltaDirection -= Vector3d::right;
            if (yunutyEngine::Input::isKeyDown(KeyCode::D) || lastMousePos.x > 1)
                deltaDirection += Vector3d::right;
            if (yunutyEngine::Input::isKeyDown(KeyCode::W) || lastMousePos.y < 0)
                deltaDirection += Vector3d::up;
            if (yunutyEngine::Input::isKeyDown(KeyCode::S) || lastMousePos.y > 1)
                deltaDirection -= Vector3d::up;

            GetTransform()->position += deltaDirection.Normalized() * Time::GetDeltaTime() * cameraSpeed;
            GetTransform()->rotation = Quaternion::Identity();
        }
        else
        {
            float cameraSpeed = 10.0f;
            Camera::Update();
            Vector3d deltaPosition = Vector3d::zero;

            if (yunutyEngine::Input::isKeyDown(KeyCode::LShift))
                cameraSpeed = 20.0f;

            if (yunutyEngine::Input::isKeyDown(KeyCode::A))
                deltaPosition -= GetTransform()->GetWorldRotation().Right();
            if (yunutyEngine::Input::isKeyDown(KeyCode::D))
                deltaPosition += GetTransform()->GetWorldRotation().Right();
            if (yunutyEngine::Input::isKeyDown(KeyCode::W))
                deltaPosition += GetTransform()->GetWorldRotation().Forward();
            if (yunutyEngine::Input::isKeyDown(KeyCode::S))
                deltaPosition -= GetTransform()->GetWorldRotation().Forward();
            if (yunutyEngine::Input::isKeyDown(KeyCode::Q))
                deltaPosition -= Vector3d::up;
            if (yunutyEngine::Input::isKeyDown(KeyCode::E))
                deltaPosition += Vector3d::up;
            static auto lastMousePos = yunutyEngine::Input::getMouseScreenPosition();

            auto dMousePos = yunutyEngine::Input::getMouseScreenPosition() - lastMousePos;
            lastMousePos = yunutyEngine::Input::getMouseScreenPosition();

            if (yunutyEngine::Input::isKeyDown(KeyCode::MouseRightClick))
            {
                constexpr float rotationSpeedFactor = 0.003f;
                //auto euler = GetTransform()->rotation.Euler();
                //euler.y += dMousePos.x * rotationSpeedFactor;
                //euler.x += dMousePos.y * rotationSpeedFactor;
                Quaternion& rot = GetTransform()->rotation;
                Vector3d newForward =
                    rot.Forward() +
                    dMousePos.x * rotationSpeedFactor * rot.Right() +
                    -dMousePos.y * rotationSpeedFactor * rot.Up();
                //Vector3d newRight =
                //    //Vector3d::right +
                //    rot.Right() +
                //    dMousePos.x * rotationSpeedFactor * -rot.Forward();
                Vector3d newUp = rot.Up() + -dMousePos.y * rotationSpeedFactor * -rot.Forward();
                Vector3d newRight = Vector3d::Cross(Vector3d::up, newForward);
                //GetTransform()->rotation = Quaternion::MakeWithAxes(euler);
                GetTransform()->rotation = Quaternion::MakeWithAxes(newRight, newUp, newForward);
            }

            GetTransform()->position += deltaPosition.Normalized() * Time::GetDeltaTime() * cameraSpeed;
        }
    }
};

