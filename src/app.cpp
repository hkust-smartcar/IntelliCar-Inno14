/*
 * app.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libsc/k60/system.h>

#include "app.h"
#include "car.h"
#include "looper.h"

using namespace libsc::k60;

namespace inno
{

App::App()
{}

App::~App()
{}

void App::Run()
{
	System::Init();
	m_car.SetLed(0, true);
	m_car.SetLed(2, true);
	m_looper.RunAfter(200, std::bind(&App::Breath, this));
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

}
