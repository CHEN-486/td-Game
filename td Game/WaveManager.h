#ifndef _WAVA_MANAGER_H_
#define _WAVA_MANAGER_H_

#include"timer.h"

#include"manager.h"
#include"ConfigManager.h"
#include"EnemyMannager.h"
#include"CoinManager.h"

class WaveManager : public Manager<WaveManager>
{
	friend Manager<WaveManager>;

public:

	void on_update(double delta)
	{
		static ConfigManager* config = ConfigManager::instance();
		if (config->is_game_over)
			return;
		
		if (!is_wave_start)
			timer_start_wave.on_update(delta);
		else
			timer_spawn_enemy.on_update(delta);

		if (is_spawn_last_enemy && EnemyManager::instance()->check_clear())
		{
			CoinManager::instance()->increase_coin(config->wave_list[idx_wave].rewards);

			idx_wave++;

			if (idx_wave >= config->wave_list.size())
			{
				config->is_game_win = true;
				config->is_game_over = true;
			}
			else
			{
				is_wave_start = false;
				is_spawn_last_enemy = false;
				idx_spaen_enemy = 0;
				
				timer_start_wave.set_wait_time(config->wave_list[idx_wave].interval);
				timer_start_wave.restart();
			}
		}
	}

protected:
	WaveManager()
	{
		static const ConfigManager* config = ConfigManager::instance();
		static const std::vector<Wave>& wave_list = config->wave_list;

		timer_start_wave.set_one_shot(true);
		timer_start_wave.set_wait_time(wave_list[0].interval);
		timer_start_wave.set_on_timeout(
			[&]()
			{
				is_wave_start = true;
				timer_spawn_enemy.set_wait_time(wave_list[0].spawn_event_list[0].interval);
				timer_spawn_enemy.restart();
			}
		);

		timer_spawn_enemy.set_one_shot(true);
		timer_spawn_enemy.set_on_timeout(
			[&]()
			{
				const std::vector<Wave::SpawnEvent>& spawn_event_list = wave_list[idx_wave].spawn_event_list;
				const Wave::SpawnEvent& spawn_event = spawn_event_list[idx_spaen_enemy];

				EnemyManager::instance()->spawn_enemy(spawn_event.enemy_type, spawn_event.spawn_point);
				idx_spaen_enemy++;

				if(idx_spaen_enemy>=spawn_event_list.size())
				{
					is_spawn_last_enemy = true;
					return;
				}

				timer_spawn_enemy.set_wait_time(spawn_event_list[idx_spaen_enemy].interval);
				timer_spawn_enemy.restart();
			}
		);
	}

	~WaveManager() = default;

private:
	int idx_wave = 0;
	int idx_spaen_enemy = 0;
	Timer timer_start_wave;
	Timer timer_spawn_enemy;
	bool is_wave_start = false;
	bool is_spawn_last_enemy = false;
};


#endif // !_WAVA_MANAGER_H_
