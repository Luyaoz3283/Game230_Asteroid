#include "ship.h"



ship::ship(float iniRadius, Vector2f iniLoc, float iniRps, float iniAngle, float acc, float Cd, float maxSpeed, Asset *assetManager)
{
	circleType = circleTypeList::ship;
	myCircleShape.setPointCount(3);
	//myCircleShape.setTexture(texture);
	maxAcc = acc;
	//reset
	myCircleShape.setRadius(iniRadius);
	myCircleShape.setOrigin(iniRadius, iniRadius);
	//setRadiusOrigin()
	setLocation(iniLoc);
	mySetRotation(iniAngle);
	this->rps = iniRps;
	velocity = Vector2f(0.0001, 0.0001);
	accValue = acc;
	this->Cd = Cd;
	this->maxSpeed = maxSpeed;
	//std::cout << "inheritance is called";
	//set thrust
	thrustAni.setRadius(40);
	thrustAni.setOrigin(Vector2f(40, 40));
	thrustAni.setPosition(getLocation());
	if (assetManager->getTexture("thrust1") != nullptr) {
		thrustAni.setTexture(assetManager->getTexture("thrust1"));
	}
	//set effects
	setTexture(assetManager, "ship");
	addAnimTex(assetManager, "thrust1", &thrustAnimTex);
	//test
	if (!thrustMusic.openFromFile("thrust.wav")) {
		cout << "no music";
	}
	//thrustMusic.play();
	thrustMusic.setLoop(true);
	this->assetManager = assetManager;
	
	
}

ship::~ship()
{
	cout << "destructor called ship";
}



void ship::update(float dtSecond)
{
	if (globalVariable::gameState == globalVariable::gameStateList::start) {
		turn(dtSecond);
		autoMove(dtSecond);
		thrust(dtSecond);
		drag(dtSecond);
		//std::cout << "ship update";
	}
	thrustAni.setPosition(getLocation());
	//stop shield when time passes
	if (isOnShield) {
		
		if (clock() / (float)CLOCKS_PER_SEC - effectStartTime > shieldPeriod) {
			stopShield();
		}
		else {
			//if on effect, keep the effect on the player
			powerUpEffect.setPosition(getLocation());
		}
	}
	
}

//C
void ship::turn(float dtSecond)
{
	if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
		float curAngle = getCurAngle() - dtSecond * rps;
		mySetRotation(curAngle);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
		float curAngle = getCurAngle() + dtSecond * rps;
		mySetRotation(curAngle);
	}
}

void ship::autoMove(float dtSecond)
{
	Vector2f destination = getLocation() + velocity * dtSecond *50.0f;
	setLocation(destination);
}

void ship::thrust(float dtSecond)
{
	if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
		isThrusting = true;
		if (thrustMusic.getStatus() == 0) {
			thrustMusic.play();
			
		}
		//
		accValue = maxAcc;
		Vector2f facingDir = getFacingUnitDir();
		Vector2f vel = velocity + facingDir * accValue * dtSecond;
		if (sqrt(pow(vel.x, 2) + pow(vel.y, 2)) <= maxSpeed) {
			velocity = velocity + facingDir * accValue * dtSecond;
		}
	}
	else {

		isThrusting = false;
		thrustMusic.stop();
		accValue = 0;
	}
}

void ship::drag(float dtSecond)
{
	Vector2f velDir = globalVariable::getUnitVector(velocity);
	float velMag = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
	Vector2f acc = velDir * Cd * velMag * (-1.0f);
	velocity = velocity + acc * dtSecond;
}

Vector2f ship::getFacingUnitDir()
{
	float angleInRad = -(90.0f - getCurAngle()) * PI / 180.0;
	Vector2f direction(cos(angleInRad), sin(angleInRad));
	return direction;
}

void ship::draw(RenderWindow* window)
{
	if (isThrusting) {
		window->draw(thrustAni);
	}
	if (isOnShield) {
		window->draw(powerUpEffect);
	}
	
	window->draw(myCircleShape);
}

void ship::setPowerUpEffect()
{
	isOnShield = true;
	powerUpEffect.setRadius(getRadius());
	powerUpEffect.setOrigin(getRadius(), getRadius());
	Texture *newTexture = assetManager->getTexture("playerShield");
	powerUpEffect.setTexture(newTexture);
	effectStartTime = clock() / (float)CLOCKS_PER_SEC;
	
}

void ship::stopShield()
{
	isOnShield = false;
}

bool ship::isShipOnShield()
{
	return isOnShield;
}







