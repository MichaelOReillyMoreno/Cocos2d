
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Started by Ángel on january of 2016                                        *
*  This is free software released into the public domain.                     *
*  angel.rodriguez@esne.edu                                                   *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "MainMenu.hpp"
#include "GameScene.hpp"
#include "SimpleAudioEngine.h"
using namespace cocos2d;

namespace mainGame
{
	bool Main::init()
	{
		// Se intenta inicializar la clase base y, si falla, se retorna con error:
		if (!Scene::init())
		{
			return false;
		}
		//se coge la musica del juego
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music.mp3");

		//se inicia la musica
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music.mp3", true);

		// Se crea una capa y se añade a la escena
		auto layer = Layer::create();
		this->addChild(layer);

		// Se toma el tamaño visible del display para colocar los elementos gráficos
		// usando posiciones relativas (en lugar de absolutas):
		visibleSize = Director::getInstance()->getVisibleSize();

		//se establece el fondo
		background = Sprite::create("BG.png");
		background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		layer->addChild(background, 1);

		//se crean los botones
		createButtons(layer);

		//se crea la label donde se printara la ayuda y se oculta
		labelHelp = Label::createWithTTF("Coge todos los bebes con la cesta, evista los monstruos. Muevete con las felchas de direccion", "fonts/pixelart.ttf", 40);
		labelHelp->setDimensions(visibleSize.width, visibleSize.height / 2);
		addChild(labelHelp, 5);
		labelHelp->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.5));
		labelHelp->setOpacity(0);
		return true;
	}

	void Main::createButtons(Layer * layer)
	{
		// Se crean los sprites:
		buttonPlay = Sprite::create();
		buttonHelp = Sprite::create();
		buttonClose = Sprite::create();

		// Se coloca el punto de anclaje (el que se tiene en cuenta para establecer la posición)
		// para facilitar la maquetación:
		buttonPlay->setAnchorPoint(Vec2(0.5f, 0.5f));
		buttonHelp->setAnchorPoint(Vec2(1.f, 0.f));
		buttonClose->setAnchorPoint(Vec2(0.5f, 0.5f));

		//se crean los items del menu
		menu_item1 = MenuItemImage::create("play.png", "play.png", CC_CALLBACK_1(Main::LoadGame, this));
		menu_item2 = MenuItemImage::create("help.png", "help.png", CC_CALLBACK_1(Main::LoadHelp, this));
		menu_item3 = MenuItemImage::create("exit.png", "exit.png", CC_CALLBACK_1(Main::CloseHelp, this));

		//se establecen sus posiciones dentro de el
		menu_item1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		menu_item2->setPosition(Vec2(visibleSize.width * 0.9, visibleSize.height * 0.9));
		menu_item3->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.5));

		//se crea el menu y se establce su posicion y se añade a la escena
		menu = Menu::create(menu_item1, menu_item2, menu_item3, NULL);
		menu->setPosition(0., 0.f);
		layer->addChild(menu, 10);

		//se oculta el boton para salir de la ayuda
		menu_item3->setVisible(false);
	}

	// Carga el juego al darle a play
	void Main::LoadGame(Ref * pSender)
	{
		auto myScene = game::GameScene::create();
		Director::getInstance()->replaceScene(myScene);
	}

	//Permite abrir la ayuda
	void Main::LoadHelp(Ref * pSender)
	{
		labelHelp->setOpacity(255);
		menu_item1->setVisible(false);
		menu_item2->setVisible(false);
		menu_item3->setVisible(true);
	}

	//Permite cerrar la ayuda
	void Main::CloseHelp(Ref * pSender)
	{
		labelHelp->setOpacity(0);
		menu_item1->setVisible(true);
		menu_item2->setVisible(true);
		menu_item3->setVisible(false);
	}
}