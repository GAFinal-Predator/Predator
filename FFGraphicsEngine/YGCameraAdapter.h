#pragma once
#include "IYunuGICamera.h"
#include "FFCamera.h"
#include "CameraManager.h"
#include "Frustum.h"

namespace yunuGIAdapter
{
    class CameraAdapter : public yunuGI::ICamera
    {
    public:
        //static CameraAdapter* mainCam{nullptr};
        CameraAdapter() {};
        virtual ~CameraAdapter() {};
        virtual void SetWorldTM(const yunuGI::Matrix4x4& tm) override
        {
            //if (this != mainCam)
                //return;
            static_assert(sizeof(Matrix) == sizeof(decltype(tm)));
            CameraManager::GetInstance().GetCamera(0)->CalculateMatrix(
                reinterpret_cast<const Matrix&>(tm)
            );

            // ī�޶� ������ ���ŵǾ��� �� ī�޶� ������ �ִ� �������ҵ� �������ش�.
            CameraManager::GetInstance().GetCamera(0)->GetFrustum()->UpdateFrustum();
        };
        virtual void SetResolution(float width, float height) override
        {
            CameraManager::GetInstance().GetCamera(0)->SetResolution(width, height);
        }

        virtual void SetFOV(float fov) override
        {
            CameraManager::GetInstance().GetCamera(0)->m_fov = fov;
        };

        virtual void SetNear(float camNear) override
        {
            CameraManager::GetInstance().GetCamera(0)->m_near = camNear;
        };

        virtual void SetFar(float camFar) override
        {
            CameraManager::GetInstance().GetCamera(0)->m_far = camFar;
        };

        virtual void SetAsMain() override
        {
            //mainCam = this;
        };
    private:
    };
}
