/*
 * car.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstddef>
#include <memory>

#include <libsc/k60/encoder.h>
#include <libsc/k60/led.h>
#include <libsc/k60/motor.h>
#include <libsc/k60/simple_buzzer.h>
#include <libsc/k60/trs_d05.h>
#include <libsc/k60/uart_device.h>
#include <libutil/misc.h>
#include <libutil/remote_var_manager.h>

#include "car.h"

using namespace libsc::k60;
using namespace std;

#define SERVO_MID_DEGREE 815
#define SERVO_AMPLITUDE 290

namespace inno
{

Car::Car()
		: m_encoder(0),
		  m_leds{Led(0), Led(1), Led(2), Led(3)},
		  m_motor(0, false),
		  m_buzzer({0, false}),
		  m_servo(0),
		  m_uart(0, libbase::k60::Uart::Config::BaudRate::k115200)
{}

Car::~Car()
{}

void Car::SetMotorPower(const int16_t power)
{
	const Uint power_ = libutil::Clamp<Uint>(0, abs(power), 1000);
	m_motor.SetClockwise(power < 0);
	m_motor.SetPower(power_);
}

void Car::SetTurning(const int16_t percentage)
{
	// Servo's rotation dir is opposite to our wheels
	const int percentage_ = libutil::Clamp<int>(-100, -percentage, 100);
	const int degree = SERVO_MID_DEGREE + (percentage_ * SERVO_AMPLITUDE / 1000);
	m_servo.SetDegree(degree);
}

}
