#include "CrossTableDriver.h"

// Rozmieszcznie po��cze� w Raspberry mo�na znale�� pod lnikiem
// https://pinout.xyz/resources/raspberry-pi-pinout.png
// Jest to program tesowy, kt�rym sprawdza�em p�ynno�� dzia�ania stolika. 
// Opis metod dla biblioteki WiringPi mo�na znale�� pod linkiem:
// http://wiringpi.com/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

#define	STEPX 4	
#define DIRX 17
#define M0	27
#define M1	22
#define M2	23
#define SLP	24
#define IRQ	26
#define STEPY 18
#define DIRY  20

#define TESTMODE 0

int main(void)
{
	/*	Inicjalizacja biblioteki wiringPi	*/

	if (wiringPiSetupGpio() < 0) {
		fprintf(stderr, "Unable to setup wiringPi: %s\n", strerror(errno));
		return 1;
	}

	/*	Inicjalizacja silnika osi x	*/

	CrossTableDriver StepperX (STEPX, DIRX, M0, M1, M2, SLP, IRQ, div8);
	StepperX.setStepsIter(0);	// Ustwiam sobie jakie� wzgl�dne zero

	/*	Inicjalizacja silnika osi y	*/

	CrossTableDriver StepperY(STEPY, DIRY, M0, M1, M2, SLP, IRQ, div8);
	StepperY.setStepsIter(0);	// Ustwiam sobie jakie� wzgl�dne zero

	/*	LOOP  */
	while (true)
	{
		switch (TESTMODE)
		{
		case 0:
			for (int i = 0; i < 100; i++)
			{
				if (StepperX.SingleStep(right) && StepperY.SingleStep(right))
					delay(100);
				else
				{
					StepperX.resetEndSWFlag();
					StepperY.resetEndSWFlag();
					printf("Koniec stolika z prawej strony \n");
					break;
				}
			}

			//delay(1000);

			for (int i = 0; i < 100; i++)
			{
				if (StepperX.SingleStep(left) && StepperY.SingleStep(left))
					delay(100);
				else
				{
					StepperX.resetEndSWFlag();
					StepperY.resetEndSWFlag();
					printf("Koniec stolika z lewej strony \n");
					break;
				}
			}

			//delay(1000);

			break;
		case 1:


			break;
		default:
			break;
		}
	}
	return 0;
}



// TODO:
// 2. Zasymulowa� mikroswitch i sprawdzi� dzia�anie przerwa�