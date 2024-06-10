#pragma once
#include "YunutyEngine.h"
#include "YunuGraphicsInterface.h"

// VFXAnimator는 AddGameObjectFromFBX해서 나오는 Root GameObject에 붙을 컴포넌트이다.
// 그래서 Start에서 자식 GameObject에서 순회해서 StaticMeshRenderer를 찾아온다.
class VFXAnimator : public yunutyEngine::Component
{
public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	void Init();
	void SetAutoActiveFalse();
	bool IsDone();
	void SetLoop(bool isLoop) { this->isLoop = isLoop; }

private:
	
	void Reset();

private:
	struct CurFrameInfo
	{
		float sumTime;
		int curFrame;
		int nextFrame;
		bool isDone = false;
	};

private:
	bool isInit = false;
	bool isAutoActivFalse = true;
	bool isLoop = false;
	yunuGI::IMeshRenderer* renderer;
	float frameRate;
	bool isPlayDone = false;

	// 키프레임의 UV좌표 값
	std::vector<std::vector<yunuGI::VFXInfo>> frameInfoVec;

	// 현재 프레임에 관련된 정보를 보관하는 vector
	std::vector<CurFrameInfo> curFrameVec;
};

