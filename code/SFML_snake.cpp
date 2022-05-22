#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <time.h>
#include <sstream>
#include "sfml_snake.h"
#include <fstream>
using namespace sf;
using namespace std;

#define WINDOW_WIDTH	21
#define WINDOW_HEIGHT	21
#define STAGE_WIDTH		20
#define STAGE_HEIGHT	20
#define GRIDSIZE		25
#define SCALE			0.5
#define MAXLENGTH		100
#define INFO_WIDTH		400
#define SAVE_LIMIT		5

bool gameOver, gamePause, gameQuit, gameSuccess, gameFail, ingameMenu, qSaved, startupMenu, saveMenu, loadMenu;
const int Width = STAGE_WIDTH;
const int Height = STAGE_HEIGHT;
int fruitX, fruitY, score, headX, headY;
int bodyX[MAXLENGTH], bodyY[MAXLENGTH], nBody;
int diff;
enum eDirection { STOP = -1, UP = 180, DOWN = 0, LEFT = -90, RIGHT = 90 };
eDirection Dir, headrotation;

int _qS_fruitX, _qS_fruitY, _qS_score, _qS_headX, _qS_headY, _qS_bodyX[MAXLENGTH], _qS_bodyY[MAXLENGTH], _qS_nBody, _qS_diff;
eDirection _qS_Dir, _qS_headrotation;

Texture tBackground, tSnakeHead, tSnakeBody, tFruit, tMenuHeader;
Sprite spBackground, spSnakeHead, spSnakeBody, spFruit, spMenuHeader;
Font font;
Text text;
SoundBuffer sbEat, sbDie;
Sound soundEat, soundDie;
Music bgMusic;
int soundVolume;
bool MusicOn;


RenderWindow window(VideoMode(WINDOW_WIDTH*GRIDSIZE + INFO_WIDTH, WINDOW_HEIGHT*GRIDSIZE), L"贪吃蛇", Style::Default);

void Init() {
	
	window.setFramerateLimit(10);

	// 字体
	if (!font.loadFromFile("data/Fonts/msyhl.ttc")) {
		cout << "字体没有找到" << endl;
	}
	text.setFont(font);

	// 贴图
	if (!tBackground.loadFromFile("data/images/BK.png")) {
		cout << "背景素材没有找到" << endl;
	}
	if (!tSnakeHead.loadFromFile("data/images/sh01.png")) {
		cout << "蛇头素材没有找到" << endl;
	}
	if (!tSnakeBody.loadFromFile("data/images/sb0102.png")) {
		cout << "蛇身素材没有找到" << endl;
	}
	if (!tFruit.loadFromFile("data/images/sb0202.png")) {
		cout << "水果素材没有找到" << endl;
	}
	if (!tMenuHeader.loadFromFile("data/images/menuheader.png")) {
		cout << "菜单头图没有找到" << endl;
	}
	// 音频
	if (!sbEat.loadFromFile("data/Audios/Eat01.ogg")) {
		cout << "进食音效没有找到" << endl;
	}
	if (!sbDie.loadFromFile("data/Audios/Die01.ogg")) {
		cout << "死亡音效没有找到" << endl;
	}
	if (!bgMusic.openFromFile("data/Audios/BGM01.ogg")) {
		cout << "背景音乐没有找到" << endl;
	}

	spBackground.setTexture(tBackground);
	spSnakeHead.setTexture(tSnakeHead);
	spSnakeBody.setTexture(tSnakeBody);
	spFruit.setTexture(tFruit);
	spMenuHeader.setTexture(tMenuHeader);

	spBackground.setOrigin(GRIDSIZE / SCALE / 2, GRIDSIZE / SCALE / 2);
	spSnakeHead.setOrigin(GRIDSIZE / SCALE / 2, GRIDSIZE / SCALE / 2);
	spSnakeBody.setOrigin(GRIDSIZE / SCALE / 2 + 2, GRIDSIZE / SCALE / 2 + 2);
	spFruit.setOrigin(GRIDSIZE / SCALE / 2, GRIDSIZE / SCALE / 2);
	spMenuHeader.setOrigin(GRIDSIZE / SCALE / 2, GRIDSIZE / SCALE / 2);

	spBackground.setScale(SCALE, SCALE);
	spSnakeHead.setScale(SCALE, SCALE);
	spSnakeBody.setScale(SCALE, SCALE);
	spFruit.setScale(SCALE, SCALE);

	soundVolume = 50;
	MusicOn = true;

	soundEat.setBuffer(sbEat);
	soundDie.setBuffer(sbDie);
	bgMusic.play();
	bgMusic.setVolume(soundVolume);
	bgMusic.setLoop(true);


	long seed = time(NULL);
	srand(seed);

	gameOver = false;
	gamePause = false;
	gameQuit = false;
	gameSuccess = false;
	gameFail = false;
	ingameMenu = false;
	qSaved = false;
	startupMenu = true;
	saveMenu = false;
	loadMenu = false;
	diff = 1;

	Dir = STOP;
	headrotation = STOP;
	score = 0;

	headX = Width / 2;
	headY = Height / 2;

	fruitX = rand() % Width;
	fruitY = rand() % Height;

	memset(bodyX, 0, MAXLENGTH * sizeof(int));
	memset(bodyY, 0, MAXLENGTH * sizeof(int));
	bodyX[0] = headX;
	bodyY[0] = headY;
	nBody = 1;

}

void Ingame_input() {

	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			gameOver = true;
			window.close();
		}
	}

	// Movement
	bool turned = false;
	if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
		if (Dir != DOWN && !turned && !gamePause && !ingameMenu && !saveMenu && !loadMenu) {
			Dir = UP;
			turned = true;
		}
	if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
		if (Dir != UP && !turned && !gamePause && !ingameMenu && !saveMenu && !loadMenu) {
			Dir = DOWN;
			turned = true;
		}
	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
		if (Dir != RIGHT && !turned && !gamePause && !ingameMenu && !saveMenu && !loadMenu) {
			Dir = LEFT;
			turned = true;
		}
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
		if (Dir != LEFT && !turned && !gamePause && !ingameMenu && !saveMenu && !loadMenu) {
			Dir = RIGHT;
			turned = true;
		}


	// Function
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		if (!ingameMenu && !loadMenu && !saveMenu) {
			if (gamePause)
				gamePause = false;
			else if (!gamePause)
				gamePause = true;
			while (Keyboard::isKeyPressed(Keyboard::Space));
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		if (gamePause)
			gamePause = false;
		if (saveMenu)
			saveMenu = false; 
		if (loadMenu)
			loadMenu = false;
		if (ingameMenu)
			ingameMenu = false;
		else if(!ingameMenu)
			ingameMenu = true;
		while (Keyboard::isKeyPressed(Keyboard::Escape));
	}
	if (Keyboard::isKeyPressed(Keyboard::Q)) {
		if (!ingameMenu && !gamePause) {
			qSave();
			qSaved = true;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::E)) {
		if (!ingameMenu && !gamePause && qSaved) {
			qLoad();
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::R)) {
		if (ingameMenu) {
			Init();
			startupMenu = false;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		if (ingameMenu) {
			ingameMenu = false;
			saveMenu = true;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::L)) {
		if (ingameMenu) {
			ingameMenu = false;
			loadMenu = true;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::X)) {
		if (ingameMenu) {
			window.close();
			gameOver = true;
			gameQuit = true;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::B)) {
		if (ingameMenu) {
			gameOver = true;
			startupMenu = true;
		}
	}	
	if (event.type == sf::Event::EventType::KeyReleased && event.key.code == Keyboard::M) {
		if (MusicOn) {
			MusicOn = false;
			bgMusic.pause();
		}
		else {
			MusicOn = true;
			bgMusic.play();
		}
		
	}
	if (event.type == sf::Event::EventType::KeyReleased && event.key.code == Keyboard::Add) {
		soundVolume += 5;
		if (soundVolume > 100)
			soundVolume = 100;
		bgMusic.setVolume(soundVolume);
	}
	if (event.type == sf::Event::EventType::KeyReleased && event.key.code == Keyboard::Subtract) {
		soundVolume -= 5;
		if (soundVolume < 0)
			soundVolume = 0;
		bgMusic.setVolume(soundVolume);
	}

	// Save&Load
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		SaveAndLoad(0);
	}
	if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		SaveAndLoad(1);
	}
	if (Keyboard::isKeyPressed(Keyboard::Num3)) {
		SaveAndLoad(2);
	}
	if (Keyboard::isKeyPressed(Keyboard::Num4)) {
		SaveAndLoad(3);
	}
	if (Keyboard::isKeyPressed(Keyboard::Num5)) {
		SaveAndLoad(4);
	}

}

void Prompt_info(int _x, int _y) {

	int initialX = 0, initialY = 0;
	int chsize = 24;
	text.setCharacterSize(chsize);
	text.setFillColor(Color(255, 255, 255, 255));
	text.setStyle(Text::Bold);
	text.setOrigin(0, 0);

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"■ 游戏说明："); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"   ●.蛇身自撞，游戏结束"); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"   ●.蛇可穿墙"); window.draw(text);
	initialY += 2*chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"■ 操作说明："); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"   □ 移动："); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"          ↑            W"); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"      ← ↓ →      A S D"); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"   □ 菜单：ESC"); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"   □ 快速保存：Q"); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"   □ 快速读档：E"); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"   □ 开关音乐：M"); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"   □ 调节音乐音量：+/-"); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"   □ 暂停 / 继续：SPACE"); window.draw(text);
	initialY += 2*chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"■ 20011721庞嘉骏"); window.draw(text);
	initialY += 2*chsize;
	text.setPosition(_x + initialX, _y + initialY);
	std::stringstream _diff;
	_diff << diff;
	text.setString(L"   ☆ 当前难度: " + _diff.str() + L" 级"); window.draw(text);
	initialY += chsize; 
	text.setPosition(_x + initialX, _y + initialY);
	stringstream _score;
	_score << score;
	text.setString(L"   ☆ 当前分数: " + _score.str()); window.draw(text);
	initialY += chsize;

}

void gameFail_info(int _x, int _y) {

	RectangleShape _mask(Vector2f(WINDOW_WIDTH * GRIDSIZE, WINDOW_HEIGHT * GRIDSIZE));
	_mask.setFillColor(Color(0, 0, 128, 128));
	window.draw(_mask);

	if (!loadMenu) {

		RectangleShape _bg(Vector2f(10 * GRIDSIZE, 9 * GRIDSIZE));
		_bg.setFillColor(Color(128, 128, 128, 128));
		_bg.setOutlineThickness(2);
		_bg.setOutlineColor(Color(200, 200, 200, 255));
		_bg.setOrigin(5 * GRIDSIZE, 4.5 * GRIDSIZE + 10);
		_bg.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2);
		window.draw(_bg);

		FloatRect rect;
		int initialY = 0;
		int chsize = 18;
		text.setFillColor(Color(255, 255, 255, 255));
		text.setOutlineThickness(2);
		text.setOutlineColor(Color(0, 0, 0, 128));
		text.setStyle(Text::Bold);

		text.setCharacterSize(36);
		text.setString(L"游戏失败！");
		rect = text.getLocalBounds();
		text.setOrigin(rect.width / 2, rect.height / 2);
		text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE);
		window.draw(text);
		initialY += 48;

		text.setCharacterSize(chsize);
		text.setString(L"[ESC]返回菜单");
		rect = text.getLocalBounds();
		text.setOrigin(rect.width / 2, rect.height / 2);
		text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE + initialY);
		window.draw(text);
		initialY += chsize + 10;

		text.setCharacterSize(chsize);
		text.setString(L"[R]重新开始");
		rect = text.getLocalBounds();
		text.setOrigin(rect.width / 2, rect.height / 2);
		text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE + initialY);
		window.draw(text);
		initialY += chsize + 10;

		text.setCharacterSize(chsize);
		text.setString(L"[E]快速读档");
		rect = text.getLocalBounds();
		text.setOrigin(rect.width / 2, rect.height / 2);
		text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE + initialY);
		window.draw(text);
		initialY += chsize + 10;

		text.setCharacterSize(chsize);
		text.setString(L"[L]读档");
		rect = text.getLocalBounds();
		text.setOrigin(rect.width / 2, rect.height / 2);
		text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE + initialY);
		window.draw(text);
		initialY += chsize + 10;

		text.setCharacterSize(chsize);
		text.setString(L"[X]退出");
		rect = text.getLocalBounds();
		text.setOrigin(rect.width / 2, rect.height / 2);
		text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE + initialY);
		window.draw(text);
		initialY += chsize + 10;

	}
	else if (loadMenu) {

		SaveAndLoad_info();

	}

}

void gameSuccess_info(int _x, int _y) {

	window.display();

	RectangleShape _mask(Vector2f(WINDOW_WIDTH * GRIDSIZE + INFO_WIDTH, WINDOW_HEIGHT * GRIDSIZE));
	_mask.setFillColor(Color(0, 0, 128, 128));
	window.draw(_mask);

	int initialX = 0, initialY = 0;
	int chsize = 24;
	text.setFillColor(Color(255, 255, 255, 255));
	text.setOutlineThickness(2);
	text.setOutlineColor(Color(0, 0, 0, 128));
	text.setStyle(Text::Bold);

	text.setPosition(_x + initialX, _y + initialY);
	text.setCharacterSize(chsize + 12);
	text.setString(L"游戏胜利！"); window.draw(text);
	initialY += 2 * chsize;

	text.setCharacterSize(chsize);
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"[Y]重新开始 / [N]退出"); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"[R]从存档处开始"); window.draw(text);
	initialY += chsize;
	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"[ESC]返回菜单"); window.draw(text);
	initialY += chsize;

	window.display();

}

void ingameMenu_info() {

	RectangleShape _mask(Vector2f(WINDOW_WIDTH * GRIDSIZE, WINDOW_HEIGHT * GRIDSIZE));
	_mask.setFillColor(Color(0, 0, 128, 128));
	window.draw(_mask);

	RectangleShape _bg(Vector2f(10 * GRIDSIZE, 10 * GRIDSIZE));
	_bg.setFillColor(Color(128, 128, 128, 128));
	_bg.setOutlineThickness(2);
	_bg.setOutlineColor(Color(200, 200, 200, 255));
	_bg.setOrigin(5 * GRIDSIZE, 5 * GRIDSIZE);
	_bg.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2);
	window.draw(_bg);

	FloatRect rect;
	int initialY = 0;
	int chsize = 18;
	text.setFillColor(Color(255, 255, 255, 255));
	text.setOutlineThickness(2);
	text.setOutlineColor(Color(0, 0, 0, 128));
	text.setStyle(Text::Bold);

	text.setCharacterSize(36);
	text.setString(L"菜单");
	rect = text.getLocalBounds();
	text.setOrigin(rect.width / 2, rect.height / 2);
	text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE);
	window.draw(text);
	initialY += 48;

	text.setCharacterSize(chsize);
	text.setString(L"[ESC]返回游戏");
	rect = text.getLocalBounds();
	text.setOrigin(rect.width / 2, rect.height / 2);
	text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE + initialY);
	window.draw(text);
	initialY += chsize + 5;

	text.setString(L"[R]重新开始");
	rect = text.getLocalBounds();
	text.setOrigin(rect.width / 2, rect.height / 2);
	text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE + initialY);
	window.draw(text);
	initialY += chsize + 5;

	text.setString(L"[S]存档");
	rect = text.getLocalBounds();
	text.setOrigin(rect.width / 2, rect.height / 2);
	text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE + initialY);
	window.draw(text);
	initialY += chsize + 5;

	text.setString(L"[L]读档");
	rect = text.getLocalBounds();
	text.setOrigin(rect.width / 2, rect.height / 2);
	text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE + initialY);
	window.draw(text);
	initialY += chsize + 5;

	text.setString(L"[O]选项");
	rect = text.getLocalBounds();
	text.setOrigin(rect.width / 2, rect.height / 2);
	text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE + initialY);
	window.draw(text);
	initialY += chsize + 5;

	text.setString(L"[B]返回游戏标题");
	rect = text.getLocalBounds();
	text.setOrigin(rect.width / 2, rect.height / 2);
	text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE + initialY);
	window.draw(text);
	initialY += chsize + 5;

	text.setString(L"[X]退出游戏");
	rect = text.getLocalBounds();
	text.setOrigin(rect.width / 2, rect.height / 2);
	text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 3.5 * GRIDSIZE + initialY);
	window.draw(text);
	initialY += chsize + 5;

}

void gamePause_info() {

	RectangleShape _mask(Vector2f(WINDOW_WIDTH * GRIDSIZE, WINDOW_HEIGHT * GRIDSIZE));
	_mask.setFillColor(Color(0, 0, 128, 128));
	window.draw(_mask);

	RectangleShape _bg(Vector2f(10 * GRIDSIZE, 4.5 * GRIDSIZE));
	_bg.setFillColor(Color(128, 128, 128, 128));
	_bg.setOutlineThickness(2);
	_bg.setOutlineColor(Color(200, 200, 200, 255));
	_bg.setOrigin(5 * GRIDSIZE, 2.25 * GRIDSIZE);
	_bg.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - 5);
	window.draw(_bg);

	FloatRect rect;
	int initialY = 0;
	int chsize = 18;
	text.setFillColor(Color(255, 255, 255, 255));
	text.setOutlineThickness(2);
	text.setOutlineColor(Color(0, 0, 0, 128));
	text.setStyle(Text::Bold);

	text.setCharacterSize(36);
	text.setString(L"暂停中");
	rect = text.getLocalBounds();
	text.setOrigin(rect.width / 2, rect.height / 2);
	text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - GRIDSIZE);
	window.draw(text);
	initialY += 48;

	text.setCharacterSize(chsize);
	text.setString(L"[SPACE]继续游戏");
	rect = text.getLocalBounds();
	text.setOrigin(rect.width / 2, rect.height / 2);
	text.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2 - GRIDSIZE + initialY);
	window.draw(text);
	initialY += chsize;

}

void Logic() {

	if (!gamePause && !ingameMenu && !saveMenu && !loadMenu) {

		if (headX == fruitX && headY == fruitY) {

			soundEat.play();
			score += 10;
			fruitX = rand() % Width;
			fruitY = rand() % Height;
			nBody++;

			diff = score / 50 + 1;

		}
		for (int i = 1; i < nBody; i++)
			if (bodyX[i] == fruitX && bodyY[i] == fruitY) {
				fruitX = rand() % Width;
				fruitY = rand() % Height;
			}

		int prevX = bodyX[0], prevY = bodyY[0];
		int prev2X, prev2Y;
		bodyX[0] = headX;
		bodyY[0] = headY;
		for (int i = 1; i < nBody; i++) {
			prev2X = bodyX[i];
			prev2Y = bodyY[i];
			bodyX[i] = prevX;
			bodyY[i] = prevY;
			prevX = prev2X;
			prevY = prev2Y;
		}

		if (Dir != STOP) {
			switch (Dir) {
			case UP: headY--; headrotation = UP; break;
			case DOWN: headY++; headrotation = DOWN; break;
			case LEFT: headX--; headrotation = LEFT; break;
			case RIGHT: headX++; headrotation = RIGHT; break;
			default: break;
			}
		}

		if (headX > Width - 1) headX -= Width;
		else if (headX < 0) headX += Width;
		if (headY > Height - 1) headY -= Height;
		else if (headY < 0) headY += Height;

		for (int i = 1; i < nBody; i++) {
			if (bodyX[i] == headX && bodyY[i] == headY) {
				bgMusic.stop();
				soundDie.play();
				gameOver = true;
				gameFail = true;
			}
		}

	}

}

void Draw() {

	window.clear(Color::Color(0,0,128,255));

	Prompt_info(WINDOW_WIDTH*GRIDSIZE, GRIDSIZE);
	int delta_x = GRIDSIZE / SCALE / 2;
	int delta_y = GRIDSIZE / SCALE / 2;

	for (int i = 0; i < Width; i++)
		for (int j = 0; j < Height; j++) {

			spBackground.setPosition(i * GRIDSIZE + delta_x, j * GRIDSIZE + delta_y);
			window.draw(spBackground);

		}

	spSnakeHead.setPosition(bodyX[0] * GRIDSIZE + delta_x, bodyY[0] * GRIDSIZE + delta_y);
	spSnakeHead.setRotation(headrotation);
	window.draw(spSnakeHead);

	for (int i = 1; i < nBody; i++) {

		spSnakeBody.setPosition(bodyX[i] * GRIDSIZE + delta_x, bodyY[i] * GRIDSIZE + delta_y);
		window.draw(spSnakeBody);

	}

	spFruit.setPosition(fruitX * GRIDSIZE + delta_x, fruitY * GRIDSIZE + delta_y);
	window.draw(spFruit);


	if (gamePause)

		gamePause_info();

	else if (ingameMenu)

		ingameMenu_info();

	else if (saveMenu || loadMenu)

		SaveAndLoad_info();

	window.display();

}

void qSave() {

	_qS_diff = diff;
	_qS_fruitX = fruitX;
	_qS_fruitY = fruitY;
	_qS_score = score;
	_qS_headX = headX;
	_qS_headY = headY;
	_qS_headrotation = headrotation;
	_qS_nBody = nBody;
	_qS_Dir = Dir;
	for (int i = 0; i < MAXLENGTH; i++) {
		_qS_bodyX[i] = bodyX[i];
		_qS_bodyY[i] = bodyY[i];
	}

}

void qLoad() {

	diff = _qS_diff;
	fruitX = _qS_fruitX;
	fruitY = _qS_fruitY;
	score = _qS_score;
	headX = _qS_headX;
	headY = _qS_headY;
	headrotation = _qS_headrotation;
	nBody = _qS_nBody;
	Dir = _qS_Dir;
	for (int i = 0; i < MAXLENGTH; i++) {
		bodyX[i] = _qS_bodyX[i];
		bodyY[i] = _qS_bodyY[i];
	}

}

void SaveAndLoad_info() {
	
	if (!startupMenu) {

		RectangleShape _mask(Vector2f(WINDOW_WIDTH * GRIDSIZE, WINDOW_HEIGHT * GRIDSIZE));
		_mask.setFillColor(Color(0, 0, 128, 128));
		window.draw(_mask);

	}

	RectangleShape _bg(Vector2f(10 * GRIDSIZE, 13 * GRIDSIZE));
	_bg.setFillColor(Color(128, 128, 128, 128));
	_bg.setOutlineThickness(2);
	_bg.setOutlineColor(Color(200, 200, 200, 255));
	_bg.setOrigin(5 * GRIDSIZE, 6.5 * GRIDSIZE);
	_bg.setPosition(WINDOW_WIDTH * GRIDSIZE / 2, WINDOW_HEIGHT * GRIDSIZE / 2);
	window.draw(_bg);

	FloatRect rect;
	RectangleShape _block;
	int chsize = 14;
	text.setFillColor(Color(255, 255, 255, 255));
	text.setOutlineThickness(1);
	text.setCharacterSize(chsize);
	text.setOutlineColor(Color(0, 0, 0, 128));
	text.setStyle(Text::Bold);

	for (int i = 0; i < 5; i++) {

		// 标号方块
		Vector2f pixel_delta = _bg.getPosition() - _bg.getOrigin();
		_block.setSize(Vector2f(2 * GRIDSIZE, 2 * GRIDSIZE));
		_block.setFillColor(Color(128, 128, 128, 128));
		_block.setOutlineThickness(1);
		_block.setOutlineColor(Color(200, 200, 200, 255));
		_block.setPosition(pixel_delta.x + 0.5 * GRIDSIZE, pixel_delta.y + 0.5 * GRIDSIZE + i * 2.5 * GRIDSIZE);
		window.draw(_block);

		// 标号
		stringstream num;
		num << i + 1;
		text.setString(num.str());
		rect = text.getLocalBounds();
		text.setOrigin(rect.width / 2, rect.height / 2);
		rect = _block.getLocalBounds();
		text.setPosition(0.4 * GRIDSIZE + rect.width / 2 + pixel_delta.x, 0.4 * GRIDSIZE + rect.height / 2 + pixel_delta.y + i * 2.5 * GRIDSIZE);
		window.draw(text);

		// 存档信息方块
		_block.setSize(Vector2f(6.5 * GRIDSIZE, 2 * GRIDSIZE));
		_block.setFillColor(Color(128, 128, 128, 128));
		_block.setOutlineThickness(1);
		_block.setOutlineColor(Color(200, 200, 200, 255));
		_block.setPosition(pixel_delta.x + 3 * GRIDSIZE, pixel_delta.y + 0.5 * GRIDSIZE + i * 2.5 * GRIDSIZE);
		window.draw(_block);

		// 存档信息
		ifstream loadfile;
		unsigned long long intemp;
		switch (i) {
		case 0: loadfile.open("data/saves/save01.svf", ios::in); break;
		case 1: loadfile.open("data/saves/save02.svf", ios::in); break;
		case 2: loadfile.open("data/saves/save03.svf", ios::in); break;
		case 3: loadfile.open("data/saves/save04.svf", ios::in); break;
		case 4: loadfile.open("data/saves/save05.svf", ios::in); break;
		default: break;
		}
		loadfile >> intemp;

		if (intemp == 1) {

			stringstream _savetime;
			loadfile >> intemp; _savetime << intemp << "-"; /*获取当前年份,从1900开始，所以要加1900*/
			loadfile >> intemp; _savetime << intemp << "-"; /*获取当前月份,范围是0-11,所以要加1*/
			loadfile >> intemp; _savetime << intemp << endl; /*获取当前月份日数,范围是1-31*/

			loadfile >> intemp; _savetime << intemp << ":"; /*获取当前时,这里获取西方的时间,刚好相差八个小时*/
			loadfile >> intemp; _savetime << intemp << ":"; /*获取当前分*/
			loadfile >> intemp; _savetime << intemp; /*获取当前秒*/

			stringstream _savescore;
			loadfile >> intemp; _savescore << intemp; /*获取score*/

			loadfile.close();

			text.setString(_savetime.str() + L"   分数: " + _savescore.str());
			rect = text.getLocalBounds();
			text.setOrigin(0, rect.height / 2);
			rect = _block.getLocalBounds();
			text.setPosition(rect.width / 2 + pixel_delta.x, 0.4 * GRIDSIZE + rect.height / 2 + pixel_delta.y + i * 2.5 * GRIDSIZE);
			window.draw(text);

		}	
	
	}

}

void SaveAndLoad(int j) {

	if (saveMenu) {

		time_t timep;
		time(&timep);
		tm* _savetime = gmtime(&timep);

		ofstream savefile;
		switch (j) {
		case 0:	savefile.open("data/saves/save01.svf", ios::out); break;
		case 1:	savefile.open("data/saves/save02.svf", ios::out); break;
		case 2:	savefile.open("data/saves/save03.svf", ios::out); break;
		case 3:	savefile.open("data/saves/save04.svf", ios::out); break;
		case 4:	savefile.open("data/saves/save05.svf", ios::out); break;
		default: break;
		}
		savefile << 1 << endl;
		savefile << 1900 + _savetime->tm_year << endl;
		savefile << 1 + _savetime->tm_mon << endl;
		savefile << 1 + _savetime->tm_mday << endl;
		savefile << (8 + _savetime->tm_hour) % 24 << endl;
		savefile << _savetime->tm_min << endl;
		savefile << _savetime->tm_sec << endl;
		savefile << score << endl;
		savefile << diff << endl;
		savefile << fruitX << endl;
		savefile << fruitY << endl;
		savefile << headX << endl;
		savefile << headY << endl;
		savefile << headrotation << endl;
		savefile << nBody << endl;
		savefile << Dir << endl;
		for (int i = 0; i < MAXLENGTH; i++) {
			savefile << bodyX[i] << endl;
			savefile << bodyY[i] << endl;
		}

		savefile.close();

	}
	else if (loadMenu) {
		ifstream loadfile;
		long long intemp;
		switch (j) {
		case 0:	loadfile.open("data/saves/save01.svf", ios::in); break;
		case 1:	loadfile.open("data/saves/save02.svf", ios::in); break;
		case 2:	loadfile.open("data/saves/save03.svf", ios::in); break;
		case 3:	loadfile.open("data/saves/save04.svf", ios::in); break;
		case 4:	loadfile.open("data/saves/save05.svf", ios::in); break;
		default: break;
		}
		loadfile >> intemp;//saved
		if (intemp == 1) {

			loadfile >> intemp;//y
			loadfile >> intemp;//m
			loadfile >> intemp;//d
			loadfile >> intemp;//h
			loadfile >> intemp;//m
			loadfile >> intemp;//s

			loadfile >> intemp;
			score = intemp;
			loadfile >> intemp;
			diff = intemp;
			loadfile >> intemp;
			fruitX = intemp;
			loadfile >> intemp;
			fruitY = intemp;
			loadfile >> intemp;
			headX = intemp;
			loadfile >> intemp;
			headY = intemp;
			loadfile >> intemp;
			headrotation = (eDirection)intemp;
			loadfile >> intemp;
			nBody = intemp;
			loadfile >> intemp;
			Dir = (eDirection)intemp;
			for (int i = 0; i < MAXLENGTH; i++) {
				loadfile >> intemp;
				bodyX[i] = intemp; 
				loadfile >> intemp;
				bodyY[i] = intemp;
			}

			loadMenu = false;

			if (startupMenu) {

				startupMenu = false;

			}

		}
		
		loadfile.close();

	}

}

void Option_info() {



}

void gameOver_input(bool& _b1, bool& _b2) {

	bool& toModify = (_b1 ? _b1 : _b2);

	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			toModify = false;
			window.close();
		}
	}


	// Function
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		if (!loadMenu) {
			toModify = false;
			startupMenu = true;
			while (Keyboard::isKeyPressed(Keyboard::Escape));
		}
		else if (loadMenu) {
			loadMenu = false;
			while (Keyboard::isKeyPressed(Keyboard::Escape));
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::E)) {
		if (qSaved) {
			toModify = false;
			gameOver = false;
			qLoad();
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::R)) {
		toModify = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::L)) {
		loadMenu = true;
	}
	if (Keyboard::isKeyPressed(Keyboard::X)) {
		toModify = false;
		gameQuit = true;
		window.close();
	}

}

void Menu() {

	window.clear(Color::Color(0, 0, 128, 255));

	FloatRect rect;
	rect = spMenuHeader.getLocalBounds();
	spMenuHeader.setOrigin(rect.width, rect.height / 2);
	spMenuHeader.setPosition((WINDOW_WIDTH * GRIDSIZE + INFO_WIDTH), (WINDOW_HEIGHT * GRIDSIZE) / 2);
	
	window.draw(spMenuHeader);

	int initialY = 0;
	int chsize = 18;
	text.setFillColor(Color(255, 255, 255, 255));
	text.setOutlineThickness(2);
	text.setOutlineColor(Color(0, 0, 0, 128));
	text.setStyle(Text::Bold);

	text.setCharacterSize(40);
	text.setString(L"[SFML版] 贪吃蛇");
	rect = text.getLocalBounds();
	text.setOrigin(0 , rect.height / 2);
	text.setPosition(40, (WINDOW_HEIGHT * GRIDSIZE) / 2 - 70);
	window.draw(text);
	initialY += 48;

	text.setCharacterSize(chsize);
	text.setString(L"[ENTER]开始游戏");
	rect = text.getLocalBounds();
	text.setOrigin(0, rect.height / 2);
	text.setPosition(40, (WINDOW_HEIGHT * GRIDSIZE) / 2 - 30 + initialY);
	window.draw(text);
	initialY += chsize + 10;

	text.setCharacterSize(chsize);
	text.setString(L"[L]读取存档");
	rect = text.getLocalBounds();
	text.setOrigin(0, rect.height / 2);
	text.setPosition(40, (WINDOW_HEIGHT * GRIDSIZE) / 2 - 30 + initialY);
	window.draw(text);
	initialY += chsize + 10;
	
	text.setCharacterSize(chsize);
	text.setString(L"[O]选项");
	rect = text.getLocalBounds();
	text.setOrigin(0, rect.height / 2);
	text.setPosition(40, (WINDOW_HEIGHT * GRIDSIZE) / 2 - 30 + initialY);
	window.draw(text);
	initialY += chsize + 10;
	
	text.setCharacterSize(chsize);
	text.setString(L"[ESC]退出游戏");
	rect = text.getLocalBounds();
	text.setOrigin(0, rect.height / 2);
	text.setPosition(40, (WINDOW_HEIGHT * GRIDSIZE) / 2 - 30 + initialY);
	window.draw(text);
	initialY += chsize + 10;

	if (loadMenu)

		SaveAndLoad_info();

	window.display();

}

void Menu_input() {

	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			startupMenu = false;
			gameOver = true;
			window.close();
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		if (loadMenu) {
			loadMenu = false;
		}
		else {
			startupMenu = false;
			gameOver = true;
			gameQuit = true;
			window.close();
		}
		while (Keyboard::isKeyPressed(Keyboard::Escape));
	}
	if (Keyboard::isKeyPressed(Keyboard::O)) {
		Option_info();
	}
	if (Keyboard::isKeyPressed(Keyboard::L)) {
		if (loadMenu)
			loadMenu = false;
		else if(!loadMenu)
			loadMenu = true;
		while (Keyboard::isKeyPressed(Keyboard::L));
	}
	if (Keyboard::isKeyPressed(Keyboard::Enter)) {
		startupMenu = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		if(loadMenu)
			SaveAndLoad(0);
	}
	if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		if (loadMenu)
			SaveAndLoad(1);
	}
	if (Keyboard::isKeyPressed(Keyboard::Num3)) {
		if (loadMenu)
			SaveAndLoad(2);
	}
	if (Keyboard::isKeyPressed(Keyboard::Num4)) {
		if (loadMenu)
			SaveAndLoad(3);
	}
	if (Keyboard::isKeyPressed(Keyboard::Num5)) {
		if (loadMenu)
			SaveAndLoad(4);
	}

}

void gameOver_draw() {

	window.clear(Color::Color(0, 0, 128, 255));

	Prompt_info(WINDOW_WIDTH * GRIDSIZE, GRIDSIZE);

	if (gameSuccess)

		gameSuccess_info(WINDOW_WIDTH * GRIDSIZE / 2 - GRIDSIZE * 4, WINDOW_HEIGHT * GRIDSIZE / 2 - GRIDSIZE * 2);

	else if (gameFail)

		gameFail_info(WINDOW_WIDTH * GRIDSIZE / 2 - GRIDSIZE * 4, WINDOW_HEIGHT * GRIDSIZE / 2 - GRIDSIZE * 2);

	window.display();

}

int main(){

	do {

		Init();

		while (startupMenu) {

			Menu();
			
			Menu_input();

		}

		while (!gameOver) {

			Ingame_input();

			Logic();

			Draw();

		}

		while (gameSuccess || gameFail) {

			gameOver_draw();

			gameOver_input(gameSuccess, gameFail);

		}

	} while (window.isOpen() && !gameQuit);
	
	exit(0);
	return 0;

}