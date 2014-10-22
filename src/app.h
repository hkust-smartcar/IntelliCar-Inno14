/*
 * app.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "car.h"
#include "looper.h"

namespace inno
{

class App
{
public:
	App();
	~App();

	void Run();

private:
	void Breath();

	Car m_car;
	Looper m_looper;
};

}
