/*
 * looper.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <functional>
#include <list>

#include <libsc/k60/system.h>
#include <libsc/k60/timer.h>

#include "looper.h"

using namespace libsc::k60;
using namespace std;

namespace inno
{

struct Looper::RunnerState
{
	Timer::TimerInt request;
	Timer::TimerInt start;
	Runner runner;
};

Looper::Looper()
		: m_is_run(true)
{}

Looper::~Looper()
{}

void Looper::Loop()
{
	Timer::TimerInt prev = System::Time();
	while (m_is_run)
	{
		if (System::Time() != prev)
		{
			Scan();
			prev = now;
		}
	}
}

void Looper::Scan()
{
	list<RunnerState>::iterator it = m_states.begin();
	while (it != m_states.end())
	{
		const Timer::TimerInt duration = Timer::TimeDiff(System::Time(),
				it->start);
		if (duration >= it->request)
		{
			it->runner(it->request, duration);
			it = m_states.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Looper::RunAfter(const Timer::TimerInt ms, const Runner &runner)
{
	RunnerState rs;
	rs.request = ms;
	rs.start = System::Time();
	rs.runner = runner;
	m_states.push_back(std::move(rs));
}

void Looper::Break()
{
	m_is_run = false;
}

}
