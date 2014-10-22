/*
 * looper.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <functional>
#include <list>

#include <libsc/k60/timer.h>

namespace inno
{

class Looper
{
public:
	typedef std::function<void(const libsc::k60::Timer::TimerInt request,
			const libsc::k60::Timer::TimerInt actual)> Runner;

	Looper();
	~Looper();

	void Loop();
	void RunAfter(const libsc::k60::Timer::TimerInt ms, const Runner &runner);
	void Break();

private:
	struct RunnerState;

	void Scan();

	std::list<RunnerState> m_states;
	bool m_is_run;
};

}
