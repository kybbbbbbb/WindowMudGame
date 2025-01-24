#pragma once

#include <Actor/DrawableActor.h>

// 게임 레벨 전방 선언.
class GameLevel;
class Player : public DrawableActor
{
	RTTI_DECLARATIONS(Player, DrawableActor)

public:
	Player(const Vector2& position, GameLevel* level);

	virtual void Update(float deltaTime) override;
	virtual bool Intersect(const DrawableActor& other) override;

private:
	// 게임 레벨을 참조하는 변수.
	GameLevel* refLevel = nullptr;

private:
	bool isJumping = false;
	int jumpVelocity = 0;
	int gravity = 10;
	float groundY;
	void Jump();

public:
	virtual void Draw() override;
	virtual void DrawAt(Vector2 InPosition) override;

private:
	const char* stickman[4] = {
	" O ",
	"/|\\",
	" | ",
	"/ \\"
	};

	int currentFrame = 0; // 현재 모션 프레임
	float frameTime = 0.1f; // 프레임 교체 간격
	float frameTimer = 0.0f; // 현재 프레임 시간 추적
	bool isWalking = false;    // 걷는 상태
	bool isWalkingLeft = true; // 걷는 방향

	const char* stickmanWalkingLeft[2][4] = {
		{ " O ", "/|\\", " | ", "/  " }, // 왼쪽 프레임 1
		{ " O ", "/|\\", " | ", "/ \\" }  // 왼쪽 프레임 2
	};

	const char* stickmanWalkingRight[2][4] = {
		{ " O ", " |\\", " | ", "  \\" }, // 오른쪽 프레임 1
		{ " O ", " /|\\", " | ", "/ \\" } // 오른쪽 프레임 2
	};


};