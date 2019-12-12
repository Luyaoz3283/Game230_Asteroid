#pragma once
#include "myCircle.h"




class asteroid: public myCircle
{
private:
	Vector2f direction;
	float speed;
	int asteroidStage;
	int radiusArray[3] = { 10,20,30 };
	
public:
	int asteroidIndex = 3;
	asteroid(int index, Asset *assetManager, float baseSpeed);
	asteroid(int index, Vector2f location, Asset* assetManager, float baseSpeed);
	void randomizeLoc();
	virtual void update(float dtSeconds) override;
	void autoMove(float dtSeconds);
	void bounce(Vector2f newDirection);
	void setAsteroidIndex(int index);
	

};

