/*
 * app.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <libutil/misc.h>

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
	void Beep(const uint32_t duration);
	void OnUartReceive(const Byte *bytes, const size_t size);

	Car m_car;
	Looper m_looper;
};

}
