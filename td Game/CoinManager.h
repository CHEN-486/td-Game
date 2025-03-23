#ifndef _CONIN_MANAGER_H_
#define _CONIN_MANAGER_H_

#include<vector>
#include<SDL.h>

#include"manager.h"
#include"ConfigManager.h"
#include"CoinProp.h"

class CoinManager : public Manager<CoinManager>
{
	friend Manager<CoinManager>;

public:
	typedef std::vector<CoinProp*> CoinList;

public:

	void increase_coin(double val)
	{
		num_coin += val;
	}

	void decrease_coin(double val)
	{
		num_coin -= val;
		if (num_coin < 0)
			num_coin = 0;
	}

	double get_current_coin_num()
	{
		return num_coin;
	}

	CoinList& get_coin_list()
	{
		return coin_list;
	}

	void on_update(double delta)
	{
		for (CoinProp* coin_prop : coin_list)\
			coin_prop->on_update(delta);

		remove_invalid_coin_prop();
	}

	void on_render(SDL_Renderer* renderer)
	{
		for (CoinProp* coin_prop : coin_list)\
			coin_prop->on_render(renderer);
	}

	void spawn_coin_prop(const Vector2& position)
	{
		CoinProp* coin_prop = new CoinProp;
		coin_prop->set_position(position);
		coin_list.push_back(coin_prop);
	}

protected:
	CoinManager()
	{
		num_coin = ConfigManager::instance()->num_initial_coin;
		
	}
	~CoinManager() = default;

private:
	double num_coin = 0;
	CoinList coin_list;

private:
	void remove_invalid_coin_prop()
	{
		coin_list.erase(std::remove_if(coin_list.begin(), coin_list.end(),
			[](const CoinProp* coin_prop)
			{
				bool deletable = coin_prop->can_remove();
				if (deletable) delete coin_prop;
				return deletable;
			}
		), coin_list.end());
	}
};


#endif // !_CONIN_MANAGER_H_

