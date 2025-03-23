#ifndef _SLIM_ENEMY_H_
#define _SLIM_ENEMY_H_

#include<vector>

#include"enemy.h"
#include"ConfigManager.h"
#include"ResourceManager.h"

class SlimeEnemy : public Enemy
{
public:
	SlimeEnemy()
	{
		static const ConfigManager::EnemyTemplate& slime_temeplate =
			ConfigManager::instance()->slim_template;
		static SDL_Texture* tex_slime =
			ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Slime)->second;
		static SDL_Texture* tex_slime_sketch =
			ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_SlimeSketch)->second;

		std::vector<int> idx_up = { 6,7,8,9,10,11 };
		std::vector<int> idx_down = { 0,1,2,3,4,5 };
		std::vector<int> idx_left = { 18,19,20,21,22,23 };
		std::vector<int> idx_right = { 12,13,14,15,16 ,17 };

		anim_up.set_loop(true);anim_up.set_interval(0.1);
		anim_up.set_frame_date(tex_slime, 6, 4, idx_up);
		anim_down.set_loop(true); anim_down.set_interval(0.1);
		anim_down.set_frame_date(tex_slime, 6, 4, idx_down);
		anim_left.set_loop(true); anim_left.set_interval(0.1);
		anim_left.set_frame_date(tex_slime, 6, 4, idx_left);
		anim_right.set_loop(true); anim_right.set_interval(0.1);
		anim_right.set_frame_date(tex_slime, 6, 4, idx_right);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.1);
		anim_up_sketch.set_frame_date(tex_slime_sketch, 6, 4, idx_up);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.1);
		anim_down_sketch.set_frame_date(tex_slime_sketch, 6, 4, idx_down);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.1);
		anim_left_sketch.set_frame_date(tex_slime_sketch, 6, 4, idx_left);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.1);
		anim_right_sketch.set_frame_date(tex_slime_sketch, 6, 4, idx_right);

		max_hp = slime_temeplate.hp;
		hp = max_hp;
		max_speed = slime_temeplate.speed;
		speed = max_speed;
		damage = slime_temeplate.damage;
		reward_ratio = slime_temeplate.reward_ratio;
		recover_interval = slime_temeplate.recover_interval;
		recover_intensity = slime_temeplate.recover_intensity;
		recover_range = slime_temeplate.recover_range;

		size.x = 48;
		size.y = 48;
	}
	~SlimeEnemy();
};

#endif // !_SLIM_H_

