#include "Motor.h"


// Variables Globales


Motor::Motor(int pin)
{
	// Contructor de la clase Motor
	pinMode(pin, OUTPUT);
	_pin = pin;
	this->AttachedPin = _pin;
	this->CurrentThrottle = 0;
	this->MaximumThrottle = 254;
	this->MinimumThrottle = 0;
	this->IsMotorRunning = false;

	
}

Motor::~Motor()
{
}


void Motor::Iniciar()
{
	_init_motor_spinning();
}


void Motor::Acelera(int v)
{
	_throttle_motor(v);
}

void Motor::_throttle_motor(int valor)
{
	int _throttle_value = valor;
	int _current_throttle = this->CurrentThrottle;
	int _new_throttle = 0;
	if (_throttle_value != 0)
	{
		// Se movio el joystick, hay que acelerar o desacelerar
		_new_throttle = _current_throttle + _throttle_value;

		if ((_new_throttle >= this->MinimumThrottle) && (_new_throttle <= this->MaximumThrottle))
		{
			analogWrite(this->AttachedPin, _new_throttle);
			this->CurrentThrottle = _new_throttle;
		}
	}
	else
	{
		if (this->CurrentThrottle == 0)
		{
			_new_throttle = this->MinimumThrottle;
		}
		else
		{
			_new_throttle = this->CurrentThrottle;
		}

		analogWrite(this->AttachedPin, _new_throttle);
		this->CurrentThrottle = _new_throttle;
	 }
}

void Motor::_init_motor_spinning()
{
	bool _exit_loop = false;
	int i = 0;
//	analogWrite(this->AttachedPin, this->MaximumThrottle);;
	delay(100);
	do
	{
		for (i = this->MinimumThrottle; i <= 10; i++)
		{
			analogWrite(this->AttachedPin, i);
			this->CurrentThrottle = i;
		}
		_exit_loop = true;
		this->IsMotorRunning = true;
	} while (_exit_loop == false);
}



