#ifndef _PLAY_MANAGER_H_
#define _PLAY_MANAGER_H_

#include<SDL.h>
#include<vector>

#include"manager.h"
#include"vector2.h"
#include"timer.h"
#include"animation.h"
#include"ConfigManager.h"
#include"ResourceManager.h"
#include"EnemyMannager.h"
#include"CoinManager.h"

class PlayManager :public Manager<PlayManager>
{
	friend Manager<PlayManager>;
public:
	enum class Facing
	{
		Up,
		Down,
		Left,
		Right
	};
protected:
	PlayManager()
	{
		timer_increase_mp.set_one_shot(false);
		timer_increase_mp.set_wait_time(0.1);
		timer_increase_mp.set_on_timeout([&]()
			{
				static double increase = 0;
				increase = 100 / (ConfigManager::instance()->player_template.skill_interval / 0.1);
				mp = std::min((mp + increase), (double)100);
			});

		timer_release_flash.set_one_shot(true);
		timer_release_flash.set_wait_time(ConfigManager::instance()->player_template.normal_attack_interval);
		timer_release_flash.set_on_timeout([&]() {
			can_release_flash = true;
			});

		static SDL_Texture* tex_player = ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Player)->second;

		static std::vector<int> tex_idle_index= { 4,5,6,7 };
		anime_player_idle_up.set_loop(true);
		anime_player_idle_up.set_interval(0.1);
		anime_player_idle_up.set_frame_date(tex_player, 4, 8, tex_idle_index);
		tex_idle_index = { 0,1,2,3 };
		anime_player_idle_down.set_loop(true);
		anime_player_idle_down.set_interval(0.1);
		anime_player_idle_down.set_frame_date(tex_player, 4, 8, tex_idle_index);
		tex_idle_index = { 8,9,10,11 };
		anime_player_idle_left.set_loop(true);
		anime_player_idle_left.set_interval(0.1);
		anime_player_idle_left.set_frame_date(tex_player, 4, 8, tex_idle_index);
		tex_idle_index = { 12,13,14,15 };
		anime_player_idle_right.set_loop(true);
		anime_player_idle_right.set_interval(0.1);
		anime_player_idle_right.set_frame_date(tex_player, 4, 8, tex_idle_index);

		static std::vector<int> tex_attack_index= { 20,21 };
		anime_player_attack_up.set_loop(true);
		anime_player_attack_up.set_interval(0.1);
		anime_player_attack_up.set_frame_date(tex_player, 4, 8, tex_attack_index);
		tex_attack_index = { 16,17 };
		anime_player_attack_down.set_loop(true);
		anime_player_attack_down.set_interval(0.1);
		anime_player_attack_down.set_frame_date(tex_player, 4, 8, tex_attack_index);
		tex_attack_index = { 24,25 };
		anime_player_attack_left.set_loop(true);
		anime_player_attack_left.set_interval(0.1);
		anime_player_attack_left.set_frame_date(tex_player, 4, 8, tex_attack_index);
		tex_attack_index = { 28,29 };
		anime_player_attack_right.set_loop(true);
		anime_player_attack_right.set_interval(0.1);
		anime_player_attack_right.set_frame_date(tex_player, 4, 8, tex_attack_index);

		static SDL_Texture* tex_flash = ResourcesManager::instance()->get_texture_pool().
			find(ResID::Tex_EffectFlash_Up)->second;
		anime_flash_up.set_loop(false);
		anime_flash_up.set_interval(0.1);
		anime_flash_up.set_frame_date(tex_flash, 5, 1, { 0,1,2,3,4 });
		anime_flash_up.set_on_finished([&]() {is_release_flash = false; });
		tex_flash = ResourcesManager::instance()->get_texture_pool().
			find(ResID::Tex_EffectFlash_Down)->second;
		anime_flash_down.set_loop(false);
		anime_flash_down.set_interval(0.1);
		anime_flash_down.set_frame_date(tex_flash, 5, 1, { 0,1,2,3,4 });
		anime_flash_down.set_on_finished([&]() {is_release_flash = false; });
		tex_flash = ResourcesManager::instance()->get_texture_pool().
			find(ResID::Tex_EffectFlash_Left)->second;
		anime_flash_left.set_loop(false);
		anime_flash_left.set_interval(0.1);
		anime_flash_left.set_frame_date(tex_flash, 1, 5, { 0,1,2,3,4 });
		anime_flash_left.set_on_finished([&]() {is_release_flash = false; });
		tex_flash = ResourcesManager::instance()->get_texture_pool().
			find(ResID::Tex_EffectFlash_Right)->second;
		anime_flash_right.set_loop(false);
		anime_flash_right.set_interval(0.1);
		anime_flash_right.set_frame_date(tex_flash, 1, 5, { 0,1,2,3,4 });
		anime_flash_right.set_on_finished([&]() {is_release_flash = false; });

		static SDL_Texture* tex_impact = ResourcesManager::instance()->get_texture_pool().
			find(ResID::Tex_EffectImpact_Up)->second;
		anime_impact_up.set_loop(false);
		anime_impact_up.set_interval(0.1);
		anime_impact_up.set_frame_date(tex_impact, 5, 1, { 0,1,2,3,4 });
		anime_impact_up.set_on_finished([&]() {is_release_impact = false; });
		tex_impact = ResourcesManager::instance()->get_texture_pool().
			find(ResID::Tex_EffectImpact_Down)->second;
		anime_impact_down.set_loop(false);
		anime_impact_down.set_interval(0.1);
		anime_impact_down.set_frame_date(tex_impact, 5, 1, { 0,1,2,3,4 });
		anime_impact_down.set_on_finished([&]() {is_release_impact = false; });
		tex_impact = ResourcesManager::instance()->get_texture_pool().
			find(ResID::Tex_EffectImpact_Left)->second;
		anime_impact_left.set_loop(false);
		anime_impact_left.set_interval(0.1);
		anime_impact_left.set_frame_date(tex_impact, 1, 5, { 0,1,2,3,4 });
		anime_impact_left.set_on_finished([&]() {is_release_impact = false; });
		tex_impact = ResourcesManager::instance()->get_texture_pool().
			find(ResID::Tex_EffectImpact_Right)->second;
		anime_impact_right.set_loop(false);
		anime_impact_right.set_interval(0.1);
		anime_impact_right.set_frame_date(tex_impact, 1, 5, { 0,1,2,3,4 });
		anime_impact_right.set_on_finished([&]() {is_release_impact = false; });

		size.x = 96, size.y = 96;

		const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
		position.x = rect_tile_map.x + rect_tile_map.w / 2;
		position.y = rect_tile_map.y + rect_tile_map.h / 2;

		speed = ConfigManager::instance()->player_template.speed;
	}

	~PlayManager() = default;

public:
	void  on_input(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				is_move_up = true;
				break;
			case SDLK_s:
				is_move_down = true;
				break;
			case SDLK_a:
				is_move_left = true;
				break;
			case SDLK_d:
				is_move_right = true;
				break;
			case  SDLK_j:
				click_on_flash();
				break;
			case SDLK_k:
				click_on_impact();
				break;
			default:
				break;
			}
		}
			break;
		case SDL_KEYUP:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				is_move_up = false;
				break;
			case SDLK_s:
				is_move_down = false;
				break;
			case SDLK_a:
				is_move_left = false;
				break;
			case SDLK_d:
				is_move_right = false;
				break;
			default:
				break;
			}
		}
			break;
		default:
			break;
		}
	}
	void  on_update(double delta)
	{
		timer_increase_mp.on_update(delta);
		timer_release_flash.on_update(delta);

		if (!is_release_flash && !is_release_impact)
		{
			Vector2 direction = Vector2(is_move_right - is_move_left, is_move_down - is_move_up).normalize();
			velocity = direction * speed * SIZE_TILE;
			position += velocity * delta;

			const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
			if (position.x < rect_tile_map.x) position.x = rect_tile_map.x;
			if (position.x > (rect_tile_map.x + rect_tile_map.w)) position.x = (rect_tile_map.x + rect_tile_map.w);
			if (position.y < rect_tile_map.y) position.y = rect_tile_map.y;
			if (position.y > (rect_tile_map.y + rect_tile_map.h)) position.y = (rect_tile_map.y + rect_tile_map.h);

			if (direction.y > 0)facing = Facing::Down;
			if (direction.y < 0)facing = Facing::Up;
			if (direction.x > 0)facing = Facing::Right;
			if (direction.x < 0)facing = Facing::Left;

			switch (facing)
			{
			case PlayManager::Facing::Up:
				current_player_anieme = &anime_player_idle_up;
				break;
			case PlayManager::Facing::Down:
				current_player_anieme = &anime_player_idle_down;
				break;
			case PlayManager::Facing::Left:
				current_player_anieme = &anime_player_idle_left;
				break;
			case PlayManager::Facing::Right:
				current_player_anieme = &anime_player_idle_right;
				break;
			}
		}
		else
		{
			switch (facing)
			{
			case PlayManager::Facing::Up:
				current_player_anieme = &anime_player_attack_up;
				break;
			case PlayManager::Facing::Down:
				current_player_anieme = &anime_player_attack_down;
				break;
			case PlayManager::Facing::Left:
				current_player_anieme = &anime_player_attack_left;
				break;
			case PlayManager::Facing::Right:
				current_player_anieme = &anime_player_attack_right;
				break;
			}
		}
		current_player_anieme->on_update(delta);

		if (is_release_flash)
		{
			current_flash_anime->on_update(delta);

		}

		if (is_release_impact)
		{
			current_impact_anime->on_update(delta);
		}

		CoinManager::CoinList coin_list = CoinManager::instance()->get_coin_list();
			for (CoinProp* coin : coin_list)
			{
				Vector2 coin_pos = coin->get_position();
				if (coin_pos.x >= position.x - size.x / 2
					&& coin_pos.x <= position.x + size.x / 2
					&& coin_pos.y >= position.y - size.y / 2
					&& coin_pos.y <= position.y + size.y / 2)
				{
					coin->make_invalid();
					CoinManager::instance()->increase_coin(10);

					Mix_PlayChannel(-1, ResourcesManager::instance()->get_sound_pool().
						find(ResID::Sound_Coin)->second, 0);
				}
			}
	}
	void on_render(SDL_Renderer* renderer)
	{
		static SDL_Point point_player;
		point_player.x = position.x - size.x / 2;
		point_player.y = position.y - size.y / 2;

		current_player_anieme->on_render(renderer, point_player);

		if (is_release_flash)
		{
			static SDL_Point point_flash;
			point_flash.x = rect_flash_hitbox.x;
			point_flash.y = rect_flash_hitbox.y;
			current_flash_anime->on_render(renderer, point_flash);
		}


		if (is_release_impact)
		{
			static SDL_Point point_impact;
			point_impact.x = rect_impact_hitbox.x;
			point_impact.y = rect_impact_hitbox.y;
			current_impact_anime->on_render(renderer, point_impact);
		}
	}

	double get_current_mp()const
	{
		return mp;
	}

private:
	void click_on_flash()
	{
		if (!can_release_flash || is_release_flash)
			return;

		switch (facing)
		{
		case PlayManager::Facing::Up:
			current_flash_anime = &anime_flash_up;
			rect_flash_hitbox.x = (int)(position.x - 68 / 2);
			rect_flash_hitbox.y = (int)(position.y - size.y / 2 - 300);
			rect_flash_hitbox.w = 68, rect_flash_hitbox.h = 300;
			break;
		case PlayManager::Facing::Down:
			current_flash_anime = &anime_flash_down;
			rect_flash_hitbox.x = (int)(position.x - 68 / 2);
			rect_flash_hitbox.y = (int)(position.y + size.y / 2);
			rect_flash_hitbox.w = 68, rect_flash_hitbox.h = 300;
			break;
		case PlayManager::Facing::Left:
			current_flash_anime = &anime_flash_left;
			rect_flash_hitbox.x = (int)(position.x - size.x / 2 - 300);
			rect_flash_hitbox.y = (int)(position.y - 68 / 2);
			rect_flash_hitbox.w = 300, rect_flash_hitbox.h = 68;
			break;
		case PlayManager::Facing::Right:
			current_flash_anime = &anime_flash_right;
			rect_flash_hitbox.x = (int)(position.x + size.x / 2);
			rect_flash_hitbox.y = (int)(position.y - 68 / 2);
			rect_flash_hitbox.w = 300, rect_flash_hitbox.h = 68;
			break;
		}

		EnemyManager::EnemyList& enemy_list = EnemyManager::instance()->get_enemy_list();

		for (Enemy* enemy : enemy_list)
		{
			if (enemy->can_remove())
				continue;

			const Vector2& enemy_pos = enemy->get_position();
			if (enemy_pos.x > rect_flash_hitbox.x && enemy_pos.x<(rect_flash_hitbox.x + rect_flash_hitbox.w)
				&& enemy_pos.y>rect_flash_hitbox.y && enemy_pos.y < (rect_flash_hitbox.y + rect_flash_hitbox.h))
			{
				enemy->decrease_hp(ConfigManager::instance()->player_template.normal_attack_damage);
			}
		}

		can_release_flash = false;
		is_release_flash = true;
		current_flash_anime->reset();
		timer_release_flash.restart();

		const static ResourcesManager::SoundPool soud_pool = ResourcesManager::instance()->get_sound_pool();

		Mix_PlayChannel(-1, soud_pool.find(ResID::Sound_Flash)->second, 0);
	}
	void click_on_impact()
	{
		if (mp < 100 || is_release_impact)
			return;

		switch (facing)
		{
		case PlayManager::Facing::Up:
			current_impact_anime = &anime_impact_up;
			rect_impact_hitbox.x = (int)(position.x - 140 / 2);
			rect_impact_hitbox.y = (int)(position.y - size.y / 2 - 60);
			rect_impact_hitbox.w = 140, rect_impact_hitbox.h = 60;
			break;
		case PlayManager::Facing::Down:
			current_impact_anime = &anime_impact_down;
			rect_impact_hitbox.x = (int)(position.x - 140 / 2);
			rect_impact_hitbox.y = (int)(position.y + size.y / 2);
			rect_impact_hitbox.w = 140, rect_impact_hitbox.h = 60;
			break;
		case PlayManager::Facing::Left:
			current_impact_anime = &anime_impact_left;
			rect_impact_hitbox.x = (int)(position.x - size.x / 2 - 60);
			rect_impact_hitbox.y = (int)(position.y - 140 / 2);
			rect_impact_hitbox.w = 60, rect_impact_hitbox.h = 140;
			break;
		case PlayManager::Facing::Right:
			current_impact_anime = &anime_impact_right;
			rect_impact_hitbox.x = (int)(position.x + size.x / 2);
			rect_impact_hitbox.y = (int)(position.y - 140 / 2);
			rect_impact_hitbox.w = 60, rect_impact_hitbox.h = 140;
			break;
		}

		EnemyManager::EnemyList& enemy_list = EnemyManager::instance()->get_enemy_list();

		for (Enemy* enemy : enemy_list)
		{
			if (enemy->can_remove())
				continue;

			const Vector2& enemy_pos = enemy->get_position();
			if (enemy_pos.x > rect_impact_hitbox.x && enemy_pos.x<(rect_impact_hitbox.x + rect_impact_hitbox.w)
				&& enemy_pos.y>rect_impact_hitbox.y && enemy_pos.y < (rect_impact_hitbox.y + rect_impact_hitbox.h))
			{
				enemy->decrease_hp(ConfigManager::instance()->player_template.skill_damage);
				enemy->slow_speed();
			}
		}

		mp = 0;
		is_release_impact = true;
		current_impact_anime->reset();

		const static ResourcesManager::SoundPool soud_pool = ResourcesManager::instance()->get_sound_pool();

		Mix_PlayChannel(-1, soud_pool.find(ResID::Sound_Impact)->second, 0);
	}

private:
	Vector2 size;
	Vector2 position;
	Vector2 velocity;

	double speed = 0;

	double mp = 100;

	Facing facing = Facing::Left;

	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;

	bool can_release_flash = true;
	bool is_release_flash = false;
	bool is_release_impact = false;

	Timer timer_increase_mp;
	Timer timer_release_flash;
	
	Animation anime_player_idle_up;
	Animation anime_player_idle_down;
	Animation anime_player_idle_left;
	Animation anime_player_idle_right;
	Animation anime_player_attack_up;
	Animation anime_player_attack_down;
	Animation anime_player_attack_left;
	Animation anime_player_attack_right;
	Animation* current_player_anieme = nullptr;

	SDL_Rect rect_flash_hitbox = { 0 };
	SDL_Rect rect_impact_hitbox = { 0 };

	Animation anime_flash_up;
	Animation anime_flash_down;
	Animation anime_flash_left;
	Animation anime_flash_right;
	Animation* current_flash_anime = nullptr;

	Animation anime_impact_up;
	Animation anime_impact_down;
	Animation anime_impact_left;
	Animation anime_impact_right;
	Animation* current_impact_anime = nullptr;

};


#endif // !_PLAY_MANAGER_H_

