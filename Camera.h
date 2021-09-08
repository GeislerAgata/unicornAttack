#pragma once

#include "Common.h"
#include "ObiektGry.h"

class Camera : public ObiektGry {
public:
	//konstruktor
	Camera(Prostokat);
	//funkcja poruszaj¹ca kamer¹
	void porusz(float dx, float dy);
};
