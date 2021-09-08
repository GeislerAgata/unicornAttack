#pragma once

//implementacja wektora, aby przechowywa� informacje dowolnego typu (void)
#define ROZMIAR 16u

typedef struct vv {
	size_t dlugosc;
	size_t rozmiar;
	void* element[];
} void_vector;

typedef enum {
	vv_sukces,
	vv_niepowodzenie,
} vv_err;

//funkcja tworzaca nowy void vector w pami�ci
void_vector* nowy_void_vector(size_t rozmiar);
//funkcja dodaj�ca element do vectora
vv_err vv_push(void_vector** vv, void* element);
//funkcja usuwaj�ca element z vectora
void* vv_pop(void_vector* vv);
//funkcja zwracaj�ca warto�� wskazanego elementu z vectora
void* vv_odczytaj_index(void_vector* vv, size_t index);
//funkcja usuwaj�ca vector z pami�ci
void usun_void_vector(void_vector* vv, void (usun_element)(void*));