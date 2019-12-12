#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "globalVariable.h"
#include "ship.h"
#include "asteroid.h"
#include "bullet.h"
#include <ctime>
#include "Asset.h"
#include "myDefinition.h"
#include "explodeAni.h"
#include "powerUp.h"

using namespace sf;
using namespace std;


//create objects
ship* ship1;
//text
Text scoreBoard;
Text lifeBoard;
Text gameOver1;
Text gameOver2;
Text mainMenu1;
Text mainMenu2;

//texture
Sprite backGroundSprite;
Texture *bgTexture;
//Texture shipTexture;
//sound
SoundBuffer* hitAsteroid;
SoundBuffer* fire;


SoundBuffer bounceWall;
SoundBuffer lose;
SoundBuffer *levelUpSoundBuffer;
Sound sound;
Sound fireBulletSound;
//Sound sound2;

//set ship parameters
//CircleShape triangle(80, 3);
float radius = 50;
float FPS = 60;

float rps = 150;
Vector2f iniLoc(500, 500);
float iniAngle = 180;
float acc = 20;
float Cd = 1.3;
float maxSpeed = 10.0f;

//asteroid
int Nasteroid = 1;
vector<asteroid> asteroidList;

//bullet
float bulletSpeed = 800.0f;
vector<bullet> bulletList;
int maxBulletN = 20;
clock_t preEmitTime;
float emitInterval = 0.1f;

//initialize
globalVariable::gameStateList globalVariable::gameState;
RenderWindow* window;
vector<myCircle*> objectList;
const int a = 10;
//bucket
int NrowsBucket = 4;
int NcolsBucket = 4;
float xInterval;
float yInterval;
vector<vector<myCircle*>> bucket;

//shield
int curShiled = 0;
float iniInvisibleTime;
float iviPeroid = 0;

//global

Asset myAsset;
vector<explodeAni> explodeAniObj;
int score = 0;
int maxLife = 3;
int curLife = maxLife;
bool stopRound = false;
float iniStopTime;
vector<powerUp> powerUpList;
int asteroidSpeedLevel[] = { 80,220,280,360,450,660,770,1500};
int NAsteroid[] = { 3,8,10,12,16,18,25,27 };
int curLevel = 0;
float iniEscTime = 0;


void drawMainMenu() {
	window->clear();
	window->draw(backGroundSprite);
	window->draw(mainMenu1);
	window->draw(mainMenu2);
	window->display();
}

void drawGameOverScreen() {
	window->clear();
	window->draw(backGroundSprite);
	window->draw(gameOver1);
	window->draw(gameOver2);
	window->display();
}

void drawGraph() {
	window->clear();
	window->draw(backGroundSprite);
	//ship1->draw(window);
	for (int i = 0; i < objectList.size(); i++) {
		objectList[i]->draw(window);
	}
	for (int i = 0; i < explodeAniObj.size(); i++) {
		explodeAniObj[i].draw(window);
	}
	//cout << "draw: bullet size" << bulletList.size();
	//draw powerups
	for (int i = 0; i < powerUpList.size(); i++) {
		powerUpList[i].draw(window);
	}
	//draw text boards
	window->draw(scoreBoard);
	window->draw(lifeBoard);
	window->display();
}
//check collision start

int getBucketIndex(Vector2f location) {
	int xIndex = floor(location.x / xInterval);
	if (xIndex <0) {
		xIndex = 0;
	}
	else if (xIndex >= NcolsBucket) {
		xIndex = NcolsBucket - 1;
	}
	int yIndex = floor(location.y / yInterval);
	if (yIndex < 0) {
		yIndex = 0;
	}
	else if (yIndex >= NcolsBucket) {
		yIndex = NrowsBucket - 1;
	}
	int buckIndex = xIndex + 4 * yIndex;
	//cout << "index:" << buckIndex;
	if (buckIndex >= 16) {
		cout << ">=16";
	}
	return buckIndex;
}

bool contain(vector<int> vectorList, int possibleIndex) {
	for (int i = 0; i < vectorList.size(); i++) {
		if (vectorList[i] == possibleIndex) {
			return true;
		}
	}
	return false;
}

vector<int> getBucketIndexList(myCircle thisCircle) {
	vector<int> indexList;
	
	Vector2f corner;
	float radius = thisCircle.getRadius();
	int bucketIndex;
	//left upper
	corner = thisCircle.getLocation() + Vector2f(-radius, -radius);
	bucketIndex = getBucketIndex(corner);
	if (!contain(indexList, bucketIndex)) {
		indexList.push_back(bucketIndex);
	}
	
	//left down
	corner = thisCircle.getLocation() + Vector2f(-radius, +radius);
	bucketIndex = getBucketIndex(corner);
	if (!contain(indexList, bucketIndex)) {
		indexList.push_back(bucketIndex);
	}
	//right upper
	corner = thisCircle.getLocation() + Vector2f(+radius, -radius);
	bucketIndex = getBucketIndex(corner);
	if (!contain(indexList, bucketIndex)) {
		indexList.push_back(bucketIndex);
	}
	//right down
	corner = thisCircle.getLocation() + Vector2f(+radius, +radius);
	bucketIndex = getBucketIndex(corner);
	if (!contain(indexList, bucketIndex)) {
		indexList.push_back(bucketIndex);
	}
	return indexList;
}

void putAllInBucket() {
	/*
	for (auto p : bucket)
	{
		delete p;
	}
	*/
	for (int i = 0; i < bucket.size(); i++) {
		bucket[i].clear();
	}
	//add all objects from the objectList to bucket
	for (int i = 0; i < objectList.size(); i++) {
		vector<int> bucketIndexList = getBucketIndexList(*objectList[i]);
		for (int j = 0; j < bucketIndexList.size(); j++) {
			bucket[bucketIndexList[j]].push_back(objectList[i]);
		}
	}
}

bool checkCollision(myCircle object1, myCircle object2) {
	if (object1.index != object2.index) {
		Vector2f distanceVector = object1.getLocation() - object2.getLocation();
		float distance = sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2));
		float radiusSum = object1.getRadius() + object2.getRadius();
		//cout << "object 1 index: " << object1.index << "object 2 index: " << object2.index << "distance:" << distance << "radiusSum: " << radiusSum;
		if (radiusSum >= distance) {
			return true;
		}
	}
	return false;
}

int asteroidSearch(asteroid *asteroidToBeSearched) {
	for (int i = 0; i < asteroidList.size(); i++) {
		if (asteroidToBeSearched == &asteroidList[i]) {
			//cout << "asteroidSearch:" << i;
			return i;
		}
	}
	return -1;
}

int powerUpSearch(powerUp* powerUpToBeSearched) {
	for (int i = 0; i < powerUpList.size(); i++) {
		if (powerUpToBeSearched == &powerUpList[i]) {
			//cout << "asteroidSearch:" << i;
			return i;
		}
	}
	return -1;
}


void destroyBullet(bullet* asteroidToBeSearched) {
	for (int i = 0; i < bulletList.size(); i++) {
		if (asteroidToBeSearched == &bulletList[i]) {
			//cout << "destroyed:" << i;
			bulletList.erase(bulletList.begin() + i);
			return;
		}
	}
	//cout << "bullet not found";
}

void invincibility(float inputPeriod) {
	iniInvisibleTime = clock() / (float)CLOCKS_PER_SEC;
	iviPeroid = inputPeriod;
}

void startRound() {
	if (globalVariable::gameState == globalVariable::gameStateList::stopRound) {
		globalVariable::gameState = globalVariable::gameStateList::start;
	}
	
	//ship
	ship1 = new ship(radius, iniLoc, rps, iniAngle, acc, Cd, maxSpeed, &myAsset);
	invincibility(2);
}


/*
void waitForSeconds(float stopPeriod) {
	float startTime = clock() / (float)CLOCKS_PER_SEC;
	while (clock() / (float)CLOCKS_PER_SEC - startTime < stopPeriod) {
		cout << "stop";
	}
}
*/

int processCollision(myCircle *object1, myCircle *object2) {
	//the function returns -1 when ship collides with an asteroid
	//asteroid & asteroid collision
	if (object1->circleType == myCircle::circleTypeList::asteroid && object2->circleType == myCircle::circleTypeList::asteroid) {
		asteroid* theAsteroid1 = dynamic_cast<asteroid*>(object1);
		asteroid* theAsteroid2 = dynamic_cast<asteroid*>(object2);
		Vector2f difference = theAsteroid1->getLocation() - theAsteroid2->getLocation();
		Vector2f unitDifference1 = globalVariable::getUnitVector(difference);
		Vector2f unitDifference2 = Vector2f(unitDifference1.x * (-1), unitDifference1.y * (-1));
		theAsteroid1->bounce(unitDifference1);
		theAsteroid2->bounce(unitDifference2);
		//bounce
	}
	//ship & asteroid collision 
	else if ((object1->circleType == myCircle::circleTypeList::ship && object2->circleType == myCircle::circleTypeList::asteroid)|| (object1->circleType == myCircle::circleTypeList::asteroid && object2->circleType == myCircle::circleTypeList::ship)) {
		bool onShield = ship1->isShipOnShield();
		if (!onShield && (clock() / (float)CLOCKS_PER_SEC) - iniInvisibleTime >= iviPeroid) {
			//play collision sound
			sound.setBuffer(*hitAsteroid);
			sound.play();
			if (curShiled <= 0) {
				curLife--;
				lifeBoard.setString("life Remaining:   " + to_string(curLife));
				explodeAniObj.push_back(explodeAni(ship1->getLocation(), &myAsset));
				delete ship1;
				ship1 = nullptr;
				
				if (curLife <= 0) {
					gameOver2.setString("            Your Score:" + to_string(score) + "\n   Press R To Main Menu");
					globalVariable::gameState = globalVariable::gameStateList::gameOver;
				}
				else {
					globalVariable::gameState = globalVariable::gameStateList::stopRound;
					iniStopTime = clock() / (float)CLOCKS_PER_SEC;
				}
				return -1;
			}
			else {
				curShiled--;
				//cout << "shield: " << curShiled;
			}
		}
		else if (onShield) {
			//remove shield and lock health for 1s
			ship1->stopShield();
			invincibility(0.7);
		}
		
	}
	//asteroid & bullet collision
	else if ((object1->circleType == myCircle::circleTypeList::bullet && object2->circleType == myCircle::circleTypeList::asteroid) || (object1->circleType == myCircle::circleTypeList::asteroid && object2->circleType == myCircle::circleTypeList::bullet)) {
		//add score by 10
		score += 10;
		scoreBoard.setString("score:   " + to_string(score));
		//play bullet asteroid collision sound
		if (hitAsteroid != nullptr) {
			sound.setBuffer(*hitAsteroid);
		}
		sound.play();
		asteroid *theAsteroid;
		bullet* theBullet;
		
		if (object2->circleType == myCircle::circleTypeList::asteroid) {
			theAsteroid = dynamic_cast <asteroid*> (object2);
			theBullet = dynamic_cast <bullet*> (object1);
		}
		else{
			theAsteroid = dynamic_cast <asteroid*> (object1);
			theBullet = dynamic_cast <bullet*> (object2);
		}
		//play explosion animation
		explodeAniObj.push_back(explodeAni(theAsteroid->getLocation(), &myAsset));
		//cout << "animation:" << explodeAniObj.size();
		if (theAsteroid->asteroidIndex == 0) {
			//destroy bullet
			destroyBullet(theBullet);
			//destroy asteroid
			//delete from the asteroid list
			int asteroidIndex = asteroidSearch(theAsteroid);
			if (asteroidIndex != -1) {
				asteroidList.erase(asteroidList.begin() + asteroidIndex);
			}
			else {
				cout << "asteroid not found";
			}
			//add powerUps
			int randN = rand() % 10;
			if (randN < 5) {
				powerUpList.push_back(powerUp(theAsteroid->getLocation(), &myAsset));
			}
			
			//cout << "asteroid exploded; add a powerUp; object index: " << theAsteroid->index;
			//return -1 to avoid double collision with the same asteroid
			return -1;
			//auto it = find(asteroidList.begin(), asteroidList.end(), object);
			//if (it != asteroidList.end()) {
				//asteroidList.erase(it);
			//}	
			//asteroidList.erase(remove(asteroidList.begin(), asteroidList.end(), theAsteroid));
			//may drop a power up
		}
		else if (theAsteroid->asteroidIndex == 1) {
			//destroy bullet
			destroyBullet(theBullet);
			//destroy asteroid
			Vector2f oldLocation = theAsteroid->getLocation();
			int asteroidIndex = asteroidSearch(theAsteroid);
			if (asteroidIndex != -1) {
				asteroidList.erase(asteroidList.begin() + asteroidIndex);
			}
			else {
				cout << "asteroid not found";
			}
			//add two new medium asteroid2
			asteroidList.push_back(asteroid(0,oldLocation, &myAsset, asteroidSpeedLevel[curLevel]));
			asteroidList.push_back(asteroid(0,oldLocation, &myAsset, asteroidSpeedLevel[curLevel]));
		}
		else if (theAsteroid->asteroidIndex == 2) {
			//destroy bullet
			destroyBullet(theBullet);
			//destroy asteroid
			Vector2f oldLocation = theAsteroid->getLocation();
			int asteroidIndex = asteroidSearch(theAsteroid);
			if (asteroidIndex != -1) {
				asteroidList.erase(asteroidList.begin() + asteroidIndex);
			}
			else {
				cout << "asteroid not found";
			}
			//add two new medium asteroid2
			asteroidList.push_back(asteroid(1, oldLocation, &myAsset, asteroidSpeedLevel[curLevel]));
			asteroidList.push_back(asteroid(1, oldLocation, &myAsset, asteroidSpeedLevel[curLevel]));
		}
		//cout << "bullet hit asteroid";
	}
	//ship & powerUp collision
	else if ((object1->circleType == myCircle::circleTypeList::ship && object2->circleType == myCircle::circleTypeList::powerUp) || (object1->circleType == myCircle::circleTypeList::powerUp && object2->circleType == myCircle::circleTypeList::ship)) {
		powerUp* thepowerUp;
		ship* theShip;
		if (object1->circleType == myCircle::circleTypeList::powerUp) {
			thepowerUp = dynamic_cast <powerUp*> (object1);
			theShip = dynamic_cast <ship*> (object2);
		}
		else {
			thepowerUp = dynamic_cast <powerUp*> (object2);
			theShip = dynamic_cast <ship*> (object1);
		}
		//switch the powerUp texture to the player
		curShiled++;
		cout << "cur shield: " << curShiled;
		//destroy powerUp
		Vector2f oldLocation = thepowerUp->getLocation();
		int powerUpIndex = powerUpSearch(thepowerUp);
		if (powerUpIndex != -1) {
			powerUpList.erase(powerUpList.begin() + powerUpIndex);
		}
		else {
			cout << "no powerUp found in the list";
		}
		//generate effect
		ship1->setPowerUpEffect();
		
	}
	return 0;
}

void checkCirclesCollision() {
	//find bucket index list for each object
	for (int i = 0; i < objectList.size(); i++) {
		vector<int> bucketIndexList = getBucketIndexList(*objectList[i]);
		//loop through the buckets and get the object inside
		for (int j = 0; j < bucketIndexList.size(); j++) {
			for (int k = 0; k < bucket[bucketIndexList[j]].size(); k++) {
				bool collided = checkCollision(*objectList[i], *bucket[bucketIndexList[j]][k]);
				if (collided) {
					int result = processCollision(objectList[i], bucket[bucketIndexList[j]][k]);
					if (result == -1) {
						return;
					}
				}
			}
			bucket[bucketIndexList[j]].push_back(objectList[i]);
		}
	}
}

//check bullet wall collision
void bulletWallCollision() {
	for (int i = 0; i < bulletList.size(); i++) {
		float bulletRadius = bulletList[i].getRadius();
		Vector2f bulletLocation = bulletList[i].getLocation();
		if (bulletLocation.y + radius <= 0 || bulletLocation.y - radius >= globalVariable::screenDim.y || 
			bulletLocation.x + radius <= 0 || bulletLocation.x - radius >= globalVariable::screenDim.x) {
			//cout << "bullet list size: " << bulletList.size();
			bulletList.erase(bulletList.begin() + i);
			//cout << "bullet list size: " << bulletList.size();
		}
	}
}
//check collision ends

void addCircleObject() {
	objectList.clear();
	//add the ship
	if (ship1 != nullptr) {
		objectList.push_back(ship1);
	}
	else {
		//cout << "null pointer" << &ship1;
	}
	//add asteroids
	for (int i = 0; i < asteroidList.size(); i++) {
		objectList.push_back(&asteroidList[i]);
	}
	//add bullets
	for (int i = 0; i < bulletList.size(); i++) {
		objectList.push_back(&bulletList[i]);
	}
	//add powerUps
	for (int i = 0; i < powerUpList.size(); i++) {
		//cout << "   added powerups  size:     " << powerUpList.size();
		objectList.push_back(&powerUpList[i]);
	}
	//index the bucket
	for (int i = 0; i < objectList.size(); i++) {
		objectList[i]->index = i;
	}
}

void startGame() {
	//cout << "start game";
	curLevel = 0;
	curLife = maxLife;
	lifeBoard.setString("life Remaining:   " + to_string(curLife));
	score = 0;
	scoreBoard.setString("score:   " + to_string(score));
	curShiled = 0;
	powerUpList.clear();
	//asteroids
	asteroidList.clear();
	for (int i = 0; i < NAsteroid[curLevel]; i++) {
		asteroidList.push_back(asteroid(2, &myAsset, asteroidSpeedLevel[curLevel]));
	}
	//inilialize the bucket
	bucket.clear();
	for (int i = 0; i < NrowsBucket * NcolsBucket; i++) {
		bucket.push_back(vector<myCircle*>());
	}
}

void nextLevel() {
	invincibility(2);
	curLevel++;
	asteroidList.clear();
	for (int i = 0; i < NAsteroid[curLevel]; i++) {
		asteroidList.push_back(asteroid(2, &myAsset, asteroidSpeedLevel[curLevel]));
	}
	powerUpList.clear();
}

void mainUpdate(float dt) {
	//test
	//cout << "the number of asteroid: " << asteroidList.size();
	//check if in the main menu
	if (globalVariable::gameState == globalVariable::gameStateList::title) {
		drawMainMenu();
		if (Keyboard::isKeyPressed(Keyboard::Key::E)) {
			globalVariable::gameState = globalVariable::gameStateList::start;
			//initialize properties
			startGame();
			startRound();
			return;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Escape) && clock() / (float)(CLOCKS_PER_SEC)-iniEscTime > 0.35f) {
			iniEscTime = clock() / (float)(CLOCKS_PER_SEC)-iniEscTime;
			window->close();
		}
	}
	//launch bullet
	if (globalVariable::gameState == globalVariable::gameStateList::start) {
		//emit bullet
		if (Keyboard::isKeyPressed(Keyboard::Key::Space) && (clock() - preEmitTime) / (float)CLOCKS_PER_SEC > emitInterval) {
			//play sound effect
			fireBulletSound.play();
			preEmitTime = clock();
			//cout << "facing direction:" << ship1->getFacingUnitDir().x << "y: " << ship1->getFacingUnitDir().y;
			bullet thisBullet(ship1->getLocation(), ship1->getFacingUnitDir(), bulletSpeed);
			bulletList.push_back(thisBullet);
		}
		//check if all asteroids are destroyed
		if (asteroidList.size() <= 0) {
			cout << "you win";
			sound.setBuffer(*levelUpSoundBuffer);
			sound.play();
			nextLevel();
			return;
		}
	}
	if (globalVariable::gameState == globalVariable::gameStateList::start || globalVariable::gameState == globalVariable::gameStateList::stopRound) {
		//Press Esc to main menu
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape) && clock()/(float) (CLOCKS_PER_SEC) - iniEscTime >0.35f) {
			globalVariable::gameState = globalVariable::gameStateList::title;
			iniEscTime = clock() / (float)(CLOCKS_PER_SEC)-iniEscTime;
			return;
		}
		//add all physical circles into the vector
		addCircleObject();
		//add all physical circles into the buckets
		putAllInBucket();

		for (int i = 0; i < objectList.size(); i++) {
			objectList[i]->update(dt);
		}
		//destroy completed animation and update
		for (int i = 0; i < explodeAniObj.size(); i++) {
			if (explodeAniObj[i].aniCompleted == true) {
				explodeAniObj.erase(explodeAniObj.begin() + i);
			}
		}
		for (int i = 0; i < explodeAniObj.size(); i++) {
			explodeAniObj[i].update();
		}
		drawGraph();
		bulletWallCollision();
		checkCirclesCollision();
	}
	if (globalVariable::gameState == globalVariable::gameStateList::stopRound && clock() / (float)CLOCKS_PER_SEC - iniStopTime >= 0.5f) {
		startRound();
	}
	if (globalVariable::gameState == globalVariable::gameStateList::gameOver) {
		drawGameOverScreen();
		if (Keyboard::isKeyPressed(Keyboard::Key::R)) {
			globalVariable::gameState = globalVariable::gameStateList::title;
			
		}
	}
}


void load() {
	//load font
	myAsset.loadFont("myFont", "myFont.ttf");
	//load textures
	myAsset.loadTexture("ship", "ship.png");
	myAsset.loadTexture("asteroid", "asteroid.png");
	myAsset.loadTexture("bgImage", "bgImage.png");
	myAsset.loadTexture("thrust1", "thrust1.png");
	myAsset.loadTexture("explode1", "explode1.png");
	myAsset.loadTexture("explode2", "explode2.png");
	myAsset.loadTexture("explode3", "explode3.png");
	myAsset.loadTexture("explode4", "explode4.png");
	myAsset.loadTexture("explode5", "explode5.png");
	myAsset.loadTexture("explode6", "explode6.png");
	myAsset.loadTexture("explode7", "explode7.png");
	myAsset.loadTexture("explode8", "explode8.png");
	myAsset.loadTexture("explode9", "explode9.png");
	myAsset.loadTexture("dropShield", "dropShield.png");
	myAsset.loadTexture("playerShield", "playerShield.png");
	//global
	window = new RenderWindow(sf::VideoMode(globalVariable::screenDim.x, globalVariable::screenDim.y), "SFML works!");
	xInterval = globalVariable::screenDim.x / NcolsBucket;
	yInterval = globalVariable::screenDim.y / NrowsBucket;
	//load sound
	myAsset.loadSoundBuffer("hitAsteroid", "hitAsteroid.wav");
	myAsset.loadSoundBuffer("fire", "fire.wav");
	myAsset.loadSoundBuffer("levelUp", "levelUp.wav");
	//load sound to main
	hitAsteroid = myAsset.getSoundBuffer("hitAsteroid");
	fire = myAsset.getSoundBuffer("fire");
	levelUpSoundBuffer = myAsset.getSoundBuffer("levelUp");
	fireBulletSound.setBuffer(*fire);
	backGroundSprite.setTexture(*(myAsset.getTexture("bgImage")));



	//set text board 
	scoreBoard.setFont(*myAsset.getFont("myFont"));
	scoreBoard.setPosition(Vector2f(globalVariable::screenDim.x * 0.04f, globalVariable::screenDim.y * 0.05f));
	
	scoreBoard.setCharacterSize(50);
	
	lifeBoard.setFont(*myAsset.getFont("myFont"));
	lifeBoard.setPosition(Vector2f(globalVariable::screenDim.x * 0.04f, globalVariable::screenDim.y * 0.12f));
	
	lifeBoard.setCharacterSize(50);

	gameOver1.setFont(*myAsset.getFont("myFont"));
	gameOver1.setPosition(Vector2f(globalVariable::screenDim.x * 0.07f, globalVariable::screenDim.y * 0.15f));
	gameOver1.setString("GAME OVER");
	gameOver1.setCharacterSize(110);

	gameOver2.setFont(*myAsset.getFont("myFont"));
	gameOver2.setPosition(Vector2f(globalVariable::screenDim.x * 0.04f, globalVariable::screenDim.y * 0.50f));
	gameOver2.setCharacterSize(60);

	mainMenu1.setFont(*myAsset.getFont("myFont"));
	mainMenu1.setPosition(Vector2f(globalVariable::screenDim.x * 0.07f, globalVariable::screenDim.y * 0.15f));
	mainMenu1.setString("MAIN MENU");
	mainMenu1.setCharacterSize(110);

	mainMenu2.setFont(*myAsset.getFont("myFont"));
	mainMenu2.setPosition(Vector2f(globalVariable::screenDim.x * 0.04f, globalVariable::screenDim.y * 0.50f));
	mainMenu2.setString("          Press E to Start \n        Press Esc to Exit\nControl: Arrows and Space");
	mainMenu2.setCharacterSize(60);

	
}

void drawgameOver() {
	window->clear();
	window->draw(backGroundSprite);
	window->draw(scoreBoard);
	window->display();
}

int main() {
	srand(time(NULL));
	load();
	//set time and clock
	Clock clock;
	Time elapsed1 = clock.getElapsedTime();
	Time frame = seconds(1 / FPS);	
	globalVariable::gameState = globalVariable::gameStateList::title;
	while (window->isOpen())
	{
		//open and close the window
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}
		Time elapsed2 = clock.getElapsedTime();
		Time dt = elapsed2 - elapsed1;
		if (dt > frame) {
			elapsed1 = clock.getElapsedTime();
			float dtSecond = dt.asSeconds();
			mainUpdate(dtSecond);
		}
	}
	return 0;

}