#include "RigidBody.h"

#include "ObjectCollider.h"
#include "CircleCollider.h"
#include "LineCollider.h"
#include "BoxCollider.h"
#include "PolygonCollider.h"
#include "RigidBodyManager.h"

int RigidBody::Count = 0;

RigidBody::RigidBody()
{
	ID = Count;
	Count++;
	RigidBodyManager::GetInstance()->AddRigidBody(this);
}

RigidBody::~RigidBody()
{
	delete pCollider;
	RigidBodyManager::GetInstance()->RemoveRigidBody(this);
}

#pragma region 콜라이더
template <>
void RigidBody::SetCollider<LineCollider>(float X, float Y, float radius, float temp)
{
	if (pCollider != nullptr)
	{
		return;
	}

	pCollider = new LineCollider(radius, Vector2D(X, Y), this);
}

template <>
void RigidBody::SetCollider<CircleCollider>(float X, float Y, float radius, float temp)
{
	if (pCollider != nullptr)
	{
		return;
	}

	pCollider = new CircleCollider(radius, Vector2D(X, Y), this);
}

template <>
void RigidBody::SetCollider<BoxCollider>(float X, float Y, float radius, float temp)
{
	if (pCollider != nullptr)
	{
		return;
	}

	pCollider = new BoxCollider(Vector2D(radius, temp), Vector2D(X, Y), this);
}

template <>
void RigidBody::SetCollider<PolygonCollider>(float X, float Y, float radius, float temp)
{
	if (pCollider != nullptr)
	{
		return;
	}

	pCollider = new PolygonCollider(temp, radius, Vector2D(X, Y), this);
}

void RigidBody::RemoveCollider()
{
	delete pCollider;
	pCollider = nullptr;
}

void RigidBody::VelocityUpdate(Vector2D gravity, float dt)
{
	if (m_invMass == 0.f)
	{
		return;
	}

	m_velocity += (m_force * m_invMass + gravity + m_acceleration) * dt;
	m_angularVelocity += m_torque * m_invInertia * dt;

	m_velocity *= std::exp(-(m_linerDamping) * dt);
	m_angularVelocity *= std::exp(-(m_angularDamping) * dt);
}

void RigidBody::Update(Vector2D gravity, float dt)
{
	if (m_invMass == 0.f)
	{
		return;
	}

	m_position += m_velocity * dt;
	m_angle += m_angularVelocity * dt;

	if (m_angle > 2 * PI)
	{
		m_angle -= 2 * PI;
	}
	if (m_angle < -2 * PI)
	{
		m_angle += 2 * PI;
	}

	VelocityUpdate(gravity, dt);
}

void RigidBody::ApplyImpulse(const Vector2D& impulse, const Vector2D& contact)
{
	if (m_invMass == 0.f)
	{
		return;
	}

	// 선형 운동
	// 충격량 = (F * dt) = (m * a * dt) = (m * dV) = dP
	// dV = 충격량 / m
	Vector2D dV = impulse * m_invMass;
	m_velocity = m_velocity + dV;

	// 회전 운동
	// t = Ia = r X F
	// 
	// 양변에 dt 곱함
	// t * dt = (r X F * dt) = (r X m * a * dt) = (r X m * dV) = r X dP
	// dL = (t * dt) = r X dP
	// dL = Ia * dt = I * dw
	// dw = dL / I
	// dw = (r X dP) / I
	float dw = contact.Cross(impulse) * m_invInertia;
	m_angularVelocity = m_angularVelocity + dw;
}

void RigidBody::ApplyForce(const Vector2D& force)
{
	if (m_invMass == 0.f)
	{
		return;
	}

	m_force += force;
}

void RigidBody::RemoveForce()
{
	m_force = {0.f, 0.f};
	m_torque = 0;
}

ObjectCollider*& RigidBody::GetCollider()
{
	if (pCollider == nullptr)
	{

	}
	return pCollider;
}

#pragma endregion 콜라이더

#pragma region Get함수
float& RigidBody::GetMass()
{
	return m_mass;
}

float& RigidBody::GetInvMass()
{
	return m_invMass;
}

Vector2D& RigidBody::GetVelocity()
{
	return m_velocity;
}

Vector2D& RigidBody::GetAcceleration()
{
	return m_acceleration;
}

float& RigidBody::GetAngularVelocity()
{
	return m_angularVelocity;
}

float& RigidBody::GetCOR()
{
	return m_e;
}

float& RigidBody::GetDensity()
{
	return m_Density;
}

float& RigidBody::GetInertia()
{
	return m_Inertia;
}

float& RigidBody::GetInvInertia()
{
	return m_invInertia;
}

float& RigidBody::GetStaticFriction()
{
	return m_StaticFriction;
}

float& RigidBody::GetDynamicFriction()
{
	return m_DynamicFriction;
}

Vector2D& RigidBody::GetPosition()
{
	return m_position;
}

float& RigidBody::GetAngle()
{
	return m_angle;
}

Vector2D& RigidBody::GetScale()
{
	return m_scale;
}
#pragma endregion Get함수

#pragma region Set함수

void RigidBody::SetMass(float mass)
{
	m_mass = mass;
	if (mass == 0)
	{
		m_invMass = 0;
	}
	else
	{
		m_invMass = 1.f / m_mass;
	}
}

void RigidBody::SetInvMass(float invMass)
{
	m_invMass = invMass;
}

void RigidBody::SetVelocity(Vector2D velocity)
{
	m_velocity = velocity;
}

void RigidBody::SetAcceleration(Vector2D acceleration)
{
	m_acceleration = acceleration;
}

void RigidBody::SetAngularVelocity(float angularVelocity)
{
	m_angularVelocity = angularVelocity;
}

void RigidBody::SetCOR(float e)
{
	m_e = e;
}

void RigidBody::SetDensity(float density)
{
	m_Density = density;
}

void RigidBody::SetStaticFriction(float Sfriction)
{
	m_StaticFriction = Sfriction;
}

void RigidBody::SetDynamicFriction(float Dfriction)
{
	m_DynamicFriction = Dfriction;
}

void RigidBody::SetInertia(float inertia)
{
	m_Inertia = inertia;
	if (inertia == 0)
	{
		m_invInertia = 0;
	}
	else
	{
		m_invInertia = 1.f / m_Inertia;
	}
}

void RigidBody::SetPosition(Vector2D position)
{
	m_position = position;
}

void RigidBody::SetAngle(float angle)
{
	m_angle = angle;
}

void RigidBody::SetScale(Vector2D scale)
{
	m_scale = scale;
}

void RigidBody::SetForce(Vector2D force)
{
	m_force = force;
}

#pragma endregion Set함수