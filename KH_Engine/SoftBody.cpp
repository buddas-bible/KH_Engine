#include "SoftBody.h"
#include "RigidBody.h"
#include "Spring.h"

#include "ColliderList.h"
#include "Matrix3x3.h"
#include "KH_Define.h"

SoftBody::SoftBody()
{

}

SoftBody::~SoftBody()
{

}

void SoftBody::SetSoftBall(size_t vertex, size_t spring, float radius, float springconstant, float damping)
{
	if (vertex < 3)
	{
		return;
	}

	float theta = (2 * PI) / vertex;

	if (vertex % 2 == 0)
	{
		for (size_t i = 0; i < vertex; i++)
		{
			RigidBody* rigid = new RigidBody();

			Vector2D pos = Vector2D{ 0.f, -1.f * radius } *Matrix3x3::RotateMatrix(-theta / 2 + theta * i);

			rigid->SetPosition(pos);
			rigid->SetDensity(10.f);
			rigid->pCollider = ColliderList::GetInstance()->CreateCircleCollider(3.f, pos, rigid);
			rigidbodyList.push_back(rigid);
		}
	}
	else
	{
		for (size_t i = 0; i < vertex; i++)
		{
			RigidBody* rigid = new RigidBody();

			Vector2D pos = Vector2D{ 0.f, -1.f * radius } *Matrix3x3::RotateMatrix(theta * i);

			rigid->SetPosition(pos);
			rigid->SetDensity(10.f);
			rigid->pCollider = ColliderList::GetInstance()->CreateCircleCollider(3.f, pos, rigid);
			rigidbodyList.push_back(rigid);
		}
	}

	for (int i = 0; i < rigidbodyList.size(); i++)
	{
		for (int j = 1; ((vertex / j) != 0) && j < spring; j++)
		{
			/// ������Ʈ�� ������� ��������
			Spring* spring = new Spring(rigidbodyList[i], rigidbodyList[(i + j) % vertex]);
			spring->SetProperties(2 * (radius * cos((PI / 2) - (theta * 0.5f * j))), springconstant, damping);
			springList.push_back(spring);
		}
	}
}

void SoftBody::SetSoftBlock(float gap, int column, int row, float springconstant, float damping)
{
	if (column < 2)
	{
		column = 2;
	}
	if (row < 2)
	{
		row = 2;
	}

	for (size_t i = 0; i < row; i++)
	{
		for (size_t j = 0; j < column; j++)
		{
			RigidBody* rigid = new RigidBody();

			Vector2D pos{ j * gap, i * gap };

			rigid->SetPosition(pos);
			rigid->SetDensity(10.f);
			rigid->pCollider = ColliderList::GetInstance()->CreateCircleCollider(3.f, pos, rigid);
			rigidbodyList.push_back(rigid);
		}
	}

	Vector2D temp{};
	for (auto& e : rigidbodyList)
	{
		temp += e->GetPosition();
	}
	temp /= rigidbodyList.size();
	for (auto& e : rigidbodyList)
	{
		Vector2D& v = e->GetPosition();
		v -= temp;
	}
	position = {};
	
	// [row][column], [��][��]
	for (int i = 0; i < rigidbodyList.size(); i++)
	{
		// i�� i + 1. ������ �������� ���������� �������ش�.
		if (((i + 1) < rigidbodyList.size()) && ((i + 1) % column != 0))
		{
			Spring* spring = new Spring(rigidbodyList[i], rigidbodyList[i + 1]);
			spring->SetProperties(gap, springconstant, damping);
			springList.push_back(spring);
		}
		// i�� i + column. �Ʒ��� �������� �������� �������ش�.
		if (i + column < rigidbodyList.size())
		{
			Spring* spring = new Spring(rigidbodyList[i], rigidbodyList[i + column]);
			spring->SetProperties(gap, springconstant, damping);
			springList.push_back(spring);
		}
		// i�� i + column + 1. ������ �Ʒ� �밢�� �������� �������ش�.
		if ((i + column + 1) < rigidbodyList.size() && ((i + 1) % column != 0))
		{
			Spring* spring = new Spring(rigidbodyList[i], rigidbodyList[i + column + 1]);
			spring->SetProperties(gap * 1.4142135623f, springconstant, damping);
			springList.push_back(spring);
		}
		// i�� i - column + 1. ������ �� �밢�� �������� �������ش�.
		if (((i - column + 1) > 0) && ((i + 1) % column != 0))
		{
			Spring* spring = new Spring(rigidbodyList[i], rigidbodyList[1 - column + i]);
			spring->SetProperties(gap * 1.4142135623f, springconstant, damping);
			springList.push_back(spring);
		}


		// �� �� �����ϰ� �������� �����ϰ� �ʹٸ�
		// i + column + 2
		// i + column * 2 + 1
		// i - column * 2 + 1
		if ((i + column + 2) < rigidbodyList.size() && (i + 2) % column != 0 && (i + 1) % column != 0)
		{
			Spring* spring = new Spring(rigidbodyList[i], rigidbodyList[i + column + 2]);
			spring->SetProperties(gap * 2, springconstant, damping);
			springList.push_back(spring);
		}
		if ((i - column + 2) > 0 && (i + 2) % column != 0 && (i + 1) % column != 0)
		{
			Spring* spring = new Spring(rigidbodyList[i], rigidbodyList[i - column + 2]);
			spring->SetProperties(gap * 2, springconstant, damping);
			springList.push_back(spring);
		}
		if ((i + 2 * column + 1) < rigidbodyList.size() && (i + 1) % column != 0)
		{
			Spring* spring = new Spring(rigidbodyList[i], rigidbodyList[i + 2 * column + 1]);
			spring->SetProperties(gap * 2, springconstant, damping);
			springList.push_back(spring);
		}
		if (((i - 2 * column + 1) > 0) && (i + 1) % column != 0)
		{
			Spring* spring = new Spring(rigidbodyList[i], rigidbodyList[i - 2 * column + 1]);
			spring->SetProperties(gap * 2, springconstant, damping);
			springList.push_back(spring);
		}
	}
}

void SoftBody::Translate(Vector2D move)
{
	for (auto& e : rigidbodyList)
	{
		e->SetPosition(e->GetPosition() + move);
	}
}

void SoftBody::SetAccelerator(Vector2D move)
{
	for (auto& e : rigidbodyList)
	{
		e->SetAcceleration(move);
	}
}

void SoftBody::Update(float dt)
{
	for (auto& e : springList)
	{
		e->PhysicsUpdate(dt);
	}
}

void SoftBody::Render()
{
	for (auto& e : springList)
	{
		e->Render();
	}
}
