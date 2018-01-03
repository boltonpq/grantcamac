/*
 *	Biblioteka do sterowania stolikiem krzy¿owym XY. Grant rektorski SKN CAMAC
 *	
 *	Dla ka¿dego silnika tworzymy jeden obiekt typu CrossTableDriver (w naszym przypadku silniki s¹ dwa.
 *	Klasa zawiera dwa konstruktory, domyœlny (którego jeszcze nie ma ;P) i konstruktor, który w argumentach
 *	przyjmuje numery portów GPIO dla RPi3 do sterowania driverem DRV8825 oraz jeden pin przerwania pochodz¹cy
 *	od krañcówki osi. Na chwilê obecn¹ s¹ cztery krañcówki (po dwie na ka¿d¹ oœ), ale obs³uguje je jedno
 *	przerwanie (zmniejsz to stopnieñ z³o¿onoœci biblioteki, ale musiscie pamietaæ o zapamietywaniu danych
 *	wejœciowych przed wyst¹pieniem przerwania (w tedy bêdzie wiadomo z którego stycznika pochodzi). Ostatnia
 *	rzecz to tryb precyzji z zakresu 1:1 do 1:32, dzielnik on poprostu liczbê kroków przez podany iloraz.
 *
 *	Do sterownia samym silnikiem s¹ dwie metody SingleStep() i NSteps(). Dzia³aja tak jak wskazuje nazwa.
 *	Wiêcej na ten tema znajdziecie w przyk³adach, które wam dorzucam. Z istotnych rzeczy zosta³ jeszcze 
 *	lincznik po³o¿enia. Zwiêkszanie go lub zminiejszanie da wam wspó³rzêdn¹ w danym wymiarze. Jest to du¿o
 *	³atwiejsze ni¿ doj¿e¿d¿anie ka¿dorazowo do krañcówki. Wystarczy na pocz¹tek jednorazowo odpaliæ algorytm
 *	kalibracji, który zapmiêta iloœæ kroków pomiêdzy stycznikami i dalej mozna bazowaæ wy³acznie na sprawdzaniu
 *	wartosci stepsIter za pomoca metody getStepsIter(). Algorytm kalibracji równie¿ znajdzienie w przyk³adach.
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
		int step;		// Sygna³ kroku
		int dir;		// Sygna³ kierunku obrotu
		int M0, M1, M2;	//	Ustawienie dzielnika kroku: 1/2, 1/4, 1/8, 1/16, 1/32
		int res_slp;	//	Wy³¹czanie silników
		int endSW;	// dodaæ piniy krañcówek

	}	DriverPins;	// Struktura do przechowywania numerów pinów komunikacyjnych

	precision StepPrec;	// Precyzja kroku
	int stepsIter;		// Licznik po³o¿enia
	
	void pulseHigh(int pin);	// Makro do generacji pojedyñczego impulsu
	void init();				// Funkcja inicjalizacji

	static volatile bool endSWFlag;		// Flaga koñca osi stolika
	static void endSWinterrupt(void);	// Funkcja obs³ugi przerwania krañcówki

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