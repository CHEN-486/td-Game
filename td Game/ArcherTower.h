#ifndef _ARCHER_TOWER_H_
#define _ARCHER_TOWER_H_

#include<SDL.h>
#include<vector>

#include "Tower.h"
#include "ResourceManager.h"

class ArcherTower : public Tower
{
public:
	ArcherTower()
	{
		static const ResourcesManager* instance = ResourcesManager::instance();
		static SDL_Texture* tex_template = 
			instance->get_texture_pool().find(ResID::Tex_Archer)->second;

		static std::vector<int> idx_idle_up = { 3,4 };
		static std::vector<int> idx_idle_down = { 0,1 };
		static std::vector<int> idx_idle_left = { 6,7 };
		static std::vector<int> idx_idle_right = { 9,10 };
		static std::vector<int> idx_fire_up = { 15,16,17 };
		static std::vector<int> idx_fire_down = { 12,13,14 };
		static std::vector<int> idx_fire_left = { 18,19,20 };
		static std::vector<int> idx_fire_right = { 21,22,23 };

		anim_idle_up.set_frame_date(tex_template, 3, 8, idx_idle_up);
		anim_idle_down.set_frame_date(tex_template, 3, 8, idx_idle_down);
		anim_idle_left.set_frame_date(tex_template, 3, 8, idx_idle_left);
		anim_idle_right.set_frame_date(tex_template, 3, 8, idx_idle_right);
		anim_up.set_frame_date(tex_template, 3, 8,   idx_fire_up);
		anim_down.set_frame_date(tex_template, 3, 8, idx_fire_down);
		anim_left.set_frame_date(tex_template, 3, 8, idx_fire_left);
		anim_right.set_frame_date(tex_template, 3, 8,idx_fire_right);

		size.x = anim_down.get_width_frame();
		size.y = anim_down.get_height_frame();
		tower_type = TowerType::Archer;

		bullet_speed = 6;
		bullet_type = BulletType::Arrow;
	}

	~ArcherTower() = default;
private:

};


#endif // !_ARCHER_TOWER_H_

