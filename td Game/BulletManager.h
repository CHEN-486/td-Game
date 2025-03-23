#ifndef _BULLET_MANAGER_H_
#define _BULLET_MANAGER_H_

#include<vector>

#include"manager.h"
#include"bullet.h"
#include"BulletType.h"
#include"ArrowBullet.h"
#include"AexBullet.h"
#include"ShellBullet.h"

class BulletManager : public Manager<BulletManager>
{
	friend Manager<BulletManager>;

public:
	typedef std::vector<Bullet*> BulletList;

protected:
	BulletManager() = default;
	~BulletManager()
	{
		for (Bullet* bullet : bullet_list)
			delete bullet;
	}

public:
	void on_update(double delta)
	{
		for (Bullet* bullet : bullet_list)
			bullet->on_update(delta);

		remove_invaliaed_bullet();
	}

	void on_render(SDL_Renderer* renderer)
	{
		for (Bullet* bullet : bullet_list)
			bullet->on_render(renderer);

	}

	BulletList& get_bullet_list()
	{
		return bullet_list;
	}

	void fire_bullet(BulletType bullet_type, const Vector2& pos, const Vector2& Velocity, const double& damage)
	{
		Bullet* bullet = nullptr;

		switch (bullet_type)
		{
		case BulletType::Arrow:
			bullet = new ArrowBullet();
			break;
		case BulletType::Axe:
			bullet = new AxeBullet();
			break;
		case BulletType::Shell:
			bullet = new ShellBullet();
			break;
		default:
			bullet = new ArrowBullet();
			break;
		}

		bullet->set_damage(damage);

		bullet->set_position(pos);

		bullet->set_velocity(Velocity);

		bullet_list.push_back(bullet);
	}

private:
	BulletList bullet_list;

private:
	void remove_invaliaed_bullet()
	{
		bullet_list.erase(
			std::remove_if(bullet_list.begin(), bullet_list.end(),
				[](const Bullet* bullet)
				{
					bool deletable = bullet->can_remove();
					if (deletable) delete bullet;
					return deletable;
				}), bullet_list.end()
					);
		
	}
};


#endif // !_BULLET_MANAGER_H_

