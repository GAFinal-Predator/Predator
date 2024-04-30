#pragma once
#include "YunutyEngine.h"

class ContentsObservee;

class ContentsObserver : public Component, public SingletonComponent<ContentsObserver>
{
private:
	std::vector<ContentsObservee*> m_observeeContainer;

public:
	void RegisterObservee(ContentsObservee* p_observee);

	void PlayObservee() const;

	void StopObservee() const;

	void ClearObservees();
};

