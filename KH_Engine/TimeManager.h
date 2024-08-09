#pragma once
#include <windows.h>

class TimeManager
{
	/// 싱글턴
private:
	TimeManager();
	~TimeManager();

public:

	static TimeManager& GetInstance();


private:

	LARGE_INTEGER currCount;
	LARGE_INTEGER prevCount;
	LARGE_INTEGER Frequency;

	double dt;		// deltatime
	double Acc;		// 1초 체크를 위한 누적 시간

	UINT callCount;	// 함수 호출 횟수
	UINT FPS;		// 초당 호출된 횟수

public:
	void Update();

	double GetDeltaTime() { return dt; };
	float GetfDeltaTime() { return (float)dt; };
	UINT GetFPS() { return FPS; };
};
