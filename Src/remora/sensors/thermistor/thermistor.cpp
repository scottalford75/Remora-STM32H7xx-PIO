#include "thermistor.h"


Thermistor::Thermistor(std::string pin, float beta, int r0, int t0) :
	pin(pin),
	r0(r0),
	t0(t0),
	beta(beta)
{
	// Thermistor math
	this->j = (1.0F / this->beta);
	this->k = (1.0F / (this->t0 + 273.15F));

    this->adc = new AnalogIn(this->pin);
	this->r1 = 0;
	this->r2 = 4700;
}

int Thermistor::newThermistorReading()
{
	return this->adc->read();
}

// This is the workhorse routine that calculates the temperature
// using the Steinhart-Hart equation for thermistors
// https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
float Thermistor::adcValueToTemperature()
{
	float adcValue = this->newThermistorReading();
	float t;

	// resistance of the thermistor in ohms
	float r = this->r2 / ((65536.0F / adcValue) - 1.0F);


	if (this->r1 > 0.0F) r = (this->r1 * r) / (this->r1 - r);

	// use Beta value
	t= (1.0F / (this->k + (this->j * logf(r / this->r0)))) - 273.15F;

	return t;
}


float Thermistor::getTemperature()
{
	// This is the standard interface
	return this->adcValueToTemperature();
}
