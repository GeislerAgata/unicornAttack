#pragma once

#define PSEUDO_DLUGOSC 100
#define DOMYSLNE_WARTOSCI 0

struct Wynik {
	char psuedo[PSEUDO_DLUGOSC];
	int punkty;
};

struct  Punkt {
	int x; //wsp�rz�dna x
	int y; //wsp�rz�dna y
	//konstruktor punktu
	Punkt(int x, int y) :x(x), y(y) {
	}
};

struct Punkt_Float {
	float x; //wsp�rz�dna x
	float y; //wsp�rz�dna y
	//konstruktor z parametrem x i y
	Punkt_Float(float x, float y) :x(x), y(y) {
	}
	//konstruktor bez parametru, domy�lna pozycja (0, 0)
	Punkt_Float() : Punkt_Float(DOMYSLNE_WARTOSCI, DOMYSLNE_WARTOSCI) {
	}
};

struct Prostokat {
	Punkt_Float pozycja;
	float szerokosc;
	float wysokosc;
	//konstruktor bez pozycji, ale z innymi parametrami domy�lna pozycja (0, 0)
	Prostokat(const Punkt_Float& pozycja, float szerokosc, float wysokosc) : pozycja(pozycja),szerokosc(szerokosc),wysokosc(wysokosc) {
	}
	//konstruktor z wszystkimi parametrami
	Prostokat(float x, float y, float szerokosc, float wysokosc) : pozycja(x, y), szerokosc(szerokosc), wysokosc(wysokosc) {
	}
};

