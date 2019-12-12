#pragma once
#include "myCircle.h"
#include "globalVariable.h"




class bullet: public myCircle
{

private:
	Vector2f direction;
	float speed;
	void autoMove(float dtSeconds);
public:
	bullet(Vector2f location, Vector2f facingDirection, float speed);
	~bullet();
	virtual void update(float dtSeconds) override;

};

