#include "Player.h"
#include "Engine/Engine.h"
#include "Game/Game.h"
#include "Level/GameLevel.h"
#include "Actor/DrawableActor.h"

Player::Player(const Vector2& position, GameLevel* level)
	: DrawableActor(""), refLevel(level)
{
	// 위치 설정.
	this->position = position;

	width = 3;
	// 색상 설정.
	color = Color::White;
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);

	isWalking = false; // 걷는 상태 초기화
	// ESC 종료.
	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		//Engine::Get().QuitGame();
		// 메뉴 토글.
		Game::Get().ToggleMenu();
	}

	// 상하좌우.
	if (Engine::Get().GetKey(VK_LEFT))
	{
		isWalking = true;
		isWalkingLeft = true;

		// 이동 가능한지 확인.
		if (refLevel->CanPlayerMove(Vector2(position.x - 1, position.y)) && refLevel->CanPlayerMove(Vector2(position.x - 1, position.y-3)))
		{
			position.x -= 1;
		}

		//position.x -= 1;
		//position.x = position.x < 0 ? 0 : position.x;
	}

	if (Engine::Get().GetKey(VK_RIGHT))
	{
		isWalking = true;
		isWalkingLeft = false;

		// 이동 가능한지 확인.
		if (refLevel->CanPlayerMove(Vector2(position.x + 3, position.y)) && refLevel->CanPlayerMove(Vector2(position.x + 3, position.y-3)))
		{
			position.x += 1;
		}
	}


	// 점프.
	if (Engine::Get().GetKeyDown(VK_SPACE) && !isJumping)
	{
		Jump();
	}

	if (!isJumping) // 점프 중이 아니라면 중력 적용
	{
		float fallSpeed = gravity * deltaTime * 6; // 중력 기반 하강 속도
		float newY = position.y + fallSpeed;

		// 아래로 이동 가능한지 확인.
		while (newY > position.y)
		{
			int nextY = static_cast<int>(newY);

			// 현재 위치에서 아래 위치가 비어있는지 확인.
			if (refLevel->CanPlayerMove(Vector2(position.x, nextY)) &&
				refLevel->CanPlayerMove(Vector2(position.x + 2, nextY)))
			{
				position.y = nextY; // 위치 업데이트
			}
			else
			{
				break; // 땅에 닿으면 멈춤
			}

			newY -= 1; // 한 칸씩 체크
		}
	}

	// 점프 중일 때 중력 적용.
	if (isJumping)
	{
		jumpVelocity += (gravity)*(deltaTime*6);
		int currentposition = position.y;
		currentposition += jumpVelocity*deltaTime*6;

		if (currentposition >= 26)
		{
			currentposition = 26;
			isJumping = false;
			jumpVelocity = 0.0f;

			position.y = currentposition;
			return;
		}

		if (currentposition < 4)
		{
			currentposition = 4;
		}

		if (refLevel->CanPlayerMove(Vector2(position.x, currentposition-3)) == false || refLevel->CanPlayerMove(Vector2(position.x + 2, currentposition-3)) == false)
		{
			return;
		}

		// 땅에 닿으면 점프 종료.
		if (refLevel->CanPlayerMove(Vector2(position.x, currentposition)) == false || refLevel->CanPlayerMove(Vector2(position.x+2, currentposition)) == false)
		{
			
			while(1)
			{
				int prevPosition = currentposition;
				currentposition--;
				if (refLevel->CanPlayerMove(Vector2(position.x, currentposition)) == false)
				{
					continue;
				}
				else
				{
					position.y = currentposition;
					break;
				}
			}

			isJumping = false;
			jumpVelocity = 0.0f;
			return;
		}
		position.y = currentposition;
	}


	// 프레임 업데이트 (걷는 모션)
	if (isWalking)
	{
		frameTimer += deltaTime;
		if (frameTimer >= frameTime)
		{
			currentFrame = (currentFrame + 1) % 2; // 프레임 순환
			frameTimer = 0.0f;
		}
	}
	else
	{
		currentFrame = 0; // 서 있는 상태로 초기화
	}

	// 카메라 갱신
	refLevel->UpdateCamera(position);
}

bool Player::Intersect(const DrawableActor& other)
{
	// AABB(Axis Aligned Bounding Box).

// 내 x좌표 최소/최대.
	int min = position.x;
	int max = position.x + width;
	
	// 다른 액터의 x좌표 최소/최대.
	int otherMin = other.Position().x;
	int otherMax = other.Position().x + other.Width();

	// 다른 액터의 왼쪽 끝 위치가 내 오른쪽 끝 위치를 벗어나면 충돌 안함.
	if (otherMin > max)
	{
		return false;
	}

	// 다른 액터의 오른쪽 끝 위치가 내 왼쪽 끝 위치보다 작으면 충돌 안함.
	if (otherMax < min)
	{
		return false;
	}

	// 위의 두 경우가 아니라면 (x좌표는 서로 겹침), y위치 비교.
	return position.y == other.Position().y;

	
}

void Player::Jump()
{
	isJumping = true;
	jumpVelocity = -13; // 초기 점프 속도.
}

void Player::Draw()
{
	if (isWalking)
	{
		const char** stickman = isWalkingLeft ? stickmanWalkingLeft[currentFrame]
			: stickmanWalkingRight[currentFrame];

		for (int i = 0; i < 4; ++i)
		{
			Engine::Get().Draw(Vector2(position.x, position.y - 3 + i), stickman[i], color);
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			Engine::Get().Draw(Vector2(position.x,  position.y - 3 + i), stickman[i], color);
		}
	}
}

void Player::DrawAt(Vector2 InPosition)
{
	Super::DrawAt(InPosition);

	if (isWalking)
	{
		const char** stickman = isWalkingLeft ? stickmanWalkingLeft[currentFrame]
			: stickmanWalkingRight[currentFrame];

		for (int i = 0; i < 4; ++i)
		{
			Engine::Get().Draw(Vector2(InPosition.x, InPosition.y - 3 + i), stickman[i], color);
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			Engine::Get().Draw(Vector2(InPosition.x, InPosition.y - 3 + i), stickman[i], color);
		}
	}
}
