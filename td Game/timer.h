#ifndef _TIMER_H_
#define _TIMER_H_

#include<functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void restart()
	{
		pass_time = 0;
		shotted = false;
	}

	void set_wait_time(double value)
	{
		wait_time = value;
	}

	void pause()
	{
		paused = true;
	}

	void resume()
	{
		paused = false;
	}

	void set_one_shot(bool yn)
	{
		one_shot = yn;
	}

	void set_on_timeout(std::function<void()>callback)
	{
		on_timeout = callback;
	}

	void on_update(double delta)
	{
		if (paused)return;

		pass_time += delta;
		if(pass_time>=wait_time)
		{
			bool can_shot = false;
			if (!one_shot || (one_shot && !shotted))
				can_shot = true;
			pass_time -= wait_time;
			shotted = true;

			if (can_shot && on_timeout)
				on_timeout();
		}
	}

private:
	double wait_time = 0;
	double pass_time = 0;
	bool paused = false;
	bool shotted = false;
	bool one_shot = false;
	std::function<void()> on_timeout;
};


#endif // !_TIMER_H_
