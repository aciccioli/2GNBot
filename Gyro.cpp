
#include "Gyro.h"
#include "Wire.h"
#include "Arduino.h"

//Direccion I2C de la IMU
#define MPU 0x68

//Ratios de conversion
#define A_R 16384.0
#define G_R 131.0

//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779

//MPU-6050 da los valores en enteros de 16 bits
//Valores sin refinar
float AcX, AcY, AcZ, GyX, GyY, GyZ;

//Angulos
float Acc[2];
float Gy[2];
float Angle[2];

Gyro::Gyro()
{
	this->GetYAngle = Angle[0];
	this->GetXAngle = Angle[1];
}


Gyro::~Gyro()
{
}

void Gyro::Iniciar()
{
	Wire.begin();
	Wire.beginTransmission(MPU);
	Wire.write(0x6B);
	Wire.write(0);
	Wire.endTransmission(true);
	Actualizar();
}

void Gyro::Actualizar()
{
	_GetAccelerometerData();
	_GetGyroscopeData();
	//Aplicar el Filtro Complementario
	Angle[0] = 0.98 *(Angle[0] + Gy[0] * 0.010) + 0.02*Acc[0];
	Angle[1] = 0.98 *(Angle[1] + Gy[1] * 0.010) + 0.02*Acc[1];
	this->GetYAngle = map((int)Angle[0],0, 85, 0, 100);
	this->GetXAngle = map((int)Angle[1],0, 85, 0, 100);
}

void Gyro::_GetAccelerometerData()
{
	//Leer los valores del Acelerometro de la IMU
	Wire.beginTransmission(MPU);
	Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
	Wire.endTransmission(false);
	Wire.requestFrom(MPU, 6, true); //A partir del 0x3B, se piden 6 registros
	AcX = Wire.read() << 8 | Wire.read(); //Cada valor ocupa 2 registros
	AcY = Wire.read() << 8 | Wire.read();
	AcZ = Wire.read() << 8 | Wire.read();

	//Se calculan los angulos Y, X respectivamente.
	Acc[1] = atan(-1 * (AcX / A_R) / sqrt(pow((AcY / A_R), 2) + pow((AcZ / A_R), 2)))*RAD_TO_DEG;
	Acc[0] = atan((AcY / A_R) / sqrt(pow((AcX / A_R), 2) + pow((AcZ / A_R), 2)))*RAD_TO_DEG;
}

void Gyro::_GetGyroscopeData()
{
	Wire.beginTransmission(MPU);
	Wire.write(0x43);
	Wire.endTransmission(false);
	Wire.requestFrom(MPU, 4, true); //A diferencia del Acelerometro, solo se piden 4 registros
	GyX = Wire.read() << 8 | Wire.read();
	GyY = Wire.read() << 8 | Wire.read();

	//Calculo del angulo del Giroscopio
	Gy[0] = GyX / G_R;
	Gy[1] = GyY / G_R;
}

