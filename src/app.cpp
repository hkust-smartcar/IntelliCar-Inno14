/*
 * app.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <functional>

#include <libsc/k60/system.h>
#include <libutil/looper.h>
#include <libutil/misc.h>

#include "app.h"
#include "car.h"

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
};

void App::Intepreter::ProcessCommand(const Byte cmd, App *parent)
{
	switch (cmd)
	{
	case 'L':
		parent->m_car.SetTurning(-1000);
		break;

	case 'M':
		parent->m_car.SetTurning(0);
		break;

	case 'R':
		parent->m_car.SetTurning(1000);
		break;

	case '0':
		parent->m_car.SetMotorPower(0);
		break;

	case '1':
		parent->m_car.SetMotorPower(210);
		break;

	case '2':
		parent->m_car.SetMotorPower(250);
		break;

	case '3':
		parent->m_car.SetMotorPower(290);
		break;

	case 'B':
		parent->m_car.SetMotorPower(-210);
		break;
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
