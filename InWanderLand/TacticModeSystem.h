#pragma once
#include "YunutyEngine.h"
#include "InputManager.h"
#include "Unit.h"
#include "PlayerUnit.h"
#include "PermanentObservee.h"
#include <unordered_map>
#include <memory>
#include <queue>

/// <summary>
/// 전술모드 시스템.
/// 활성화 시 TimeScale(0.0f), queue에 스킬 등록.
/// 확정 시 queue 순서대로 스킬 순차적 사용.
/// 돌진 스킬은 등속 운동... 시간을 어떻게 가져올 것인가 -> 좋은 로직은 시간을 가져오는 것이 아닌, 스킬이 끝날 때 알려주는 것.
/// 투사체 스킬은 던지는 모션이 끝나면 스킬이 끝나는 것으로 간주.
/// </summary>

class RTSCam;
class CursorDetector;
class PlaytimeWave;
class UnitCommand;

class TacticModeSystem : public SingletonComponent<TacticModeSystem>, public Component, public PermanentObservee
{
public:
    virtual void OnEnable() override;
    virtual void Start() override;
    virtual void Update() override;

    virtual Component* GetComponent() override { return this; }

public:
    // 전술모드가 현재 쿨타임중인지 반환하는 함수
    bool IsCoolTime() { return this->isCoolTime; }
    // 현재 전술모드가 활성화되어 있는지 반환하는 함수 (반환값이 true일 때 만 Command를 Enqueue할 수 있다.)
    bool IsOperation() { return this->isOperating; }
    // 현재 전술모드가 등록된 명령들을 실행중인지 반환하는 함수
    bool IsExecuting() { return this->isExecuting; }

    // 전술모드를 활성화하는 함수입니다.
    void EngageTacticSystem();

    // 유닛의 행동을 큐에 등록할 수 있게 해주는 함수 입니다.
    void EnqueueCommand(std::shared_ptr<UnitCommand> command);

    // 맨 마지막 명령 하나를 지우는 함수입니다.
    void PopCommand();

    // 모든 명령을 지우는 함수입니다.
    void ClearCommand();

    std::shared_ptr<UnitCommand>& GetRobinLastCommand() { return robinLastCommand; }
    std::shared_ptr<UnitCommand>& GetUrsulaLastCommand() { return ursulaLastCommand; }
    std::shared_ptr<UnitCommand>& GetHanselLastCommand() { return hanselLastCommand; }

    // 전술 모드가 풀리면 실행될 함수 입니다.
    void ExecuteCommands();

private:
    // 전술모드 내부에서 등록된 명령들을 실행해주는 함수입니다.
    coroutine::Coroutine ExecuteInternal();

private:
    std::deque<std::shared_ptr<UnitCommand>> commandQueue;
    std::shared_ptr<UnitCommand> robinLastCommand;
    std::shared_ptr<UnitCommand> ursulaLastCommand;
    std::shared_ptr<UnitCommand> hanselLastCommand;

    bool isOperating = false;
    bool isCoolTime = false;
    bool isExecuting = false;
    float coolTime;
    float elapsedTime;


    friend class PlayerController;
};

