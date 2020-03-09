#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Asset.h"

using namespace sf;


class myCircle
{
private:
	float curAngle;
	
	

protected:
	CircleShape myCircleShape;
	
public:
	int index;
	enum circleTypeList { ship, bullet, asteroid, powerUp};
	circleTypeList circleType;
	myCircle();
	//myCircle(float raius);
	virtual void update(float deSeconds);
	void setLocation(Vector2f destination);
	Vector2f getLocation();
	void mySetRotation(float desiredAngle);
	float getCurAngle();
	float getRadius();
	void setRadiusOrigin(float radius);
	virtual void draw(RenderWindow *window);
	bool setTexture(Asset* assetManager, String assetName);
	void addAnimTex(Asset* assetManager, String aniTexture, vector<Texture*>* texVector);
};

