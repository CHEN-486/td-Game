#ifndef _BULLET_H_
#define _BULLET_H_

#include<SDL.h>

#include"vector2.h"
#include"ConfigManager.h"
#include"animation.h"
#include"enemy.h"

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	void set_velocity(const Vector2& velocity)
	{
		this->velocity = velocity;

		if (can_rotate)
		{
			double radian = std::atan2(velocity.y, velocity.x);
			angle_anim_rotate = radian * 180 / 3.1415926;
		}
	}

	void set_position(const Vector2& pos)
	{	
		this->position = pos;
	}

	void set_damage(const double& damage)
	{
		this->damage = damage;
	}

	void make_invalided()
	{
		is_valid = false;
		is_collisional = false;
	}

	void make_uncollisioned()
	{
		is_collisional = false;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	const Vector2& get_velocity() const
	{
		return velocity;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	const double& get_damage() const
	{
		return damage;
	}

	const double& get_damage_range() const
	{
		return damage_range;
	}

	bool can_collisional() const
	{
		return is_collisional;
	}

	bool can_remove() const
	{
		return !is_valid;
	}

	virtual void on_update(double delta)
	{
		anim_bullet.on_update(delta);
		
		position += velocity * delta;
		static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
		if (position.x<rect_tile_map.x || position.x>(rect_tile_map.x + rect_tile_map.w) ||
			position.y<rect_tile_map.y || position.y>(rect_tile_map.y + rect_tile_map.h))
		{
			make_invalided();
		}
	}

	virtual void on_render(SDL_Renderer* renderer)
	{
		static SDL_Point pos_dst;
		pos_dst.x = (int)(position.x - size.x / 2);
		pos_dst.y = (int)(position.y - size.y / 2);

		anim_bullet.on_render(renderer,pos_dst,angle_anim_rotate);
	}

	virtual void on_collisioned(Enemy* enemy)
	{
		is_valid = false;
		is_collisional = false;
	}

protected:
	Vector2 size;
	Vector2 velocity;
	Vector2 position;
	
	Animation anim_bullet;
	bool can_rotate = false;

	double damage = 0;
	double damage_range = -1;

private:
	bool is_valid = true;
	bool is_collisional = true;
	double angle_anim_rotate = 0;
};


#endif // !_BULLET_H_

