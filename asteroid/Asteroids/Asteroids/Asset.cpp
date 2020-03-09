#include <iostream>
#include "Asset.h"

Asset::Asset()
{
}

Asset::~Asset()
{
}

void Asset::loadTexture(string textureName, string FileName)
{
	sf::Texture curTex;
	if (curTex.loadFromFile(FileName)) {
		(textureMap)[textureName] = curTex;
	}
	else {	
		cout << "load not successful:" << FileName;
	}
}

sf::Texture* Asset::getTexture(string desiredTexName)
{
	sf::Texture* curTex;
	if (textureMap.find(desiredTexName) != textureMap.end()) {
		curTex = &(textureMap.find(desiredTexName)->second);
		//curTex = &(textureMap.find(desiredTexName)->second);
	}
	else {
		cout << "cant find texture:" << desiredTexName;
		curTex = nullptr;
	}
	return curTex;
	
}

void Asset::loadSoundBuffer(string soundName, string FileName)
{
	sf::SoundBuffer curAsset;
	if (curAsset.loadFromFile(FileName)) {
		soundBufferMap[soundName] = curAsset;
	}
	else {
		cout << "load not successful:" << FileName;
	}
}

sf::SoundBuffer* Asset::getSoundBuffer(string desiredSoundName)
{
	sf::SoundBuffer* curAsset;
	if (soundBufferMap.find(desiredSoundName) != soundBufferMap.end()) {
		curAsset = &(soundBufferMap.find(desiredSoundName)->second);
		//curTex = &(textureMap.find(desiredTexName)->second);
	}
	else {
		cout << "cant find texture:" << desiredSoundName;
		curAsset = nullptr;
	}
	return curAsset;
}

void Asset::loadFont(string assetName, string FileName)
{
	sf::Font curAsset;
	if (curAsset.loadFromFile(FileName)) {
		fontMap[assetName] = curAsset;
	}
	else {
		cout << "load not successful:" << FileName;
	}
}

sf::Font* Asset::getFont(string desiredAssetName)
{
	sf::Font* curAsset;
	if (fontMap.find(desiredAssetName) != fontMap.end()) {
		curAsset = &(fontMap.find(desiredAssetName)->second);
		//curTex = &(textureMap.find(desiredTexName)->second);
	}
	else {
		cout << "cant find font:" << desiredAssetName;
		curAsset = nullptr;
	}
	return curAsset;
}
