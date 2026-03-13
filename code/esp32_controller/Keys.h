#pragma once

#include <Arduino.h>
#include "controller_keys.h"

#include <Wire.h>		    // I2C锟斤拷
#include <MPU6050_tockn.h>  // 锟斤拷锟斤拷锟角匡拷
#include <MCP23017.h>       // IO锟斤拷展锟斤拷
#include <Bounce2.h>

// 锟斤拷值锟结构锟藉定锟斤拷
struct KVS
{
	// 4锟斤拷前锟剿帮拷锟斤拷
	bool L_up = 1;
	bool L_down = 1;
	bool R_up = 1;
	bool R_down = 1;

	// 锟斤拷位锟斤拷锟斤拷摇锟斤拷  ------------------ int8_t 锟斤拷锟叫凤拷锟斤拷char锟斤拷 -128 锟斤拷 127
	int8_t L_knob = 0;
	int8_t LX = 0;
	int8_t LY = 0;
	int8_t R_knob = 0;
	int8_t RX = 0;
	int8_t RY = 0;

	// 2锟斤拷锟斤拷锟截帮拷锟斤拷
	bool board_L = 1;
	bool board_R = 1;

	// 8锟斤拷锟斤拷锟杰帮拷锟斤拷
	bool up = 1;
	bool down = 1;
	bool left = 1;
	bool right = 1;
	bool o = 1;
	bool x = 1;
	bool a = 1;
	bool b = 1;

	// 4锟斤拷锟斤拷锟剿匡拷锟斤拷
	bool switch_L1 = 1;
	bool switch_L2 = 1;
	bool switch_R1 = 1;
	bool switch_R2 = 1;

	// 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
	int8_t angleX = 0;  // -128 锟斤拷 127
	int8_t angleY = 0;
};

class MCP_bounce
{
public:
	MCP_bounce(uint8_t pin, bool default_state);   // 锟斤拷锟届函锟斤拷锟斤拷锟斤拷锟斤拷IO锟节猴拷默锟斤拷状态
	bool pressed();     // 锟斤拷锟斤拷
	bool read();        // 锟斤拷取锟斤拷锟斤拷状态
	bool on();          // 锟斤拷 锟斤拷锟斤拷read() != _default_state;锟斤拷锟斤拷锟节诧拷锟剿匡拷锟截ｏ拷on()锟斤拷off()锟斤拷直锟斤拷
	bool off();         // 锟截憋拷 锟斤拷锟斤拷read() != _default_state;锟斤拷锟斤拷锟节诧拷锟剿匡拷锟截ｏ拷on()锟斤拷off()锟斤拷直锟斤拷

private:
	uint8_t _pin;
	bool _default_state;
	bool _isPressed_flag = false;
};




class Keys
{
public:
	// MCP IO锟斤拷展锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟襟，凤拷锟斤拷为锟斤拷写
	MCP_bounce up;
	MCP_bounce down;
	MCP_bounce left;
	MCP_bounce right;
	MCP_bounce o;
	MCP_bounce x;
	MCP_bounce a;
	MCP_bounce b;
	MCP_bounce L1;
	MCP_bounce L2;
	MCP_bounce R1;
	MCP_bounce R2;

	// Bounce2锟解按锟斤拷锟斤拷锟斤拷
	Bounce2::Button L_up;
	Bounce2::Button L_down;
	Bounce2::Button R_up;
	Bounce2::Button R_down;
	Bounce2::Button board_L;
	Bounce2::Button board_R;

	MPU6050 mpu6050;
	
	// 锟斤拷值锟结构锟斤拷
	KVS kvs;

	// 锟斤拷锟届函锟斤拷锟叫筹拷始锟斤拷 MPU6050 锟斤拷锟斤拷,	锟斤拷MCP23017锟斤拷锟斤拷锟斤拷锟斤拷	
	Keys();

	// 锟斤拷始锟斤拷锟斤拷锟斤拷
	void init(int conID);

	// 锟斤拷锟斤拷锟斤拷锟铰ｏ拷锟斤拷kvs锟结构锟斤拷
	void kvs_update();

	// 锟斤拷锟节达拷印锟斤拷锟斤拷锟斤拷锟斤拷
	void ShowInSerial();

	// 锟斤拷锟斤拷锟斤拷锟皆ｏ拷锟斤拷锟斤拷锟斤拷锟叫э拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷隆锟斤拷头锟�
	void dounnceTest();

	// 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟铰ｏ拷前锟斤拷4锟斤拷锟斤拷锟斤拷+锟斤拷锟斤拷2锟斤拷锟斤拷锟斤拷
	void bounce_update();

private:

	// 遥锟斤拷锟斤拷锟斤拷锟�
	int _conID = 0;

	// 摇锟剿撅拷锟斤拷锟斤拷位值
	int _zero = 15;
};

