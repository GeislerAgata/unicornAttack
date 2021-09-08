#include "Wektor.h"
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


void_vector* nowy_void_vector(size_t nowy_rozmiar) {
	if (!nowy_rozmiar) {
		nowy_rozmiar = ROZMIAR;
	}
	void_vector* vv = (void_vector*)malloc(sizeof * vv + sizeof * vv->element * nowy_rozmiar);
	if (vv) {
		vv->rozmiar = nowy_rozmiar;
		vv->dlugosc = 0;
	}
	return vv;
}

vv_err vv_push(void_vector** vv, void* nowy_element) {
	if ((*vv)->dlugosc >= (*vv)->rozmiar) {
		void_vector* nowy_vv = (void_vector*)realloc((*vv), sizeof(void_vector) + sizeof(void*) * (*vv)->rozmiar * 2);
		if (!nowy_vv) {
			return vv_niepowodzenie;
		}
		(*vv) = nowy_vv;
		(*vv)->rozmiar *= 2;
	}
	(*vv)->element[(*vv)->dlugosc] = nowy_element;
	(*vv)->dlugosc++;
	return vv_sukces;
}

void* vv_pop(void_vector* vv) {
	if (vv->dlugosc == 0) {
		return NULL;
	}
	vv->dlugosc--;
	return vv->element[vv->dlugosc];
}

void* vv_odczytaj_index(void_vector* vv, size_t index) {
	if (index > vv->dlugosc) {
		return NULL;
	}
	return (vv->element[index]);
}

void usun_void_vector(void_vector* vv, void (usun_element)(void*)) {
	if (!usun_element) {
		usun_element = &free;
	}
	for (int i = vv->dlugosc; i; i --) {
		usun_element(vv->element[i - 1]);
	}
	free(vv);
}