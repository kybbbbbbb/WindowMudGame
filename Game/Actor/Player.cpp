#include "Player.h"
#include "Engine/Engine.h"
#include "Game/Game.h"
#include "Level/GameLevel.h"
#include "Actor/DrawableActor.h"

Player::Player(const Vector2& position, GameLevel* level)
	: DrawableActor(""), refLevel(level)
{
	// ��ġ ����.
	this->position = position;

	width = 3;
	// ���� ����.
	color = Color::White;
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);

	isWalking = false; // �ȴ� ���� �ʱ�ȭ
	// ESC ����.
	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		//Engine::Get().QuitGame();
		// �޴� ���.
		Game::Get().ToggleMenu();
	}

	// �����¿�.
	if (Engine::Get().GetKey(VK_LEFT))
	{
		isWalking = true;
		isWalkingLeft = true;

		// �̵� �������� Ȯ��.
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

		// �̵� �������� Ȯ��.
		if (refLevel->CanPlayerMove(Vector2(position.x + 3, position.y)) && refLevel->CanPlayerMove(Vector2(position.x + 3, position.y-3)))
		{
			position.x += 1;
		}
	}


	// ����.
	if (Engine::Get().GetKeyDown(VK_SPACE) && !isJumping)
	{
		Jump();
	}

	if (!isJumping) // ���� ���� �ƴ϶�� �߷� ����
	{
		float fallSpeed = gravity * deltaTime * 6; // �߷� ��� �ϰ� �ӵ�
		float newY = position.y + fallSpeed;

		// �Ʒ��� �̵� �������� Ȯ��.
		while (newY > position.y)
		{
			int nextY = static_cast<int>(newY);

			// ���� ��ġ���� �Ʒ� ��ġ�� ����ִ��� Ȯ��.
			if (refLevel->CanPlayerMove(Vector2(position.x, nextY)) &&
				refLevel->CanPlayerMove(Vector2(position.x + 2, nextY)))
			{
				position.y = nextY; // ��ġ ������Ʈ
			}
			else
			{
				break; // ���� ������ ����
			}

			newY -= 1; // �� ĭ�� üũ
		}
	}

	// ���� ���� �� �߷� ����.
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

		// ���� ������ ���� ����.
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


	// ������ ������Ʈ (�ȴ� ���)
	if (isWalking)
	{
		frameTimer += deltaTime;
		if (frameTimer >= frameTime)
		{
			currentFrame = (currentFrame + 1) % 2; // ������ ��ȯ
			frameTimer = 0.0f;
		}
	}
	else
	{
		currentFrame = 0; // �� �ִ� ���·� �ʱ�ȭ
	}

	// ī�޶� ����
	refLevel->UpdateCamera(position);
}

bool Player::Intersect(const DrawableActor& other)
{
	// AABB(Axis Aligned Bounding Box).

// �� x��ǥ �ּ�/�ִ�.
	int min = position.x;
	int max = position.x + width;
	
	// �ٸ� ������ x��ǥ �ּ�/�ִ�.
	int otherMin = other.Position().x;
	int otherMax = other.Position().x + other.Width();

	// �ٸ� ������ ���� �� ��ġ�� �� ������ �� ��ġ�� ����� �浹 ����.
	if (otherMin > max)
	{
		return false;
	}

	// �ٸ� ������ ������ �� ��ġ�� �� ���� �� ��ġ���� ������ �浹 ����.
	if (otherMax < min)
	{
		return false;
	}

	// ���� �� ��찡 �ƴ϶�� (x��ǥ�� ���� ��ħ), y��ġ ��.
	return position.y == other.Position().y;

	
}

void Player::Jump()
{
	isJumping = true;
	jumpVelocity = -13; // �ʱ� ���� �ӵ�.
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
