#ifndef _GUNNER_H
#define _GUNNER_H

#include<SDL.h>
#include<vector>

#include "Tower.h"
#include "ResourceManager.h"

class Gunner : public Tower
{
public:
	Gunner()
	{
		static const ResourcesManager* instance = ResourcesManager::instance();
		static SDL_Texture* tex_template =
			instance->get_texture_pool().find(ResID::Tex_Gunner)->second;

		static std::vector<int> idx_idle_up = { 4,5 };
		static std::vector<int> idx_idle_down = { 0,1 };
		static std::vector<int> idx_idle_right = { 8,9 };
		static std::vector<int> idx_idle_left = { 12,13 };
		static std::vector<int> idx_fire_up = { 20,21,22,23 };
		static std::vector<int> idx_fire_down = { 16,17,18,19 };
		static std::vector<int> idx_fire_right = { 24,25,26,27 };
		static std::vector<int> idx_fire_left = { 28,29,30,31 };

		anim_idle_up.set_frame_date(tex_template, 4, 8, idx_idle_up);
		anim_idle_down.set_frame_date(tex_template, 4, 8, idx_idle_down);
		anim_idle_left.set_frame_date(tex_template, 4, 8, idx_idle_left);
		anim_idle_right.set_frame_date(tex_template, 4, 8, idx_idle_right);
		anim_up.set_frame_date(tex_template, 4, 8, idx_fire_up);
		anim_down.set_frame_date(tex_template, 4, 8, idx_fire_up);
		anim_left.set_frame_date(tex_template, 4, 8, idx_fire_up);
		anim_right.set_frame_date(tex_template, 4, 8, idx_fire_up);

		size.x = anim_down.get_width_frame();
		size.y = anim_down.get_height_frame();
		tower_type = TowerType::Gunner;

		bullet_speed = 6;
		bullet_type = BulletType::Shell;
	}

	~Gunner() = default;

private:

};


#endif // !_SHELL_TOWER_H

