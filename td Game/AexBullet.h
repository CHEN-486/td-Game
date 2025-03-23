#ifndef _AEX_BULLET_H_
#define _AEX_BULLET_H_

#include<SDL.h>
#include<vector>

#include "bullet.h"
#include"ResourceManager.h"

class AxeBullet : public Bullet
{
public:
	AxeBullet()
	{
		static SDL_Texture* tex_bullet =
			ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_BulletAxe)->second;
		static const std::vector<int> idx_bullet = { 0,1,2,3,4,5,6,7 };

		anim_bullet.set_loop(true);
		anim_bullet.set_interval(0.1);
		anim_bullet.set_frame_date(tex_bullet, 4, 2, idx_bullet);

		size.x = anim_bullet.get_width_frame();
		size.y = anim_bullet.get_height_frame();
	}

	~AxeBullet() = default;

	void on_collisioned(Enemy* enemy) override
	{
		Bullet::on_collisioned(enemy);

		static const ResourcesManager::SoundPool& sound_pool =
			ResourcesManager::instance()->get_sound_pool();
		switch (rand() % 3)
		{
		case 0:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeHit_1)->second, 0);
			break;
		case 1:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeHit_2)->second, 0);
			break;
		case 2:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeHit_3)->second, 0);
			break;
		}

		enemy->slow_speed();
	}

};


#endif // !_AEX_BULLET_H_
