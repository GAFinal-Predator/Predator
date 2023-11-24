#pragma once
#include <assert.h>
#include "YunutyEngine.h"
#include "YunutyForceType.h"
#include "YunutyCollider.h"
#include "PxPhysicsAPI.h"
#include "_YunutyPhysxGlobal.h"

using namespace std;
using namespace physx;
namespace yunutyEngine
{
    namespace physics
    {
        // �� Ŭ������ �������� �ǵ帮�� �ݶ��̴��� ���� ��ü, Ʈ���� ������ ����� ��� ������ �� ������ ������ ������ ���忡���� �� Ŭ������ ���縦 �� �� ����.
        // �� ��ü�� Collider ������Ʈ �ȿ��� ���ǵ�����, �� ��ü�� ������ �ǵ帮�� �������̽��� Rigidbody Component�� Collider Component�� �����մϴ�.
        class Collider::Impl
        {
        private:
            Collider* colliderComponent;
            physx::PxRigidStatic* pxRigidStatic{ nullptr };
            physx::PxRigidDynamic* pxRigidDynamic{nullptr};
            physx::PxShape* pxShape{nullptr};
            physx::PxMaterial* pxMaterial{nullptr};
            physx::PxRigidActor* pxActor{nullptr};
            PxShapeFlags pxShapeFlags{ PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eTRIGGER_SHAPE };
            bool isStatic{ true };
            bool isKinematic{ false };
            bool ccdEnabled{ false };
            friend Collider;
            friend RigidBody;
            friend _PhysxGlobal;

        protected:
        public:
            // Rigidbody�� �������� ������ isTrigger�� ��, Rigidbody�� �����ϸ� Trigger�� �����Դϴ�.
            /*bool GetIsTrigger()
            {
                return pxShapeFlags.isSet(physx::PxShapeFlag::eTRIGGER_SHAPE);
            }
            void SetAsTrigger(bool isTrigger)
            {
                if (isTrigger)
                    pxShapeFlags.raise(physx::PxShapeFlag::eTRIGGER_SHAPE);
                else
                    pxShapeFlags.clear(physx::PxShapeFlag::eTRIGGER_SHAPE);

                if (pxShape)
                    pxShape->setFlags(pxShapeFlags);
            }*/

            Impl(Collider* colliderComponent)
            {
                this->colliderComponent = colliderComponent;
            }
            bool isRigidBody()
            {
                return pxShapeFlags.isSet(physx::PxShapeFlag::eSIMULATION_SHAPE);
            }
            void SetAsRigidbody(RigidBody* rigidBody)
            {
                isStatic = false;
                colliderComponent->rigidBody = rigidBody;
                if (rigidBody != nullptr)
                {
                    pxShapeFlags.clear(physx::PxShapeFlag::eTRIGGER_SHAPE);
                    pxShapeFlags.raise(physx::PxShapeFlag::eSIMULATION_SHAPE);
                }
                else
                {
                    pxShapeFlags.clear(physx::PxShapeFlag::eSIMULATION_SHAPE);
                    pxShapeFlags.raise(physx::PxShapeFlag::eTRIGGER_SHAPE);
                }

                if (pxShape)
                    pxShape->setFlags(pxShapeFlags);
            }
            bool IsUsingCCD()
            {
                return ccdEnabled;
            }
            void EnableCCD(bool enable)
            {
                ccdEnabled = enable;
                if (pxRigidDynamic)
                    pxRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, enable);
            }
            bool isStaticShape()
            {
                return isStatic;
            }
            void SetAsStatic(bool isStatic)
            {
                // �ѹ� ����ƽ���� ������ RigidBody�� �ٽ� �ٲ��� ���ÿ�. �׷� �ൿ�� �õ��ϸ� �Ʒ� assert ������ ��Ÿ�� ������ ������ �˴ϴ�.
                // �� �׷� ������ �ʿ��ϴٸ� ���� ���� �����ڿ��� �����Ͻÿ�.
                assert((pxRigidStatic == nullptr && pxRigidDynamic == nullptr) || this->isStatic == isStatic);
                this->isStatic = isStatic;
            }
            void SetAsKinematic(bool isKinematic)
            {
                this->isKinematic = isKinematic;
                if (pxRigidDynamic)
                    pxRigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isKinematic);
            }
            bool IsKinematic()
            {
                return isKinematic;
            }
            virtual ~Impl()
            {
                if (pxActor)
                {
                    _PhysxGlobal::SingleInstance().UnRegisterCollider(this);
                    pxActor->release();
                }
                if (pxShape)
                    pxShape->release();
            }
            virtual PxGeometry& GetGeometry() = 0;
            // ���� ������Ʈ�� �����Ǵ� physX ��ü���� �ʱ�ȭ�ϴ� �����̴�. Start�Լ����� ȣ��ȴ�.
            void InitializePhysXActor()
            {
                static_assert(sizeof(yunuGI::Matrix4x4) == sizeof(PxMat44));
                auto worldTM = colliderComponent->GetTransform()->GetWorldTM();
                pxMaterial = _PhysxGlobal::SingleInstance().pxPhysics->createMaterial(1.0f, 1.0f, 0.0f);

                pxShape = _PhysxGlobal::SingleInstance().pxPhysics->createShape(GetGeometry(), *pxMaterial, true, pxShapeFlags);

                if (isStatic)
                {
                    pxRigidStatic = _PhysxGlobal::SingleInstance().pxPhysics->createRigidStatic(PxTransform(reinterpret_cast<const PxMat44&>(worldTM)));
                    _PhysxGlobal::SingleInstance().RequestPxScene(colliderComponent->GetGameObject()->GetScene())->addActor(*pxRigidStatic);
                    pxRigidStatic->attachShape(*pxShape);
                    pxActor = pxRigidStatic;
                }
                else
                {
                    pxRigidDynamic = _PhysxGlobal::SingleInstance().pxPhysics->createRigidDynamic(PxTransform(reinterpret_cast<const PxMat44&>(worldTM)));
                    _PhysxGlobal::SingleInstance().RequestPxScene(colliderComponent->GetGameObject()->GetScene())->addActor(*pxRigidDynamic);
                    pxRigidDynamic->attachShape(*pxShape);
                    if (ccdEnabled)
                        pxRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
                    if (isKinematic)
                        pxRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
                    pxActor = pxRigidDynamic;
                }
                _PhysxGlobal::SingleInstance().RegisterCollider(this);
                pxShape->release();
            }
            void LockTranslation(bool x, bool y, bool z)
            {
                if (isStatic)
                    return;

                pxRigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, x);
                pxRigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, y);
                pxRigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, z);
            }
            void LockRotation(bool x, bool y, bool z)
            {
                if (isStatic)
                    return;

                pxRigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, x);
                pxRigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, y);
                pxRigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, z);
            }
            void SetMass(float mass)
            {
                if (isStatic)
                    return;

                pxRigidDynamic->setMass(mass);
            }
            void AddForce(const Vector3f& forceVector, ForceType forceType)
            {
                if (isStatic)
                    return;

                static_assert((int)ForceType::FORCE == (int)PxForceMode::eFORCE);
                static_assert((int)ForceType::IMPULSE == (int)PxForceMode::eIMPULSE);
                static_assert((int)ForceType::VELOCITY_CHANGE == (int)PxForceMode::eVELOCITY_CHANGE);
                static_assert((int)ForceType::ACCELERATION == (int)PxForceMode::eACCELERATION);

                pxRigidDynamic->addForce(reinterpret_cast<const PxVec3&>(forceVector), reinterpret_cast<const PxForceMode::Enum&>(forceType));
            }
            void AddTorque(const Vector3f& forceVector, ForceType torqueType)
            {
                if (isStatic)
                    return;

                static_assert((int)ForceType::FORCE == (int)PxForceMode::eFORCE);
                static_assert((int)ForceType::IMPULSE == (int)PxForceMode::eIMPULSE);
                static_assert((int)ForceType::VELOCITY_CHANGE == (int)PxForceMode::eVELOCITY_CHANGE);
                static_assert((int)ForceType::ACCELERATION == (int)PxForceMode::eACCELERATION);

                pxRigidDynamic->addTorque(reinterpret_cast<const PxVec3&>(forceVector), reinterpret_cast<const PxForceMode::Enum&>(torqueType));
            }
            void SetActorWorldTransform(const yunuGI::Matrix4x4& worldTM)
            {
                static_assert(sizeof(yunuGI::Matrix4x4) == sizeof(PxMat44));
                pxActor->setGlobalPose(PxTransform{ reinterpret_cast<const PxMat44&>(worldTM) });
            }
            yunutyEngine::Vector3f GetActorPosition()
            {
                static_assert(sizeof(Vector3f) == sizeof(PxVec3));
                return reinterpret_cast<const Vector3f&>(pxActor->getGlobalPose().p);
            }
            yunutyEngine::Quaternion GetActorRotation()
            {
                static_assert(sizeof(Quaternion) == sizeof(PxQuatd));

                auto pxQuat = pxActor->getGlobalPose().q;
                yunutyEngine::Quaternion quat { pxQuat.w, pxQuat.x, pxQuat.y, pxQuat.z };
                return quat;
            }
        };
    }
}
