#include "asteroid.h"
#include "globalVariable.h"







#define PI 3.14159265

//initial build
asteroid::asteroid(int index, Asset* assetManager, float baseSpeed)
{
	//speed = baseSpeed;
	//commen initialize
	circleType = circleTypeList::asteroid;
	this->asteroidIndex = index;
	setRadiusOrigin(radiusArray[index]);
	speed = rand() % 30 + baseSpeed;

	int iniAngle = rand() % 360;
	//std::cout << "direction" << iniAngle;
	direction = Vector2f(cos(iniAngle * PI / 180), sin(iniAngle * PI / 180));
	//random location
	randomizeLoc();
	setTexture(assetManager, "asteroid");
	
}

//second build when the previous asteroid is destroyed
asteroid::asteroid(int index, Vector2f location, Asset *assetManager, float baseSpeed)
{
	//commen initialize
	circleType = circleTypeList::asteroid;
	this->asteroidIndex = index;
	setRadiusOrigin(radiusArray[index]);
	speed = rand() % 30 + baseSpeed;
	int iniAngle = rand() % 360;
	//std::cout << "direction" << iniAngle;
	direction = Vector2f(cos(iniAngle * PI / 180), sin(iniAngle * PI / 180));
	//fixed location
	setLocation(location);
	setTexture(assetManager, "asteroid");
}

void asteroid::randomizeLoc()
{	
	int screenX = globalVariable::screenDim.x;
	int screenY = globalVariable::screenDim.y;
	float ranX = rand() % screenX;
	float ranY = rand() % screenY;
	setLocation(Vector2f(ranX, ranY));
}

void asteroid::update(float dtSeconds)
{
	//std::cout << "asteroid update";
	if (globalVariable::gameState == globalVariable::gameStateList::start) {
		autoMove(dtSeconds);
	}

	/*
	if (Keyboard::isKeyPressed(Keyboard::Key::A)) {
		myCircleShape.setRadius(200);
	}
	*/
	
}

void asteroid::autoMove(float dtSeconds)
{
	Vector2f destination = getLocation() + direction * speed * dtSeconds;
	setLocation(destination);
	//std::cout << "auto move";
}

void asteroid::bounce(Vector2f newDirection)
{
	direction = newDirection;
}

void asteroid::setAsteroidIndex(int index)
{
	this->asteroidIndex = index;
	float radius = radiusArray[index];
	setRadiusOrigin(radius);
}


