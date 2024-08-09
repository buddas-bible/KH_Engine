#pragma once
#include <windows.h>

class TimeManager
{
	/// �̱���
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
	double Acc;		// 1�� üũ�� ���� ���� �ð�

	UINT callCount;	// �Լ� ȣ�� Ƚ��
	UINT FPS;		// �ʴ� ȣ��� Ƚ��

public:
	void Update();

	double GetDeltaTime() { return dt; };
	float GetfDeltaTime() { return (float)dt; };
	UINT GetFPS() { return FPS; };
};
