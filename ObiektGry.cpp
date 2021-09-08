#include "ObiektGry.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define SCENE_HEIGHT	600 
#define WIELKOSC_JEDNOROZCA 30
#define X_START 5 //startowa pozycja x jednoro¿ca
#define Y_START 230 //startowa pozycja u jednoro¿ca
#define MOVE	1 //przemieszczenie 
#define PRZEMIESZCZENIE 10


int Jednorozec::returnZderzenie() {
	return zderzenie_jednorozca;
}

void  Jednorozec::getSzerokosc(int szerokosc) {
	szerokosc_sceny = szerokosc;
}

void Jednorozec::getObiektyGry(void_vector* obiektyGry) {
	obiektyGry2 = obiektyGry;
}

void Jednorozec::reset() {
	Granice.pozycja.x = X_START;
	Granice.pozycja.y = Y_START;
	punkty = 0;
}

void Jednorozec::reset_bez_pkt() {
	Granice.pozycja.x = X_START;
	Granice.pozycja.y = Y_START;
}

void Jednorozec::reset_zawin() {
	Granice.pozycja.x = X_START;
}

int Jednorozec::zawin() {
	if (Granice.pozycja.x + SCREEN_WIDTH >= szerokosc_sceny) { //jeœli scena siê koñczy
		zawin_zmienna = 1; // zawiñ
	}
	else {
		zawin_zmienna = 0; //nie zawijaj
	}
	return zawin_zmienna;
}

void Jednorozec::getZderzenie(int zderzenie) {
	zderzenie_jednorozca = zderzenie;
}

void Jednorozec::uaktualnij_punkty(float sx, float bonus) {
	const float punkty_a = 5.0; 
	float punkty_dodane = punkty_a;
	punkty += punkty_dodane * sx / 50 + bonus;
}

float Jednorozec::returnPunkty() {
	return punkty;
}

//sterowanie zaawansowane
void Jednorozec::skok(float sila) {
	if (licznik_skok < 2) { //jeœli mo¿e wyst¹piæ skok
		licznik_skok++;
		if (a_temp.y == 0) //jeœli przyspieszenie chwilowe jest równe 0 ustaw je na si³e skok
			a_temp.y = sila;
	}
}

void Jednorozec::dash(float sila) {
	if (licznik_dash < 1) { //jeœli mo¿e wyst¹piæ dash
		licznik_dash++;
		if (licznik_skok == 2) { //reset skoku o jeden
			licznik_skok--;
		}
		if (a_temp.x == 0) //jeœli przyspieszenie chwilowe jest równe 0 ustaw je na si³e dash
			a_temp.x = sila;
	}
}

void Jednorozec::sprawdz(float dt) {
	float sx, sy; //droga w osi x, droga w osi y
	int bonus = 0;
	if (sterowanie == 0) { 	// sterowanie podstawowe
		v.x = v_temp.x;
		v.y = v_temp.y;
		sx = v.x;
		sy = v.y;
	}
	else { 	// sterowanie zaawansowane
		const float g = 100.0; //grawitacja
		const float a_stale = 5.0; //pocz¹tkowe przyspieszenie jednorozca
		if (pomoc > 0) { // d³ugoœæ dash'a
			pomoc++;
			if (pomoc == 30) {
				v.x = vx_po_dash;
				if (kolizja_test == 1) { //jeœli wyst¹pi³a kolizja z pod³o¿em
					v.y = g;
					kolizja_test = 0;
				}
				else { //jeœli nie wyst¹pi³a kolizja z pod³o¿em
					v.y = vy_po_dash;
				}
				pomoc = 0;
			}	
		}
		if ((licznik_dash == 0) && (licznik_skok == 0)) { //nie by³o ani skoku ani dasha
			a.x = a_stale;
			a.y = g;
			v.x += a.x * dt;
			v.y += a.y * dt; //prêdkoœæ w osi y = grawitacja
		}
		else if ((licznik_skok > 0) && (licznik_dash > 0) && (licznik_dash < 2)) { //pojawi³ siê skok i dash
			a.x = a_stale + a_temp.x;
			a.y = 0;
			if (pomoc == 0) { //jeœli wyst¹pi³ dash
				vx_po_dash = v.x;
				vy_po_dash = v.y;
			}
			v.x += a.x * dt;
			v.y = 0;
			pomoc++;
			licznik_dash++;
		}
		else if (licznik_skok > 0) { //pojawi³ siê skok
			a.x = a_stale;
			a.y = g - a_temp.y;
			v.x += a.x * dt;
			v.y += a.y * dt;
		}
		else if ((licznik_dash > 0) && (licznik_dash < 2)){ //pojawi³ siê dash
			a.x = a_stale + a_temp.x;
			a.y = 0;
			if (pomoc == 0) { //jeœli wyst¹pi³ dash
				vx_po_dash = v.x;
				vy_po_dash = v.y;
			}
			v.x += a.x * dt;
			v.y = 0;
			pomoc++;
			licznik_dash++;
		}
		sx = v.x * dt;
		sy = v.y * dt;
		if (zderzenie_jednorozca > 0) { // jeœli wyst¹pi³o zderzenie 
			sx = 0;
			sy = 0;
			v.x = 0;
			v.y = 0;
		}
	}
	int pomoc_y = sy;
	int pomoc_x = sx;
	//wypadniêcie poza doln¹ krawêdŸ
	if (Granice.pozycja.y + sy + Granice.wysokosc >= SCENE_HEIGHT) { // kolizja z do³em ekranu
		zderzenie_jednorozca = 1;
		sx = 0;
		sy = 0;
		v.x = 0;
		v.y = 0;
	}
	//testowanie kolizji
	for (int i = 0; i < obiektyGry2->dlugosc; i++) {
		ObiektGry* aktualny = (ObiektGry*)vv_odczytaj_index(obiektyGry2, i);
		Prostokat p = aktualny->getGranice();
		if ((dynamic_cast<Platforma*>(aktualny) != nullptr) || (dynamic_cast<Przeszkoda*>(aktualny) != nullptr)) { //obiekt gry jest platform¹ lub przeszkod¹
			if (kolizja_prawo(sx, sy, p) == 1) { //kolizja z prawej strony
				if ((pomoc != 0) && (dynamic_cast<Przeszkoda*>(aktualny) != nullptr)) { //jeœli wyst¹pi³ dash i przeszkoda to przeszkoda
					bonus = 100 * licznik_gwiazd; //bonusowe punkty
					licznik_gwiazd++;
				}
				else{ //w przeciwnym wypadku nastêpuje zderzenie
					zderzenie_jednorozca = 1;
					sx = 0;
					sy = 0;
					v.x = 0;
					v.y = 0;
				}
			}
			else if (kolizja_lewo(sx, sy, p) == 1) { //kolizja z lewej strony
				if (sx < 0) {
					sx = 0;
				}
			}
			if (kolizja_dol(sx, sy, p) == 1) { //kolizja z do³u
				licznik_skok = 0;
				licznik_dash = 0;
				if (sy > 0) {
					sy = 0;
					v.y = 0;
				}
				v.y = 1;
				kolizja_test = 1;
			}
			else if (kolizja_gora(sx, sy, p) == 1) { //kolizja z góry
				zderzenie_jednorozca = 1;
				sx = 0;
				sy = 0;
				v.x = 0;
				v.y = 0;
			}
		}
	}
	if (zderzenie_jednorozca != 1) { //jeœli nie wyst¹pi³o zderzenie wykonaj aktualizacje punktów
		uaktualnij_punkty(sx, bonus);
	}
	uaktualnij(dt, sx, sy);
}

//wymaganie 2 i 4 podstawowe
/*if (Granice.pozycja.y + sy + Granice.wysokosc >= SCENE_HEIGHT) {
	zderzenie_jednorozca = 1;
	sx = 0;
	sy = 0;
	v.x = 0;
	v.y = 0;
}
if (Granice.pozycja.y + sy <= 0) {
	if (sy < 0) {
		sy = 0;
	}
}
if (Granice.pozycja.x + sx + Granice.szerokosc >= SCENE_WIDTH) {
	if (sx > 0) {
		sx = 0;
	}
}
if (Granice.pozycja.x + sx <= 0) {
	if (sx < 0) {
		sx = 0;
	}
}*/

//obsluga kolizji
int Jednorozec::kolizja_gora(float sx, float sy, Prostokat p) {
	if ((Granice.pozycja.y + sy <= p.pozycja.y + p.wysokosc + p.wysokosc) && (Granice.pozycja.y + sy >= p.pozycja.y + p.wysokosc - MOVE)) {
		if ((Granice.pozycja.y + sy <= p.pozycja.y + p.wysokosc) &&
			(((Granice.pozycja.x + sx >= p.pozycja.x) && (Granice.pozycja.x + sx + Granice.szerokosc <= p.pozycja.x + p.szerokosc)) ||
				((Granice.pozycja.x + sx + Granice.szerokosc > p.pozycja.x + p.szerokosc) && (Granice.pozycja.x + sx < p.pozycja.x + p.szerokosc) && (Granice.pozycja.x + sx > p.pozycja.x)) || ((Granice.pozycja.x + sx < p.pozycja.x) && (Granice.pozycja.x + sx + Granice.szerokosc > p.pozycja.x) && (Granice.pozycja.x + sx + Granice.szerokosc < p.pozycja.x + p.szerokosc)) || 

				((p.pozycja.x >= Granice.pozycja.x + sx) && (p.pozycja.x <= Granice.pozycja.x + Granice.szerokosc + sx) && (p.pozycja.x + p.szerokosc >= Granice.pozycja.x + sx) && (p.pozycja.x + p.szerokosc<= Granice.pozycja.x + Granice.szerokosc + sx)  )  )) {
			return 1;
		}
	}
	return 0;
}

int Jednorozec::kolizja_dol(float sx, float sy, Prostokat p) {
	if ((Granice.pozycja.y + sy + Granice.wysokosc >= p.pozycja.y - p.wysokosc) && (Granice.pozycja.y + sy + Granice.wysokosc <= p.pozycja.y + PRZEMIESZCZENIE)){
		if ((Granice.pozycja.y + sy + Granice.wysokosc >= p.pozycja.y) &&
			(((Granice.pozycja.x + sx >= p.pozycja.x) && (Granice.pozycja.x + sx + Granice.szerokosc <= p.pozycja.x + p.szerokosc)) ||
				((Granice.pozycja.x + sx + Granice.szerokosc > p.pozycja.x + p.szerokosc) && (Granice.pozycja.x + sx < p.pozycja.x + p.szerokosc) && (Granice.pozycja.x + sx > p.pozycja.x)) || ((Granice.pozycja.x + sx < p.pozycja.x) && (Granice.pozycja.x + sx + Granice.szerokosc > p.pozycja.x) && (Granice.pozycja.x + sx + Granice.szerokosc < p.pozycja.x + p.szerokosc)))) {
			return 1;
		}
	}
	return 0;
}

int Jednorozec::kolizja_prawo(float sx, float sy, Prostokat p) {
	if ((Granice.pozycja.x + sx + Granice.szerokosc >= p.pozycja.x - p.szerokosc) && (Granice.pozycja.x + sx + Granice.szerokosc <= p.pozycja.x + MOVE)) {
		if ((Granice.pozycja.x + sx + Granice.szerokosc >= p.pozycja.x) && (((Granice.pozycja.y + sy >= p.pozycja.y) && (Granice.pozycja.y + sy + Granice.wysokosc <= p.pozycja.y + p.wysokosc)) || ((Granice.pozycja.y + sy + Granice.wysokosc >= p.pozycja.y + p.wysokosc) && (Granice.pozycja.y + sy <= p.pozycja.y + p.wysokosc) && (Granice.pozycja.y + sy >= p.pozycja.y)) || ((Granice.pozycja.y + sy <= p.pozycja.y) && (Granice.pozycja.y + sy + Granice.wysokosc >= p.pozycja.y) && (Granice.pozycja.y + sy + Granice.wysokosc <= p.pozycja.y + p.szerokosc)))) {
			return 1;
		}
	}
	return 0;
}

int Jednorozec::kolizja_lewo(float sx, float sy, Prostokat p) {
	if ((Granice.pozycja.x + sx <= p.pozycja.x + p.szerokosc + p.szerokosc) && (Granice.pozycja.x + sx >= p.pozycja.x + p.szerokosc - MOVE)) {
		if ((Granice.pozycja.x + sx <= p.pozycja.x + p.szerokosc) && (((Granice.pozycja.y + sy >= p.pozycja.y) && (Granice.pozycja.y + sy + Granice.wysokosc <= p.pozycja.y + p.wysokosc)) ||
			((Granice.pozycja.y + sy + Granice.wysokosc >= p.pozycja.y + p.wysokosc) && (Granice.pozycja.y + sy <= p.pozycja.y + p.wysokosc) && (Granice.pozycja.y + sy >= p.pozycja.y)) || ((Granice.pozycja.y + sy <= p.pozycja.y) && (Granice.pozycja.y + sy + Granice.wysokosc >= p.pozycja.y) && (Granice.pozycja.y + sy + Granice.wysokosc <= p.pozycja.y + p.szerokosc)))) {
			return 1;
		}
	}
	return 0;
}

void Jednorozec::uaktualnij(float dt, float sx, float sy) {
	this->Granice.pozycja.x += sx;
	this->Granice.pozycja.y += sy;
	a_temp.x = 0;
	a_temp.y = 0;
	v_temp.x = 0;
	v_temp.y = 0;
}

void Jednorozec::ustawSterowanie(int sterowanieGry) {
	sterowanie = sterowanieGry;
}

//poruszanie podstawowe
void Jednorozec::prawo() {
	v_temp.x = MOVE;
}

void Jednorozec::lewo() {
	v_temp.x = -1 * MOVE;
}

void Jednorozec::gora() {
	v_temp.y = -1 * MOVE;
}

void Jednorozec::dol() {
	v_temp.y = MOVE;
}

ObiektGry::ObiektGry(const Prostokat& granice) : Granice(granice) {	
}

Platforma::Platforma(const Prostokat& granice) : ObiektGry(granice) {
}

Przeszkoda::Przeszkoda(const Prostokat& granice) : ObiektGry(granice) {
}
 
Jednorozec::Jednorozec(const Punkt_Float& punkt) : ObiektGry (Prostokat(punkt, WIELKOSC_JEDNOROZCA, WIELKOSC_JEDNOROZCA)), a(0,0),v(0,0),a_temp(0,0) {
}