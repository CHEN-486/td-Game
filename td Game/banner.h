#ifndef _BANNER_H_
#define _BANNER_H_

#include"vector2.h"
#include"timer.h"
#include"ResourceManager.h"
#include"ConfigManager.h"

#include<SDL.h>

class Banner
{
public:
	Banner()
	{
		timer_display.set_one_shot(true);
		timer_display.set_wait_time(5);
		timer_display.set_on_timeout([&]() {is_end_display = true; });

		size_background = { 1282,209 };
		size_foreground = { 646,215 };
	}

	~Banner() = default;

public:
	bool chect_display_end() const
	{
		return is_end_display;
	}

	void set_center_pos(const Vector2& vec)
	{
		center_pos = vec;
	}

	void on_update(double delta)
	{
		timer_display.on_update(delta);

		const ResourcesManager::TexturePool tex_pool = ResourcesManager::instance()->get_texture_pool();
		tex_background = tex_pool.find(ResID::Tex_UIGameOverBar)->second;
		tex_foreground = (ConfigManager::instance()->is_game_win ?
			tex_pool.find(ResID::Tex_UIWinText)->second : tex_pool.find(ResID::Tex_UILossText)->second);
	}

	void on_render(SDL_Renderer* renderer)
	{
		static SDL_Rect rect_dst;
		rect_dst.x = center_pos.x - size_background.x / 2;
		rect_dst.y = center_pos.y - size_background.y / 2;
		rect_dst.w = size_background.x;
		rect_dst.h = size_background.y;

		SDL_RenderCopy(renderer, tex_background, nullptr, &rect_dst);

		rect_dst.x = center_pos.x - size_foreground.x / 2;
		rect_dst.y = center_pos.y - size_foreground.y / 2;
		rect_dst.w = size_foreground.x;
		rect_dst.h = size_foreground.y;

		SDL_RenderCopy(renderer, tex_foreground, nullptr, &rect_dst);
	}

private:
	Vector2 center_pos;

	Vector2 size_foreground;
	Vector2 size_background;

	SDL_Texture* tex_foreground;
	SDL_Texture* tex_background;

	Timer timer_display;
	bool is_end_display = false;
};


#endif // !_BANNER_H_

