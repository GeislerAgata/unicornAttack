#include "Gra.h"
#include "Wektor.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROZMIAR 10 //wielkosc vectora
#define X_START 5 //startowa pozycja x jednoro¿ca
#define Y_START 230 //startowa pozycja u jednoro¿ca
#define PARAMETRY 4 //liczba parametrów ka¿dego z obiektów gry

Gra::Gra() {
	obiektyGry = nowy_void_vector(ROZMIAR);
	wyniki = nowy_void_vector(ROZMIAR);
	jednorozec = new Jednorozec(Punkt_Float(X_START, Y_START));
	vv_push(&obiektyGry,jednorozec);
}

 Gra::~Gra() {
	delete jednorozec;
}

void Gra::Odczytaj() {
	wysokosc = 0;
	szerokosc = 0;
	int przeszkod, platform;

	char** linie = new char* [100];
	for (int i = 0; i < 100; i++) {
		linie[i] = new char[1000];
	}

	FILE* plik = fopen("stan_gry.txt", "r");
	int i = 0;
	fgets(linie[i], 1000, plik);
	char* wyraz;
	wyraz = strtok(linie[0], " \n");
	szerokosc = atoi(wyraz); // szerokoœæ planszy
	for (i = 0; i < 3; i++) {
		wyraz = strtok(NULL, " \n");
		if (i == 0) {
			wysokosc = atoi(wyraz); // wysokoœæ planszy
		}
		else if (i == 1) {
			przeszkod = atoi(wyraz); // ilosc_przeszkód
		}
		else if (i == 2) {
			platform = atoi(wyraz); // ilosc_platform 
		}
	}
	i = 1;
	while (!feof(plik)) {
		fgets(linie[i], 1000, plik);
		i++;
	}
	if (i == 1 + przeszkod + platform) {
		printf("Podane dane sa poprawne\n");
		for (i = 1; i <= przeszkod; i++) {
			int prostokat[PARAMETRY] = { 0 };
			float float_prostokat[PARAMETRY] = { 0 };
			char* dane;
			dane = strtok(linie[i], " \n");
			prostokat[0] = atoi(dane);
			float_prostokat[0] = (float)prostokat[0];
			for (int j = 1; j < PARAMETRY; j++) {
				dane = strtok(NULL, " \n");
				prostokat[j] = atoi(dane);
				float_prostokat[j] = (float)prostokat[j];
			}
			Prostokat pr(float_prostokat[0], float_prostokat[1], float_prostokat[2], float_prostokat[3]);
			Platforma* p = new Platforma(pr);
			vv_push(&obiektyGry, p);
		}
		for (i = przeszkod + 1; i <= przeszkod + platform; i++) {
			int prostokat[PARAMETRY] = { 0 };
			float float_prostokat[PARAMETRY] = { 0 };
			char* dane;
			dane = strtok(linie[i], " \n");
			prostokat[0] = atoi(dane);
			float_prostokat[0] = (float)prostokat[0];
			for (int j = 1; j < 4; j++) {
				dane = strtok(NULL, " \n");
				prostokat[j] = atoi(dane);
				float_prostokat[j] = (float)prostokat[j];
			}
			Prostokat pr2(float_prostokat[0], float_prostokat[1], float_prostokat[2], float_prostokat[3]);
			Przeszkoda* pa = new Przeszkoda(pr2);
			vv_push(&obiektyGry, pa);
		}
		jednorozec->getObiektyGry(obiektyGry);
	}
	else {
		printf("Dane sa niepoprawne\n");
		zakoncz_gre = 1;
	}
	fclose(plik);
}

void Gra::ZapiszWynik(char* pseudonim, float wynik) {
	FILE* plik = fopen("wyniki.txt", "a");
	fprintf(plik, "%s %.2f\n", pseudonim, wynik);
	fclose(plik);
}

void Gra::OdczytajWyniki() {
	CzyscWyniki();
	void_vector* wyniki_nieposortowane;
	wyniki_nieposortowane = nowy_void_vector(ROZMIAR);
	FILE* plik = fopen("wyniki.txt", "r");
	int licznik = 0;
	while (!feof(plik)) {
		char linia[1000] = { "" };
		fgets(linia, 1000, plik);

		Wynik* w = new Wynik();
	
		char* wynik = strtok(linia, " \n");
		strcpy_s(w->psuedo, wynik);
		
		wynik = strtok(NULL, " \n");
		w->punkty = atoi(wynik);

		vv_push(&wyniki_nieposortowane, w);
		licznik ++;
	}
	fclose(plik);

	for (int i = 0; i < wyniki_nieposortowane->dlugosc / 2; i++) {
		int index_max = 0;
		for (int j = 1; j < wyniki_nieposortowane->dlugosc; j+=2) {
			if (vv_odczytaj_index(wyniki_nieposortowane, j) > vv_odczytaj_index(wyniki_nieposortowane, index_max)) {
				index_max = j;
			}
		}
		void* abc = vv_odczytaj_index(wyniki_nieposortowane, index_max - 1);
		vv_push(&wyniki, abc); //pseudonim
		vv_push(&wyniki, vv_odczytaj_index(wyniki_nieposortowane, index_max)); //wynik
	}
	usun_void_vector(wyniki_nieposortowane, nullptr);
}

void Gra::uaktualnij(float dt) {	
	jednorozec->sprawdz(dt);
}

int Gra::getZakoncz() {
	return zakoncz_gre;
}

void_vector* Gra::getObiektyGry() {
	return obiektyGry;
}

Jednorozec* Gra::getJednorozec() {
	return jednorozec;
}

int Gra::getSzerokosc() {
	return szerokosc;
}

int Gra::getWysokosc() {
	return wysokosc;
}

void Gra::restart() {
	jednorozec->reset();
}

void Gra::restart_bez_pkt() {
	jednorozec->reset_bez_pkt();
}

void Gra::restart_zawin() {
	jednorozec->reset_zawin();
}

void Gra::CzyscWyniki() {
	if (wyniki != nullptr) {
		usun_void_vector(wyniki, nullptr);
	}
}

void_vector* Gra::getWyniki() {
	return wyniki;
}