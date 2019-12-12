#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <ctime>

#include "globalVariable.h"
#include "Asset.h"

using namespace std;


class explodeAni
{

private:
	vector<sf::Texture*> aniFrames;
	clock_t preTime;
	float aniInterval = 0.2f;
	int frameIndex = 0;
	
	sf::CircleShape circleShape;
	float radius = 50;

public:
	bool aniCompleted = false;
	explodeAni(sf::Vector2f desiredLocation, Asset* assetManager);
	~explodeAni();
	
	void update();
	void draw(RenderWindow* window);
	void addExploAniTex(Asset* assetManager);
	

};

