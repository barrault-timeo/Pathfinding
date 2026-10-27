#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <conio.h>
#include <cstdlib>
#include <cctype>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

const int SIZE = 10;
int MoveCount = 0;

struct Player {
	int row;
	int col;

	void Up() { row = std::max(0, row - 1); }
	void Down() { row = std::min(SIZE - 1, row + 1); }
	void Left() { col = std::max(0, col - 1); }
	void Right() { col = std::min(SIZE - 1, col + 1); }
};

struct Cursor {
	int row;
	int col;

	void Up() { row = std::max(0, row - 1); }
	void Down() { row = std::min(SIZE - 1, row + 1); }
	void Left() { col = std::max(0, col - 1); }
	void Right() { col = std::min(SIZE - 1, col + 1); }
};

struct Cell {
	bool IsWall = false;
	bool IsPath = false;
};

using Grid = std::vector<std::vector<Cell>>;
using Path = std::vector<std::pair<int, int>>;

Grid CreateGrid(int Size) {
	return Grid(Size, std::vector<Cell>(Size));
}

void SetWall(Grid& Array, int row, int col) {
	Array[row][col].IsWall = true;
}

void SetPath(Grid& Array, int row, int col) {
	Array[row][col].IsPath = true;
}

void ResetPath(Grid& Array) {
	for (auto& ligne : Array)
		for (auto& cell : ligne)
			cell.IsPath = false;
}

void BuildMaze(Grid& Array) {
	// Ligne 1
	SetWall(Array, 1, 1);
	SetWall(Array, 1, 2);
	SetWall(Array, 1, 3);
	SetWall(Array, 1, 4);
	SetWall(Array, 1, 5);
	SetWall(Array, 1, 6);
	SetWall(Array, 1, 7);
	SetWall(Array, 1, 8);

	// Ligne 2
	SetWall(Array, 2, 8);

	// Ligne 3
	SetWall(Array, 3, 1);
	SetWall(Array, 3, 2);
	SetWall(Array, 3, 3);
	SetWall(Array, 3, 4);
	SetWall(Array, 3, 5);
	SetWall(Array, 3, 6);
	SetWall(Array, 3, 8);

	// Ligne 4
	SetWall(Array, 4, 1);
	SetWall(Array, 4, 6);
	SetWall(Array, 4, 8);

	// Ligne 5
	SetWall(Array, 5, 1);
	SetWall(Array, 5, 3);
	SetWall(Array, 5, 4);
	SetWall(Array, 5, 6);
	SetWall(Array, 5, 8);

	// Ligne 6
	SetWall(Array, 6, 1);
	SetWall(Array, 6, 4);
	SetWall(Array, 6, 6);
	SetWall(Array, 6, 8);

	// Ligne 7
	SetWall(Array, 7, 1);
	SetWall(Array, 7, 2);
	SetWall(Array, 7, 4);
	SetWall(Array, 7, 8);

	// Ligne 8
	SetWall(Array, 8, 4);
	SetWall(Array, 8, 5);
	SetWall(Array, 8, 6);
	SetWall(Array, 8, 7);
	SetWall(Array, 8, 8);
}

void ShowGrid(Grid& Array, Player& player, Cursor& cursor) {

	system("cls");
	for (int row = 0; row < (int)Array.size(); row++)
	{
		for (int col = 0; col < (int)Array[row].size(); col++)
		{
			if (player.row == row && player.col == col)
				std::cout << "\033[94m[O]\033[0m";

			else if (cursor.row == row && cursor.col == col)
				std::cout << "\033[93m[O]\033[0m";

			else if (Array[row][col].IsWall)
				std::cout << "\033[95m[#]\033[0m";

			else if (Array[row][col].IsPath)
				std::cout << "\033[96m[P]\033[0m";

			else
				std::cout << "[ ]";
		}
		std::cout << std::endl;
	}
	std::cout << "Total Moves: " << MoveCount << std::endl;
}

Path ComputePath(Grid& Array, Player& player, Cursor& cursor) {

	Path path;

	std::queue<std::pair<int, int>> file;
	std::vector<std::vector<bool>> visited(SIZE, std::vector<bool>(SIZE, false));
	std::vector<std::vector<std::pair<int, int>>> parent(
		SIZE, std::vector<std::pair<int, int>>(SIZE, { -1, -1 }));

	int dr[] = { -1, 1, 0, 0 };
	int dc[] = { 0, 0, -1, 1 };

	visited[player.row][player.col] = true;
	file.push({ player.row, player.col });

	while (!file.empty()) {

		auto [row, col] = file.front();
		file.pop();

		if (row == cursor.row && col == cursor.col)
			break;

		for (int d = 0; d < 4; d++) {
			int newRow = row + dr[d];
			int newCol = col + dc[d];

			if (newRow >= 0 && newRow < SIZE &&
				newCol >= 0 && newCol < SIZE &&
				!visited[newRow][newCol] &&
				!Array[newRow][newCol].IsWall)
			{
				visited[newRow][newCol] = true;
				parent[newRow][newCol] = { row, col };
				file.push({ newRow, newCol });
			}
		}
	}

	
	if (!visited[cursor.row][cursor.col])
		return path;

	
	int r = cursor.row;
	int c = cursor.col;
	while (!(r == player.row && c == player.col)) {
		path.push_back({ r, c });
		auto [pr, pc] = parent[r][c];
		r = pr;
		c = pc;
	}

	
	std::reverse(path.begin(), path.end());
	return path;
}


void PrePath(Grid& Array, Player& player, Cursor& cursor) {

	ResetPath(Array);

	Path path = ComputePath(Array, player, cursor);
	for (auto [r, c] : path)
		SetPath(Array, r, c);
}


void PathFinding(Grid& Array, Player& player, Cursor& cursor) {

	Path path = ComputePath(Array, player, cursor);

	MoveCount = 0;

	for (auto [r, c] : path) {
		player.row = r;
		player.col = c;
		MoveCount++;
		ShowGrid(Array, player, cursor);
		std::this_thread::sleep_for(200ms);
	}
}

void Tp(Player& player, Cursor& cursor) {
	player.row = cursor.row;
	player.col = cursor.col;
}

void Move(Grid& Array, Player& player, Cursor& cursor) {

	int oldRowP = player.row;
	int oldColP = player.col;

	int oldRowC = cursor.row;
	int oldColC = cursor.col;

	switch (std::tolower(_getch())) {

	case 'z': player.Up(); break;
	case 'q': player.Left(); break;
	case 's': player.Down(); break;
	case 'd': player.Right(); break;
	case 't': Tp(player, cursor); break;
	case 'p': PathFinding(Array, player, cursor); break;
	case 'i': cursor.Up(); break;
	case 'j': cursor.Left(); break;
	case 'k': cursor.Down(); break;
	case 'l': cursor.Right(); break;

	}

	
	if (Array[player.row][player.col].IsWall) {
		player.row = oldRowP;
		player.col = oldColP;
	}

	if (Array[cursor.row][cursor.col].IsWall) {
		cursor.row = oldRowC;
		cursor.col = oldColC;
	}
}

int main()
{
	bool IsPlaying = true;

	Grid Array = CreateGrid(SIZE);
	Player player{ 5, 5 };
	Cursor cursor{ 2, 2 };

	BuildMaze(Array);

	while (IsPlaying) {

		PrePath(Array, player, cursor);
		ShowGrid(Array, player, cursor);
		Move(Array, player, cursor);
	}
}