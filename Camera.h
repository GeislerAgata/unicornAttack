#pragma once

#include "Common.h"
#include "ObiektGry.h"

class Camera : public ObiektGry {
public:
	//konstruktor
	Camera(Prostokat);
	//funkcja poruszaj�ca kamer�
	void porusz(float dx, float dy);
};
