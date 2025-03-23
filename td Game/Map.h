#ifndef _MAP_H_
#define _MAP_H_

#include"tile.h"
#include"Route.h"

#include"SDL.h"

#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<unordered_map>

class Map
{
public:
	typedef std::unordered_map<int, Route> SpawnerRoutePool;

public:
	Map() = default;
	~Map() = default;

	bool load(const std::string path)
	{
		std::ifstream file(path);
		if (!file.good())return false;

		TileMap tile_map_tem;
		int idx_x = -1, idx_y = -1;

		std::string str_line;

		while(getline(file,str_line))
		{
			str_line = trim_str(str_line);
			if (str_line.empty())
				continue;

			idx_x = -1, idx_y++;
			tile_map_tem.emplace_back();

			std::string str_tile;
			std::istringstream str_stream(str_line);

			while(std::getline(str_stream,str_tile,','))
			{
				idx_x++;
				tile_map_tem[idx_y].emplace_back();
				Tile& tile = tile_map_tem[idx_y].back();
				load_str_from_tile(tile, str_tile);

			}
		}

		file.close();

		if (tile_map_tem.empty() || tile_map_tem[0].empty())
			return false;

		tile_map = tile_map_tem;

		ganerate_map_cache();

		return true;

	}

	size_t get_width() const
	{
		if (tile_map.empty())
			return 0;

		return tile_map[0].size();
	}

	size_t get_height() const
	{
		return tile_map.size();
	}

	const TileMap& get_tile_map()const
	{
		return tile_map;
	}

	const SDL_Point& get_idx_home()const
	{
		return idx_home;
	}

	const SpawnerRoutePool& get_spawner_route_pool()const
	{
		return spawner_route_pool;
	}

	void place_tower(const SDL_Point& idx_tower)
	{
		tile_map[idx_tower.y][idx_tower.x].has_tower = true;
	}

private:
	TileMap tile_map;
	SDL_Point idx_home = { 0,0 };
	SpawnerRoutePool spawner_route_pool;

private:
	std::string trim_str(const std::string& str)
	{
		size_t idx_begin = str.find_first_not_of(' \t');
		if (idx_begin == std::string::npos)
			return " ";

		size_t idx_end = str.find_last_not_of(' \t');

		size_t idx_range = idx_end - idx_begin + 1;

		return str.substr(idx_begin, idx_range);
	}

	void load_str_from_tile(Tile& tile,const std::string& str_tile)
	{
		std::string str_tidy = trim_str(str_tile);
		
		std::string str_value;
		std::istringstream str_stream(str_tidy);
		std::vector<int> vec_values;
		int value;

		while(std::getline(str_stream,str_value,'\\'))
		{
			try
			{
				 value = std::stoi(str_value);
			}
			catch (const std::invalid_argument&)
			{
				value = -1;
			}
			vec_values.push_back(value);
		}

		tile.terrian = (vec_values.size() < 1 || vec_values[0] < 0) ? 0 : vec_values[0];
		tile.decoration = vec_values.size() < 2 ? -1 : vec_values[1];
		tile.direction = (Tile::Direction)((vec_values.size() < 3 || vec_values[2] < 0) ? 0 : vec_values[2]);
		tile.special_flag = vec_values.size() < 4 ? -1 : vec_values[3];
	}

	void ganerate_map_cache()
	{
		for (int y = 0;y<get_height();++y)
		{
			for (int x = 0;x<get_width();++x)
			{
				const Tile& tile = tile_map[y][x];

				if (tile.special_flag < 0)
					continue;

				if (tile.special_flag == 0)
				{
					idx_home.y = y;
					idx_home.x = x;
				}
				else
					spawner_route_pool[tile.special_flag] = Route(tile_map, { x,y });
			}
		}
	}
};


#endif // !_MAP_H_

