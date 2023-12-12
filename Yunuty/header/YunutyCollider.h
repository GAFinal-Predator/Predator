#pragma once
#include <Windows.h>
#include <vector>
#include <unordered_set>
#include "Graphic.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Rect.h"

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

using namespace std;
namespace yunutyEngine
{
    namespace physics
    {
        class RigidBody;
        class YUNUTY_API Collider : public Component
        {
        public:
            class Impl;
            Collider(Impl* impl);
            virtual ~Collider();
        protected:
            virtual void Start() override;
            virtual void Update()override;
            virtual void OnEnable()override;
            virtual void OnDisable()override;
            RigidBody* rigidBody;

            Impl* impl;
        public:
            //bool IsTrigger();
            //void SetAsTrigger(bool isTrigger);
            bool IsUsingCCD();
            // Continuous Collision Detection ����� Ȱ��ȭ�ϸ� �� �����ӿ��� ���� �����ӱ����� �浹�� ���������� üũ�մϴ�.
            // CCD ����� Ȱ��ȭ�Ǹ� Bullet through paper ������ �߻����� �ʽ��ϴ�.
            void EnableCCD(bool enable);
        private:
            friend RigidBody;
#ifdef _DEBUG
            Vector3d firstLocation;
            Quaternion firstRotation;
#endif
        };
    }
}
