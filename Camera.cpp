#include "Camera.h"
#include "ObiektGry.h"

#define ZMNIEJSZENIE 10

Camera::Camera(Prostokat granice) : ObiektGry(granice){
}

void Camera::porusz(float dx, float dy) {
	Granice.pozycja.x = dx;
	Granice.pozycja.y = dy / ZMNIEJSZENIE;
}
