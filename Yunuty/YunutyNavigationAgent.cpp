#include "YunutyEngine.h"
#include "YunutyNavigationField.h"
#include "YunutyNavigationAgent.h"
#include "_YunutyNavigationAgentImpl.h"
#include "_YunutyNavigationFieldImpl.h"

using namespace yunutyEngine;

yunutyEngine::NavigationAgent::NavigationAgent() : impl(new Impl{ this })
{ }
yunutyEngine::NavigationAgent::~NavigationAgent()
{
    delete impl;
}
void yunutyEngine::NavigationAgent::Update()
{
    if (impl->crowd == nullptr)
        return;

    const dtCrowdAgent* agent = impl->crowd->getAgent(impl->agentIdx);
    GetTransform()->SetWorldPosition(Vector3f(agent->npos[0], agent->npos[1], agent->npos[2]));
}
void yunutyEngine::NavigationAgent::AssignToNavigationField(NavigationField* navField)
{
    this->navField = navField;

    if (impl->crowd != nullptr)
        impl->crowd->removeAgent(impl->agentIdx);

    if (navField != nullptr)
    {
        Vector3f pos = GetTransform()->GetWorldPosition();
        impl->crowd = navField->impl->crowd;
        impl->agentIdx = impl->crowd->addAgent(reinterpret_cast<float*>(&pos), &impl->agentParams);
        //auto agent = impl->crowd->getAgent(impl->agentIdx);
    }
}
void yunutyEngine::NavigationAgent::SetSpeed(float speed)
{
    impl->agentParams.maxSpeed = speed;
    if (impl->crowd != nullptr)
        impl->crowd->updateAgentParameters(impl->agentIdx, &impl->agentParams);
}
void yunutyEngine::NavigationAgent::SetAcceleration(float accel)
{
    impl->agentParams.maxAcceleration = accel;
    if (impl->crowd != nullptr)
        impl->crowd->updateAgentParameters(impl->agentIdx, &impl->agentParams);
}
void yunutyEngine::NavigationAgent::SetRadius(float radius)
{
    impl->agentParams.radius = radius;
    if (impl->crowd != nullptr)
        impl->crowd->updateAgentParameters(impl->agentIdx, &impl->agentParams);
}
float yunutyEngine::NavigationAgent::GetSpeed()
{
    return impl->agentParams.maxSpeed;
}
float yunutyEngine::NavigationAgent::GetAcceleration()
{
    return impl->agentParams.maxAcceleration;
}
float yunutyEngine::NavigationAgent::GetRadius()
{
    return impl->agentParams.radius;
}
void yunutyEngine::NavigationAgent::Relocate(Vector3f destination)
{
    const float* pos = &destination.x;
    constexpr float agentPlacementHalfExtents[3]{ 1000,1000,1000 };
    float nearest[3];
    dtPolyRef ref = 0;
    dtVcopy(nearest, pos);

    dtStatus status = navField->impl->navQuery->findNearestPoly(pos, agentPlacementHalfExtents, impl->crowd->getFilter(0), &ref, nearest);
    if (dtStatusFailed(status))
    {
        dtVcopy(nearest, pos);
        ref = 0;
    }
    dtCrowdAgent* ag = impl->crowd->getEditableAgent(impl->agentIdx);
    ag->corridor.reset(ref, nearest);
    ag->boundary.reset();
    ag->partial = false;

    ag->topologyOptTime = 0;
    ag->targetReplanTime = 0;
    ag->nneis = 0;

    dtVset(ag->dvel, 0, 0, 0);
    dtVset(ag->nvel, 0, 0, 0);
    dtVset(ag->vel, 0, 0, 0);
    dtVcopy(ag->npos, nearest);

    ag->desiredSpeed = 0;

    if (ref)
        ag->state = DT_CROWDAGENT_STATE_WALKING;
    else
        ag->state = DT_CROWDAGENT_STATE_INVALID;

    ag->targetState = DT_CROWDAGENT_TARGET_NONE;
}
void yunutyEngine::NavigationAgent::MoveTo(Vector3f destination)
{
    if (navField == nullptr)
        return;
    const dtQueryFilter* filter{ impl->crowd->getFilter(0) };
    const dtCrowdAgent* agent = impl->crowd->getAgent(impl->agentIdx);
    const float* halfExtents = impl->crowd->getQueryExtents();

    navField->impl->navQuery->findNearestPoly(reinterpret_cast<float*>(&destination), halfExtents, filter, &impl->targetRef, impl->targetPos);
    impl->crowd->requestMoveTarget(impl->agentIdx, impl->targetRef, impl->targetPos);
}
const Vector3f& yunutyEngine::NavigationAgent::GetTargetPosition()
{
    return reinterpret_cast<const Vector3f&>(*impl->targetPos);
}
