#pragma once

#include <Level/Level.h>
#include <Math/Vector2.h>

// ���ڹ� ���� ����.
class DrawableActor;
class Player;
class Box;
class Target;
class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();

	// ������Ʈ �Լ�.
	virtual void Update(float deltaTime) override;

	// ������ �׸��� �Լ�.
	virtual void Draw() override;

	// �÷��̾ �̵��� ������ �� Ȯ���ϴ� �Լ�.
	bool CanPlayerMove(const Vector2& position);
	bool CanMonsterMove(const Vector2& position);

private:
	// �ڽ��� �ű� �� ������ Ŭ�����ߴ��� Ȯ���ϴ� �Լ�.
	bool CheckGameClear();

private:
	// ��/�� ���� �迭.
	List<DrawableActor*> map;

	// �ڽ� ����.
	List<Box*> boxes;

	// Ÿ�� ����.
	List<Target*> targets;

	// �÷��̾� ����.
	Player* player = nullptr;

	// ���� Ŭ���� ����.
	bool isGameClear = false;

public:
	void UpdateCamera(const Vector2& playerPosition);
private:
	Vector2 CameraPosition;
	int cameraStartX = 0; // ī�޶��� ���� ��ġ
	const int viewportWidth = 100; // ������ �ʺ�
	const int viewportHeight = 28; // ������ ����
};