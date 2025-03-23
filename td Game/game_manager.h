#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

#include"manager.h"
#include"ConfigManager.h"
#include"ResourceManager.h"
#include"EnemyMannager.h"
#include"WaveManager.h"
#include"BulletManager.h"
#include"TowerMananger.h"
#include"CoinManager.h"
#include"play_manager.h"
#include"StateBar.h"
#include"panel.h"
#include"PlacePanel.h"
#include"UpgradePanel.h"
#include"banner.h"
#include"tile.h"

class GameManager : public Manager<GameManager>
{
	friend class Manager<GameManager>;

public:
	int run(int argv,char** argc)
	{
		
		Mix_FadeInMusic(ResourcesManager::instance()->get_music_pool().find(ResID::Music_BGM)->second, -1, 1500);

		Uint64 last_counter = SDL_GetPerformanceCounter();
		Uint64 counter_frq = SDL_GetPerformanceFrequency();

		while (!is_quit)
		{	//input
			while(SDL_PollEvent(&event))
			{
				on_input();
			}

			//delta
			Uint64 current_counter = SDL_GetPerformanceCounter();
			double delta = (double)(current_counter - last_counter) / counter_frq;
			last_counter = current_counter;
			if (delta * 1000 < 1000.0 / 60)
				SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));

			//update
			on_update(delta);
			
			//render
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			on_render();

			SDL_RenderPresent(renderer);
		}

		return 0;
	}

protected:
	GameManager()
	{
		init_assert(!SDL_Init(SDL_INIT_EVERYTHING), u8"SDL2初始化失败");
		init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"IMG初始化失败");
		init_assert(Mix_Init(MIX_INIT_MP3), u8"MIX初始化失败");
		init_assert(!TTF_Init(), u8"TTF初始化失败");

		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

		ConfigManager* mana_config = ConfigManager::instance();
		init_assert(mana_config->map.load("map.csv"), u8"地图加载失败");
		init_assert(mana_config->load_game_config("config.json"), u8"游戏配置加载失败");
		init_assert(mana_config->load_level_config("level.json"), u8"等级配置加载失败");

		window = SDL_CreateWindow(mana_config->basic_template.window_title.c_str()
			, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			mana_config->basic_template.window_width, mana_config->basic_template.window_height, SDL_WINDOW_SHOWN);
		init_assert(window, u8"窗口初始化失败");

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		init_assert(renderer, u8"渲染器初始化失败");

		init_assert(ResourcesManager::instance(), u8"游戏资源管理器初始化失败");
		init_assert(ResourcesManager::instance()->load_from_file(renderer), u8"游戏资源初始化失败");
		place_panel = new  PlacePanel();
		upgrade_panel = new UpgradePanel();

		init_assert(generate_tile_map_texture(), u8"生成瓦片地图失败");

		state_bar.set_position(15, 15);
	}
	~GameManager()//与渲染器初始化顺序相反
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		TTF_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}

private:
	SDL_Window* window = nullptr;

	SDL_Renderer* renderer = nullptr;

	bool is_quit = false;

	SDL_Event event;

	SDL_Texture* tex_tile_map = nullptr;

	StateBar state_bar;

	Banner banner;

	Panel* place_panel = nullptr;
	Panel* upgrade_panel = nullptr;

private:
	void init_assert(bool flag,const char* err_msg)
	{
		if (flag) return;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"游戏初始化失败", err_msg, window);
		exit(-1);
	}

	void on_input()
	{
		static const ConfigManager* instance = ConfigManager::instance();
		static SDL_Point center_pos;
		static SDL_Point idx_selected_tile;

		switch (event.type)
		{
		case SDL_QUIT:
			is_quit = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (instance->is_game_over)
				return;
			if (get_cursor_idx_tile(idx_selected_tile, event.motion.x, event.motion.y))
			{
				get_selected_tile_center_pos(idx_selected_tile,center_pos);

					if (check_home(idx_selected_tile))
					{
						upgrade_panel->set_center_pos(center_pos);
						upgrade_panel->set_idx_tile(idx_selected_tile);
						upgrade_panel->show();
					}
					else if (can_place_tower(idx_selected_tile))
					{;
						place_panel->set_center_pos(center_pos);
						place_panel->set_idx_tile(idx_selected_tile);
						place_panel->show();
					}
				
			}
			break;
		default:
			break;
		}

		if (!instance->is_game_over)
		{
			upgrade_panel->on_input(event);
			place_panel->on_input(event);
			PlayManager::instance()->on_input(event);
		}
	}

	void on_update(double delta)
	{
		static const ConfigManager* config = ConfigManager::instance();
		if (!config->is_game_over)
		{ 
			state_bar.on_update(renderer);
			place_panel->on_update(renderer);
			upgrade_panel->on_update(renderer);
			
			WaveManager::instance()->on_update(delta);
			EnemyManager::instance()->on_update(delta);

			BulletManager::instance()->on_update(delta);
			TowerManager::instance()->on_update(delta);
			CoinManager::instance()->on_update(delta);

			PlayManager::instance()->on_update(delta);

			return;
		}

		banner.on_update(delta);
		if (banner.chect_display_end())
			is_quit = true;
	}

	void on_render()
	{
		static ConfigManager* instance = ConfigManager::instance();
		static SDL_Rect& rec_dst = instance->rect_tile_map;
		SDL_RenderCopy(renderer, tex_tile_map, nullptr, &rec_dst);

		static bool is_game_over_last_tick = false;

		EnemyManager::instance()->on_renderer(renderer);
		BulletManager::instance()->on_render(renderer);
		TowerManager::instance()->on_render(renderer);
		CoinManager::instance()->on_render(renderer);
		PlayManager::instance()->on_render(renderer);

		if (!instance->is_game_over)
		{
			state_bar.on_render(renderer);
			place_panel->on_render(renderer);
			upgrade_panel->on_render(renderer);

			return;
		}

		if (!is_game_over_last_tick && instance->is_game_over)
		{
			const ResourcesManager::SoundPool& sound_pool = ResourcesManager::instance()->get_sound_pool();
			Mix_FadeOutMusic(1500);
			Mix_PlayChannel(-1,
				(instance->is_game_win ? sound_pool.find(ResID::Sound_Win)->second : sound_pool.find(ResID::Sound_Loss)->second), 0);
		}
		is_game_over_last_tick = instance->is_game_over;

		int window_w, window_h;
		SDL_GetWindowSizeInPixels(window, &window_w, &window_h);
		banner.set_center_pos(Vector2({ (double)window_w / 2,(double)window_h / 2 }));
		banner.on_render(renderer);

	}
 
	bool generate_tile_map_texture()
	{
		const Map& map = ConfigManager::instance()->map;
		const TileMap& tile_map = map.get_tile_map();
		SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
		SDL_Texture* tex_tile_set = ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Tileset)->second;
		if (!tex_tile_set)return false;

		int width_tex_tile_set, height_tex_tile_set;
		SDL_QueryTexture(tex_tile_set, nullptr, nullptr, &width_tex_tile_set, &height_tex_tile_set);
		int num_tile_single_line = (int)std::ceil((double)width_tex_tile_set / SIZE_TILE);

		int width_tex_tile_map, height_tex_tile_map;
		width_tex_tile_map = (int)map.get_width() * SIZE_TILE;
		height_tex_tile_map = (int)map.get_height() * SIZE_TILE;
		tex_tile_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, width_tex_tile_map, height_tex_tile_map);
		if (!tex_tile_map) return false;

		ConfigManager* config = ConfigManager::instance();
		rect_tile_map.x = (config->basic_template.window_width - width_tex_tile_map) / 2;
		rect_tile_map.y = (config->basic_template.window_height - height_tex_tile_map) / 2;
		rect_tile_map.w = width_tex_tile_map;
		rect_tile_map.h = height_tex_tile_map;

		SDL_SetTextureBlendMode(tex_tile_map, SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(renderer, tex_tile_map);

		for (int y = 0; y < map.get_height(); y++)
		{
			for (int x = 0; x < map.get_width(); x++)
			{
				SDL_Rect rect_src;
				const Tile& tile = tile_map[y][x];

				const SDL_Rect& rect_dst =
				{
					x * SIZE_TILE, y * SIZE_TILE,
					SIZE_TILE, SIZE_TILE
				};

				rect_src =
				{
					(tile.terrian % num_tile_single_line) * SIZE_TILE,
					(tile.terrian / num_tile_single_line) * SIZE_TILE,
					SIZE_TILE, SIZE_TILE
				};
				SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);

				if (tile.decoration >= 0)
				{
					rect_src =
					{
						(tile.decoration % num_tile_single_line) * SIZE_TILE,
						(tile.decoration / num_tile_single_line) * SIZE_TILE,
						SIZE_TILE, SIZE_TILE
					};
					SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);
				}
			}
		}

		const SDL_Point& idx_home = map.get_idx_home();
		const SDL_Rect rect_dst =
		{
			idx_home.x * SIZE_TILE, idx_home.y * SIZE_TILE,
			SIZE_TILE, SIZE_TILE
		};
		SDL_RenderCopy(renderer, ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Home)->second, nullptr, &rect_dst);

		SDL_SetRenderTarget(renderer, nullptr);

		return true;
	}


	bool get_cursor_idx_tile(SDL_Point& idx_tile_selected, int screen_x, int screen_y)const
	{
		static const Map& map = ConfigManager::instance()->map;
		static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;

		if (screen_x<rect_tile_map.x || screen_x>(rect_tile_map.x + rect_tile_map.w) ||
			screen_y<rect_tile_map.y || screen_y>(rect_tile_map.y + rect_tile_map.h))
			return false;

		idx_tile_selected.x = std::min((screen_x - rect_tile_map.x) / SIZE_TILE, (int)map.get_width() - 1);
		idx_tile_selected.y = std::min((screen_y - rect_tile_map.y) / SIZE_TILE, (int)map.get_height() - 1);

		return true;
	}

	bool can_place_tower(const SDL_Point& idx_tile_selected) const
	{
		static const Map& map = ConfigManager::instance()->map;
		const Tile& tile = map.get_tile_map()[idx_tile_selected.y][idx_tile_selected.x];

		if (tile.decoration < 0 && tile.direction == Tile::Direction::None && !tile.has_tower)
			return true;
		else
		{
			return false;
		}
	}

	bool check_home(const SDL_Point& idx_tile_selected) const
	{
		static const Map& map = ConfigManager::instance()->map;
		static const SDL_Point& idx_home = map.get_idx_home();

		return (idx_home.x == idx_tile_selected.x && idx_home.y == idx_tile_selected.y);
	}

	void get_selected_tile_center_pos(const SDL_Point& idx_tile_selected, SDL_Point& center_pos)
	{
		static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;

		center_pos.x = rect_tile_map.x + idx_tile_selected.x * SIZE_TILE + SIZE_TILE / 2;
		center_pos.y = rect_tile_map.y + idx_tile_selected.y * SIZE_TILE + SIZE_TILE / 2;
	}
};




#endif // !_GAME_MANAGER_H_

