#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>
#include <conio.h>


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
		std::string new_sprite(16, ' '); // Инициализируем новую фигуру пустыми клетками

		for (int i = 0; i < sprite.size(); i++) {
			if (sprite[i] == '$') {
				int x = i % 4; // Текущий X
				int y = i / 4; // Текущий Y

				// Поворот на 90 градусов по часовой стрелке
				int new_x = y;          // Новый X = старый Y
				int new_y = 3 - x;     // Новый Y = 3 - старый X

				// Записываем символ в новую позицию
				new_sprite[new_y * 4 + new_x] = '$'; 
			}
		}

		sprite = new_sprite; // Обновляем спрайт фигуры
	}
	void moving() {
		if (_kbhit()) {
			clear();
			switch (_getch()) {
			case 'a':
				pos_x -= 1;
				break;
			case 'd':
				pos_x += 1;
				break;
			case 's':
				rotate();
				break;
			}
			draw();
		}
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
		const std::string sprite = detail.get_sprite(); // Используем ссылку, чтобы избежать копирования
		const int det_x = detail.x();
		const int det_y = detail.y();

		// Проверка коллизии для каждой клетки спрайта
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (sprite[j * 4 + i] == '$') {
					int new_x = det_x + i + 1; // X-координата клетки на поле
					int new_y = det_y + j + 1; // Y-координата клетки на поле

					// Проверка выхода за границы поля
					if (new_x < 1 || new_x >= F_WIDTH + 1 || new_y >= F_HEIGHT) {
						return true; // Коллизия с границами поля
					}
					// Проверка коллизии с другими фигурами
					if (map[new_y * F_WIDTH + new_x - 1] != ' ') {
						return true; // Коллизия с другой фигурой
					}
				}
			}
		}

		return false; // Коллизии нет
	}

	void clear_lines(std::string& map) {
		for (int y = F_HEIGHT - 1; y >= 0; y--) {
			bool isFull = true;
			for (int x = 0; x < F_WIDTH; x++) {
				if (map[y * F_WIDTH + x] == ' ') {
					isFull = false;
					break;
				}
			}
			if (isFull) {
				std::cout << "line for clear: " << y << std::endl;
				for (int yy = y; yy > 0; --yy) {
					for (int x = 0; x < F_WIDTH; x++) {
						map[yy * F_WIDTH + x] = map[(yy - 1) * F_WIDTH + x];
					}
				}
				for (int x = 0; x < F_WIDTH; x++) {
					map[x] = ' ';
				}
				y++;
			}
		}
	}

	void place_figure(const Detail& detail) {
		const std::string& sprite = detail.get_sprite();
		const int det_x = detail.x();
		const int det_y = detail.y();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (sprite[j * 4 + i] == '$') {
					int new_x = det_x + i + 1; 
					int new_y = det_y + j + 1; 

					if (new_x >= 1 && new_x < F_WIDTH + 1 && new_y >= 1 && new_y < F_HEIGHT + 1) {
						map[(new_y - 1) * F_WIDTH + (new_x - 1)] = '$';
					}
				}
			}
		}
		//clear_lines(map);
	}
	
};

int main() {
	ShowConsoleCursor(false);
	Map map;
	map.draw_frame();
	std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now(); //берется начальное время
	std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now(); //берется конечное время
	while (isPlaying) {
		Detail cur_det = details[0];
		bool stand = false;
		while (!stand) {
			endTime = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = endTime - startTime; //расчеты времени скока прошло
			stand = map.collision(cur_det);
			cur_det.moving();
			if (diff.count() > 0.2 && !stand) { //и если прошло больше 0.2 секунды и деталька не стоит на поле выполняются функции
				cur_det.clear();
				cur_det.move_down();
				cur_det.draw();
				startTime = std::chrono::system_clock::now();
			}
		}
		map.place_figure(cur_det);
	}
}
