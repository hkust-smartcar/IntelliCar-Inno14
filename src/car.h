/*
 * car.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>

#include <libsc/k60/encoder.h>
#include <libsc/k60/led.h>
#include <libsc/k60/motor.h>
#include <libsc/k60/simple_buzzer.h>
#include <libsc/k60/tower_pro_mg995.h>
#include <libsc/k60/uart_device.h>

namespace inno
{

class Car
{
public:
	Car();
	~Car();

	/**
	 * Set the power of the motor, a negative power will drive the car backwards
	 *
	 * @param power Power scale in [-1000, 1000]
	 */
	void SetMotorPower(const int16_t power);

	/**
	 * Set the turning percentage, negative input means turning left
	 *
	 * @param percentage Specifying how aggressively should the car turn,
	 * in [-1000, 1000], where passing 0 basically means going straight
	 */
	void SetTurning(const int16_t percentage);

	void SwitchLed(const uint8_t id)
	{
		m_leds[id].Switch();
	}
	void SetLed(const uint8_t id, const bool flag)
	{
		m_leds[id].SetEnable(flag);
	}

	void UpdateEncoder()
	{
		m_encoder.Update();
	}

	int16_t GetEncoderCount()
	{
		return m_encoder.GetCount();
	}

	void UartSendLiteral(const char *literal)
	{
		return m_uart.SendStrLiteral(literal);
	}

	bool UartPeekChar(char *out_ch)
	{
		return m_uart.PeekChar(out_ch);
	}

	void EnableUartRx(const libsc::k60::UartDevice::OnReceiveListener &l)
	{
		m_uart.EnableRx(l);
	}

	void SetBeep(const bool flag)
	{
		m_buzzer.SetBeep(flag);
	}

	bool GetBeep() const
	{
		return m_buzzer.GetBeep();
	}

private:
	libsc::k60::Encoder m_encoder;
	libsc::k60::Led m_leds[4];
	libsc::k60::Motor m_motor;
	libsc::k60::SimpleBuzzer m_buzzer;
	libsc::k60::TowerProMg995 m_servo;
	libsc::k60::UartDevice m_uart;
};

}
