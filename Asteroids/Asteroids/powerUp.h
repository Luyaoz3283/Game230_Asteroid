#pragma once
#include "myCircle.h"



class powerUp: public myCircle
{
private:
	Asset* assetManager;
	
public:
	powerUp(Vector2f destroyedLocation, Asset *assetManager);
	~powerUp();
};

