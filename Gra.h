#pragma once
#include "Wektor.h"
#include "ObiektGry.h"
#include "Camera.h"
#include<stdio.h>

class Gra
{
private:
	void_vector* obiektyGry;
	Jednorozec* jednorozec;
	void_vector* wyniki;
	int szerokosc; //szerokosc sceny
	int wysokosc; //wysokosc sceny
	int zakoncz_gre = 0; //przyjmuje wartosc 1, gdy dane z pliku s¹ niepoprawne
public:
	//konstruktor
	Gra();
	//destruktor
	~Gra();
	//obsluga restartu i zawijania sceny
	void restart();
	void restart_bez_pkt();
	void restart_zawin();
	//aktualizowanie jednorozca i kamery
	void uaktualnij(float dt);
	//odczytywanie parametrów planszy
	void Odczytaj();
	//obsluga wynikow graczy
	void ZapiszWynik(char* pseudonim, float wynik);
	void OdczytajWyniki();
	void CzyscWyniki();
	//funkcje pomocnicze pobieraj¹ce konkretne elementy
	void_vector* getObiektyGry();
	Jednorozec* getJednorozec();
	int getSzerokosc();
	int getWysokosc();
	void_vector* getWyniki();
	int getZakoncz();
};

