#include "powerUp.h"




powerUp::powerUp(Vector2f destroyedLocation, Asset* assetManager)
{
	circleType = circleTypeList::powerUp;
	this->assetManager = assetManager;
	myCircleShape.setRadius(20);
	myCircleShape.setPosition(destroyedLocation);
	setTexture(this->assetManager, "dropShield");

	
}

powerUp::~powerUp()
{
}
