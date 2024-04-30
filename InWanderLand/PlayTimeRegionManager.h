#pragma once
#include "YunutyEngine.h"
#include "ContentsObservee.h"

namespace application::editor
{
	class RegionData;
	class OrnamentData;
}
class Dotween;

/// <summary>
/// region간의 상관관계를 성립시켜주는 클래스.
/// </summary>
class PlayTimeRegionManager : public Component, public SingletonComponent<PlayTimeRegionManager>, public ContentsObservee
{
private:
	Vector3d stage2StartPosition;
	std::function<void()> stage1ToStage2Function;
	
	Dotween* cameraDotween;

	std::vector<GameObject*> stage1Ornaments;
	std::vector<GameObject*> stage2Ornaments;

public:
	virtual void Start() override;
	virtual void PlayFunction() override;
	virtual void StopFunction() override;

	void AddRegionData(application::editor::RegionData* p_data);
	void RegisterOrnament(GameObject* p_obj, int p_stageNum);
	void SetCameraDotween(Dotween* p_dot);

	friend PlaytimeRegion;
};

