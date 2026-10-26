#include <iostream>
#include <vector>
#include <conio.h>

const int SIZE = 10;


struct Player {

	int row;
	int col;

	void Up() {
		row = std::max(0, row - 1);
	}

	void Down() {
		row = std::min(SIZE - 1, row + 1);
	}

	void Left() {
		col = std::max(0, col - 1);
	}

	void Right() {
		col = std::min(SIZE - 1, col + 1);
	}

};

struct Cursor {

	int row;
	int col;

	void Up() {
		row = std::max(0, row - 1);
	}

	void Down() {
		row = std::min(SIZE - 1, row + 1);
	}

	void Left() {
		col = std::max(0, col - 1);
	}

	void Right() {
		col = std::min(SIZE - 1, col + 1);
	}
};

struct Cell {

	char cell = ' ';
	bool IsWall = false;
	bool IsCursor = false;
};

std::vector<std::vector<Cell>> Grid(int Size) {

	std::vector<std::vector<Cell>> Array;
	std::vector<Cell> Vec;

	for (int i = 0; i < Size; i++) {
		Vec.push_back(Cell());
	}
	for (int j = 0; j < Size; j++)
		Array.push_back(Vec);

	return Array;
}

void ShowGrid(std::vector<std::vector<Cell>> Array, Player& player, Cursor& cursor) {

	for (int row = 0; row < Array.size(); row++)
	{
		for (int col = 0; col < Array[row].size(); col++)
		{
			if (player.row == row && player.col == col)
				std::cout << "\033[31m[" << "O" << "]\033[31m";

			else if(cursor.row == row && cursor.col == col)
				std::cout << "\033[33m[" << "O" << "]\033[33m";
			else
				std::cout << "\033[0m[" << Array[row][col].cell << "]\033[0m";
		}
		std::cout << "" << std::endl;
	}
}

void Tp(Player& player, Cursor& cursor) {
	player.row = cursor.row;
	player.col = cursor.col;
}

void Move(Player& player, Cursor& cursor) {

	switch (_getch()) {

	case'z': player.Up(); break;
	case'q': player.Left(); break;
	case's': player.Down(); break;
	case'd': player.Right(); break;
	case't': Tp(player, cursor); break;
	case'i': cursor.Up(); break;
	case'j': cursor.Left(); break;
	case'k': cursor.Down(); break;
	case'l': cursor.Right(); break;

	}
}
int main()
{
	bool IsPlaying = true;

	std::vector<std::vector<Cell>> Array = Grid(SIZE);
	Player player(5, 5);
	Cursor cursor(2, 2);

	while (IsPlaying) {

		system("cls");
		ShowGrid(Array, player, cursor);
		Move(player, cursor);
	}
}

