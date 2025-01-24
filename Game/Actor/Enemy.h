#pragma once

#include <Actor/DrawableActor.h>


class GameLevel;
class Enemy : public DrawableActor
{
	RTTI_DECLARATIONS(Enemy, DrawableActor)

public:
	Enemy(const Vector2& position, GameLevel* level);

	virtual void Update(float deltaTime) override;

private:
	// 게임 레벨을 참조하는 변수.
	GameLevel* refLevel = nullptr;

private:
	bool isJumping = false;
	int jumpVelocity = 0;
	int gravity = 10;
	float groundY;

public:
	virtual void Draw() override;
	virtual void DrawAt(Vector2 InPosition) override;
	void Move(int WalkVelocity);

private:

	const char* enemy[2][3] = {
		{" i_i","l[ ]l","[]"},
		{" i_i","l[ ]l","[]"}
	};

	const char* enemyWalk[2][3] = {
		{"i__i","l[ ]l"," | |"},
		{"i__i","-[ ]l"," < |" }
	};

	int currentFrame = 0; // 현재 모션 프레임

	float frameTime = 0.1f; // 프레임 교체 간격
	float frameTimer = 0.0f; // 현재 프레임 시간 추적
	bool isWalking = false;    // 걷는 상태
	bool isWalkingLeft = true; // 걷는 방향


	float timer = 0.0f;
	int walkingnum = 0;
	int speed = 5;
	float breakTime = 0.0f;





};