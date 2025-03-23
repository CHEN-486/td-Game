#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include<vector>
#include<SDL.h>

#include"manager.h"
#include"ConfigManager.h"
#include"HomeManager.h"
#include"BulletManager.h"
#include"CoinManager.h"

#include"enemy.h"
#include"SlimEnemy.h"
#include"king_slim_enemy.h"
#include"skeleton_enemy.h"
#include"goblin_enemy.h"
#include"goblin_priest_enemy.h"

#include"vector2.h"



class EnemyManager : public Manager<EnemyManager>
{
	friend Manager<EnemyManager>;

public:
	typedef std::vector<Enemy*> EnemyList;

public:
	void on_update(double delta)
	{
		for (Enemy* enemy : enemy_list)
			enemy->on_update(delta);

		process_bullet_collision();

		process_home_collision();

		remove_invalied_enemy();
	}

	void on_renderer(SDL_Renderer* renderer)
	{
		for (Enemy* enemy : enemy_list)
			enemy->on_render(renderer);
	}
	
	void spawn_enemy(EnemyType type, int idx_spawn_point)
	{
		static const Map::SpawnerRoutePool& spawner_route_pool =
			ConfigManager::instance()->map.get_spawner_route_pool();
		static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
		static  Vector2 position;

		const auto& itor = spawner_route_pool.find(idx_spawn_point);
		if (itor == spawner_route_pool.end())
			return;

		Enemy* enemy = nullptr;
		switch (type)
		{
		case EnemyType::Slim:
			enemy = new SlimeEnemy();
			break;
		case EnemyType::KingSlim:
			enemy = new KingSlimeEnemy();
			break;
		case EnemyType::Skeleton:
			enemy = new SkeletonEnemy();
			break;
		case EnemyType::Goblin:
			enemy = new GoblinEnemy();
			break;
		case EnemyType::GoblinPriest:
			enemy = new GoblinPriestEnemy();
			break;
		default:
			enemy = new SlimeEnemy();
			break;
		}

		enemy->set_on_skill_released(
			[&](Enemy* enemy_src)
			{
				double recover_raduis = enemy_src->get_recover_radius() * SIZE_TILE;
				if (recover_raduis < 0)return;

				
				const Vector2& pos_src = enemy_src->get_position();
				for (Enemy* enemy_dst : enemy_list)
				{
					const Vector2& pos_dst = enemy_dst->get_position();
					const double length = (pos_dst - pos_src).length();
					
					if (length <= recover_raduis)
						enemy_dst->increase_hp(enemy_src->get_recover_intensity());
				}
			}
		);

		const Route::IdxList& idx_list = spawner_route_pool.find(idx_spawn_point)->second.get_idx_list();
		position.x = (double)(rect_tile_map.x + idx_list[0].x * SIZE_TILE + SIZE_TILE / 2);
		position.y = (double)(rect_tile_map.y + idx_list[0].y * SIZE_TILE + SIZE_TILE / 2);

		enemy->set_position(position);
		enemy->set_route(&itor->second);

		enemy_list.push_back(enemy);
	}

	bool check_clear() const
	{
		return enemy_list.empty();
	}

	EnemyList& get_enemy_list()
	{
		return enemy_list;
	}

protected:
	EnemyManager() = default;
	~EnemyManager()
	{
		for (Enemy* enemy : enemy_list)
			delete enemy;
	}

private:
	EnemyList enemy_list;

private:
	void process_home_collision()
	{
		static const SDL_Point& idx_home = ConfigManager::instance()->map.get_idx_home();
		static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
		static const Vector2 vec_home =
		{
			(double)(rect_tile_map.x + idx_home.x * SIZE_TILE),
			(double)(rect_tile_map.y + idx_home.y * SIZE_TILE)
		};

		for (Enemy* enemy : enemy_list)
		{
			const Vector2& position = enemy->get_position();
			if (position.x > vec_home.x && position.y > vec_home.y
				&& position.x < (vec_home.x + SIZE_TILE) && position.y < (vec_home.y + SIZE_TILE))
			{
				enemy->make_invalid();


				HomeManager::instance()->decrease_hp(enemy->get_damage());
			}
			
		}
	}

	void process_bullet_collision()
	{
		for (Enemy* enemy : enemy_list)
		{
			if (enemy->can_remove()) continue;

			const Vector2& pos_enemy = enemy->get_position();
			const Vector2& size_enemy = enemy->get_size();
			const double reward_ration = enemy->get_reward_ratio();

			static BulletManager* bullet_manager = BulletManager::instance();
			BulletManager::BulletList bullet_list = bullet_manager->get_bullet_list();
			for (Bullet* bullet : bullet_list)
			{
				if (bullet->can_remove()) continue;

				const Vector2& pos_bullet = bullet->get_position();
				const double damage = bullet->get_damage();

				if (pos_bullet.x > pos_enemy.x - size_enemy.x / 2
					&& pos_bullet.y > pos_enemy.y - size_enemy.y / 2
					&& pos_bullet.x < pos_enemy.x + size_enemy.x / 2
					&& pos_bullet.y < pos_enemy.y + size_enemy.y / 2)
				{
					const double damage_range = bullet->get_damage_range();

					if(damage_range<0)
					{
						enemy->decrease_hp(damage);
						if (enemy->can_remove())
							try_spawn_coin_prop(pos_enemy, reward_ration);
					}
					else
					{
						for (Enemy* enemy_target : enemy_list)
						{
							if (enemy_target->can_remove()) continue;

							bool be_explosed = (enemy_target->get_position() - pos_bullet).length() <= damage_range;
							if (be_explosed)
							{
								enemy_target->decrease_hp(damage);
								if (enemy_target->can_remove())
									try_spawn_coin_prop(enemy_target->get_position(), enemy_target->get_reward_ratio());
							}
						}
					}

					bullet->on_collisioned(enemy);
				}
			}
		}
	}

	void remove_invalied_enemy()
	{
		enemy_list.erase(std::remove_if(enemy_list.begin(), enemy_list.end(),
			[](const Enemy* enemy)
			{
				bool deletable = enemy->can_remove();
				if (deletable)delete enemy;
				return deletable;
			}), enemy_list.end());
	}

	void try_spawn_coin_prop(const Vector2& position , const double reward_ratio)
	{
		if ((rand() % 100) / 100.0 <= reward_ratio)
		{
			static CoinManager* instance = CoinManager::instance();
			instance->spawn_coin_prop(position);
		}
	}
};


#endif // !_ENEMY_MANAGER_H_

