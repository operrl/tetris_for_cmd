#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>



void ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(showFlag);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);

}


void gotoxy(int x, int y) { 
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) }; 
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE); //?
	SetConsoleCursorPosition(output, pos); 
}
bool isPlaying = true;

const int F_WIDTH = 10;
const int F_HEIGHT = 20;

class Detail {
	int pos_x = F_WIDTH / 2;
	int pos_y = 0;
	std::string sprite =
		"0000"
		"0000"
		"0000"
		"0000";
public:
	Detail(const std::string& sprite_) {
		sprite = sprite_;
	}
	void draw() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				gotoxy(pos_x + j, pos_y + i);
				char sym = sprite[i * 4 + j];
				if (sym == '$') {
					std::cout << sym;
				}
			}
		}
	}
	void rotate() {
		std::string new_sprite(
			"0000"
			"0000"
			"0000"
			"0000");

		int min_x = 100;
		int min_y = 100;
		for (int i = 0; i < sprite.size(); i++) {
			if (sprite[i] == '$') {
				double x = (i % 4);
				double y = (i / 4);
				min_x = (y * -1) < min_x ? (y * -1) : min_x;
				min_y = x < min_y ? x : min_y;
			}
		}
	}
};

std::vector < Detail > details = {
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

int main() {
	ShowConsoleCursor(false);
	while (isPlaying) {
		Detail cur_det = details[0];
		bool stand = false;
		while (!stand) {
			cur_det.draw();
		}
	}
}
