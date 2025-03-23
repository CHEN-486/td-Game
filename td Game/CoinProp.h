#ifndef _CONIN_PROP_
#define _CONIN_PROP_

#include<SDL.h>

#include"vector2.h"
#include"tile.h"
#include"timer.h"
#include"ConfigManager.h"
#include"ResourceManager.h"

class CoinProp
{
public:
	CoinProp()
	{
		timer_jumping.set_one_shot(true);
		timer_jumping.set_wait_time(interval_jumping);
		timer_jumping.set_on_timeout(
			[&]()
			{
				is_jumping = false;
			}
		);

		timer_disappear.set_one_shot(true);
		timer_disappear.set_wait_time(interval_disappear);
		timer_disappear.set_on_timeout(
			[&]()
			{
				is_valid = false;
			}
		);

		velocity.x = (rand() % 2 ? 1 : -1) * 2 * SIZE_TILE;
		velocity.y = -3 * SIZE_TILE;
	}

	~CoinProp() = default;

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

	void make_invalid()
	{
		is_valid = false;
	}

	bool can_remove() const
	{
		return !is_valid;
	}

	void on_update(double delta)
	{
		timer_jumping.on_update(delta);
		timer_disappear.on_update(delta);

		if (is_jumping)
		{
			velocity.y += delta * gravity;
		}
		else
		{
			velocity.x = 0;
			velocity.y = sin(SDL_GetTicks() / 1000.0 * 4) * 30;
		}

		position += velocity * delta;
	}

	void on_render(SDL_Renderer* renderer)
	{
		static ConfigManager* instance = ConfigManager::instance();
		static SDL_Rect rect_dst = { 0,0,(int)(size.x),(int)(size.y) };
		rect_dst.x = (int)(instance->rect_tile_map.x + position.x  - size.x / 2);
		rect_dst.y = (int)(instance->rect_tile_map.y + position.y  - size.y / 2);
		static SDL_Texture* tex_coin = ResourcesManager::instance()->get_texture_pool()
			.find(ResID::Tex_Coin)->second;
		SDL_RenderCopy(renderer, tex_coin, nullptr, &rect_dst);
	}
private:
	Vector2 position;
	Vector2 velocity;
	Vector2 size = { 16,16 };

	Timer timer_jumping;
	Timer timer_disappear;

	bool is_jumping = true;
	bool is_valid = true;

	double gravity = 490;
	double interval_disappear = 10;
	double interval_jumping = 0.75;
};


#endif // !_CONIN_PROP_

