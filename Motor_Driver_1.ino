#include "Motor.h"
#include "Gyro.h"


Motor _motor_1(3);  // Y -
Motor _motor_2(5); // X -
Motor _motor_3(6);  // X +
Motor _motor_4(9); // Y +

Gyro _gyro;

int acelera = 4;

void setup() 
{
	_motor_1.Iniciar();
	_motor_2.Iniciar();
	_motor_3.Iniciar();
	_motor_4.Iniciar();
	_gyro.Iniciar();

	Serial.begin(115200);



}

void loop() 
  {
	  _gyro.Actualizar();
	  /* Calcula el porcentaje de aceleracion para el motor a partir del joystick
	    map(analogRead(4), 0, 640, -10, 10)
	  */

	  _motor_1.Acelera(acelera);
	  _motor_3.Acelera(acelera);
	  _motor_2.Acelera(acelera);
	  _motor_4.Acelera(acelera);


	  //Estabilizar(_gyro.GetXAngle, _gyro.GetYAngle);

	  Serial.println();
	  Serial.print("X:"); Serial.print(_gyro.GetXAngle); Serial.print(", Y:");	Serial.print(_gyro.GetYAngle);
	  Serial.print(" | Motor X- :"); Serial.print(_motor_2.CurrentThrottle);
	  Serial.print(" | Motor X+ :"); Serial.print(_motor_3.CurrentThrottle);
	  Serial.print(" | Motor Y- :"); Serial.print(_motor_1.CurrentThrottle);
	  Serial.print(" | Motor Y+ :"); Serial.println(_motor_4.CurrentThrottle);

  }

void Estabilizar(int _GyroX, int _GyroY)
{
	double ax = 0;
	double ay = 0;
	/*
	_motor_1(9)		--> Y -
	_motor_2(10)	--> X -
	_motor_3(3)		--> X +
	_motor_4(11)	--> Y +
	*/
	if (_GyroX < 0)
	{

		/* Inclinacion negativa en el Eje X */

		ay = 1 - (_GyroX / 100);

		/* Verifico velocidad actual en el motor X - */
		if ((_motor_2.CurrentThrottle == _motor_2.MaximumThrottle))
		{
			// Motor X - esta a maxima velocidad
			// descacelero motor X+ para compensar

			_motor_3.Acelera(_GyroX);
		}
		else
		{
			// Motor X - NO esta a maxima velocidadx=
			// Acelero motor X- para compensar
			_motor_2.Acelera(_GyroX * -1);
		}
	}
	else if (_GyroX > 0)
	{
		/* Inclinacion positiva en el Eje X */

		/* Verifico velocidad actual en el motor en X+ */
		if ((_motor_3.CurrentThrottle == _motor_3.MaximumThrottle))
		{
			/* Motor X + esta a maxima velocidad */

			/* Desacelero X- para compensar */
			_motor_2.Acelera(_GyroX * -1);
		}
		else
		{
			/* Motor X + NO esta a maxima velocidad */

			/*¨Acelero X + para compensar */
			_motor_3.Acelera(_GyroX);
		}
	}

	if (_GyroY < 0)
	{
		/* Inclonacion Negativa en el eje Y */

		/* Verifico la velocidad actual del motor en Y -*/
		if ((_motor_1.CurrentThrottle == _motor_1.MaximumThrottle))
		{
			/* El Motor en Y - esta a maxima velocidad */
			/* Desacelero el motor en Y + */
			_motor_4.Acelera(_GyroY);
		}
		else
		{
			/* El Motor en Y - NO esta a maxima velocidad */
			/* Acelero el motor en Y -*/
			_motor_1.Acelera(_GyroY * -1);
		}
	}
	else
	{
		/* Inclinacion Positiva en eje Y */

		/* Verifico la velocidad del motor en Y + */
		if ((_motor_4.CurrentThrottle == _motor_4.MaximumThrottle))
		{
			/* El motor en Y + esta a maxima velocidad */
			/* Desacelero el motor en Y - */
			_motor_1.Acelera(_GyroY * -1);
		}
		else
		{
			/* El motor en Y + NO esta a maxima velocidad */
			/* Acelero el motoro en Y + */
			_motor_4.Acelera(_GyroY);
		}
	}
}