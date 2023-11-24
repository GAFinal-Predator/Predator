#include "YunutyRigidBody.h"
#include "YunutyCollider.h"
#include "_YunutyColliderImpl.h"
#include "YunutyEngine.h"

namespace yunutyEngine
{
    namespace physics
    {
        RigidBody::RigidBody()
        {
            attachedCollider = GetGameObject()->GetComponent<Collider>();

            // rigidBody ������Ʈ�� �ݵ�� ���� ������Ʈ�� �ݶ��̴��� �ִ� ���¿��� �߰��Ǿ�� �մϴ�.
            assert(attachedCollider != nullptr);
            attachedCollider->impl->SetAsRigidbody(this);
        }
        RigidBody::~RigidBody()
        {

        }
        bool RigidBody::GetIsStatic()
        {
            return attachedCollider->impl->isStaticShape();
        }
        void RigidBody::SetAsStatic(bool isStatic)
        {
            attachedCollider->impl->SetAsStatic(isStatic);
        }
        bool RigidBody::GetIsKinematic()
        {
            return attachedCollider->impl->IsKinematic();
        }
        void RigidBody::SetAsKinematic(bool isStatic)
        {
            attachedCollider->impl->SetAsKinematic(isStatic);
        }

        void RigidBody::LockTranslation(bool x, bool y, bool z)
        {
            if (attachedCollider->impl->pxActor == nullptr)
                onStartCallbacks.push_back([=]() {attachedCollider->impl->LockTranslation(x, y, z); });
            else
                attachedCollider->impl->LockTranslation(x, y, z);
        }
        void RigidBody::LockRotation(bool x, bool y, bool z)
        {
            if (attachedCollider->impl->pxActor == nullptr)
                onStartCallbacks.push_back([=]() {attachedCollider->impl->LockRotation(x, y, z); });
            else
                attachedCollider->impl->LockRotation(x, y, z);
        }
        void RigidBody::SetMass(float mass)
        {
            if (attachedCollider->impl->pxActor == nullptr)
                onStartCallbacks.push_back([=]() {attachedCollider->impl->SetMass(mass); });
            else
                attachedCollider->impl->SetMass(mass);
        }
        void RigidBody::AddForce(const Vector3f& forceVector, ForceType forceType)
        {
            if (attachedCollider->impl->pxActor == nullptr)
                onStartCallbacks.push_back([=]() {attachedCollider->impl->AddForce(forceVector, forceType); });
            else
                attachedCollider->impl->AddForce(forceVector, forceType);
        }
        void RigidBody::AddTorque(const Vector3f& forceVector, ForceType torqueType)
        {
            if (attachedCollider->impl->pxActor == nullptr)
                onStartCallbacks.push_back([=]() {attachedCollider->impl->AddTorque(forceVector, torqueType); });
            else
                attachedCollider->impl->AddTorque(forceVector, torqueType);
        }
        void RigidBody::Start()
        {
            for (auto each : onStartCallbacks)
                each();
        }
        void RigidBody::OnEnable()
        {

        }
        void RigidBody::OnDisable()
        {

        }
    }
}
