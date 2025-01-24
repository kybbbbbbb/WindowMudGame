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
	// 커서 감추기.
	Engine::Get().SetCursorType(CursorType::NoCursor);

	// 맵 파일 불러와 레벨 로드.
	// 파일 읽기.
	FILE* file = nullptr;
	fopen_s(&file, "../Assets/Maps/Map.txt", "rb");

	// 파일 처리.
	if (file == nullptr)
	{
		std::cout << "맵 파일 열기 실패.\n";
		__debugbreak();
		return;
	}

	// 파일 읽기.
	// 끝위치로 이동.
	fseek(file, 0, SEEK_END);

	// 이동한 위치의 FP 가져오기.
	size_t readSize = ftell(file);

	// FP 원위치.
	//fseek(file, 0, SEEK_SET);
	rewind(file);

	// 파일 읽어서 버퍼에 담기.
	char* buffer = new char[readSize + 1];
	size_t bytesRead = fread(buffer, 1, readSize, file);

	if (readSize != bytesRead)
	{
		std::cout << "읽어온 크기가 다름\n";
		__debugbreak();
		return;
	}

	buffer[readSize] = '\0';

	// 파일 읽을 때 사용할 인덱스.
	int index = 0;

	// 좌표 계산을 위한 변수 선언.
	int xPosition = 0;
	int yPosition = 0;

	// 해석 (파싱-Parcing).
	while (index < (int)bytesRead)
	{
		// 한 문자씩 읽기.
		char mapChar = buffer[index++];

		// 개행 문자인 경우 처리.
		if (mapChar == '\n')
		{
			++yPosition;
			xPosition = 0;
			continue;
		}

		// 맵 문자가 1이면 Wall 액터 생성.
		if (mapChar == '1')
		{
			Wall* wall = new Wall(Vector2(xPosition, yPosition));
			actors.PushBack(wall);
			map.PushBack(wall);
		}

		// 맵 문자가 .이면 그라운드 액터 생성.
		else if (mapChar == '.')
		{
			Ground* ground = new Ground(Vector2(xPosition, yPosition));
			actors.PushBack(ground);
			map.PushBack(ground);
		}

		// 맵 문자가 b이면 박스 액터 생성.
		else if (mapChar == 'b')
		{
			Ground* ground = new Ground(Vector2(xPosition, yPosition));
			actors.PushBack(ground);
			map.PushBack(ground);

			Box* box = new Box(Vector2(xPosition, yPosition));
			actors.PushBack(box);
			boxes.PushBack(box);
		}

		// 맵 문자가 E이면 Enemy 액터 생성.
		else if (mapChar == 'E')
		{
			Ground* ground = new Ground(Vector2(xPosition, yPosition));
			actors.PushBack(ground);
			map.PushBack(ground);

			Enemy* target = new Enemy(Vector2(xPosition, yPosition),this);
			actors.PushBack(target);
			map.PushBack(target);
			
		}

		// 맵 문자가 p이면 플레이어 액터 생성.
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

	// 버퍼 삭제.
	delete[] buffer;

	// 파일 닫기.
	fclose(file);
}

void GameLevel::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// 게임이 클리어됐으면, 게임 종료 처리.
	if (isGameClear)
	{
		// 대략 한 프레임 정도의 시간 대기.
		//static float elapsedTime = 0.0f;
		//elapsedTime += deltaTime;
		//if (elapsedTime < 0.1f)
		//{
		//	return;
		//}

		// 타이머.
		static Timer timer(0.1f);
		timer.Update(deltaTime);
		if (!timer.IsTimeOut())
		{
			return;
		}

		// 커서 이동.
		Engine::Get().SetCursorPosition(0, Engine::Get().ScreenSize().y);

		// 메시지 출력.
		Log("Game Clear!");

		// 쓰레드 정지.
		Sleep(2000);

		// 게임 종료 처리.
		Engine::Get().QuitGame();
	}
}

//void GameLevel::Draw()
//{
//	// 맵 그리기.
//	for (auto* actor : map)
//	{
//		// 플레이어 위치 확인.
//		if (actor->Position() == player->Position())
//		{
//			continue;
//		}
//
//		// 박스 위치 확인.
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
//		// 맵 액터 그리기.
//		if (shouldDraw)
//		{
//			actor->Draw();
//		}
//	}
//
//	// 타겟 그리기.
//	for (auto* target : targets)
//	{
//		// 플레이어 위치 확인.
//		if (target->Position() == player->Position())
//		{
//			continue;
//		}
//
//		// 박스 위치 확인.
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
//		// 타겟 액터 그리기.
//		if (shouldDraw)
//		{
//			target->Draw();
//		}
//	}
//
//	// 박스 그리기.
//	for (auto* box : boxes)
//	{
//		box->Draw();
//	}
//
//	// 플레이어 그리기.
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

	// 현재 카메라가 보여줄 영역 계산
	int startX = cameraStartX;
	int endX = cameraStartX + Engine::Get().ScreenSize().x;
	int startY = 0; 
	int endY = startY + Engine::Get().ScreenSize().y;

	// 모든 액터(플레이어 제외) 그리기
	for (auto* actor : map)
	{
		Vector2 actorPos = actor->Position();

		// 카메라 범위 내의 액터만 그리기
		if (actorPos.x >= startX && actorPos.x < endX &&
			actorPos.y >= startY && actorPos.y < endY)
		{
			bool isOverlapping = false;

			// 박스와 위치 중복 체크
			for (auto* box : boxes)
			{
				if (actorPos == box->Position())
				{
					isOverlapping = true;
					break;
				}
			}

			// 플레이어와 위치 중복 체크
			if (actorPos == player->Position())
			{
				isOverlapping = true;
			}

			// 중복이 없으면 그리기
			if (!isOverlapping)
			{
				// 화면 좌표로 변환하여 그리기
				int screenX = actorPos.x - cameraStartX;
				int screenY = actorPos.y ;
				actor->DrawAt(Vector2(screenX, screenY)); 
				
			}
		}
	}

	// 몬스터 그리기.
	for (Enemy* enemy : enemies)
	{
		auto position = enemy->Position();
		int screenX = position.x - cameraStartX;
		int screenY = position.y;
		enemy->DrawAt(Vector2(screenX, screenY));
	}


	// 플레이어 그리기
	const Vector2& playerPos = player->Position();
	int screenX = playerPos.x - cameraStartX;
	int screenY = playerPos.y;
	player->DrawAt(Vector2(screenX, screenY));  // 플레이어의 화면 좌표에서 그리기
}


bool GameLevel::CanPlayerMove(const Vector2& position)
{
	// 게임이 클리어된 경우 바로 종료.
	if (isGameClear)
	{
		return false;
	}


	// 이동하려는 위치에 벽이 있는지 확인.
	DrawableActor* searchedActor = nullptr;

	// 몬스터 찾기.
	// 몬스터를 못찾았으면 나머지 레벨 액터에서 이동 불가능한 액터 처리.

	// 먼저 이동하려는 위치의 액터 찾기.
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

	// 검색한 액터가 벽인지 확인.
	if (searchedActor->As<Wall>())
	{
		return false;
	}

	// 검색한 액터가 이동 가능한 액터(땅/타겟)인지 확인.
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
	// 게임이 클리어된 경우 바로 종료.
	if (isGameClear)
	{
		return false;
	}

	DrawableActor* searchedActor = nullptr;
	// 먼저 이동하려는 위치의 액터 찾기.
	for (auto* actor : map)
	{
		if (actor->Position() == position)
		{
			searchedActor = actor;
			break;
		}
	}

	// 검색한 액터가 벽인지 확인.
	if (searchedActor->As<Wall>())
	{
		char buffer[256];
		sprintf_s(buffer, 256, "Enemy Detect Wall!\n");
		OutputDebugStringA(buffer);
		return false;
	}

	// 검색한 액터가 이동 가능한 액터(땅/타겟)인지 확인.
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
	// 점수 확인을 위한 변수.
	int currentScore = 0;
	int targetScore = targets.Size();

	// 타겟 위치에 배치된 박스 개수 세기.
	for (auto* box : boxes)
	{
		for (auto* target : targets)
		{
			// 점수 확인.
			if (box->Position() == target->Position())
			{
				++currentScore;
				break;
			}
		}
	}

	// 획득한 점수가 목표 점수와 같은지 비교.
	return currentScore == targetScore;
}

void GameLevel::UpdateCamera(const Vector2& playerPosition)
{
	if (playerPosition.x > cameraStartX + viewportWidth - 30)
	{
		cameraStartX = min(cameraStartX + 1, 1000 - viewportWidth);
	}
	// 플레이어가 화면의 왼쪽 끝에서 10칸 이내로 다가오면 카메라 이동
	else if (playerPosition.x < cameraStartX + 30)
	{
		cameraStartX = max(cameraStartX - 1, 0);
	}

}
