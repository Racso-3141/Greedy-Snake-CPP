
//Greedy Snake
#include<Windows.h>
#include<iostream>
#include<fstream>
#include<ctime>
#include<stdlib.h>
using namespace std;
//-----------------------
struct Xsnake {
	unsigned x, y;
	Xsnake* next;
}*head;
struct Xfood {
	unsigned x, y, colour;//colour:0white 1red 2blue
}Food;
unsigned Score = 0;
unsigned Max = 0;
unsigned Length = 5;
unsigned SleepTime = 150;
unsigned Direction = 8;
unsigned SnakeColour = 0;
int GameStatus = 0;
//-----------------------
int main();
void XWelcome();
void Xstart();
void Xinterface();
void Xgame();
void Xend();
void Pos(unsigned, unsigned);
void SnakeShow(Xsnake*);
void GetSnake(Xsnake*);
Xsnake* SnakeAdd(Xsnake*, unsigned, unsigned);
Xsnake* SnakeMove(Xsnake*, unsigned);
Xsnake* SnakePush_back(Xsnake*, const unsigned, const unsigned);
inline void SnakeTail(Xsnake*, unsigned&, unsigned&);
inline void SnakeClear(unsigned, unsigned);
int SnakeBite(Xsnake*);
void SnakeKey(unsigned&);
void SnakeDel(Xsnake*);
void FoodCreate(Xsnake*, Xfood&);
void FoodShow(const Xfood&);
//-----------------------

void Pos(unsigned x, unsigned y) {
	COORD p;
	p.X = y; p.Y = x;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, p);
}
void Xstart() {
	const CONSOLE_CURSOR_INFO cursor{ (DWORD)100,FALSE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	head = new Xsnake;
	head->x = 11; head->y = 38;
	head->next = nullptr;
	head = SnakeAdd(head, 10, 38); head = SnakeAdd(head, 9, 38);
	head = SnakeAdd(head, 8, 38); head = SnakeAdd(head, 7, 38);
	ifstream i("snake_data.dat");
	if (!i) i.close();
	else {
		unsigned s;
		while (i >> s)
			if (s > Max)Max = s;
		i.close();
	}
	Pos(16, 48); cout << "Load success£¬"; system("pause");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
	Pos(18, 50); cout << "    Start    ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	Pos(20, 50); cout << "    Leave    ";
	int YN = 8;
	while (!GetAsyncKeyState(VK_RETURN)) {
		if (GetAsyncKeyState(VK_UP)) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
			Pos(18, 50); cout << "    Start    ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			Pos(20, 50); cout << "    Leave    ";
			YN = 8;
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			Pos(18, 50); cout << "    Start    ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
			Pos(20, 50); cout << "    Leave    ";
			YN = 4;
		}
	}
	if (YN == 4) exit(0);
	system("cls");
}
void Xinterface() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 136);
	Pos(0, 0);
	cout << "¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö\n";//80x
	for (unsigned i = 1; i <= 28; ++i)cout << "¡ö\n";
	for (unsigned i = 1; i <= 28; ++i) {
		Pos(i, 78);
		cout << "¡ö";
	}
	cout << endl;
	cout << "¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö¡ö";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	Pos(2, 88); cout << "score:" << Score;
	Pos(3, 88); cout << "length:" << Length;
	Pos(4, 88); cout << "speed:" << 300 - SleepTime;
	Pos(5, 88); cout << "highest score:" << Max;
}
void Xgame() {
	Xinterface();
	unsigned tail_x = 0, tail_y = 0;
	FoodCreate(head, Food);
	FoodShow(Food);
	while (Length < 1026) {
		if (Food.x == head->x && Food.y == head->y) {
			if (SnakeColour == 1)SleepTime += 40;
			else if (SnakeColour == 2)SleepTime -= 40;
			if (Food.colour == 1) {
				SleepTime -= 40;
				Score += 8;
			}
			else if (Food.colour == 2) {
				SleepTime += 40;
				Score -= 6;
			}
			SnakeColour = Food.colour;
			Score += 10;
			++Length;
			head = SnakePush_back(head, tail_x, tail_y);
			FoodCreate(head, Food);
			FoodShow(Food);
			if (SleepTime > 80) SleepTime -= 3;
			Pos(2, 93); cout << Score;
			Pos(3, 93); cout << Length;
			Pos(4, 93); cout << 300 - SleepTime;
			Pos(5, 101); cout << Max;
			char bel = 7; cout << bel;
		}
		else SnakeClear(tail_x, tail_y);
		SnakeTail(head, tail_x, tail_y);
		SnakeShow(head);
		Sleep(SleepTime);
		SnakeKey(Direction);
		if (Direction == 0) {
			GameStatus = 1;
			break;
		}
		head = SnakeMove(head, Direction);
		if (head->x == 0 || head->x == 29 || head->y == 0 || head->y == 78) {
			GameStatus = 2;
			break;
		}
		if (SnakeBite(head)) {
			GameStatus = 3;
			break;
		}
	}
}
void Xend() {
	ofstream o("snake_data.dat", ios_base::app);
	if (!o) o.close();
	else {
		o << Score << endl;
		o.close();
	}
	switch (GameStatus)
	{
	case 1: {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
		Pos(10, 17); cout << "  Hint                                    ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		Pos(11, 17); cout << "¨‡                                      ¨Š";
		Pos(12, 17); cout << "¨‡                                      ¨Š";
		Pos(13, 17); cout << "¨‡¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨Š\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		Pos(12, 19); cout << "Going to leave,";
		break;
	}
	case 2: {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 207);
		Pos(10, 17); cout << "  Game Over                                    ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		Pos(11, 17); cout << "¨‡                                      ¨Š";
		Pos(12, 17); cout << "¨‡                                      ¨Š";
		Pos(13, 17); cout << "¨‡¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨Š\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		break;
	}
	case 3: {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 223);
		Pos(10, 17); cout << "  Game Over                                    ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
		Pos(11, 17); cout << "¨‡                                      ¨Š";
		Pos(12, 17); cout << "¨‡                                      ¨Š";
		Pos(13, 17); cout << "¨‡¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨Š\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		break;
	}
	default: {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 175);
		Pos(10, 17); cout << "  Congratulations                                    ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		Pos(11, 17); cout << "¨‡                                      ¨Š";
		Pos(12, 17); cout << "¨‡                                      ¨Š";
		Pos(13, 17); cout << "¨‡¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨Š\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		Pos(12, 19); cout << "You succeed,";
		break;
	}
	}
	Pos(12, 28); system("pause");
	Pos(27, 88); SnakeDel(head);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 191);
	Pos(10, 17); cout << "  Hint                                    ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	Pos(11, 17); cout << "¨‡                                      ¨Š";
	Pos(12, 17); cout << "¨‡                                      ¨Š";
	Pos(13, 17); cout << "¨‡                                      ¨Š";
	Pos(14, 17); cout << "¨‡                                      ¨Š";
	Pos(15, 17); cout << "¨‡¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨x¨Š\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	Pos(12, 19); cout << "Want to play one more round£¿";
	Pos(14, 24); cout << "  Yes  ";
	Pos(14, 46); cout << "  No  ";
	int YN = 6;
	while (!GetAsyncKeyState(VK_RETURN)) {
		if (GetAsyncKeyState(VK_LEFT)) {
			YN = 4;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 191);
			Pos(14, 24); cout << "  Yes  ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			Pos(14, 46); cout << "  No  ";
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			YN = 6;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			Pos(14, 24); cout << "  Yes  ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 191);
			Pos(14, 46); cout << "  No  ";
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	if (YN == 4) {
		Score = 0;
		Max = 0;
		Length = 5;
		SleepTime = 150;
		Direction = 8;
		SnakeColour = 0;
		main();
	}
	else Pos(29, 88);
}
void FoodCreate(Xsnake* s, Xfood& f) {
	srand((unsigned)time(0));
	do {
		f.y = 2 * (rand() % 38 + 1);
		f.x = rand() % 28 + 1;
		while (s) {
			if (s->x == f.x && s->y == f.y) break;
			s = s->next;
		}
	} while (s);
	f.colour = rand() % 8;
	if (f.colour > 2) f.colour = 0;
}
void FoodShow(const Xfood& f) {
	Pos(f.x, f.y);
	if (f.colour == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	else if (f.colour == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	cout << "¡ñ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
Xsnake* SnakeAdd(Xsnake* s, unsigned xx, unsigned yy) {
	Xsnake* n = new Xsnake;
	n->x = xx; n->y = yy;
	n->next = s;
	return n;
}
Xsnake* SnakeMove(Xsnake* s, unsigned direction) {
	Xsnake* h = s;
	Xsnake* n = s;
	while (s->next) {
		n = s;
		s = s->next;
	}
	s->next = h;
	n->next = nullptr;
	switch (direction)
	{
	case 4: s->y = (s->next)->y - 2, s->x = (s->next)->x; break;
	case 6: s->y = (s->next)->y + 2, s->x = (s->next)->x; break;
	case 8: s->x = (s->next)->x - 1, s->y = (s->next)->y; break;
	case 2: s->x = (s->next)->x + 1, s->y = (s->next)->y; break;
	default: exit(1);
	}
	return s;
}
int SnakeBite(Xsnake* s) {
	unsigned xx = s->x, yy = s->y;
	while (s->next) {
		s = s->next;
		if (s->x == xx && s->y == yy) return 1;
	}
	return 0;
}
Xsnake* SnakePush_back(Xsnake* s, const unsigned xx, const unsigned yy) {
	Xsnake* ss = s;
	while (s->next) s = s->next;
	Xsnake* n = new Xsnake;
	n->x = xx, n->y = yy;
	n->next = nullptr;
	s->next = n;
	return ss;
}
inline void SnakeTail(Xsnake* s, unsigned& xx, unsigned& yy) {
	while (s->next)s = s->next;
	xx = s->x; yy = s->y;
}
inline void SnakeClear(unsigned xx, unsigned yy) {
	if (xx && yy) {
		Pos(xx, yy); cout << "  ";
	}
}
void SnakeDel(Xsnake* s) {
	Xsnake* n;
	int i = 0;
	while (s) {
		n = s;
		s = s->next;
		delete n;
		++i;
	}
	cout << "Length of Snake in this round: " << i << '.';
}
void SnakeShow(Xsnake* s) {
	if (SnakeColour == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	else if (SnakeColour == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	while (s) {
		Pos(s->x, s->y); cout << "¡ö";
		s = s->next;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void SnakeKey(unsigned& d) {
	if (GetAsyncKeyState(VK_UP) && d != 2) d = 8;
	else if (GetAsyncKeyState(VK_DOWN) && d != 8) d = 2;
	else if (GetAsyncKeyState(VK_LEFT) && d != 6) d = 4;
	else if (GetAsyncKeyState(VK_RIGHT) && d != 4) d = 6;
	else if (GetAsyncKeyState(VK_SPACE)) {
		Pos(14, 35); cout << "  Pause  ";
		do {
			Sleep(200);
		} while (!GetAsyncKeyState(VK_SPACE));
		Pos(14, 35); cout << "       ";
		SnakeShow(head);
		FoodShow(Food);
	}
	else if (GetAsyncKeyState(VK_ESCAPE)) d = 0;
	else if (GetAsyncKeyState(0x52)) Xinterface(), FoodShow(Food);
}

//-----------------------
int main()
{
	system("mode con cols=120 lines=30");
	system("cls");
	Pos(0, 0);
	Xstart();
	Xgame();
	Xend();
	return 0;
}
