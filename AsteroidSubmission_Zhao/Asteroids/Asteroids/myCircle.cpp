#include "myCircle.h"
#include "globalVariable.h"




/*
myCircle::myCircle(float radius)
{
	myCircleShape.setRadius(radius);
	myCircleShape.setOrigin(radius, radius);
}
*/


myCircle::myCircle()
{
}

void myCircle::update(float deSeconds)
{
	//std::cout << "update base";
}

void myCircle::setLocation(Vector2f destination)
{
	float radius = getRadius();
	Vector2f circleCenter = getLocation();
	//std::cout << "center1:" << circleCenter.x << "y:" << circleCenter.y;
	Vector2f screenDim = globalVariable::screenDim;
	if (circleCenter.x + radius < 0) {
		myCircleShape.setPosition(Vector2f(screenDim.x + radius, circleCenter.y));
		//std::cout << "location:" << getLocation().x << "y:" << getLocation().y;
		//std::cout << "center:" << circleCenter.x << "y:" << circleCenter.y;
	}
	else if (circleCenter.x - radius > screenDim.x) {
		myCircleShape.setPosition(Vector2f(0 - radius, circleCenter.y));
	}
	else if (circleCenter.y + radius < 0) {
		myCircleShape.setPosition(Vector2f(circleCenter.x, screenDim.y + radius));
	}
	else if (circleCenter.y - radius > screenDim.y) {
		myCircleShape.setPosition(Vector2f(circleCenter.x, 0 - radius));
	}
	else {
		myCircleShape.setPosition(destination);
	}

	
}
Vector2f myCircle::getLocation()
{
	return myCircleShape.getPosition();
}

void myCircle::mySetRotation(float desiredAngle)
{
	curAngle = desiredAngle;
	myCircleShape.setRotation(desiredAngle);
}

float myCircle::getCurAngle()
{
	return curAngle;
}

float myCircle::getRadius()
{
	return myCircleShape.getRadius();
}

void myCircle::setRadiusOrigin(float radius)
{
	//std::cout << "radius set:" << radius;
	myCircleShape.setRadius(radius);
	myCircleShape.setOrigin(radius, radius);
}

void myCircle::draw(RenderWindow* window)
{
	window->draw(myCircleShape);
}

bool myCircle::setTexture(Asset* assetManager, String assetName)
{
	Texture *curAsset = assetManager->getTexture(assetName);
	if (curAsset != nullptr) {
		myCircleShape.setTexture(curAsset);
		return true;
	}
	else return false;
}
void myCircle::addAnimTex(Asset* assetManager, String aniTexture, vector<Texture*>* texVector)
{
	Texture* curAsset = assetManager->getTexture(aniTexture);
	if (curAsset != nullptr) {
		texVector->push_back(curAsset);
		//myCircleShape.setTexture(curAsset);
	}
}



