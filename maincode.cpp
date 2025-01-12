#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
using std::string, std::cout, std::vector;


void ShowConsoleCursor() {
	HANDLE out = GetStdHandle(bool showFlag);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
		
}


void gotoxy(int x, int y) { //функция для перемещения курсора в центр карты
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) }; //получение координат курсора 
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE); //?
	SetConsoleCursorPosition(output, pos); //устанавливает курсор на заданное значение 
}
bool isPlaying = true;

const int F_WIDTH = 10;
const int F_HEIGHT = 20;

class Detail {
	int pos_x = F_WIDTH / 2;
	int pos_y = 0;
	string sprite = 
		"0000"
		"0000"
		"0000"
		"0000";
public:
	Detail(const string& sprite_) {
			sprite = sprite_;
	}
	void draw() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				char sym = sprite[i * 4 + j];
				if (sym == "$") {
					cout << sym;
					}
		} 
	}
};

vector < Detail > details = {
	Detail(
		"$$00"
		"$$00"
		"0000"
		"0000"),
	Detail(
		"$000"
		"$000"
		"$000"
		"$000"),
	Detail(
		"000$"
		"000$"
		"000$"
		"$$$$"),
	Detail(
		"$000"
		"$000"
		"$000"
		"$$$$")
};

int main(){
	ShowConsoleCursor(false);
	while (isPlaying) {

	 }
}