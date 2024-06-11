#include "PassiveRobinBleed.h"
#include "InWanderLand.h"

POD_PassiveRobinBleed PassiveRobinBleed::pod;
void PassiveRobinBleed::Init(std::weak_ptr<Unit> owner)
{
    owner.lock()->onAttackHit.AddCallback([this, owner](std::weak_ptr<Unit> enemyHit)
        {
            if (enemyHit.lock()->IsAlive())
            {
                enemyHit.lock()->ApplyBuff(UnitBuffBleeding{ owner, pod.dpsPerStack, pod.duration });
            }
        });
}
