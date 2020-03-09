#include "explodeAni.h"

explodeAni::explodeAni(sf::Vector2f desiredLocation, Asset* assetManager)
{
	preTime = -10;
	circleShape.setRadius(radius);
	circleShape.setPosition(desiredLocation);
	addExploAniTex(assetManager);
}
explodeAni::~explodeAni()
{
}

void explodeAni::update()
{
	//cout << "updated";
	if ((clock()/ (float)CLOCKS_PER_SEC - preTime) > aniInterval) {
		if (frameIndex >= aniFrames.size()) {
			//cout << "completed: frame index:" << frameIndex << "container size: " << aniFrames.size();
			aniCompleted = true;
		}
		else {
			circleShape.setTexture(aniFrames[frameIndex]);
			frameIndex++;
		}
		preTime = clock() / (float)CLOCKS_PER_SEC;
	}
}

void explodeAni::draw(RenderWindow* window)
{
	window->draw(circleShape);
}

void explodeAni::addExploAniTex(Asset* assetManager)
{
	Texture* curTex = assetManager->getTexture("explode1");
	if (curTex != nullptr) {
		aniFrames.push_back(curTex);
	}
	curTex = assetManager->getTexture("explode2");
	if (curTex != nullptr) {
		aniFrames.push_back(curTex);
	}
	curTex = assetManager->getTexture("explode3");
	if (curTex != nullptr) {
		aniFrames.push_back(curTex);
	}
	curTex = assetManager->getTexture("explode4");
	if (curTex != nullptr) {
		aniFrames.push_back(curTex);
	}
	curTex = assetManager->getTexture("explode5");
	if (curTex != nullptr) {
		aniFrames.push_back(curTex);
	}
	curTex = assetManager->getTexture("explode6");
	if (curTex != nullptr) {
		aniFrames.push_back(curTex);
	}
	curTex = assetManager->getTexture("explode7");
	if (curTex != nullptr) {
		aniFrames.push_back(curTex);
	}
	curTex = assetManager->getTexture("explode8");
	if (curTex != nullptr) {
		aniFrames.push_back(curTex);
	}
	curTex = assetManager->getTexture("explode9");
	if (curTex != nullptr) {
		aniFrames.push_back(curTex);
	}

}

