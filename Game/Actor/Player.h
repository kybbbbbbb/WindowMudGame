#pragma once

#include <Actor/DrawableActor.h>

// ���� ���� ���� ����.
class GameLevel;
class Player : public DrawableActor
{
	RTTI_DECLARATIONS(Player, DrawableActor)

public:
	Player(const Vector2& position, GameLevel* level);

	virtual void Update(float deltaTime) override;
	virtual bool Intersect(const DrawableActor& other) override;

private:
	// ���� ������ �����ϴ� ����.
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

	int currentFrame = 0; // ���� ��� ������
	float frameTime = 0.1f; // ������ ��ü ����
	float frameTimer = 0.0f; // ���� ������ �ð� ����
	bool isWalking = false;    // �ȴ� ����
	bool isWalkingLeft = true; // �ȴ� ����

	const char* stickmanWalkingLeft[2][4] = {
		{ " O ", "/|\\", " | ", "/  " }, // ���� ������ 1
		{ " O ", "/|\\", " | ", "/ \\" }  // ���� ������ 2
	};

	const char* stickmanWalkingRight[2][4] = {
		{ " O ", " |\\", " | ", "  \\" }, // ������ ������ 1
		{ " O ", " /|\\", " | ", "/ \\" } // ������ ������ 2
	};


};