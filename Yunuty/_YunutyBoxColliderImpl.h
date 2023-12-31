#pragma once
#include "_YunutyColliderImpl.h"
#include "YunutyBoxCollider.h"

using namespace std;
namespace yunutyEngine
{
    namespace physics
    {
        class BoxCollider::Impl : public Collider::Impl
        {
        public:
            Impl(Collider* collider) : Collider::Impl(collider) {}
            Vector3f halfExtent{ 0.5f,0.5f,0.5f };
            PxBoxGeometry geometry;
            virtual PxGeometry& GetGeometry()override
            {
                static_assert(sizeof(Vector3f) == sizeof(PxVec3));
                geometry = PxBoxGeometry{ reinterpret_cast<const PxVec3&>(halfExtent) };
                return geometry;
            }
        };
    }
}
