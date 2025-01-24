#include "Enemy.h"
#include "Engine/Engine.h"
#include "Level/GameLevel.h"

Enemy::Enemy(const Vector2& position, GameLevel* level)
	:DrawableActor(""), refLevel(level)
{
	// 위치 설정.
	this->position = position;

	width = 3;
	// 색상 설정.
	color = Color::Red;
}

void Enemy::Update(float deltaTime)
{
	currentFrame++;
	if (currentFrame > 60)
		currentFrame = 0;


	if (breakTime > 0.01f)
	{
		breakTime -= deltaTime;
		if (breakTime <= 0.01f)
			breakTime = 0.0f;
		return;
	}
	//좌우 랜덤 이동(방향 설정되면 2~3초 이동)
	if (isWalking == false)
	{
		walkingnum = Random(1, 2);
		isWalking = true;
		timer = Random(2, 4);

	}

	if (isWalking == true)
	{
		timer -= deltaTime;

		if (timer < 0)
		{
			timer = 0.0f;
			isWalking = false;
			breakTime = 2.0f;
		}
	}


	if (isWalking == true && timer > 0.01f)
	{
		Move(walkingnum);
	}



}

void Enemy::Draw()
{

}

void Enemy::DrawAt(Vector2 InPosition)
{
	int frameAnimcount = 0;

	if (currentFrame <= 30 && currentFrame >= 0)
	{
		frameAnimcount = 0;

	}
	if (currentFrame <= 60 && currentFrame > 30)
	{
		frameAnimcount = 1;
	}

	

	if (isWalking)
	{
		for (int i = 0; i < 3; ++i)
		{
			Engine::Get().Draw(Vector2(InPosition.x, InPosition.y - 2 + i), enemyWalk[frameAnimcount][i], color);
		}
	}
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			Engine::Get().Draw(Vector2(InPosition.x, InPosition.y - 2 + i), enemy[frameAnimcount][i], color);
		}
	}
}

void Enemy::Move(int WalkVelocity)
{
	if (currentFrame == 20 || currentFrame == 40 || currentFrame == 60)
	{
		switch (WalkVelocity)
		{
		case 1://왼쪽
			if (refLevel->CanMonsterMove(Vector2(position.x - 1, position.y)) == true)
			{
				position.x -= 1;
			}
			break;

		case 2://오른쪽
		
			if (refLevel->CanMonsterMove(Vector2(position.x + 3, position.y)) == true)
			{
				position.x += 1;
			}
			break;

		case 3:
			break;

		default:
			break;
		}
	}
	




}
