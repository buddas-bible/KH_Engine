#pragma once
#include "Vector2D.h"

class ObjectCollider;

class RigidBody
{
public:
	RigidBody();
	~RigidBody();

	/// 배운 거 써먹어보려고 만든 템플릿
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
	// 강체가 형태를 가졌으면 좋겠는데...
	ObjectCollider* pCollider = nullptr;

private:
	/// 물리적 정보를 담은 변수
	Vector2D	m_velocity{};				// 속도
	Vector2D	m_acceleration{};			// 가속도
	float		m_angularVelocity{};		// 각속도

	Vector2D	m_force{};					// 힘
	float		m_torque{};					// 토크

	/// 이미 계산된 물리량
	float		m_mass{};					// 질량
	float		m_invMass{0.f};				// 역 질량
	float		m_Inertia{};				// 관성 모멘트
	float		m_invInertia{};				// 역 관성 모멘트  // 쓸 일이 많기 때문에 따로 저장해둠
	
	float		m_e{ 0.2f };				// 반발계수
	float		m_Density{ 5.f };			// 밀도  // 면적과 밀도를 가지고 질량을 얻어옴.
	float		m_StaticFriction{ 0.5f };	// 정지 마찰력
	float		m_DynamicFriction{ 0.3f };	// 운동 마찰력
	float		m_linerDamping{ 0.1f };
	float		m_angularDamping{ 0.1f };

	/// 변환에 필요한 변수
	Vector2D	m_position{};				// 위치
	float		m_angle{};					// 회전
	Vector2D	m_scale{ 1.f, 1.f };		// 축척

public:
	#pragma region Get 함수
	Vector2D&	GetVelocity();
	Vector2D&	GetAcceleration();
	float&		GetAngularVelocity();

	float&		GetMass();
	float&		GetInvMass();
	float&		GetInertia();
	float&		GetInvInertia();

	float&		GetCOR();					//  반발 계수 (coefficient of restitution)
	float&		GetDensity();
	float&		GetStaticFriction();
	float&		GetDynamicFriction();

	Vector2D&	GetPosition();
	float&		GetAngle();
	Vector2D&	GetScale();
	#pragma endregion Get 함수

public:
	#pragma region Set 함수
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
	#pragma endregion Set 함수

public:
};