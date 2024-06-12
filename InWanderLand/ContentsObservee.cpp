#include "ContentsObservee.h"
#include "ContentsObserver.h"

ContentsObservee::~ContentsObservee()
{
	ContentsObserver::Instance().RemoveObservee(this);
}

ContentsObservee::ContentsObservee()
{
	ContentsObserver::Instance().RegisterObservee(this);
}

