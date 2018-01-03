/*
 *	Biblioteka do sterowania stolikiem krzy�owym XY. Grant rektorski SKN CAMAC
 *	
 *	Dla ka�dego silnika tworzymy jeden obiekt typu CrossTableDriver (w naszym przypadku silniki s� dwa.
 *	Klasa zawiera dwa konstruktory, domy�lny (kt�rego jeszcze nie ma ;P) i konstruktor, kt�ry w argumentach
 *	przyjmuje numery port�w GPIO dla RPi3 do sterowania driverem DRV8825 oraz jeden pin przerwania pochodz�cy
 *	od kra�c�wki osi. Na chwil� obecn� s� cztery kra�c�wki (po dwie na ka�d� o�), ale obs�uguje je jedno
 *	przerwanie (zmniejsz to stopnie� z�o�ono�ci biblioteki, ale musiscie pamieta� o zapamietywaniu danych
 *	wej�ciowych przed wyst�pieniem przerwania (w tedy b�dzie wiadomo z kt�rego stycznika pochodzi). Ostatnia
 *	rzecz to tryb precyzji z zakresu 1:1 do 1:32, dzielnik on poprostu liczb� krok�w przez podany iloraz.
 *
 *	Do sterownia samym silnikiem s� dwie metody SingleStep() i NSteps(). Dzia�aja tak jak wskazuje nazwa.
 *	Wi�cej na ten tema znajdziecie w przyk�adach, kt�re wam dorzucam. Z istotnych rzeczy zosta� jeszcze 
 *	lincznik po�o�enia. Zwi�kszanie go lub zminiejszanie da wam wsp�rz�dn� w danym wymiarze. Jest to du�o
 *	�atwiejsze ni� doj�e�d�anie ka�dorazowo do kra�c�wki. Wystarczy na pocz�tek jednorazowo odpali� algorytm
 *	kalibracji, kt�ry zapmi�ta ilo�� krok�w pomi�dzy stycznikami i dalej mozna bazowa� wy�acznie na sprawdzaniu
 *	wartosci stepsIter za pomoca metody getStepsIter(). Algorytm kalibracji r�wnie� znajdzienie w przyk�adach.
 *
 */


#ifndef CROSTABLEDRIVER_H
#define CROSTABLEDRIVER_H

#include <wiringPi.h>

enum direction { left = LOW, right = HIGH };
enum precision { div1 = 0b000, div2 = 0b100, div4 = 0b010, div8 = 0b110, div16 = 0b001, div32 = 0b111 };

class CrossTableDriver {

private:
	
	struct inputPins
	{
		int step;		// Sygna� kroku
		int dir;		// Sygna� kierunku obrotu
		int M0, M1, M2;	//	Ustawienie dzielnika kroku: 1/2, 1/4, 1/8, 1/16, 1/32
		int res_slp;	//	Wy��czanie silnik�w
		int endSW;	// doda� piniy kra�c�wek

	}	DriverPins;	// Struktura do przechowywania numer�w pin�w komunikacyjnych

	precision StepPrec;	// Precyzja kroku
	int stepsIter;		// Licznik po�o�enia
	
	void pulseHigh(int pin);	// Makro do generacji pojedy�czego impulsu
	void init();				// Funkcja inicjalizacji

	static volatile bool endSWFlag;		// Flaga ko�ca osi stolika
	static void endSWinterrupt(void);	// Funkcja obs�ugi przerwania kra�c�wki

public:

	CrossTableDriver();
	CrossTableDriver(int step, int dir, int m0, int m1, int m2, int slp, int endSW, precision mode);
	~CrossTableDriver();

	bool SingleStep(direction dir);								//	Funkcje kroku
	bool NSteps(int nSteps, direction dir, int stepsPerSec);	//

	void setStepsIter(int iter);	// Metody kalibracji stolika
	int getStepsIter(void);			//

	void setPrecision(precision prec);	// Ustawianie precyzji kroku
	void resetEndSWFlag(void);	// Zerowanie flagi przerwania
};

#endif