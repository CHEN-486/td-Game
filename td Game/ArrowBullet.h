#ifndef _ARROW_BULLET_H_
#define _ARROW_BULLET_H_

#include<SDL.h>
#include<vector>

#include "bullet.h"
#include"ResourceManager.h"

class ArrowBullet : public Bullet
{
public:
	ArrowBullet()
	{
		static SDL_Texture* tex_bullet = 
			ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_BulletArrow)->second;
		static const std::vector<int> idx_bullet = { 0,1 };

		anim_bullet.set_loop(true);
		anim_bullet.set_interval(0.1);
		anim_bullet.set_frame_date(tex_bullet, 2, 1, idx_bullet);

		can_rotate = true;
		size.x = anim_bullet.get_width_frame();
		size.y = anim_bullet.get_height_frame();
	}

	~ArrowBullet() = default;

	void on_collisioned(Enemy* enemy) override
	{
		Bullet::on_collisioned(enemy);

		static const ResourcesManager::SoundPool& sound_pool = 
			ResourcesManager::instance()->get_sound_pool();
		switch (rand()%3)
		{
		case 0:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_1)->second, 0);
			break;
		case 1:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_2)->second, 0);
			break;
		case 2:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_3)->second, 0);
			break;
		}
	}

};


#endif // !_ARROW_BULLET_H_
