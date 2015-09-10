#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>
#include <Windows.h>

#include "concol.h"

struct Point2D {
	int x;
	int y;

	Point2D(int _x = 0, int _y = 0): x(_x), y(_y) {}
	Point2D operator+(const Point2D & r) {
		return Point2D(x+r.x, y+r.y);
	}

}; 
	
bool operator==(const Point2D & l, const Point2D & r) {
	return l.x == r.x && l.y == r.y;
}

struct Cell {

	Cell * parent;
	Point2D coords;
	int G, H;

public:
	Cell(Cell * _parent = 0, Point2D _coords = Point2D(0, 0), int _G = 0, int _H = 0): parent(_parent), coords(_coords), G(_G), H(_H) {}

};

bool operator<(const Cell & l, const Cell & r) {
	return (l.G + l.H) < (r.G + r.H);
}

enum DIST_TYPE { DIST_EUCLID, DIST_MANHATTAN, DIST_CHEB };

int distance(const Point2D & a, const Point2D & b, DIST_TYPE dist_type = DIST_EUCLID) {
	switch (dist_type) {
		case DIST_EUCLID:
			return (int)sqrt((float)(a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
			break;
		case DIST_MANHATTAN:
			return abs(a.x-b.x) + abs(a.y-b.y);
			break;
		case DIST_CHEB:
			return max(abs(a.x-b.x), abs(a.y-b.y));
			break;
	}
	return -1;
}

struct Move {
	Point2D coord;
	int cost;
};

void put_path_symbol(Point2D point) {

	COORD crd = {point.x, point.y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), crd);
	setcolor(yellow, black);
	std::cout << 'o';
}

Move possible_moves[8] = {

	{Point2D(-1, -1), 14},
	{Point2D(-1, 0), 10},
	{Point2D(-1, +1), 14},
	{Point2D(0, -1), 10},
	{Point2D(0, +1), 10},
	{Point2D(+1, -1), 14},
	{Point2D(+1, 0), 10},
	{Point2D(+1, +1), 14}

};

std::vector<Cell> open_list;
std::vector<Cell> closed_list;

const static size_t MAX_FIELD_SIZE = 80;
char field[MAX_FIELD_SIZE][MAX_FIELD_SIZE];

int main()
{
	setcolor(white, black);

	std::ifstream file("field_3.txt");

	size_t height, width;

	file >> height >> width;
	
	Point2D start_point;

	Point2D end_point;

	for (size_t h=0; h<height; h++) {
		for (size_t w=0; w<width; w++) {
			file >> field[h][w];
			if (field[h][w] == 's')
				start_point = Point2D(w, h);
			if (field[h][w] == 'f')
				end_point = Point2D(w,h);
			if (field[h][w] == '.') {
				std::cout << ' ';
				continue;
			}
			std::cout << field[h][w];
		}
		std::cout << std::endl;
	}

	file.close();

	clock_t start_time = clock();

	Cell start_cell(0, start_point, 0, 0);
	open_list.push_back(start_cell);

	Cell current_cell;

	int c = 0;

	std::vector<Cell*> cell_ptrs;

	while (1) {

		std::sort(open_list.begin(), open_list.end());

		if (open_list.empty()) {
			std::cout << "Couldn't find path.\n";
			break;
		}

		current_cell = *open_list.begin();

		Cell * cell_to_add = new Cell(current_cell);
		cell_ptrs.push_back(cell_to_add);
		c++;

		if (current_cell.coords == end_point) {
			std::cout << "Path found!\n";
			break;
		}

		closed_list.push_back(current_cell);
		open_list.erase(open_list.begin());

		for (size_t i=0; i<8; i++) {

			Point2D move = possible_moves[i].coord;
			Point2D new_coords = current_cell.coords + move;

			if (new_coords.x < 0 || new_coords.y < 0 || new_coords.x >= width || new_coords.y >= height)
				continue;

			if (std::find_if(closed_list.begin(), closed_list.end(), [&new_coords] (const Cell & cell) {
				return cell.coords == new_coords;
			}) != closed_list.end() || field[new_coords.y][new_coords.x] == '#')
				continue;

			std::vector<Cell>::iterator it = std::find_if(open_list.begin(), open_list.end(), [&new_coords] (const Cell & cell) {
				return cell.coords == new_coords;
			});

			if (it == open_list.end()) {
				Cell new_cell(cell_to_add, new_coords, current_cell.G + possible_moves[i].cost, distance(new_coords, end_point, DIST_MANHATTAN));
				open_list.push_back(new_cell);
			} else {
				if ((current_cell.G + possible_moves[i].cost) < it->G) {
					Cell new_cell = *it;

					new_cell.parent = cell_to_add;
					new_cell.G = current_cell.G + possible_moves[i].cost;

					open_list.erase(it);
					open_list.push_back(new_cell);

				}
			}

		}

	}

	clock_t end_time = clock();

	std::cout << "Took " << (end_time - start_time)/(float)CLOCKS_PER_SEC << "sec.\n";
	
	do {
		//std::cout << "(" << current_cell.coords.x << ", " << current_cell.coords.y << ")\n";
		put_path_symbol(current_cell.coords);
		current_cell = *current_cell.parent;
	} while (current_cell.parent != 0);
	//std::cout << "(" << current_cell.coords.x << ", " << current_cell.coords.y << ")\n";
	put_path_symbol(current_cell.coords);
	
	int k = 0;
	for (size_t i=0; i<cell_ptrs.size(); i++) {
		delete cell_ptrs[i];
		k++;
	}

	getchar();
	return 0;
}

