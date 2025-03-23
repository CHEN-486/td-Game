#ifndef _TOWER_H_
#define _TOWER_H_

#include<SDL.h>

#include"vector2.h"
#include"animation.h"
#include"TowerType.h"
#include"BulletType.h"
#include"timer.h"
#include"Facing.h"
#include"EnemyMannager.h"
#include"ConfigManager.h"
#include"ResourceManager.h"
#include"BulletManager.h"

class Tower
{
public:
	Tower()
	{
		timer_fire.set_one_shot(true);
		timer_fire.set_on_timeout(
			[&]()
			{
				can_fire = true;
			}
		);
		

		anim_idle_up.set_loop(true);
		anim_idle_up.set_interval(0.2);
		anim_idle_down.set_loop(true);
		anim_idle_down.set_interval(0.2);
		anim_idle_left.set_loop(true);
		anim_idle_left.set_interval(0.2);
		anim_idle_right.set_loop(true);
		anim_idle_right.set_interval(0.2);
		anim_up.set_loop(false);
		anim_up.set_interval(0.2);
		anim_up.set_on_finished(
			[&]()
			{
				update_idle_anim();
			}
		);
		anim_down.set_loop(false);
		anim_down.set_interval(0.2);
		anim_down.set_on_finished(
			[&]()
			{
				update_idle_anim();
			}
		);
		anim_left.set_loop(false);
		anim_left.set_interval(0.2);
		anim_left.set_on_finished(
			[&]()
			{
				update_idle_anim();
			}
		);
		anim_right.set_loop(false);
		anim_right.set_interval(0.2);
		anim_right.set_on_finished(
			[&]()
			{
				update_idle_anim();
			}
		);
	}

	~Tower() = default;

	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	void on_update(double delta)
	{
		timer_fire.on_update(delta);

		anim_current->on_update(delta);

		if (can_fire)
		{
			on_fire();
		}

	}

	void on_render(SDL_Renderer* renderer)
	{
		static SDL_Point pos_dst;
		pos_dst.x = (int)(position.x - size.x / 2);
		pos_dst.y = (int)(position.y - size.y / 2);

		
		anim_current->on_render(renderer, pos_dst);
	}

protected:
	Vector2 size;

	Animation anim_idle_up;
	Animation anim_idle_down;
	Animation anim_idle_left;
	Animation anim_idle_right;
	Animation anim_up;
	Animation anim_down;
	Animation anim_left;
	Animation anim_right;

	TowerType tower_type = TowerType::Archer;

	double bullet_speed = 0;
	BulletType bullet_type = BulletType::Arrow;

private:
	Vector2 position;
	
	Animation* anim_current = &anim_idle_left;

	Timer timer_fire;
	bool can_fire = true;

	Facing facing = Facing::up;

private:
	void update_idle_anim()
	{
		switch (facing)
		{
		case Facing::up:
			anim_current = &anim_idle_up;
			break;
		case Facing::down:
			anim_current = &anim_idle_down;
			break;
		case Facing::left:
			anim_current = &anim_idle_left;
			break;
		case Facing::right:
			anim_current = &anim_idle_right;
			break;
		}
	}

	Enemy* find_target_enemy()
	{
		static const EnemyManager::EnemyList& enemy_list =
			EnemyManager::instance()->get_enemy_list();

		double view_range = 0;
		static const ConfigManager* instance = ConfigManager::instance();
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

		double process = -1;
		double new_process = -1;
		double length = 0;
		Vector2 enemy_pos;
		Enemy* target_enemy = nullptr;
		for (Enemy* enemy : enemy_list)
		{
			enemy_pos = enemy->get_position();
			length = (enemy_pos - position).length();

			if (length <= view_range*SIZE_TILE)
			{
				new_process = enemy->get_route_process();

				if (new_process > process)
				{
					target_enemy = enemy;
					process = new_process;
				}
			}
		}

		return target_enemy;
	}

	void on_fire()
	{
		Enemy* enemy = find_target_enemy();
		if (enemy == nullptr)
		{
			return;
		}

		double interval = 0;
		double damage = 0;
		static const ConfigManager* instance = ConfigManager::instance();
		static const ResourcesManager::SoundPool& sound_pool =
			ResourcesManager::instance()->get_sound_pool();

		switch (tower_type)
		{
		case TowerType::Archer:
			interval = instance->archer_template.interval[instance->level_archar];
			damage = instance->archer_template.damage[instance->level_archar];
			switch (rand() % 2)
			{
			case 0:
				Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowFire_1)->second, 0);
				break;
			case 1:
				Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowFire_2)->second, 0);
				break;
			}
			break;
		case TowerType::Axeman:
			interval = instance->axeman_template.interval[instance->level_axeman];
			damage = instance->axeman_template.damage[instance->level_axeman];
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeFire)->second, 0);
			break;
		case TowerType::Gunner:
			interval = instance->gunner_template.interval[instance->level_gunner];
			damage = instance->gunner_template.damage[instance->level_gunner];
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ShellFire)->second, 0);
			break;
		}

		Vector2 direction = (enemy->get_position() - position).normalize();
		bool is_shown_x = abs(direction.x) >= abs(direction.y);
		if (is_shown_x)
		{
			if (direction.x >= 0)
			{
				facing = Facing::right;
				anim_current = &anim_right;
			}
			else
			{
				facing = Facing::left;
				anim_current = &anim_left;
			}
		}
		else
		{
			if (direction.y >= 0)
			{
				facing = Facing::down;
				anim_current = &anim_down;
			}
			else
			{
				facing = Facing::up;
				anim_current = &anim_up;
			}
		}
		Vector2 bullet_velocity = direction * bullet_speed * SIZE_TILE;
		static BulletManager* bullet_manager = BulletManager::instance();
		bullet_manager->fire_bullet(bullet_type, position, bullet_velocity, damage);

		can_fire = false;
		timer_fire.set_wait_time(interval);
		timer_fire.restart();

		anim_current->reset();
	}
};


#endif // !_TOWER_H_

