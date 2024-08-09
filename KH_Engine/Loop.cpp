#include "Loop.h"

#include "Graphics.h"
#pragma comment(lib, "Graphics.lib")
#include <iostream>

#include "RigidBody.h"
#include "SoftBody.h"

#include "LineCollider.h"
#include "CircleCollider.h"
#include "BoxCollider.h"
#include "PolygonCollider.h"

#include "ColliderList.h"
#include "OnCollision.h"
#include "Spring.h"

#include "InputManager.h"
#include "TimeManager.h"
#include "KH_Define.h"


Loop::Loop() :
	pGraphics(Graphics::GetInstance()),
	pColliderList(ColliderList::GetInstance()),
	rigidbodyManager(RigidBodyManager::GetInstance()),
	pInputManager(&InputManager::GetInstance()),
	state(State::State_GJK)
{

}

Loop::~Loop()
{
	for (size_t i = 0; i < RigidBodyList.size(); i++)
	{
		delete RigidBodyList[i];
		RigidBodyList[i] = nullptr;
	}
}

void Loop::Initialize()
{
	/*
	// CircleCollider* circle1(new CircleCollider{ 40.f, {100.f, 200.f}, {}, {} });
	// BoxCollider* box1(new BoxCollider{ {40.f, 40.f}, {200.f, 400.f}, {}, {} });
	// PolygonCollider* polygon1(new PolygonCollider{ 12, {40.f},{600.f, 200.f}, {}, {} });
	// LineCollider* line1(new LineCollider{ {80.f}, {500.f, 700.f}, {}, {} });
	

	// CircleCollider* circle2(new CircleCollider{ 40.f, {200.f, 200.f}, {}, {} });
	// PolygonCollider* polygon2(new PolygonCollider{ 4, {40.f},{400.f, 400.f}, {}, {} });
	// PolygonCollider* polygon3(new PolygonCollider{ 4, {40.f},{440.f, 400.f}, {}, {} });
	// LineCollider* line2(new LineCollider{ Vector2D(10.f, 250.f), Vector2D(10.f, 720.f) });
	// LineCollider* line3(new LineCollider{ Vector2D(-100.f, 700.f), Vector2D(1700.f, 700.f) });
	// LineCollider* line4(new LineCollider{ Vector2D(1570.f, 250.f), Vector2D(1570.f, 720.f) });

	// pGraphics->LoadBitmapFile(L"../mario.bmp");
	*/

	SoftBody* softbodyblock = new SoftBody();
	softbodyblock->SetSoftBlock(30.f, 5, 8, 30000.f, 500.f);
	softbodyblock->Translate({ 400.f, 500.f });
	softbodyList.push_back(softbodyblock);

	SoftBody* softbodyball = new SoftBody();
	softbodyball->SetSoftBall(20, 7, 80.f, 30000.f, 500.f);
	softbodyball->Translate({ 900.f, 500.f });
	softbodyList.push_back(softbodyball);


	RigidBody* triangle = new RigidBody;
	triangle->SetDensity(0.f);
	triangle->SetCollider<PolygonCollider>(780.f, 700.f, 50.f, 3);
	
	RigidBody* square = new RigidBody;
	square->SetDensity(100.f);
	square->SetCollider<PolygonCollider>(800.f, 400.f, 50.f, 4);

	RigidBody* pentagon = new RigidBody;
	pentagon->SetDensity(100.f);
	pentagon->SetCollider<PolygonCollider>(800.f, 600.f, 50.f, 5);


	RigidBody* ground = new RigidBody;
	ground->SetCollider<PolygonCollider>(800.f, 1700.f, 1200.f, 4);
	ground->SetMass(0.f);

	RigidBody* ground2 = new RigidBody;
	ground2->SetCollider<PolygonCollider>(-700.f, 200.f, 1000.f, 4);
	ground2->SetMass(0.f);

	RigidBody* ground3 = new RigidBody;
	ground3->SetCollider<PolygonCollider>(2280.f, 200.f, 1000.f, 4);
	ground3->SetMass(0.f);

	RigidBody* ground4 = new RigidBody;
	ground4->SetCollider<PolygonCollider>(800.f, -840.f, 1200.f, 4);
	ground4->SetMass(0.f);

	// collisionDetection.m_currCollision.resize(CList.size(), std::vector<size_t>(0));
	// collisionDetection.m_prevCollision.resize(CList.size(), std::vector<size_t>(0));
}


void Loop::Update(float dt)
{
	KeyUpdate();
}

void Loop::PhysicsUpdate(float dt)
{
	static Vector2D gravity{ 0.f, 50.f };

	/// 매니폴드 초기화 및 업데이트
	manifold.clear();
	for (size_t i = 0; i < rigidbodyManager->rigidbodyList.size(); i++)
	{
		RigidBody* a = rigidbodyManager->rigidbodyList[i];

		for (size_t j = i + 1; j < rigidbodyManager->rigidbodyList.size(); j++)
		{
			RigidBody* b = rigidbodyManager->rigidbodyList[j];

			if (a->GetInvMass() == 0.f && b->GetInvMass() == 0.f)
			{
				continue;
			}
			Manifold m(a, b);
			m.Collision();
			if (m.contactPoint.size())
			{
				manifold.push_back(m);
			}
		}
	}

	for (auto& e : softbodyList)
	{
		e->Update(dt);
	}

	/// 속력 업데이트
	for (size_t i = 0; i < rigidbodyManager->rigidbodyList.size(); i++)
	{
		rigidbodyManager->rigidbodyList[i]->VelocityUpdate(gravity, dt);

		// ID 출력
		pGraphics->PrintValue(
			rigidbodyManager->rigidbodyList[i]->ID, 
			rigidbodyManager->rigidbodyList[i]->GetPosition().X, 
			rigidbodyManager->rigidbodyList[i]->GetPosition().Y - 20, 10);
	}

	/// 충돌 감지
	for (size_t i = 0; i < manifold.size(); i++)
	{
		manifold[i].Initialize(gravity, dt);

		// 충돌 관계
		pGraphics->PrintString(L"A", manifold[i].A->GetPosition().X, manifold[i].A->GetPosition().Y - 10, 10);
		pGraphics->PrintString(L"B", manifold[i].B->GetPosition().X, manifold[i].B->GetPosition().Y - 10, 10);
	}

	/// 충격량
	for (size_t i = 0; i < manifold.size(); i++)
	{
		manifold[i].CollisionEvent();

		// 속력 출력
		pGraphics->PrintValue(manifold[i].A->GetVelocity().X, manifold[i].A->GetPosition().X, manifold[i].A->GetPosition().Y, 10);
		pGraphics->PrintValue(manifold[i].A->GetVelocity().Y, manifold[i].A->GetPosition().X, manifold[i].A->GetPosition().Y + 10, 10);
		pGraphics->PrintValue(manifold[i].B->GetVelocity().X, manifold[i].B->GetPosition().X, manifold[i].B->GetPosition().Y, 10);
		pGraphics->PrintValue(manifold[i].B->GetVelocity().Y, manifold[i].B->GetPosition().X, manifold[i].B->GetPosition().Y + 10, 10);

		if (manifold[i].contactPoint.size());
		{
			float a1 = manifold[i].contactPoint[0].X;
			float a2 = manifold[i].contactPoint[0].Y;
			pGraphics->DrawFillCircle(D2D1::Point2(a1, a2), 3);
			if (manifold[i].contactPoint.size() == 2)
			{
				float b1 = manifold[i].contactPoint[1].X;
				float b2 = manifold[i].contactPoint[1].Y;
				pGraphics->DrawFillCircle(D2D1::Point2(b1, b2), 3);
			}
		}
	}

	/// 위치 업데이트
	for (size_t i = 0; i < rigidbodyManager->rigidbodyList.size(); i++)
	{
		rigidbodyManager->rigidbodyList[i]->Update(gravity, dt);
	}

	/// 위치 조정 / 겹치면 일정 부분 밀어냄.
	for (size_t i = 0; i < manifold.size(); i++)
	{
		manifold[i].PushOut();
	}

	for (auto& e : rigidbodyManager->rigidbodyList)
	{
		e->RemoveForce();
	}

	/// 콜라이더 위치를 강체에 맞춤
	for (auto e : CList)
	{
		e->Update(dt);
	}
}

/*
void Loop::CollisionCheck()
{
	collisionDetection.m_currCollision.clear();
	collisionDetection.m_currCollision.resize(CList.size(), std::vector<size_t>(0));

	if (CList.size() <= 1)
	{
		return;
	}

	for (size_t i = 0; i < CList.size(); i++)
	{
		for (size_t j = i + 1; j < CList.size(); j++)
		{
			
			if (i == j)
			{
				continue;
			}
			
	#pragma region AABB
				if ((state == State_AABB) && collisionDetection.AABB(CList[i], CList[j]))
				{
					CList[i]->isCollision = true;
					CList[j]->isCollision = true;
				}
	#pragma endregion AABB

	#pragma region OBB
				else if (state == State_OBB)
				{
					if (CList[i]->Ctype == eColliderType::Type_Line)
					{
						LineCollider* line1 = dynamic_cast<LineCollider*>(CList[i]);

						if (CList[j]->Ctype == eColliderType::Type_Line)
						{
							LineCollider* line2 = dynamic_cast<LineCollider*>(CList[j]);

							if (collisionDetection.LineToLine(*line1, *line2))
							{
								CList[i]->isCollision = true;
								CList[j]->isCollision = true;
							}
						}
						else if (CList[j]->Ctype == eColliderType::Type_Polygon)
						{
							PolygonCollider* poly1 = dynamic_cast<PolygonCollider*>(CList[j]);

							if (collisionDetection.LineToOBB(*line1, *poly1))
							{
								CList[i]->isCollision = true;
								CList[j]->isCollision = true;
							}
						}
						else if (CList[j]->Ctype == eColliderType::Type_Circle)
						{
							CircleCollider* circle1 = dynamic_cast<CircleCollider*>(CList[j]);

							if (collisionDetection.LineToCircle(*line1, *circle1))
							{
								CList[i]->isCollision = true;
								CList[j]->isCollision = true;
							}
						}
					}

					else if (CList[i]->Ctype == eColliderType::Type_Polygon)
					{
						PolygonCollider* poly1 = dynamic_cast<PolygonCollider*>(CList[i]);

						if (CList[j]->Ctype == eColliderType::Type_Line)
						{
							LineCollider* line1 = dynamic_cast<LineCollider*>(CList[j]);

							if (collisionDetection.LineToOBB(*line1, *poly1))
							{
								CList[i]->isCollision = true;
								CList[j]->isCollision = true;
							}
						}
						else if (CList[j]->Ctype == eColliderType::Type_Polygon)
						{
							PolygonCollider* poly2 = dynamic_cast<PolygonCollider*>(CList[j]);

							if (collisionDetection.OBB(*poly1, *poly2))
							{
								CList[i]->isCollision = true;
								CList[j]->isCollision = true;
							}
						}
						else if (CList[j]->Ctype == eColliderType::Type_Circle)
						{
							CircleCollider* circle1 = dynamic_cast<CircleCollider*>(CList[j]);

							if (collisionDetection.CircleToOBB(*circle1, *poly1))
							{
								CList[i]->isCollision = true;
								CList[j]->isCollision = true;
							}
						}
					}

					else if (CList[i]->Ctype == eColliderType::Type_Circle)
					{
						CircleCollider* circle1 = dynamic_cast<CircleCollider*>(CList[i]);

						if (CList[j]->Ctype == eColliderType::Type_Line)
						{
							LineCollider* otherCollider = dynamic_cast<LineCollider*>(CList[j]);

							if (collisionDetection.LineToCircle(*otherCollider, *circle1))
							{
								OnCollision::EventCircleToLine(*circle1, *otherCollider);
						
								CList[i]->isCollision = true;
								CList[j]->isCollision = true;
							}
						}
						else if (CList[j]->Ctype == eColliderType::Type_Polygon)
						{
							PolygonCollider* otherCollider = dynamic_cast<PolygonCollider*>(CList[j]);

							if (collisionDetection.CircleToOBB(*circle1, *otherCollider))
							{
								CList[i]->isCollision = true;
								CList[j]->isCollision = true;
							}
						}
						else if (CList[j]->Ctype == eColliderType::Type_Circle)
						{
							CircleCollider* otherCollider = dynamic_cast<CircleCollider*>(CList[j]);

							if (collisionDetection.CircleToCircle(*circle1, *otherCollider))
							{
								// 이전 프레임에 충돌한 적이 없는지 확인
								if (find(collisionDetection.m_prevCollision[i].begin(),
									collisionDetection.m_prevCollision[i].end(), j) == collisionDetection.m_prevCollision[i].end())
								{
									// 이전 프레임에 충돌한 적 없고
									// 현재 프레임에 충돌 안 했는지 확인
									if (find(collisionDetection.m_currCollision[i].begin(), 
										collisionDetection.m_currCollision[i].end(),j) == collisionDetection.m_currCollision[i].end())
									{
										/// 이전, 이번 프레임에 충돌한 적 없으면 리스트에 아이디를 넣음
										collisionDetection.m_currCollision[i].push_back(j);

								
										/// 충돌 이벤트
										OnCollision::EventCircleToCircle(*circle1, *otherCollider);
								
									}
								}
								CList[i]->isCollision = true;
								CList[j]->isCollision = true;
							}
						}
					}
				}
	#pragma endregion OBB, CC
				
	#pragma region GJK
				else if ((state == State_GJK) && collisionDetection.GJK(CList[i], CList[j]))
				{
					float a1 = collisionDetection.manifold.contactPoint[0].X;
					float a2 = collisionDetection.manifold.contactPoint[0].Y;
					pGraphics->PrintString(L"충돌지점", a1, a2, 10);
					if (collisionDetection.manifold.contactPoint.size() == 2)
					{
						float b1 = collisionDetection.manifold.contactPoint[1].X;
						float b2 = collisionDetection.manifold.contactPoint[1].Y;
						pGraphics->PrintString(L"충돌지점", b1, b2, 10);
					}
					manifold[i] = collisionDetection.manifold;
					manifold[i].isCollision = true;

					CList[i]->isCollision = true;
					CList[j]->isCollision = true;
				}
	#pragma endregion GJK
		}
	}

	/// 현재 프레임에 충돌 여부를 이전 프레임 정보로 옮김
	// collisionDetection.m_prevCollision = collisionDetection.m_currCollision;
}
*/

void Loop::Render()
{
	// pGraphics->BeginDraw();

	#pragma region 도형 랜더
	for (size_t i = 0; i < CList.size(); i++)
	{
		CList[i]->Render();
		if (state == State_AABB)
		{
			CList[i]->AABBDebugRender();
		}
		CList[i]->isCollision = false;
	}

	for (auto& e : springList)
	{
		e->Render();
	}
#pragma endregion 도형 랜더

	#pragma region 비트맵 랜더
	pGraphics->DrawBitmap(0, 400, 400, 400, 400);
	#pragma endregion 비트맵 랜더

	for (auto& e : softbodyList)
	{
		e->Render();
	}

	#pragma region 조작 안내
	// 조작 안내
	// pGraphics->PrintString(L"F1 : 원", 10, 10, 20);
	// pGraphics->PrintString(L"F2 : 폴리곤", 10, 30, 20);
	// pGraphics->PrintString(L"F3 : 박스", 10, 50, 20);
	// pGraphics->PrintString(L"F4 : 선", 10, 70, 20);
	// pGraphics->PrintString(L"F6 : AABB", 200, 10, 20);
	// pGraphics->PrintString(L"F7 : OBB + 선분 + 원", 200, 30, 20);
	// pGraphics->PrintString(L"F8 : GJK", 200, 55, 20);

	static size_t cSize = 15;
	pGraphics->PrintString(L"Q :      반시계 회전", 10, 750, cSize);
	pGraphics->PrintString(L"E :      시계 회전", 10, 765, cSize);
	// pGraphics->PrintString(L"W :      스케일 Y 증가", 10, 780, cSize);
	// pGraphics->PrintString(L"S :      스케일 Y 감소", 10, 795, cSize);
	// pGraphics->PrintString(L"D :      스케일 X 증가", 10, 810, cSize);
	// pGraphics->PrintString(L"A :      스케일 X 감소", 10, 825, cSize);
	pGraphics->PrintString(L"십자키 :  이동", 10, 840, cSize);
	#pragma endregion 조작 안내

	#pragma region 디버그 정보
	size_t fontSize = 15;
	pGraphics->PrintString(L"FPS: ", 1420, 10, cSize + 15);
	pGraphics->PrintValue(fFPS, 1510, 10, fontSize + 15);

	pGraphics->PrintString(L"X좌표: ", 1420, 60, cSize);
	pGraphics->PrintValue((int)CList[index]->m_Position.X, 1480, 60, fontSize);

	pGraphics->PrintString(L"Y좌표: ", 1420, 80, cSize);
	pGraphics->PrintValue((int)CList[index]->m_Position.Y, 1480, 80, fontSize);
	
	pGraphics->PrintString(L"X속도: ", 1420, 100, cSize);
	pGraphics->PrintValue((int)CList[index]->pRigidBody->GetVelocity().X, 1480, 100, fontSize);

	pGraphics->PrintString(L"Y속도: ", 1420, 120, cSize);
	pGraphics->PrintValue((int)CList[index]->pRigidBody->GetVelocity().Y, 1480, 120, fontSize);

	pGraphics->PrintString(L"각도: ", 1420, 140, cSize);
	pGraphics->PrintFValue(DEGREE(CList[index]->m_Angle), 1480, 140, fontSize);

	pGraphics->PrintString(L"각속도: ", 1420, 160, cSize);
	pGraphics->PrintFValue(DEGREE(CList[index]->pRigidBody->GetAngularVelocity()), 1480, 160, fontSize);
	
	pGraphics->PrintString(L"X축척: ", 1420, 180, cSize);
	pGraphics->PrintValue((int)CList[index]->m_Scale.X, 1480, 180, fontSize);

	pGraphics->PrintString(L"Y축척: ", 1420, 200, cSize);
	pGraphics->PrintValue((int)CList[index]->m_Scale.Y, 1480, 200, fontSize);

	pGraphics->PrintString(L"질량: ", 1420, 220, cSize);
	pGraphics->PrintValue((int)CList[index]->pRigidBody->GetMass(), 1480, 220, fontSize);
	
	//  pGraphics->PrintString(L": ", 1420, 240, cSize);
	// pGraphics->PrintValue((int)CList[index]->pRigidBody->GetPosition().X, 1480, 240, fontSize);
	// 
	// pGraphics->PrintString(L"질량 중심: ", 1420, 260, cSize);
	// pGraphics->PrintValue((int)CList[index]->pRigidBody->GetPosition().Y, 1480, 260, fontSize);
	#pragma endregion 디버그 정보

	#pragma region 모드 변경
	if (state == State_AABB)
	{
		// pGraphics->PrintString(L"AABB", 1500, 810, 20);
	}
	else if (state == State_OBB)
	{
		// pGraphics->PrintString(L"OBB, 원, 선", 1450, 810, 20);
	}
	else if (state == State_GJK)
	{
		pGraphics->PrintString(L"GJK", 1500, 810, 20);
	}
	#pragma endregion 모드 변경

	// pGraphics->EndDraw();
}

void Loop::Finalize()
{

}

void Loop::KeyUpdate()
{
	pInputManager->Update();
	#pragma region 키입력
	
	/// 이동
	Vector2D move{};
	if (pInputManager->PressedKey(VK_UP))
	{
		move.Y = -400.f;
	}
	if (pInputManager->PressedKey(VK_DOWN))
	{
		move.Y = 400.f;
	}
	if (pInputManager->PressedKey(VK_LEFT))
	{
		move.X = -400.f;
	}
	if (pInputManager->PressedKey(VK_RIGHT))
	{
		move.X = 400.f;
	}
	if (pInputManager->ReleasedKeyNow(VK_UP) || pInputManager->ReleasedKeyNow(VK_DOWN) || pInputManager->ReleasedKeyNow(VK_LEFT) || pInputManager->ReleasedKeyNow(VK_RIGHT))
	{
		move = Vector2D(0.f, 0.f);
	}
	
	
	rigidbodyManager->rigidbodyList[index]->SetAcceleration(move);
	softbodyList[0]->SetAccelerator(move);

	/// 회전
	float rotate{};
	// Q
	if (pInputManager->PressedKey(0x51))
	{
		rotate = -(PI / 2);
	}
	if (pInputManager->ReleasedKeyNow(0x51))
	{
		rotate = 0.f;
	}
	// E
	if (pInputManager->PressedKey(0x45))
	{
		rotate = PI / 2;
	}
	if (pInputManager->ReleasedKeyNow(0x45))
	{
		rotate = 0.f;
	}

	rigidbodyManager->rigidbodyList[index]->SetAngularVelocity(rotate);
	/*
	/// 스케일
	// W
	if (pInputManager->PressedKey(0x57))
	{
		CList[index]->m_Scale->Y += 0.1f;
	}
	// S
	else if (pInputManager->PressedKey(0x53))
	{
		CList[index]->m_Scale->Y -= 0.1f;
	}
	// A
	else if (pInputManager->PressedKey(0x41))
	{
		CList[index]->m_Scale->X -= 0.1f;
	}
	// D
	else if (pInputManager->PressedKey(0x44))
	{
		CList[index]->m_Scale->X += 0.1f;
	}
	*/

	// 조종하는 도형을 변경
	if (pInputManager->PressedKey(VK_F1))
	{
		index = 0;
	}
	if (pInputManager->PressedKey(VK_F2))
	{
		index = 1;
	}
	if (pInputManager->PressedKey(VK_F3))
	{
		index = 2;
	}
	if (pInputManager->PressedKey(VK_F4))
	{
		index = 3;
	}

	// 충돌 감지 상태 변경
	if (pInputManager->PressedKey(VK_F6))
	{
		state = State_AABB;
	}
	if (pInputManager->PressedKey(VK_F7))
	{
		state = State_OBB;
	}
	if (pInputManager->PressedKey(VK_F8))
	{
		state = State_GJK;
	}
	#pragma endregion 키입력
}

void Loop::MainLoop()
{
	MSG msg{};
	TimeManager& timeManager = TimeManager::GetInstance();
	Initialize();
	
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)	break;
			DispatchMessage(&msg);
		}
		else
		{
			pGraphics->BeginDraw();
			/// 델타타임 ****중요****
			timeManager.Update();

			Update(timeManager.GetfDeltaTime());
			// Update(0.01f);
			// CollisionCheck();
			PhysicsUpdate(timeManager.GetfDeltaTime());
			// PhysicsUpdate(0.01f);
			Render();
			pGraphics->EndDraw();
		}
	}

	Finalize();
}