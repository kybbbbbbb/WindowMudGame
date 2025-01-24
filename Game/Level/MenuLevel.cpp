#include "MenuLevel.h"
#include "Game/Game.h"
#include <fstream>
#include <sstream>

MenuLevel::MenuLevel()
{
	menuItems.PushBack(new MenuItem("                 Resume Game", []() { Game::Get().ToggleMenu(); }));
	menuItems.PushBack(new MenuItem("                 Quit Game", []() { Game::Get().QuitGame(); }));
	length = menuItems.Size();

	// 커서 감추기
	Engine::Get().SetCursorType(CursorType::NoCursor);

	// 맵 파일 불러와 레벨 로드
	LoadMapFromFile("../Assets/Maps/StartScene.txt");
}

MenuLevel::~MenuLevel()
{
	for (auto* item : menuItems)
	{
		delete item;
	}
}

void MenuLevel::Update(float deltaTime)
{
	if (Game::Get().GetKeyDown(VK_ESCAPE))
	{
		Game::Get().ToggleMenu();
	}

	if (Game::Get().GetKeyDown(VK_UP))
	{
		currentIndex = (currentIndex - 1 + length) % length;
	}
	if (Game::Get().GetKeyDown(VK_DOWN))
	{
		currentIndex = (currentIndex + 1) % length;
	}

	if (Game::Get().GetKeyDown(VK_RETURN))
	{
		menuItems[currentIndex]->onSelected();
	}
}

void MenuLevel::Draw()
{
	Super::Draw();

	// 메뉴 텍스트 출력
	Vector2 position(0, 0);

	position.y += 2;

	// 아스키 아트 출력 (mapData를 줄 단위로 출력)
	for (size_t i = 0; i < strlen(mapData); ++i)
	{
		if (mapData[i] == '\n')  // 줄 바꿈 처리
		{
			position.y++;
			position.x = 0;  // 왼쪽으로 이동
		}
		else
		{
			if (mapData[i] == '1')
				mapData[i] = ' ';
			Engine::Get().Draw(position, &mapData[i], unselectedColor);
			position.x++;  // 오른쪽으로 이동
		}
	}

	position.x = 0;
	position.y++;
	// 메뉴 항목 출력
	for (int ix = 0; ix < length; ++ix)
	{
		// 선택된 항목과 비선택 항목의 색상을 구분
		Color color = (ix == currentIndex) ? selectedColor : unselectedColor;

		// 메뉴 항목 텍스트 출력
		Engine::Get().Draw(position, menuItems[ix]->menuText, color);
		position.y++;
	}

	position.x = 0;
	position.y++;
	Engine::Get().Draw(position, "========================================================", unselectedColor);
}

void MenuLevel::LoadMapFromFile(const char* filePath)
{
	// 파일 열기
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "맵 파일 열기 실패.\n";
		return;
	}

	// 파일 크기 구하기
	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	// 파일 크기만큼 메모리 할당
	mapData = new char[size + 1];  // +1 for null terminator

	// 파일 읽기
	if (file.read(mapData, size))
	{
		mapData[size] = '\0';  // 널 종료 문자 추가
	}

	// 파일 닫기
	file.close();


}
