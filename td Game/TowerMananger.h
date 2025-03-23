#ifndef _TOWER_MANAGER_
#define _TOWER_MANAGER_

#include<vector>
#include<SDL.h>

#include"manager.h"
#include"Tower.h"
#include"TowerType.h"
#include"ConfigManager.h"
#include"ArcherTower.h"
#include"Axeman.h"
#include"Gunner.h"
#include"vector2.h"
#include"ResourceManager.h"

class TowerManager : public Manager<TowerManager>
{
	friend Manager<TowerManager>;

public:
	typedef std::vector<Tower*> TowerList;

protected:
	TowerManager() = default;
	~TowerManager() = default;

public:
	void on_update(double delta)
	{

		for (Tower* tower : tower_list)
			tower->on_update(delta);

	}

	void on_render(SDL_Renderer* renderer)
	{
		for (Tower* tower : tower_list)
			tower->on_render(renderer);
	}

	double get_place_cost(TowerType tower_type)
	{
		static ConfigManager* instance = ConfigManager::instance();
		double place_cost = 0;

		switch (tower_type)
		{
		case TowerType::Archer:
			place_cost = instance->archer_template.cost[instance->level_archar];
			break;
		case TowerType::Axeman:
			place_cost = instance->axeman_template.cost[instance->level_axeman];
			break;
		case TowerType::Gunner:
			place_cost = instance->gunner_template.cost[instance->level_gunner];
			break;
		}

		return place_cost;
	}

	double get_upgrade_cost(TowerType tower_type)
	{
		static ConfigManager* instance = ConfigManager::instance();
		double upgrade_cost = 0;

		switch (tower_type)
		{
		case TowerType::Archer:
			upgrade_cost = instance->level_archar == 9 ? 
				-1 : instance->archer_template.upgrade_cost[instance->level_archar];
			break;
		case TowerType::Axeman:
			upgrade_cost = instance->level_archar == 9 ?
				-1 : instance->axeman_template.upgrade_cost[instance->level_axeman];
			break;
		case TowerType::Gunner:
			upgrade_cost = instance->level_archar == 9 ?
				-1 : instance->gunner_template.upgrade_cost[instance->level_gunner];
			break;
		}

		return upgrade_cost;
	}

	double get_damage_range(TowerType tower_type)
	{
		static ConfigManager* instance = ConfigManager::instance();
		double view_range = 0;

		switch (tower_type)
		{
		case TowerType::Archer:
			view_range = instance->archer_template.view_range[instance->level_archar];
			break;
		case TowerType::Axeman:
			view_range = instance->axeman_template.view_range[instance->level_axeman];
			break;
		case TowerType::Gunner:
			view_range = instance->gunner_template.view_range[instance->level_gunner];
			break;
		}

		return view_range;
	}

	void place_tower(TowerType tower_type, SDL_Point idx_pos)
	{
		Tower* tower = nullptr;

		switch (tower_type)
		{
		case TowerType::Archer:
			tower = new ArcherTower;
			break;
		case TowerType::Axeman:
			tower = new AxeMan;
			break;
		case TowerType::Gunner:
			tower = new Gunner;
			break;
		default:
			tower = new AxeMan;
			break;
		}

		static Vector2 position;
		static SDL_Rect rect = ConfigManager::instance()->rect_tile_map;
		position.x = (double)(rect.x + idx_pos.x * SIZE_TILE + SIZE_TILE / 2);
		position.y = (double)(rect.y + idx_pos.y * SIZE_TILE + SIZE_TILE / 2);
		tower->set_position(position);


		tower_list.push_back(tower);

		static Map& map = ConfigManager::instance()->map;
		map.place_tower(idx_pos);

		static ResourcesManager* resorcese = ResourcesManager::instance();
		Mix_PlayChannel(-1, resorcese->get_sound_pool().find(ResID::Sound_PlaceTower)->second, 0);
	}

	void upgrade_tower(TowerType tower_type)
	{
		static ConfigManager* instance = ConfigManager::instance();

		switch (tower_type)
		{
		case TowerType::Archer:
			instance->level_archar >= 9 ? 9 : ++instance->level_archar;
			break;
		case TowerType::Axeman:
			instance->level_axeman >= 9 ? 9 : ++instance->level_axeman;
			break;
		case TowerType::Gunner:
			instance->level_gunner >= 9 ? 9 : ++instance->level_gunner;
			break;
		}

		static ResourcesManager* resorcese = ResourcesManager::instance();
		Mix_PlayChannel(-1, resorcese->get_sound_pool().find(ResID::Sound_TowerLevelUp)->second, 0);
	}

private:
	TowerList tower_list;
};


#endif // !_TOWER_MANAGER_
