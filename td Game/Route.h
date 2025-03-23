#ifndef _ROUTE_H_
#define _ROUTE_H_

#include"tile.h"

#include"SDL.h"

class Route
{
public:
	typedef std::vector<SDL_Point> IdxList;

public:
	Route() = default;
	~Route() = default;

	Route(const TileMap& tile_map,const SDL_Point& idx_origin)
	{
		size_t width_map = tile_map[0].size();
		size_t height_map = tile_map.size();

		SDL_Point idx_now = idx_origin;

		while (true)
		{
			if (idx_now.x > width_map || idx_now.y > height_map)
				break;

			if (check_duplicate_idx(idx_now))
				break;
			else
				idx_list.push_back(idx_now);

			const Tile& tile = tile_map[idx_now.y][idx_now.x];
			if (tile.special_flag == 0)
				break;

			bool is_have_next = true;
			switch (tile.direction)
			{case Tile::Direction::Up:
				idx_now.y--;
				break;
			case Tile::Direction::Down:
				idx_now.y++;
				break;
			case Tile::Direction::Left:
				idx_now.x--;
				break;
			case Tile::Direction::Right:
				idx_now.x++;
				break;
			default:
				is_have_next = false;
				break;
			}

			if (!is_have_next)
				break;
		}
	}

	const IdxList& get_idx_list()const 
	{
		return idx_list;
	}

private:
	IdxList idx_list;

private:
	bool check_duplicate_idx(const SDL_Point& idx_target)
	{
		for (SDL_Point& idx_now : idx_list)
		{
			if (idx_now.x == idx_target.x && idx_now.y == idx_target.y)
				return true;

		}

		return false;
	}
};


#endif // !_ROUTE_H_

