#pragma once
#include "Vector2D.h"

class ObjectCollider;

class RigidBody
{
public:
	RigidBody();
	~RigidBody();

	/// ��� �� ��Ծ���� ���� ���ø�
	template <typename T>
	void		SetCollider(float X, float Y, float radius, float temp);
	void		RemoveCollider();

	void VelocityUpdate(Vector2D gravity, float dt);
	void Update(Vector2D gravity, float dt);
	void ApplyImpulse(const Vector2D& impulse, const Vector2D& contact);
	void ApplyForce(const Vector2D& force);
	void RemoveForce();
	ObjectCollider*& GetCollider();

	static int Count;
	int ID;

	public:
	// ��ü�� ���¸� �������� ���ڴµ�...
	ObjectCollider* pCollider = nullptr;

private:
	/// ������ ������ ���� ����
	Vector2D	m_velocity{};				// �ӵ�
	Vector2D	m_acceleration{};			// ���ӵ�
	float		m_angularVelocity{};		// ���ӵ�

	Vector2D	m_force{};					// ��
	float		m_torque{};					// ��ũ

	/// �̹� ���� ������
	float		m_mass{};					// ����
	float		m_invMass{0.f};				// �� ����
	float		m_Inertia{};				// ���� ���Ʈ
	float		m_invInertia{};				// �� ���� ���Ʈ  // �� ���� ���� ������ ���� �����ص�
	
	float		m_e{ 0.2f };				// �ݹ߰��
	float		m_Density{ 5.f };			// �е�  // ������ �е��� ������ ������ ����.
	float		m_StaticFriction{ 0.5f };	// ���� ������
	float		m_DynamicFriction{ 0.3f };	// � ������
	float		m_linerDamping{ 0.1f };
	float		m_angularDamping{ 0.1f };

	/// ��ȯ�� �ʿ��� ����
	Vector2D	m_position{};				// ��ġ
	float		m_angle{};					// ȸ��
	Vector2D	m_scale{ 1.f, 1.f };		// ��ô

public:
	#pragma region Get �Լ�
	Vector2D&	GetVelocity();
	Vector2D&	GetAcceleration();
	float&		GetAngularVelocity();

	float&		GetMass();
	float&		GetInvMass();
	float&		GetInertia();
	float&		GetInvInertia();

	float&		GetCOR();					//  �ݹ� ��� (coefficient of restitution)
	float&		GetDensity();
	float&		GetStaticFriction();
	float&		GetDynamicFriction();

	Vector2D&	GetPosition();
	float&		GetAngle();
	Vector2D&	GetScale();
	#pragma endregion Get �Լ�

public:
	#pragma region Set �Լ�
	void		SetVelocity(Vector2D velocity);
	void		SetAcceleration(Vector2D acceleration);
	void		SetAngularVelocity(float angularVelocity);

	void		SetMass(float mass);
	void		SetInvMass(float invMass);
	void		SetInertia(float inertia);
	
	void		SetCOR(float e);
	void		SetDensity(float density);
	void		SetStaticFriction(float Sfriction);
	void		SetDynamicFriction(float Dfriction);

	void		SetPosition(Vector2D position);
	void		SetAngle(float angle);
	void		SetScale(Vector2D scale);

	void		SetForce(Vector2D force);
	#pragma endregion Set �Լ�

public:
};