/*
 * app.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <functional>

#include <libsc/k60/system.h>
#include <libutil/misc.h>

#include "app.h"
#include "car.h"
#include "looper.h"

using namespace libsc::k60;
using namespace std;

#define CMD_MASK 0xC0
#define CMD_SHIFT 6
#define CMD(x) ((x << CMD_SHIFT) & CMD_MASK)

#define TURN_MASK 0x3F
#define TURN_SHIFT 0

#define SPD_MASK 0x3F
#define SPD_SHIFT 0

namespace inno
{

namespace
{

class Intepreter
{
public:
	static void ProcessCommand(const Byte cmd, Car *car);

private:
	static void ProcessTurnCommand(const Byte cmd, Car *car);
	static void ProcessSpeedCommand(const Byte cmd, Car *car);
};

void Intepreter::ProcessCommand(const Byte cmd, Car *car)
{
	if (cmd == CMD(3))
	{

	}
	else if (cmd == CMD(2))
	{
		ProcessTurnCommand(cmd, car);
	}
	else if (cmd == CMD(1))
	{
		ProcessSpeedCommand(cmd, car);
	}
	else // cmd == CMD(0)
	{

	}
}

void Intepreter::ProcessTurnCommand(const Byte cmd, Car *car)
{
	const int turn = cmd & TURN_MASK;
	// [0, 0x3F] -> [-1000, 1000]
	const int expand = turn * 2000 / TURN_MASK - 1000;
	car->SetTurning(expand);
}

void Intepreter::ProcessSpeedCommand(const Byte cmd, Car *car)
{
	const int spd = cmd & SPD_MASK;
	// [0, 0x3F] -> [-1000, 1000]
	const int expand = spd * 2000 / TURN_MASK - 1000;
	car->SetMotorPower(expand);
}

}

App::App()
{}

App::~App()
{}

void App::Run()
{
	System::Init();
	m_car.SetLed(0, true);
	m_car.SetLed(2, true);
	m_car.EnableUartRx(std::bind(&App::OnUartReceive, this, placeholders::_1,
			placeholders::_2));
	m_looper.RunAfter(200, std::bind(&App::Breath, this));
	Beep(200);

	m_looper.Loop();
}

void App::Breath()
{
	m_car.SwitchLed(0);
	m_car.SwitchLed(1);
	m_car.SwitchLed(2);
	m_car.SwitchLed(3);
	m_looper.RunAfter(200, std::bind(&App::Breath, this));
}

void App::Beep(const uint32_t duration)
{
	m_car.SetBeep(true);
	m_looper.RunAfter(duration,
			[this](const Timer::TimerInt, const Timer::TimerInt)
			{
				m_car.SetBeep(false);
			});
}

void App::OnUartReceive(const Byte *bytes, const size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		Intepreter::ProcessCommand(bytes[i], &m_car);
	}
}

}
