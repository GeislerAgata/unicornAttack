#include "Renderer.h"
#include "Common.h"
#include "Wektor.h"
#include "ObiektGry.h"
#include "Gra.h"

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define SCENE_HEIGHT	600

Renderer::Renderer(void_vector* obiektyGry) : obiektyGry(obiektyGry) {
	Prostokat poczatek(0, (SCENE_HEIGHT - SCREEN_HEIGHT) / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
	camera = new Camera(poczatek); //tworzenie kamery o sta³ych parametrach pocz¹tkowych uzale¿nionych od rozmiaru sceny i okna
}

void Renderer::DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
}

void Renderer::DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
}

void Renderer::DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {

	if (x < 0 || y < 0 || x >= surface->w || y >= surface->h) {
		return;
	}	
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
}

void Renderer::DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
}

void Renderer::DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
}


void Renderer::kameraDoJednorozca() {
	for (size_t i = 0; i < obiektyGry->dlugosc; i++) {
		ObiektGry* aktualny = (ObiektGry*)vv_odczytaj_index(obiektyGry, i);

		if (dynamic_cast<Jednorozec*>(aktualny) != nullptr) { // jeœli obiekt jest jednoro¿cem
			Prostokat p = aktualny->getGranice(); //wspó³rzêdne wzglêdem sceny nie kamery
			camera->porusz(p.pozycja.x, p.pozycja.y); //poruszenie kamery
			break;
		}				
	}
}

void Renderer::render_Frame(SDL_Surface* screen)
{
	SDL_Surface* jednorozec_bmp;
	jednorozec_bmp = SDL_LoadBMP("Stasiek.bmp"); 
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int fioletowy = SDL_MapRGB(screen->format, 128, 0, 128);
	int rozowy = SDL_MapRGB(screen->format, 255, 0, 255);
	int brazowy = SDL_MapRGB(screen->format, 139, 69, 19);
	kameraDoJednorozca();
		
	for (size_t i = 0; i < obiektyGry->dlugosc; i++) {
		ObiektGry* aktualny = (ObiektGry*)vv_odczytaj_index(obiektyGry, i); //wyodrêbnianie kolejnych obiektów gry
		Prostokat wzgledemKamery = aktualny->getGranice(); //wspó³rzêdne wzglêdem sceny nie kamery
		
		int kolor = 0;
		if (dynamic_cast<Platforma*>(aktualny) != nullptr) { //je¿eli obiekt jest platform¹
			kolor = brazowy;
		}
		else if (dynamic_cast<Przeszkoda*>(aktualny) != nullptr) { //je¿eli obiekt jest przeszkod¹
			kolor = rozowy;
		}
		else if (dynamic_cast<Jednorozec*>(aktualny) != nullptr) { //je¿eli obiekt jest jednoro¿cem
			kolor = fioletowy;
		}
		wzgledemKamery.pozycja.x -= camera->getGranice().pozycja.x; //zmiana wspó³rzêdnej x na wspó³rzêdn¹ wzglêdem kamery
		wzgledemKamery.pozycja.y -= camera->getGranice().pozycja.y; //zmiana wspó³rzêdnej y na wspó³rzêdn¹ wzglêdem kamery

		if (kolor == fioletowy) { //rysowanie jednoro¿ca
			DrawSurface(screen, jednorozec_bmp, wzgledemKamery.pozycja.x + wzgledemKamery.szerokosc / 2 + 5, wzgledemKamery.pozycja.y + wzgledemKamery.wysokosc / 2);
			//DrawRectangle(screen, wzgledemKamery.pozycja.x, wzgledemKamery.pozycja.y, wzgledemKamery.szerokosc, wzgledemKamery.wysokosc, czarny, NULL);
		}
		else { // rysowanie pozosta³ych elementów
			DrawRectangle(screen, wzgledemKamery.pozycja.x, wzgledemKamery.pozycja.y, wzgledemKamery.szerokosc, wzgledemKamery.wysokosc, czarny, kolor);
		}

	}
}

