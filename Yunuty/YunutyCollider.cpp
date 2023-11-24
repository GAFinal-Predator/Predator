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
            // rigidbody�� static�̸� ���� Ʈ�������� �ٲ� �ȵȴ�.
#ifdef _DEBUG
            if (impl->isStaticShape())
            {
                // static���� ������ ������ٵ�� ���� �������� ���ƾ� �մϴ�!
                // ���� �ùķ��̼��� ������ ���� �����鼭�� Ʈ�������� ������� �ٲٰ� �ʹٸ� RigidBody ������Ʈ�� SetAsKinematic ����� Ȱ���ϼ���. 
                assert((firstLocation - GetTransform()->GetWorldPosition()).MagnitudeSqr() < 0.01);
                assert(Quaternion::Dot(firstRotation, GetTransform()->GetWorldRotation()) > 0.99);
                return;
            }
#endif
            // rigidbody�� kinematic�̸� physXActor�� ������ ���� ������Ʈ�� Ʈ�������� ���󰡾� �Ѵ�.
            // rigidbody�� dynamic�϶��� ���� ������Ʈ�� Ʈ�������� physXActor�� ���󰡸� �ȴ�.
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
            impl->pxActor->setActorFlag({PxActorFlag::eDISABLE_SIMULATION},false);
            if (impl->pxRigidDynamic->isSleeping())
                impl->pxRigidDynamic->wakeUp();
        }
        void Collider::OnDisable()
        {
            impl->pxActor->setActorFlag({PxActorFlag::eDISABLE_SIMULATION},true);
        }
    }
}
