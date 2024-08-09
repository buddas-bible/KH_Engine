#include "TimeManager.h"

TimeManager::TimeManager() :
	currCount{},
	prevCount{},
	Frequency{},
	Acc{ 0.f },
	callCount{ 0 }
{
	QueryPerformanceCounter(&prevCount);
	QueryPerformanceFrequency(&Frequency);
}

TimeManager::~TimeManager()
{

}

TimeManager& TimeManager::GetInstance()
{
	static TimeManager instance;
	return instance;
}

void TimeManager::Update()
{
	QueryPerformanceCounter(&(TimeManager::GetInstance().currCount));

	dt = (double)(currCount.QuadPart - prevCount.QuadPart) / (double)(Frequency.QuadPart);

	prevCount = currCount;
	++(callCount);
	Acc += dt;

	if (Acc >= 1.0f)
	{
		FPS = callCount;
		Acc = 0.f;
		callCount = 0;
	}
}