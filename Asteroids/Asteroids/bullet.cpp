#include "bullet.h"

void bullet::autoMove(float dtSeconds)
{
	myCircleShape.move(direction * dtSeconds * speed);
	//setLocation(getLocation() + direction * dtSeconds * speed);
	//???remove when touch the boundary
}

bullet::bullet(Vector2f location, Vector2f facingDirection, float speed)
{
	circleType = circleTypeList::bullet;
	
	direction = globalVariable::getUnitVector(facingDirection);
	myCircleShape.setRadius(5);
	setLocation(location + direction * 50.0f);
	this->speed = speed;
	
}

bullet::~bullet()
{
	//std::cout << "destructor is called";
}

void bullet::update(float dtSeconds)
{
	autoMove(dtSeconds);
}
