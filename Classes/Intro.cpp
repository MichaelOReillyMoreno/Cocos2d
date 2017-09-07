
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Started by Ángel on january of 2016                                        *
*  This is free software released into the public domain.                     *
*  angel.rodriguez@esne.edu                                                   *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Intro.hpp"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
namespace IntroGame
{
	bool Intro::init()
	{
		// Se intenta inicializar la clase base y, si falla, se retorna con error:
		if(!Scene::init())
		{
			return false;
		}
		// Se crea una capa y se añade a la escena
		auto layer = Layer::create();
		this->addChild(layer);

		// Se toma el tamaño visible del display para colocar los elementos gráficos
		// usando posiciones relativas (en lugar de absolutas):
		visibleSize = Director::getInstance()->getVisibleSize();

		//se establece la imagen de intro
		introImage = Sprite::create("cartela.jpg");
		introImage->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		layer->addChild(introImage, 1);

		// Se solicita que se llame al método update() cada fotograma:
		scheduleUpdate();

		return true;
	}
	void Intro::update(float delta)
	{
		++delay;
		if (delay == 30)
		{
			//se incia un fade out y se carga el menu
			introImage->runAction(Sequence::create(Show::create(), FadeOut::create(1.0), NULL));
		}
		if (delay == 120)
		{
			LoadGame();
		}
	}
	void Intro::LoadGame()
	{
		auto myScene = mainGame::Main::create();
		Director::getInstance()->replaceScene(myScene);
	}
}