#ifndef _TILE_H_
#define _TILE_H_

#include<vector>

#define SIZE_TILE 48

struct Tile
{
	enum class Direction
	{
		None = 0,
		Up,
		Down,
		Left,
		Right
	};

	int terrian = 0;
	int decoration = -1;
	Direction direction = Direction::None;
	int special_flag = -1;
	bool has_tower = false;
};

typedef std::vector<std::vector<Tile>> TileMap;

#endif // !_TILE_H_

