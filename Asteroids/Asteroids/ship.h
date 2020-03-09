#pragma once
#include <iostream>
#include "globalVariable.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "myCircle.h"
#define PI 3.14159265



using namespace sf;

class ship:public myCircle
{

private:
	//CircleShape myCircle;
	Window mainWindow;
	float rps;
	//Vector2f unitDirection;
	Vector2f velocity;
	//Vector2f accDir;
	float maxAcc;
	float accValue;
	float Cd;
	float maxSpeed;
	SoundBuffer* shipSoundBuffer;
	vector<Texture*> thrustAnimTex;
	CircleShape thrustAni;
	bool isThrusting = false;
	sf::Music thrustMusic;
	sf::CircleShape powerUpEffect;
	Asset* assetManager;
	bool isOnShield = false;
	float effectStartTime;
	
public:
	float shieldPeriod = 5;
	ship(float iniRadius, Vector2f iniLoc, float iniRps, float iniAngle, float acc, float Cd, float maxSpeed, Asset *assetManager);
	~ship();
	//void draw(RenderWindow* window);
	void update(float dtSecond);
	void turn(float dtSecond);
	void autoMove(float dtSecond);
	void thrust(float dtSecond);
	void drag(float dtSecond);
	Vector2f getFacingUnitDir();
	void draw(RenderWindow* window);
	void setPowerUpEffect();
	void stopShield();
	bool isShipOnShield();
};

