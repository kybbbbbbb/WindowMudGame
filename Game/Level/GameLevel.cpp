#include "GameLevel.h"
#include "Engine/Engine.h"

#include "Actor/Wall.h"
#include "Actor/Ground.h"
#include "Actor/Box.h"
#include "Actor/Target.h"
#include "Actor/Player.h"
#include <math.h>

#include "Engine/Timer.h"
#include "Actor/Enemy.h"

GameLevel::GameLevel()
{
	// Ŀ�� ���߱�.
	Engine::Get().SetCursorType(CursorType::NoCursor);

	// �� ���� �ҷ��� ���� �ε�.
	// ���� �б�.
	FILE* file = nullptr;
	fopen_s(&file, "../Assets/Maps/Map.txt", "rb");

	// ���� ó��.
	if (file == nullptr)
	{
		std::cout << "�� ���� ���� ����.\n";
		__debugbreak();
		return;
	}

	// ���� �б�.
	// ����ġ�� �̵�.
	fseek(file, 0, SEEK_END);

	// �̵��� ��ġ�� FP ��������.
	size_t readSize = ftell(file);

	// FP ����ġ.
	//fseek(file, 0, SEEK_SET);
	rewind(file);

	// ���� �о ���ۿ� ���.
	char* buffer = new char[readSize + 1];
	size_t bytesRead = fread(buffer, 1, readSize, file);

	if (readSize != bytesRead)
	{
		std::cout << "�о�� ũ�Ⱑ �ٸ�\n";
		__debugbreak();
		return;
	}

	buffer[readSize] = '\0';

	// ���� ���� �� ����� �ε���.
	int index = 0;

	// ��ǥ ����� ���� ���� ����.
	int xPosition = 0;
	int yPosition = 0;

	// �ؼ� (�Ľ�-Parcing).
	while (index < (int)bytesRead)
	{
		// �� ���ھ� �б�.
		char mapChar = buffer[index++];

		// ���� ������ ��� ó��.
		if (mapChar == '\n')
		{
			++yPosition;
			xPosition = 0;
			continue;
		}

		// �� ���ڰ� 1�̸� Wall ���� ����.
		if (mapChar == '1')
		{
			Wall* wall = new Wall(Vector2(xPosition, yPosition));
			actors.PushBack(wall);
			map.PushBack(wall);
		}

		// �� ���ڰ� .�̸� �׶��� ���� ����.
		else if (mapChar == '.')
		{
			Ground* ground = new Ground(Vector2(xPosition, yPosition));
			actors.PushBack(ground);
			map.PushBack(ground);
		}

		// �� ���ڰ� b�̸� �ڽ� ���� ����.
		else if (mapChar == 'b')
		{
			Ground* ground = new Ground(Vector2(xPosition, yPosition));
			actors.PushBack(ground);
			map.PushBack(ground);

			Box* box = new Box(Vector2(xPosition, yPosition));
			actors.PushBack(box);
			boxes.PushBack(box);
		}

		// �� ���ڰ� E�̸� Enemy ���� ����.
		else if (mapChar == 'E')
		{
			Ground* ground = new Ground(Vector2(xPosition, yPosition));
			actors.PushBack(ground);
			map.PushBack(ground);

			Enemy* target = new Enemy(Vector2(xPosition, yPosition),this);
			actors.PushBack(target);
			map.PushBack(target);
			
		}

		// �� ���ڰ� p�̸� �÷��̾� ���� ����.
		else if (mapChar == 'p')
		{
			Ground* ground = new Ground(Vector2(xPosition, yPosition));
			actors.PushBack(ground);
			map.PushBack(ground);

			player = new Player(
				Vector2(xPosition, yPosition),
				this
			);
			actors.PushBack(player);
		}

		++xPosition;
	}

	// ���� ����.
	delete[] buffer;

	// ���� �ݱ�.
	fclose(file);
}

void GameLevel::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// ������ Ŭ���������, ���� ���� ó��.
	if (isGameClear)
	{
		// �뷫 �� ������ ������ �ð� ���.
		//static float elapsedTime = 0.0f;
		//elapsedTime += deltaTime;
		//if (elapsedTime < 0.1f)
		//{
		//	return;
		//}

		// Ÿ�̸�.
		static Timer timer(0.1f);
		timer.Update(deltaTime);
		if (!timer.IsTimeOut())
		{
			return;
		}

		// Ŀ�� �̵�.
		Engine::Get().SetCursorPosition(0, Engine::Get().ScreenSize().y);

		// �޽��� ���.
		Log("Game Clear!");

		// ������ ����.
		Sleep(2000);

		// ���� ���� ó��.
		Engine::Get().QuitGame();
	}
}

//void GameLevel::Draw()
//{
//	// �� �׸���.
//	for (auto* actor : map)
//	{
//		// �÷��̾� ��ġ Ȯ��.
//		if (actor->Position() == player->Position())
//		{
//			continue;
//		}
//
//		// �ڽ� ��ġ Ȯ��.
//		bool shouldDraw = true;
//		for (auto* box : boxes)
//		{
//			if (actor->Position() == box->Position())
//			{
//				shouldDraw = false;
//				break;
//			}
//		}
//
//		// �� ���� �׸���.
//		if (shouldDraw)
//		{
//			actor->Draw();
//		}
//	}
//
//	// Ÿ�� �׸���.
//	for (auto* target : targets)
//	{
//		// �÷��̾� ��ġ Ȯ��.
//		if (target->Position() == player->Position())
//		{
//			continue;
//		}
//
//		// �ڽ� ��ġ Ȯ��.
//		bool shouldDraw = true;
//		for (auto* box : boxes)
//		{
//			if (target->Position() == box->Position())
//			{
//				shouldDraw = false;
//				break;
//			}
//		}
//
//		// Ÿ�� ���� �׸���.
//		if (shouldDraw)
//		{
//			target->Draw();
//		}
//	}
//
//	// �ڽ� �׸���.
//	for (auto* box : boxes)
//	{
//		box->Draw();
//	}
//
//	// �÷��̾� �׸���.
//	player->Draw();
//}

void GameLevel::Draw()
{
	List<Enemy*> enemies;
	for (auto* actor : map)
	{
		Enemy* enemy = actor->As<Enemy>();
		if (enemy)
		{
			enemies.PushBack(enemy);
		}
	}

	// ���� ī�޶� ������ ���� ���
	int startX = cameraStartX;
	int endX = cameraStartX + Engine::Get().ScreenSize().x;
	int startY = 0; 
	int endY = startY + Engine::Get().ScreenSize().y;

	// ��� ����(�÷��̾� ����) �׸���
	for (auto* actor : map)
	{
		Vector2 actorPos = actor->Position();

		// ī�޶� ���� ���� ���͸� �׸���
		if (actorPos.x >= startX && actorPos.x < endX &&
			actorPos.y >= startY && actorPos.y < endY)
		{
			bool isOverlapping = false;

			// �ڽ��� ��ġ �ߺ� üũ
			for (auto* box : boxes)
			{
				if (actorPos == box->Position())
				{
					isOverlapping = true;
					break;
				}
			}

			// �÷��̾�� ��ġ �ߺ� üũ
			if (actorPos == player->Position())
			{
				isOverlapping = true;
			}

			// �ߺ��� ������ �׸���
			if (!isOverlapping)
			{
				// ȭ�� ��ǥ�� ��ȯ�Ͽ� �׸���
				int screenX = actorPos.x - cameraStartX;
				int screenY = actorPos.y ;
				actor->DrawAt(Vector2(screenX, screenY)); 
				
			}
		}
	}

	// ���� �׸���.
	for (Enemy* enemy : enemies)
	{
		auto position = enemy->Position();
		int screenX = position.x - cameraStartX;
		int screenY = position.y;
		enemy->DrawAt(Vector2(screenX, screenY));
	}


	// �÷��̾� �׸���
	const Vector2& playerPos = player->Position();
	int screenX = playerPos.x - cameraStartX;
	int screenY = playerPos.y;
	player->DrawAt(Vector2(screenX, screenY));  // �÷��̾��� ȭ�� ��ǥ���� �׸���
}


bool GameLevel::CanPlayerMove(const Vector2& position)
{
	// ������ Ŭ����� ��� �ٷ� ����.
	if (isGameClear)
	{
		return false;
	}


	// �̵��Ϸ��� ��ġ�� ���� �ִ��� Ȯ��.
	DrawableActor* searchedActor = nullptr;

	// ���� ã��.
	// ���͸� ��ã������ ������ ���� ���Ϳ��� �̵� �Ұ����� ���� ó��.

	// ���� �̵��Ϸ��� ��ġ�� ���� ã��.
	for (auto* actor : map)
	{

		if (actor->Position() == position)
		{
			searchedActor = actor;
			break;
		}
	}

	if (!searchedActor)
	{
		return false;
	}

	if (searchedActor->As<Enemy>())
	{
		return false;
	}

	// �˻��� ���Ͱ� ������ Ȯ��.
	if (searchedActor->As<Wall>())
	{
		return false;
	}

	// �˻��� ���Ͱ� �̵� ������ ����(��/Ÿ��)���� Ȯ��.
	if (searchedActor->As<Ground>()
		|| searchedActor->As<Target>())
	{
		return true;
	}

	for (auto* actor : map)
	{
		if ((player->Intersect(*actor)) == true)
		{
			return true;
		}
	}

	return false;
}



bool GameLevel::CanMonsterMove(const Vector2& position)
{
	// ������ Ŭ����� ��� �ٷ� ����.
	if (isGameClear)
	{
		return false;
	}

	DrawableActor* searchedActor = nullptr;
	// ���� �̵��Ϸ��� ��ġ�� ���� ã��.
	for (auto* actor : map)
	{
		if (actor->Position() == position)
		{
			searchedActor = actor;
			break;
		}
	}

	// �˻��� ���Ͱ� ������ Ȯ��.
	if (searchedActor->As<Wall>())
	{
		char buffer[256];
		sprintf_s(buffer, 256, "Enemy Detect Wall!\n");
		OutputDebugStringA(buffer);
		return false;
	}

	// �˻��� ���Ͱ� �̵� ������ ����(��/Ÿ��)���� Ȯ��.
	if (searchedActor->As<Ground>()
		|| searchedActor->As<Target>())
	{
		return true;
	}

	char buffer[256];
	sprintf_s(buffer, 256, "Enemy Detect Nothing!\n");
	OutputDebugStringA(buffer);
	return false;
}

bool GameLevel::CheckGameClear()
{
	// ���� Ȯ���� ���� ����.
	int currentScore = 0;
	int targetScore = targets.Size();

	// Ÿ�� ��ġ�� ��ġ�� �ڽ� ���� ����.
	for (auto* box : boxes)
	{
		for (auto* target : targets)
		{
			// ���� Ȯ��.
			if (box->Position() == target->Position())
			{
				++currentScore;
				break;
			}
		}
	}

	// ȹ���� ������ ��ǥ ������ ������ ��.
	return currentScore == targetScore;
}

void GameLevel::UpdateCamera(const Vector2& playerPosition)
{
	if (playerPosition.x > cameraStartX + viewportWidth - 30)
	{
		cameraStartX = min(cameraStartX + 1, 1000 - viewportWidth);
	}
	// �÷��̾ ȭ���� ���� ������ 10ĭ �̳��� �ٰ����� ī�޶� �̵�
	else if (playerPosition.x < cameraStartX + 30)
	{
		cameraStartX = max(cameraStartX - 1, 0);
	}

}
