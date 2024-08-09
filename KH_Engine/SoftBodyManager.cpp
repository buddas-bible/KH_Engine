#include "SoftBodyManager.h"

SoftBodyManager::SoftBodyManager()
{
	softbodyList.clear();
}

SoftBodyManager::~SoftBodyManager()
{
	for (auto& e : softbodyList)
	{
		delete e;
		e = nullptr;
	}
	softbodyList.clear();
}

SoftBodyManager* SoftBodyManager::GetInstance()
{
	static SoftBodyManager* instance = nullptr;
	if (instance == nullptr)
	{
		instance = new SoftBodyManager;
	}
	return instance;
}

void SoftBodyManager::AddSoftBody(SoftBody* softbody)
{
	softbodyList.push_back(softbody);
}

void SoftBodyManager::RemoveSoftBody(SoftBody* softbody)
{
	softbodyList.erase(remove(softbodyList.begin(), softbodyList.end(), softbody));
}
