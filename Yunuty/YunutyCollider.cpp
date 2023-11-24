#pragma once
#include "YunutyEngine.h"
#include "YunutyCollider.h"
#include "YunutyRigidBody.h"
#include "_YunutyColliderImpl.h"

using namespace std;
using namespace physx;
namespace yunutyEngine
{
    namespace physics
    {
        Collider::Collider(Impl* impl) :impl{ impl }
        {
            rigidBody = GetGameObject()->GetComponent<RigidBody>();
            if (rigidBody != nullptr)
                rigidBody->attachedCollider = this;
        }
        Collider::~Collider()
        {
            delete impl;
        }
        /*bool Collider::IsTrigger()
        {
            return impl->GetIsTrigger();
        }
        void Collider::SetAsTrigger(bool isTrigger)
        {
            impl->SetAsTrigger(isTrigger);
        }*/
        bool Collider::IsUsingCCD()
        {
            return impl->IsUsingCCD();
        }
        void Collider::EnableCCD(bool enable)
        {
            impl->EnableCCD(enable);
        }
        void Collider::Start()
        {
            impl->InitializePhysXActor();
#ifdef _DEBUG
            if (impl->isStaticShape())
            {
                firstLocation = GetTransform()->GetWorldPosition();
                firstRotation = GetTransform()->GetWorldRotation();
            }
#endif
        }
        void Collider::Update()
        {
            // rigidbody가 static이면 절대 트랜스폼이 바뀌어선 안된다.
#ifdef _DEBUG
            if (impl->isStaticShape())
            {
                // static으로 지정된 리지드바디는 결코 움직이지 말아야 합니다!
                // 물리 시뮬레이션의 영향을 받지 않으면서도 트랜스폼은 마음대로 바꾸고 싶다면 RigidBody 컴포넌트의 SetAsKinematic 기능을 활용하세요. 
                assert((firstLocation - GetTransform()->GetWorldPosition()).MagnitudeSqr() < 0.01);
                assert(Quaternion::Dot(firstRotation, GetTransform()->GetWorldRotation()) > 0.99);
                return;
            }
#endif
            // rigidbody가 kinematic이면 physXActor가 무조건 게임 오브젝트의 트랜스폼을 따라가야 한다.
            // rigidbody가 dynamic일때는 게임 오브젝트의 트랜스폼이 physXActor를 따라가면 된다.
            if (rigidBody != nullptr && (rigidBody->GetIsKinematic()))
            {
                impl->SetActorWorldTransform(GetTransform()->GetWorldTM());
            }
            else
            {
                GetTransform()->SetWorldPosition(impl->GetActorPosition());
                GetTransform()->SetWorldRotation(impl->GetActorRotation());
            }
        }
        void Collider::OnEnable()
        {
        }
        void Collider::OnDisable()
        {

        }
    }
}
