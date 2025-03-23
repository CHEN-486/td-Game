#ifndef _WAVE_H_
#define _WAVE_H_

#include<vector>

#include"EnemyType.h"

struct Wave
{
	struct SpawnEvent
	{
		double interval = 0;
		int spawn_point = 1;
		EnemyType enemy_type = EnemyType::Slim;
	};

	double interval = 0;
	double rewards = 0;
	std::vector<SpawnEvent> spawn_event_list;
};

#endif // !_WAVE_H_

