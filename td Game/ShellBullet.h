#ifndef _SHELL_BULLET_H_
#define _SHELL_BULLET_H_

#include<SDL.h>
#include<vector>

#include"ResourceManager.h"
#include"bullet.h"

class ShellBullet : public Bullet
{
public:
	ShellBullet()
	{
		static SDL_Texture* tex_bullet = 
			ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_BulletShell)->second;
		static SDL_Texture* tex_exploed_bullet =
			ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_EffectExplode)->second;

		static const std::vector<int> idx_bullet = { 0,1 };
		static const std::vector<int> idx_exploed_bullet = { 0,1,2,3,4 };

		anim_bullet.set_loop(true);
		anim_bullet.set_interval(0.1);
		anim_bullet.set_frame_date(tex_bullet, 2, 1, idx_bullet);
		anim_exploed_bullet.set_loop(false);
		anim_exploed_bullet.set_interval(0.1);
		anim_exploed_bullet.set_frame_date(tex_exploed_bullet, 5, 1, idx_exploed_bullet);
		anim_exploed_bullet.set_on_finished(
			[&]()
			{
				make_invalided();
			}
		);

		damage_range = 96;
		size.x = anim_bullet.get_width_frame();
		size.y = anim_bullet.get_height_frame();
	}

	~ShellBullet() = default;

	void on_update(double delta) override
	{
		if (can_collisional())
		{
			Bullet::on_update(delta);
			return;
		}

		anim_exploed_bullet.on_update(delta);
	}

	void on_render(SDL_Renderer* renderer) override
	{
		if (can_collisional())
		{
			Bullet::on_render(renderer);
			return;
		}

		static SDL_Point pos_dst;
		pos_dst.x = (int)(position.x - 96 / 2);
		pos_dst.y = (int)(position.y - 96 / 2);
		anim_exploed_bullet.on_render(renderer, pos_dst);
	}
	
	void on_collisioned(Enemy* enemy) override
	{
		make_uncollisioned();

		static const ResourcesManager::SoundPool& sound_pool =
			ResourcesManager::instance()->get_sound_pool();
		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ShellHit)->second, 0);
	}

private:
	Animation anim_exploed_bullet;
};


#endif // !_SHELL_BULLET_H_

