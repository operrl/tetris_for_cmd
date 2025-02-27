#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <chrono>


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

	const std::string& get_sprite() const {
		return sprite;
	}
	int x() const {
		return pos_x;
	}
	int y() const{
		return pos_y;
	}


	void draw() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				gotoxy(1 + pos_x + j,1 + pos_y + i);
				char sym = sprite[i * 4 + j];
				if (sym == '$') {
					std::cout << sym;
				}
			}
		}
	}
	void clear() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				gotoxy(1 + pos_x + j, 1 + pos_y + i);
				char sym = sprite[i * 4 + j];
				if (sym == '$') {
					std::cout << ' ';
				}
			}
		}
	}
	void move_down() {
		pos_y += 1;
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
		for (int i = 0; i < sprite.size(); i++) {
			if (sprite[i] == '$') {
				double x = (1 % 4);
				double y = (i / 4);

				int new_x = -y - min_x;
				int new_y = x - min_y;
				new_sprite[new_y * 4 + new_x] = sprite[i];
			}
		}
		sprite = new_sprite;
	}
};

std::vector < Detail > details = {
	Detail(
		"0000"
		"0000"
		"$$00"
		"$$00"),
	Detail(
		"$000"
		"$000"
		"$000"
		"$000"
	),
	Detail(
		"$000"
		"$000"
		"$000"
		"$$00"
	),
	Detail(
		"$$00"
		"0$$0"
		"0000"
		"0000"
	),
	Detail(
		"0$00"
		"$$$0"
		"0000"
		"0000"
	),
	Detail(
		"000$"
		"000$"
		"000$"
		"00$$"
	),
	Detail(
		"00$$"
		"0$$0"
		"0000"
		"0000"
	)
};
class Map {
	std::string map = std::string(F_HEIGHT * F_WIDTH, ' ');
public:
	void draw_frame() {
		for (int h = 0; h < F_HEIGHT + 2; h++) {
			for (int w = 0; w < F_WIDTH + 2; w++) {
				gotoxy(w, h);
				if (h == 0 || w == 0 || h == F_HEIGHT + 1 || w == F_WIDTH + 1) {
					std::cout << 'x';
				}
			}
		}
	}
	bool collision(const Detail& detail) {
		const std::string sprite = detail.get_sprite();
		const int det_x = detail.x();
		const int det_y = detail.y();
		//проверка колизии с нижней границей 
		for (int i = 0; i < 4; i++) {
			int bot_pos = 0;
			for (int j = 0; j < 4; j++) {
				if (sprite[j * 4 + i] == '$') {
					bot_pos = det_y + j + 1;
				}
			}
			if (bot_pos * F_WIDTH + i + det_x >= map.size() || map[bot_pos * F_WIDTH + i + det_x] != ' ') {
				for (int k = 0; k < 4; k++) {
					for (int j = 0; j < 4; j++){
						if (sprite[j * 4 + k] == '$') {
							map[(det_y + j) * F_WIDTH + k + det_x] = '$';
						}
					}
				}
				return true;
			}
		}
		return false;
	}
};

int main() {
	ShowConsoleCursor(false);
	Map map;
	map.draw_frame();
	std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now(); //обращение к пространству имен std, установка точки времени через системное время, название переменной старттайм? 
	std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now(); // приравнивается это все к сисетмному времен
	while (isPlaying) {
		Detail cur_det = details[0];
		bool stand = false;
		while (!stand) {
			endTime = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = endTime - startTime;
			stand = map.collision(cur_det);
			if (diff.count() > 0.2 && !stand) {
				cur_det.clear();
				cur_det.move_down();
				cur_det.draw();
				startTime = std::chrono::system_clock::now();
			}
		}
	}
}
