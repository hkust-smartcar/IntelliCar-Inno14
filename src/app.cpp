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

#define FLAG_BEEP_MASK 0x1
#define FLAG_BEEP_SHIFT 0
#define FLAG_LED_MASK 0x2
#define FLAG_LED_SHIFT 1

namespace inno
{

class App::Intepreter
{
public:
	static void ProcessCommand(const Byte cmd, App *parent);

private:
	static void ProcessTurnCommand(const Byte cmd, App *parent);
	static void ProcessSpeedCommand(const Byte cmd, App *parent);
	static void ProcessFlagCommand(const Byte cmd, App *parent);
};

void App::Intepreter::ProcessCommand(const Byte cmd, App *parent)
{
	if ((cmd & CMD_MASK) == CMD(3))
	{

	}
	else if ((cmd & CMD_MASK) == CMD(2))
	{
		ProcessTurnCommand(cmd, parent);
	}
	else if ((cmd & CMD_MASK) == CMD(1))
	{
		ProcessSpeedCommand(cmd, parent);
	}
	else // (cmd & CMD_MASK) == CMD(0)
	{
		ProcessFlagCommand(cmd, parent);
	}
}

void App::Intepreter::ProcessTurnCommand(const Byte cmd, App *parent)
{
	const int turn = cmd & TURN_MASK;
	// [0, 0x3F] -> [-1000, 1000]
	const int expand = turn * 2000 / TURN_MASK - 1000;
	parent->m_car.SetTurning(expand);
}

void App::Intepreter::ProcessSpeedCommand(const Byte cmd, App *parent)
{
	const int spd = cmd & SPD_MASK;
	// [0, 0x3F] -> [-1000, 1000]
	const int expand = spd * 2000 / TURN_MASK - 1000;
	parent->m_car.SetMotorPower(expand);
}

void App::Intepreter::ProcessFlagCommand(const Byte cmd, App *parent)
{
	if ((cmd & FLAG_BEEP_MASK) && !parent->m_car.GetBeep())
	{
		parent->Beep(500);
	}
	if (cmd & FLAG_LED_MASK)
	{
		parent->m_car.SwitchLed(3);
	}
}

App::App()
{}

App::~App()
{}

void App::Run()
{
	System::Init();
	m_car.UartSendLiteral("Welcome to InnoCarnival 2014\n");
	m_car.UartSendLiteral("Hello from Smart Car\n");
	m_car.SetLed(0, true);
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
		Intepreter::ProcessCommand(bytes[i], this);
	}
}

}
