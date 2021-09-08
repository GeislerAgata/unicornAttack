#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "Gra.h"
#include "Wektor.h"
#include "Renderer.h"
#include "Common.h"
#include "ObiektGry.h"


extern "C" {
#include"../template-project-2/SDL2-2.0.10/include/SDL.h"
#include"../template-project-2/SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define	JUMP_STRENGHT	15000
#define	DASH_STRENGHT	15000
#define ZYCIA 3


// main
#ifdef __cplusplus
extern "C"
#endif

//funkcja rysuj¹ca informacje o zderzeniu i utracie ¿ycia
void draw_zderzenie(Renderer r, SDL_Surface * screen, SDL_Surface * charset);
//funkcja rysuj¹ca informacje o wykorzystaniu ¿yæ i koñcu gry
void draw_koniec_gry(Renderer r, SDL_Surface* screen, SDL_Surface* charset);
//funkcja rysuj¹ca tlo i ¿ycia jednoro¿ca
void draw_tlo_zycia(Renderer r, int liczba_zyc, SDL_Surface* screen, SDL_Surface* charset, SDL_Surface* tlo, SDL_Surface* serce);
//funkcja rysuj¹ca menu koñcowe
void draw_menu(Renderer r, SDL_Surface* screen, SDL_Surface* charset, SDL_Surface* menu_end, float punkty);
//funkcja rysuj¹ca obiekty w grze
void draw_gra(Renderer r, SDL_Surface* screen, SDL_Surface* charset, double worldTime, double fps, float punkty);
//funkcja uaktualniaj¹ca dane na ekranie
void upgrade_texture(SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* screen);
//funkcje tworz¹ce kolory u¿ywane do rysowania
int fioletowy(SDL_Surface* screen);
int rozowy(SDL_Surface* screen);
int czarny(SDL_Surface* screen);

int main(int argc, char **argv) {	
	int t1, t2, quit = 0, frames = 0, rc, restart, zmiana_sterowania = 0, start_game = 0, liczba_zyc = ZYCIA, menu = 0, wyswietl_menu = 0;
	double delta, worldTime = 0, fpsTimer = 0, fps = 0;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *tlo, *start, *menu_start, *menu_end, *serce;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
	
	Gra gra; //tworzenie nowej gry
	gra.Odczytaj(); //pobieranie danych o planszy z gry
	Renderer r(gra.getObiektyGry()); //tworzenie renderera z obiektami gry
	gra.getJednorozec()->getSzerokosc(gra.getSzerokosc()); //przekazywanie szerokoœci ekranu do obiektu gry jednoro¿ec

	//instrukcje pocz¹tkowe
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, "Gra unicorn atack"); //nazwa okna gry

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE); // wy³¹czenie widocznoœci kursora myszy

	charset = SDL_LoadBMP("cs8x8.bmp"); //wczytywanie charset
	if(charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	tlo = SDL_LoadBMP("tlo (1).bmp"); //wczytanie t³a
	start = SDL_LoadBMP("start.bmp"); //wczytanie ekranu startowego
	menu_start = SDL_LoadBMP("start_menu.bmp");	//wcztanie menu startowego
	menu_end = SDL_LoadBMP("end_menu.bmp"); //wczytanie menu koñcowego
	serce = SDL_LoadBMP("serce.bmp"); //wczytanie serca obrazuj¹cego iloœæ ¿yæ jednoro¿ca

	t1 = SDL_GetTicks();
	if (gra.getZakoncz() == 1) quit = 1; //sprawdzanie poprawnosci pliku wejsciowego

	while (!quit) { //rozpoczêcie pêtli gry
		//obliczanie czasu gry oraz fps
		t2 = SDL_GetTicks();
		delta = (t2 - t1) * 0.001; 	// delta to t2 - t1 przeliczone na milisekundy, czas od ostatniego narysowania
		t1 = t2;
		worldTime += delta;
		fpsTimer += delta;
		if (fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};	
		if (wyswietl_menu == 1) { //wyswietlanie menu koncowego
			draw_menu(r, screen, charset, menu_end, gra.getJednorozec()->returnPunkty());
			upgrade_texture(scrtex, renderer, screen);

			while (SDL_PollEvent(&event)) {
				const Uint8* state = SDL_GetKeyboardState(NULL);
				switch (event.type) {
				case SDL_KEYDOWN:
					if (state[SDL_SCANCODE_ESCAPE]) { //wyjœcie esc
						quit = 1;
					}
					else if (state[SDL_SCANCODE_R]) { //restart ca³ej gry
						liczba_zyc = ZYCIA; //odnowienie ilosci ¿yæ
						gra.restart();
						worldTime = 0;
						wyswietl_menu = 0; //zakoñczenie wyœwietlania menu koñcowego
						menu = 0; //zakoñczenie wyœwietlania
						gra.getJednorozec()->getZderzenie(0); //zmiana stanu zderzenia na 0 - brak zderzenia
					}
					break;
				case SDL_KEYUP:
					break;
				case SDL_QUIT:
					quit = 1;
					break;
				}
			}
		}		
		else { //wyswietlanie gry
			draw_tlo_zycia(r,liczba_zyc, screen, charset, tlo, serce);		
			gra.uaktualnij(delta);	
			if (menu == 1) { //obsluga braku dostêpnych ¿yæ
				draw_koniec_gry(r, screen, charset);
			}		
			else if (gra.getJednorozec()->returnZderzenie() == 1) { //obsluga zderzenia
				worldTime = 0;
				draw_zderzenie(r, screen, charset);

				while (SDL_PollEvent(&event)) {
					const Uint8* state = SDL_GetKeyboardState(NULL);
					switch (event.type) {
					case SDL_KEYDOWN:
						if (state[SDL_SCANCODE_S]) { //rozpoczêcie gry po utracie ¿ycia
							liczba_zyc--;
							if (liczba_zyc <= 0) { //¿ycia jednoro¿ca skoñczy³y siê
								menu = 1;
							}
							else { //jednoro¿ec posiada jeszcze ¿ycia
								gra.getJednorozec()->getZderzenie(0); 
								gra.restart_bez_pkt();
							}
						}
						else if (state[SDL_SCANCODE_ESCAPE]) { //wyjœcie esc
							quit = 1;
						}
						break;
					case SDL_KEYUP: 
						break;
					case SDL_QUIT: 
						quit = 1;
						break;
					};
				}
			}
			r.render_Frame(screen); //renderowanie obiektów gry oraz przemieszczanie kamery
			if (gra.getJednorozec()->zawin() == 1) { //zawijanie etapu gry
				gra.restart_zawin();
			}
			draw_gra(r, screen, charset, worldTime, fps, gra.getJednorozec()->returnPunkty());
			upgrade_texture(scrtex, renderer, screen);

			while (SDL_PollEvent(&event)) {				
				if (zmiana_sterowania == 0) { //tryb sterowania podstawowego
					const Uint8* state = SDL_GetKeyboardState(NULL);
					switch (event.type) {
					case SDL_KEYDOWN: 
						if (state[SDL_SCANCODE_ESCAPE]) { //wyjœcie esc
							quit = 1;
						}
						else if (state[SDL_SCANCODE_N]) { //restart gry
							liczba_zyc = ZYCIA; //odnowienie liczby ¿yæ
							gra.restart();
							worldTime = 0;
						}
						else if (state[SDL_SCANCODE_D]) { //zmiana sterowania na zaawansowane
							zmiana_sterowania = 1;
							gra.getJednorozec()->ustawSterowanie(zmiana_sterowania);
						}
						else if (state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_UP]) { //ruch prawo i góra 
							gra.getJednorozec()->prawo();
							gra.getJednorozec()->gora();
						}
						else if (state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_DOWN]) { //ruch prawo i dó³
							gra.getJednorozec()->prawo();
							gra.getJednorozec()->dol();
						}
						else if (state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_UP]) { //ruch lewo i góra
							gra.getJednorozec()->lewo();
							gra.getJednorozec()->gora();
						}
						else if (state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_DOWN]) { //ruch lewo i dó³
							gra.getJednorozec()->lewo();
							gra.getJednorozec()->dol();
						}
						else if (state[SDL_SCANCODE_RIGHT]) { //ruch prawo
							gra.getJednorozec()->prawo();
						}
						else if (state[SDL_SCANCODE_LEFT]) { //ruch lewo
							gra.getJednorozec()->lewo();
						}
						else if (state[SDL_SCANCODE_UP]) { //ruch góra
							gra.getJednorozec()->gora();
						}
						else if (state[SDL_SCANCODE_DOWN]) { //ruch dó³
							gra.getJednorozec()->dol();
						}
						else if (state[SDL_SCANCODE_M]) { //wyjœcie do menu
							wyswietl_menu = 1;
						}
						else if (state[SDL_SCANCODE_W]) { //zapisanie wyniku do pliku
							char pseudonim[6] = { "gracz" };
							gra.ZapiszWynik(pseudonim, gra.getJednorozec()->returnPunkty()); 
						}
						break;
					case SDL_KEYUP:
						break;
					case SDL_QUIT:
						quit = 1;
						break;
					};
				}			
				else { //tryb sterowanie rozszerzonego
					const Uint8* state = SDL_GetKeyboardState(NULL);
					switch (event.type) {
					case SDL_KEYDOWN: 
						if (state[SDL_SCANCODE_ESCAPE]) { //wyjœcie esc
							quit = 1;
						}
						else if (state[SDL_SCANCODE_N]) { //restart gry
							gra.restart();
						}
						else if (state[SDL_SCANCODE_D]) { //zmiana sterowania na podstawowe
							zmiana_sterowania = 0;
							gra.getJednorozec()->ustawSterowanie(zmiana_sterowania);
						}
						else if (state[SDL_SCANCODE_Z]) { //skok
							gra.getJednorozec()->skok(JUMP_STRENGHT);
						}
						else if (state[SDL_SCANCODE_X]) { //dash
							gra.getJednorozec()->dash(DASH_STRENGHT);
						}
						else if (state[SDL_SCANCODE_M]) { //wyjœcie do menu
							wyswietl_menu = 1;	
						}
						else if (state[SDL_SCANCODE_W]) { //zapisanie wyniku do pliku
							char pseudonim[6] = { "gracz" };
							gra.ZapiszWynik(pseudonim, gra.getJednorozec()->returnPunkty());
						}
						break;
					case SDL_KEYUP:
						break;
					case SDL_QUIT: 
						quit = 1;
						break;
					};
				}
			};
			frames++;
		}
	};
	// zwolnienie powierzchni 
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
	};


	int fioletowy(SDL_Surface* screen) {
		int fioletowy = SDL_MapRGB(screen->format, 128, 0, 128);
		return fioletowy;
	}

	int rozowy(SDL_Surface* screen) {
		int rozowy = SDL_MapRGB(screen->format, 255, 0, 255);
		return rozowy;
	}

	int czarny(SDL_Surface* screen) {
		int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
		return czarny;
	}

	void upgrade_texture(SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* screen) {
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	void draw_zderzenie(Renderer r, SDL_Surface* screen, SDL_Surface*charset) {
		char tekst[128];
		r.DrawRectangle(screen, 4, 380, SCREEN_WIDTH - 8, 36, fioletowy(screen), rozowy(screen));
		sprintf(tekst, "Straciles zycie, czy chcesz kontynuowac? Wcisnij s - start, aby kontynuowac");
		r.DrawString(screen, screen->w / 2 - strlen(tekst) * 8 / 2, 395, tekst, charset);
	}

	void draw_koniec_gry(Renderer r, SDL_Surface* screen, SDL_Surface* charset) {
		char tekst[128];
		r.DrawRectangle(screen, 4, 380, SCREEN_WIDTH - 8, 36, rozowy(screen), fioletowy(screen));
		sprintf(tekst, "Koniec gry, jesli chcesz zapisac wynik wcisnij w - zapisz wynik");
		r.DrawString(screen, screen->w / 2 - strlen(tekst) * 8 / 2, 387, tekst, charset);
		sprintf(tekst, "Aby przejsc do menu wcisnij m - menu");
		r.DrawString(screen, screen->w / 2 - strlen(tekst) * 8 / 2, 402, tekst, charset);
	}

	void draw_tlo_zycia(Renderer r, int liczba_zyc, SDL_Surface* screen, SDL_Surface* charset, SDL_Surface* tlo, SDL_Surface* serce) {
		SDL_FillRect(screen, NULL, czarny(screen));
		r.DrawSurface(screen, tlo, 0, 0);
		if (liczba_zyc == ZYCIA) {
			r.DrawSurface(screen, serce, 245, 440);
			r.DrawSurface(screen, serce, 315, 440);
			r.DrawSurface(screen, serce, 385, 440);
		}
		else if (liczba_zyc == ZYCIA - 1) { //po utracie 1 ¿ycia
			r.DrawSurface(screen, serce, 315, 440);
			r.DrawSurface(screen, serce, 385, 440);
		}
		else if (liczba_zyc == ZYCIA - 2) { //po utracie 2 ¿yæ
			r.DrawSurface(screen, serce, 385, 440);
		}
	}

	void draw_menu(Renderer r, SDL_Surface* screen, SDL_Surface* charset, SDL_Surface* menu_end, float punkty) {
		char tekst[128];
		SDL_FillRect(screen, NULL, czarny(screen));
		r.DrawSurface(screen, menu_end, 320, 240);

		r.DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 80, rozowy(screen), fioletowy(screen));
		sprintf(tekst, "Koniec gry");
		r.DrawString(screen, screen->w / 2 - strlen(tekst) * 8 / 2, 30, tekst, charset);
		sprintf(tekst, "Rozgrywka zakonczyla sie, twoj wynik to: %.2f", punkty);
		r.DrawString(screen, screen->w / 2 - strlen(tekst) * 8 / 2, 50, tekst, charset);

		r.DrawRectangle(screen, 4, 420, SCREEN_WIDTH - 8, 36, rozowy(screen), fioletowy(screen));
		sprintf(tekst, "Aby rozpoczac gre od nowa wcisnij r - restart gry");
		r.DrawString(screen, screen->w / 2 - strlen(tekst) * 8 / 2, 435, tekst, charset);
	}

	void draw_gra(Renderer r, SDL_Surface* screen, SDL_Surface* charset, double worldTime, double fps, float punkty) {
		char tekst[128];
		r.DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, fioletowy(screen), rozowy(screen));
		sprintf(tekst, "Gra unicorn atack, czas trwania = %.1lf s  %.0lf klatek / s", worldTime, fps);
		r.DrawString(screen, screen->w / 2 - strlen(tekst) * 8 / 2, 10, tekst, charset);
		sprintf(tekst, "Esc - wyjscie, n - restart, d - zmiana sterowania (z - skok, x - dash)");
		r.DrawString(screen, screen->w / 2 - strlen(tekst) * 8 / 2, 26, tekst, charset);

		r.DrawRectangle(screen, 4, 44, SCREEN_WIDTH - 8, 18, fioletowy(screen), rozowy(screen));
		sprintf(tekst, "Twoj aktualny wynik to: %.2f", punkty);
		r.DrawString(screen, screen->w / 2 - strlen(tekst) * 8 / 2, 50, tekst, charset);
	}