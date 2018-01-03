#include "CrossTableDriver.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

					/*		Konstruktory		*/

CrossTableDriver::CrossTableDriver()
{

}

CrossTableDriver::CrossTableDriver(int step, int dir, int m0, int m1, int m2, int slp, int endSW, precision mode)
{
	DriverPins = { step, dir, m0, m1, m2, slp, endSW };
	//endSWFlag = 0;
	init();
	setPrecision(mode);
}

CrossTableDriver::~CrossTableDriver()
{
	/*	Wy³aczanie silników	*/
	digitalWrite(DriverPins.res_slp, LOW);
}

					/*		Funkcje prywatne		*/

void CrossTableDriver::pulseHigh(int pin)
{
	digitalWrite(pin, HIGH); delayMicroseconds(50); digitalWrite(pin, LOW);
}

void CrossTableDriver::init()
{
	/*	Inicjalizacja portów komunikacyjnych */
	pinMode(DriverPins.step, OUTPUT);
	pinMode(DriverPins.dir, OUTPUT);
	pinMode(DriverPins.M0, OUTPUT);
	pinMode(DriverPins.M1, OUTPUT);
	pinMode(DriverPins.M2, OUTPUT);
	pinMode(DriverPins.res_slp, OUTPUT);

	/*	W³¹czamy silnik	*/
	digitalWrite(DriverPins.res_slp, HIGH);

	/*	Ustawienie przerwania dla krañcówki	*/
	if (wiringPiISR(DriverPins.endSW, INT_EDGE_BOTH , &endSWinterrupt) < 0) {
		fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
	}
}

					/*		Funkcje publiczne		*/

bool CrossTableDriver::SingleStep(direction dir) {

	digitalWrite(DriverPins.dir, dir);		//	Ustawienie kierunku
	
	if (dir == left && !endSWFlag) {
		pulseHigh(DriverPins.step);			//	Generacja sygna³u 1 kroku
		stepsIter--;
	} 
	
	if(dir == right && !endSWFlag) {
		pulseHigh(DriverPins.step);			//	Generacja sygna³u 1 kroku
		stepsIter++;
	}
	
	return !endSWFlag;		// W przypadku za³¹czenia siê krañcówki funkcja zwraca wartoœæ TRUE
}

bool CrossTableDriver::NSteps(int nSteps, direction dir, int stepsPerSec) {

	digitalWrite(DriverPins.dir, dir);		//	Ustawienie kierunku

	for (int i = 0; i < nSteps && (dir == left || !endSWFlag); i++) {

		pulseHigh(DriverPins.dir);			//	Generacja sygna³u 1 kroku
		stepsIter--;
		delay(100);
	}

	for (int i = 0; i < nSteps && (dir == right || !endSWFlag); i++) {
		pulseHigh(DriverPins.dir);			//	Generacja sygna³u 1 kroku
		stepsIter++;
		delay(100);
	}


	return !endSWFlag;		// W przypadku za³¹czenia siê krañcówki funkcja zwraca wartoœæ TRUE

}

void CrossTableDriver::setStepsIter(int iter)
{
	stepsIter = iter;
}

int CrossTableDriver::getStepsIter(void)
{
	return stepsIter;
}

void CrossTableDriver::setPrecision(precision prec)
{
	StepPrec = prec;
	/*	Ustawienie precyzji kroku */
	digitalWrite(DriverPins.M0, (0x1)&(StepPrec >> 2));
	digitalWrite(DriverPins.M1, (0x1)&(StepPrec >> 1));
	digitalWrite(DriverPins.M2, (0x1)&StepPrec);
}

void CrossTableDriver::resetEndSWFlag(void)
{
	endSWFlag = 0;
}

					/*		Obs³uga przerwañ		*/

bool volatile CrossTableDriver::endSWFlag = 0;

void CrossTableDriver::endSWinterrupt(void)
{
		endSWFlag = 1;
}

/*
void CrossTableDriver::endSWinterrupt(void)
{
	if (!digitalRead(26))
		endSWFlag = 0;
	else if (!endSWFlag)
		endSWFlag = 1;
	else
		fprintf(stderr, "Podwójne przerwanie!! %s\n", strerror(errno));
}
*/