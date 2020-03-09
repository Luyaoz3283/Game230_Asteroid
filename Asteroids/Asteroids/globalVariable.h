#include <iostream>
#pragma once
#include <SFML/Graphics.hpp>


using namespace sf;


class globalVariable
{
public:
	static Vector2f screenDim;
	enum gameStateList{title, start, stopRound, gameOver};
	static gameStateList gameState;
	static Vector2f getUnitVector(Vector2f vector);
	
	
};

