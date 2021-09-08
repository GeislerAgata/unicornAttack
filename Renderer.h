#pragma once

extern "C" {
#include"../template-project-2/SDL2-2.0.10/include/SDL.h"
#include"../template-project-2/SDL2-2.0.10/include/SDL_main.h"
}

#include "Camera.h"
#include "Wektor.h"
#include "Gra.h"
#include "ObiektGry.h"

class Renderer
{
	void_vector* obiektyGry;
	Camera* camera;
	int szerokosc_sceny;
	int wysokosc_sceny;

public:
	// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
	// charset to bitmapa 128x128 zawieraj¹ca znaki
	void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);;
	// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
	// (x, y) to punkt œrodka obrazka sprite na ekranie
	void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);;
	// rysowanie pojedynczego pixela
	void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);;
	// rysowanie linii o d³ugoœci l w pionie (gdy dx = 0, dy = 1) 
	void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);;
	// rysowanie prostok¹ta o d³ugoœci boków l i k
	void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);;
	//przesuwa kamerê do jednoro¿ca
	void kameraDoJednorozca();
	//konstruktor
	Renderer(void_vector* obiektyGry);
	//renderuje okno gry
	void render_Frame(SDL_Surface* screen);
};
