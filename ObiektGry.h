#pragma once
#include "Common.h"
#include "Wektor.h"

class ObiektGry { 
private:
	virtual void test(){ //polimorfizm
	};
protected:
	Prostokat Granice; // lewy gorny rog
public:
	ObiektGry(const Prostokat&);
	ObiektGry() : ObiektGry(Prostokat(0, 0, 10, 10)) {
	}
	const Prostokat& getGranice() {
		return Granice;
	}
};

class Platforma : public ObiektGry {
public:
	Platforma(const Prostokat&);
};

class Przeszkoda : public ObiektGry {
public:
	Przeszkoda(const Prostokat&);
};

class Jednorozec : public ObiektGry {
private:
	Punkt_Float a; // przyspieszenie
	Punkt_Float v; // prêdkoœæ
	Punkt_Float a_temp; // przyspieszenie chwilowe
	Punkt_Float v_temp; // predkosc chwilowa
	void_vector* obiektyGry2; //obiekty gry
	int sterowanie = 0; //tryb sterowania, pocz¹tkowo domyœlny
	int zawin_zmienna; //informacja o potrzebie zawiniêcia sceny
	int szerokosc_sceny;
	//zmienne pomocnicze do funkcji dash
	float vx_po_dash = 0;
	float vy_po_dash = 0;
	int pomoc = 0;
	//punkty jednorozca
	float punkty = 0;
	//liczniki
	int licznik_gwiazd = 1;
	int licznik_skok = 0;
	int licznik_dash = 0;
	int kolizja_test = 0;
	int zderzenie_jednorozca; //informacja o zderzeniu
public:
	//konstruktor
	Jednorozec(const Punkt_Float& punkt);
	//funkcja zliczaj¹ca punkty gracza
	void uaktualnij_punkty(float dt, float bonus);
	float returnPunkty();
	//funkcje poruszaj¹ce jednoro¿cem
	void uaktualnij(float dt, float sx, float sy);
	void sprawdz(float dt);
	//zmiana sterowania
	void ustawSterowanie(int sterowanieGry);
	//obsluga kolizji
	int kolizja_gora(float sx, float sy, Prostokat p);
	int kolizja_dol(float sx, float sy, Prostokat p);
	int kolizja_prawo(float sx, float sy, Prostokat p);
	int kolizja_lewo(float sx, float sy, Prostokat p);
	//sterowanie podstaowowe
	void prawo();
	void lewo();
	void dol();
	void gora();
	//sterowanie zaawansowane
	void skok(float sila);
	void dash(float sila);
	//pobieranie elementów gry z pliku
	void getObiektyGry(void_vector* obiektyGry);
	//obsluga resetu
	void reset();
	void reset_bez_pkt();
	//obsluga zawijania sceny
	void reset_zawin();
	int zawin();
	//obsluga zderzenia
	void getZderzenie(int zderzenie);
	int returnZderzenie();
	//pobieranie szerokosci sceny z pliku
	void getSzerokosc(int szerokosc);
};


