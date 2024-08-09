#pragma once
#include <vector>
#include "Vector2D.h""

class Spring;
class RigidBody;

class SoftBody
{
public:
	SoftBody();
	~SoftBody();

	void SetSoftBall(size_t vertex, size_t spring, float radius, float springconstant, float damping);
	void SetSoftBlock(float gap, int column, int row, float springconstant, float damping);
	void SetProperties();

	void Translate(Vector2D move);
	void SetAccelerator(Vector2D move);
	
	void Update(float dt);
	void Render();

	Vector2D position;

	std::vector<Spring*> springList;
	std::vector<RigidBody*> rigidbodyList;
};

