#pragma once

#include <vector>

class SoftBody;

class SoftBodyManager
{
private:
	SoftBodyManager();

public:
	~SoftBodyManager();

	static SoftBodyManager* GetInstance();
	std::vector<SoftBody*> softbodyList;

	void AddSoftBody(SoftBody* softbody);
	void RemoveSoftBody(SoftBody* softbody);
};