#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


using namespace std;

class Asset
{
private:
	map<string, sf::Texture> textureMap;
	map<string, sf::SoundBuffer> soundBufferMap;
	map<string, sf::Font> fontMap;


public:
	Asset();
	~Asset();
	void loadTexture(string textureName, string FileName);
	sf::Texture* getTexture(string desiredTexName);

	void loadSoundBuffer(string soundName, string FileName);
	sf::SoundBuffer* getSoundBuffer(string desiredSoundName);

	void loadFont(string assetName, string FileName);
	sf::Font* getFont(string desiredAssetName);
};

