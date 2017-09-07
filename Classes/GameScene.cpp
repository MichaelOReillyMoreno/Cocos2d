
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on january of 2016                                        *
 *  This is free software released into the public domain.                     *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//doxigen documentar
//giroscopio movimiento

#include "GameScene.hpp"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

namespace game
{
    bool GameScene::init()
    {
        // Se intenta inicializar la clase base y, si falla, se retorna con error:
        if (!Scene::init())
        {
            return false;
        }
		//se coge la musica del juego
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music.mp3");

        // Se crea una capa y se añade a la escena
        auto layer = Layer::create();

        this->addChild(layer);

        // Se establece la tasa de fotogramas por segundo:
        Director::getInstance()->setAnimationInterval(1.0f / framesPerSecondLimit);

         // Se toma el tamaño visible del display para colocar los elementos gráficos
        // usando posiciones relativas (en lugar de absolutas):
       visibleSize = Director::getInstance()->getVisibleSize();

	   //se establece la puntuacion a 0
	   score = 0;

	   //se crea el sprite del fondo y se establece su posicion en el centro de la pantalla
		background = Sprite::create("BG.png");
		background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		layer->addChild(background, 1);

        // Se crea el sprite de la cesta y se establece su posicion
		box.sprite = Sprite::create ("cesta.png");
		box.sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.25));
        layer->addChild (box, 1);

        // Se crean los bebes
		babyList.resize(maxSimultaneouBabys);
		int cont = 0;
		for(auto & baby : babyList)
		{
			//si es mayor de startEnemies es enemigo
			if(cont > startEnemies)
			{
				baby.isEnemy = true;
			}
			else
			{
				baby.isEnemy = false;
			}
			//se le  asigna el sprite
			baby.sprite = Sprite::create(sprites[cont]);
			cont++;
			//si el numero de bebes que se desean es mayor que el numero de sprites se reinicia la cuenta
			if(cont >= indexSprites)
			{
				cont = 0;
			}
			layer->addChild(baby, 2);
		}
        // Se crean las explosiones
        explosions.resize(maxSimultaneouBabys);
        for(auto & explosion : explosions)
        {
            explosion.sprite = Sprite::create("explosion.png");

            layer->addChild(explosion, 5);
        }

		//se crea la lista de puntos
		pointsList.resize(maxSimultaneouPoints);
		for(auto & point : pointsList)
		{
			point.label = Label::createWithTTF("1", "fonts/pixelart.ttf", 45);
			point.label->setColor(Color3B(0, 0, 0));
			point.speed = Vec2(0.f, 120.f);
			addChild(point.label, 5);
		}

        // Se crea el sprite con el mensaje de game over:
        gameOver = Sprite::create("game-over.png");
        gameOver->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.5));
        layer->addChild(gameOver, 2);

        // Se crean los botones con los que el usuario controlará la cesta
        createButtons(layer);

        // Se registran los eventos de la pantalla táctil:
        auto touchListener = EventListenerTouchAllAtOnce::create ();
        touchListener->onTouchesBegan = CC_CALLBACK_2(GameScene::touchBegan, this);
        touchListener->onTouchesEnded = CC_CALLBACK_2(GameScene::touchEnded, this);

        layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority (touchListener, layer);

        // Se registran los eventos del teclado:
        auto keyboardListener = EventListenerKeyboard::create ();
        keyboardListener->onKeyPressed  = CC_CALLBACK_2(GameScene::keyPressed,  this);
        keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::keyReleased, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority (keyboardListener, this);

        // Se inicializa el generador de números aleatorios:
        srand(time(NULL));

		//se crea la label donde se printara la puntuacion del jugador
		labelScore = Label::createWithTTF("label test", "fonts/pixelart.ttf", 90);
		addChild(labelScore, 5);

        // Se inicializan todos los elementos de la escena:
        resetGame();

        // Se solicita que se llame al método update() cada fotograma:
        scheduleUpdate();

        return true;
    }

    void GameScene::createButtons(Layer * layer)
    {
        // Se crean los sprites:
        buttonLeft .sprite = Sprite::create ("button-left.png" );
        buttonRight.sprite = Sprite::create ("button-right.png");
		buttonReset.sprite = Sprite::create("reset.png");
		buttonMain.sprite = Sprite::create("home.png");
		buttonPause.sprite = Sprite::create("pause.png");
		buttonPlay.sprite = Sprite::create("play.png");

         // Se coloca el punto de anclaje (el que se tiene en cuenta para establecer la posición) en una esquina
        // para facilitar la maquetación:
        buttonLeft .sprite->setAnchorPoint(Vec2(0.f, 0.f));
        buttonRight.sprite->setAnchorPoint(Vec2(1.f, 0.f));
		buttonReset.sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		buttonMain.sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		buttonPause.sprite->setAnchorPoint(Vec2(1.f, 1.f));
		buttonPlay.sprite->setAnchorPoint(Vec2(0.5f, 0.5f));

        // Se colocan los botones en la escena de modo que estén en la parte inferior, pegados a los lados:
        buttonLeft .sprite->setPosition(0.f, 0.f);
        buttonRight.sprite->setPosition(Director::getInstance()->getVisibleSize().width, 0.f);
		buttonReset.sprite->setPosition(Vec2(visibleSize.width / 1.6, visibleSize.height / 1.8));
		buttonMain.sprite->setPosition(Vec2(visibleSize.width / 2.4, visibleSize.height / 1.8));
		buttonPause.sprite->setPosition(Vec2(visibleSize.width / 1, visibleSize.height / 1.1));
		buttonPlay.sprite->setPosition(Vec2(visibleSize.width / 1.6, visibleSize.height / 1.8));

        // Se hacen un poco transparentes
        buttonLeft.sprite->setOpacity(128);
        buttonRight.sprite->setOpacity(128);

        // Se añaden los sprites a la capa de la escena:
        layer->addChild(buttonLeft , 10);
        layer->addChild(buttonRight, 10);
		layer->addChild(buttonReset, 10);
		layer->addChild(buttonMain, 10);
		layer->addChild(buttonPause, 10);
		layer->addChild(buttonPlay, 10);
    }

	// Se reinicial los valores de los que depende la puntuacion, el impulso de la cesta y el numero de bebes en pantalla([0]=1 bebe)
    void GameScene::resetGame()
    {
		//se inicia la musica
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music.mp3", true);

		score = 0;
		boxAdvanceImpulse = 10;

		//se macha el valor de la label para que vuelva a ser 0 y se resetea su posicion
		std::ostringstream buffer;
		buffer << score;
		labelScore->setString(buffer.str());
		labelScore->setPosition(Vec2(visibleSize.width * 0.09, visibleSize.height * 0.93));
        Size sceneSize = Director::getInstance()->getVisibleSize();

        // Se coloca la cesta en mitad del eje de la X y en la cuarta parte inferior del eje de la Y
		box.sprite->setPosition(Vec2(sceneSize.width / 2, sceneSize.height * 0.25));

		//se inicializa su velocidad a 0
		box.speed = Vec2(0.f, 0.f);

		//se establece los valores iniciales de los bebes
        for(auto & baby : babyList)
        {
			//su direccion y velocidad
			Vec2 direction = Vec2(0.f, -1.0f);
			baby.speed = direction.getNormalized() * babyAdvanceSpeed;

			//y por defecto como no visibles
			baby.sprite->setVisible (false);
        }

		//se añade un bebe al juego
		AddNoUseBaby(false);

        // Se ocultan los sprites de las explosiones
        for(auto & explosion : explosions) 
			explosion.sprite->setVisible(false);

		//Se ocultan los puntos
		for(auto & point : pointsList)
		{
			point.isActive = false;
			point.label->setOpacity(0);
		}

		//y de game over
        gameOver->setVisible(false);

		//se ocultan los botones de menu y play
		buttonReset.sprite->setVisible(false);
		buttonMain.sprite->setVisible(false);
		buttonPlay.sprite->setVisible(false);
		buttonPause.sprite->setVisible(true);

        // Se resetean las variables que controlan la interacción con el usuario:
        buttonLeft .touch = NO_TOUCH;
        buttonRight.touch = NO_TOUCH;
		buttonReset.touch = NO_TOUCH;
		buttonMain.touch = NO_TOUCH;
		buttonPause.touch = NO_TOUCH;
		buttonPlay.touch = NO_TOUCH;

        keyLeftIsPressed  = false;
        keyRightIsPressed = false;
        keyMoveIsPressed  = false;

        state = PLAYING;
    }

	//establece el game over
    void GameScene::setGameOverState()
    {
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
        gameOver->setVisible(true);
		buttonReset.sprite->setVisible(true);
		buttonMain.sprite->setVisible(true);
		buttonPause.sprite->setVisible(false);
        state = GAME_OVER;
    }

	//establece el pause o lo deshabilita
	void GameScene::setPauseStateOn()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		buttonMain.sprite->setVisible(true);
		buttonPlay.sprite->setVisible(true);
		buttonPause.sprite->setVisible(false);
	}
	//deshabilita el pause
	void GameScene::setPauseStateOff()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		buttonMain.sprite->setVisible(false);
		buttonPlay.sprite->setVisible(false);
		buttonPause.sprite->setVisible(true);
	}
    void GameScene::makeExplosion(const Vec2 & position)
    {
        // Se busca un sprite de explosión que no esté visible para usarlo:
        for(auto & explosion : explosions)
        {
            if (explosion.sprite->isVisible() == false)
            {
                // Se establecen los atributos del sprite de la explosión. Se usan Actions para que
                // la imagen vaya creciendo y haciéndose invisible rápida y automáticamente:
                explosion.sprite->setScale(0.25f);
                explosion.sprite->setOpacity(255);
                explosion.sprite->runAction(ScaleTo::create (0.25f, 2.5f));
                explosion.sprite->runAction(FadeOut::create (0.25f));
                explosion.sprite->setPosition(position);
                explosion.sprite->setVisible(true);
                break;
            }
        }
    }

    void GameScene::update(float delta)
    {
        if(state == PLAYING)
        {
            // Se actualiza el estado de todas las entidades:
            updateExplosions();
			updateBox(delta);
			updateBabys(delta);
			updatePoints(delta);

            // Se comprueban las colisiones de los bebe con la cesta
			checkBabyBoxCollision();
			updateButtonsOnPlaying();
        }
		else if(state == PAUSE)
		{
			updateButtonsPause();
		}
		else if(state == GAME_OVER)
		{
			updateButtonsOnGameOver();
		}
    }

	// Cuando el estado es PAUSE, tocar el boton pause reanuda el juego:
	void GameScene::updateButtonsPause()
	{
		if(buttonPlay.touch != NO_TOUCH)
		{
			buttonPlay.touch = NO_TOUCH;
			state = PLAYING;
			setPauseStateOff();
		}
		if (buttonMain.touch != NO_TOUCH)
		{
			buttonMain.touch = NO_TOUCH;
			auto myScene = mainGame::Main::create();
			Director::getInstance()->replaceScene(myScene);
		}
	}

	// Cuando el estado es PLAYING, tocar el boton pause pausa el juego:
	void GameScene::updateButtonsOnPlaying()
	{
		if(buttonPause.touch != NO_TOUCH)
		{
			buttonPause.touch = NO_TOUCH;
			state = PAUSE;
			setPauseStateOn();
		}
	}

	// Cuando el estado es GAME_OVER, tocar el boton reset reinicia el juego, home te devuelve al menu:
	void GameScene::updateButtonsOnGameOver()
	{
		if (buttonReset.touch != NO_TOUCH)
		{
			buttonReset.touch = NO_TOUCH;
			resetGame();
		}
		if (buttonMain.touch != NO_TOUCH)
		{
			buttonMain.touch = NO_TOUCH;
			auto myScene = mainGame::Main::create();
			Director::getInstance()->replaceScene(myScene);
		}
	}

    void GameScene::updateBox(float delta)
    {
		Vec2 impulse(0, 0);

        if(buttonLeft.touch  != NO_TOUCH || keyLeftIsPressed)
        {
			impulse.x = -boxAdvanceImpulse;
			box.speed += impulse * delta;
        }
		else if(buttonRight.touch != NO_TOUCH || keyRightIsPressed)
        {
			impulse.x = boxAdvanceImpulse;
			box.speed += impulse * delta;
        }

        // Se actualiza la posición de la cesta según su velocidad actual
		box.sprite->setPosition(box.sprite->getPosition() + box.speed);

        // Si la caja se sale por un lado de la escena, se la hace aparecer por el otro extremo
        correctBoxLocation(box.sprite);
    }
	//actualiza los bebes
    void GameScene::updateBabys(float delta)
    {
        // Se actualiza la posición de todos los bebes visibles y, si se comprueba su posicion para realizar acciones en consecuencia
        // se los hace aparecer por el otro extremo:
        for(auto & baby : babyList)
        {
            if (baby.sprite->isVisible())
            {
				baby.sprite->setPosition (baby.sprite->getPosition() + baby.speed * delta);

				CheckBabyLocation(baby);
            }
        }
    }

	// Se actualiza la posición de todos los puntos visibles
	void GameScene::updatePoints(float delta)
	{
		for(auto & point : pointsList)
		{		
			if(point.isActive)
			{
				//los puntos suben hacia arriba a la vez que van desvaneciendose a lo largo del tiempo
				point.label->setPosition(point.label->getPosition() + point.speed * delta);
				int currentOpacity = point.label->getOpacity();
				currentOpacity = currentOpacity - 10;
				point.label->setOpacity(currentOpacity);

				//si se desvanecen completamente se desactivan 
				if (point.label->getOpacity() <= 10)
				{
					point.label->setOpacity(0);
					point.isActive = false;
				}
			}
		}
	}
    void GameScene::updateExplosions()
    {
          // Realmente la animación se las explosiones se actualiza automáticamente gracias
         // a las Actions de Cocos2d-x, pero aquí se comprueba cuando ha terminado su animación
        // para ocultar los sprites (dejándolos disponibles para poderlos volver a usar):
        for(auto & explosion : explosions)
        {
            if(explosion.sprite->isVisible() && explosion.sprite->getScale() > 2.f)
            {
                explosion.sprite->setVisible(false);
            }
        }
    }

	// Se comprueba si el sprite se sale de la escena por algún lado y, de ser así, se recoloca
	// en el lado opuesto. Sólo se modifica la coordenada X:
    void GameScene::correctBoxLocation(cocos2d::Sprite * sprite)
    {
        // Se toman las dimensiones del sprite:
        float leftX      = sprite->getBoundingBox().getMinX ();
        float rightX     = sprite->getBoundingBox().getMaxX ();
        Size  sceneSize  = Director::getInstance()->getVisibleSize();
        Size  spriteSize = sprite->getContentSize();

		// La cesta se sale completamente por el lado izquierdo de la escena
        if(rightX < 0.f)                   
        {
            sprite->setPositionX(sceneSize.width + spriteSize.width / 2.f);
        }

		// La cesta se sale completamente por el lado derecho de la escena
        else if(leftX > sceneSize.width)        
        {
            sprite->setPositionX(-spriteSize.width / 2.f);
        }
    }

	// Se comprueba si el bebe/enemigo ha llegado al suelo(limite)
	void GameScene::CheckBabyLocation(Entity& baby)
	{
		float topY = baby.sprite->getBoundingBox().getMaxY();
		Size sceneSize = Director::getInstance()->getVisibleSize();
		Size spriteSize = baby.sprite->getContentSize();
		float limit = sceneSize.height * 0.25;

		if(topY < limit)                      
		{
			//si es un bebe, se inicia el estado game over
			if(!baby.isEnemy)
			{
				setGameOverState();
			}
			//si es un enemigo se realiza una explosion en sus coordenadas y se cambia su localizacion a la parte de arrriba de la pantalla, ademas de su sprite
			else
			{
				makeExplosion(baby.sprite->getPosition());
				//oculta dicho bebe
				baby.sprite->setVisible(false);
				//se añade a la escena un nuevo bebe que no este en uso
				AddNoUseBaby(false);
				//se reproduce un sonido
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explosion.mp3", false, 1.0f, 1.0f, 1.0f);
			}
		}

	}

	//selecciona un bebe de la lista que no este en uso y lo establece como visible
	void GameScene::AddNoUseBaby(bool randY)
	{
		Size sceneSize = Director::getInstance()->getVisibleSize();
		int ranIndex = 0;

		do
		{
			ranIndex = rand() % maxSimultaneouBabys;

		} while (babyList[ranIndex].sprite->isVisible());

		babyList[ranIndex].sprite->setVisible(true);
		Size spriteSize = babyList[ranIndex].sprite->getContentSize();
		babyList[ranIndex].sprite->setPositionX(rand() % (int)sceneSize.width);
		if(randY)
		{
			babyList[ranIndex].sprite->setPositionY(sceneSize.height + (rand() % (int)((sceneSize.height * 1.5) - sceneSize.height + 1)) + babyList[ranIndex].sprite->getContentSize().height);
		}
		else
		{
			babyList[ranIndex].sprite->setPositionY(sceneSize.height + babyList[ranIndex].sprite->getContentSize().height);
		}
	}
	//comprueba si los rectangulos de la cesta y un bebe han colisionado
    bool GameScene::checkSquareCollision(const Sprite * a, const Sprite * box)
    {
        // Sólo se comprueba la colisión si ambos sprites están visibles:
        if (a->isVisible() && box->isVisible())
        {
			Size aSize = a->getContentSize();
			Size boxSize = box->getContentSize();

			// el rectangulo de la cesta como se observa esta alterado para tener una forma y posicion mas acordes a las mecanicas del juego
			float width1 = aSize.width;
			float BoxWidth = boxSize.width/1.7f;
			float height1 = aSize.height;
			float BoxHeight = boxSize.height/4;
			float aX = a->getPositionX();
			float boxX = box->getPositionX();
			float aY = a->getPositionY() - BoxHeight*2;
			float boxY = box->getPositionY();

			return aX < boxX + BoxWidth && boxX < aX + width1 && aY < boxY + BoxHeight && boxY < aY + height1;
        }

        return false;
    }

	// comprueban las colisiones de los bebe con la cesta
    void GameScene::checkBabyBoxCollision()
    {
        // Se comprueba si algún bebe se solapa con la cesta
        for(auto & baby : babyList)
        {
            if(checkSquareCollision(baby.sprite, box.sprite) == true)
            {
				//si no es enemigo
				if(!baby.isEnemy)
				{
					//se cambia la puntuacion
					ChangeScore();
					//se oculta
					baby.sprite->setVisible(false);
					//se añade a la escena un nuevo bebe que no este en uso
					AddNoUseBaby(false);
				}
				else
				{
					//crea una explosion, hace invisible el sprite y establece el estado a game over
					makeExplosion(baby.sprite->getPosition());
					baby.sprite->setVisible(false);
					setGameOverState();

					//se reproduce un sonido
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explosion.mp3", false, 1.0f, 1.0f, 1.0f);
				}
                break;
            }
        }
    }

	//cambia la puntuacion del jugador y si llega a la puntuacion adecuada modifica los bebes en escena y el impulso de la cesta
	void GameScene::ChangeScore()
	{
		++score;

		//se reproduce un sonido
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("coinSound.mp3", false, 1.0f, 1.0f, 1.0f);

		//si tiene dos digitos la mueve ligeramente a la derecha para que entre dentro de la pantalla
		if(score == 10)
		{
			labelScore->setPositionX(visibleSize.width * 0.12);

		}
		//igual
		else if(score == 100)
		{
			labelScore->setPositionX(visibleSize.width * 0.16);
		}

		//actualiza la label con la nueva puntuacion
		std::ostringstream buffer;
		buffer << score;
		labelScore->setString(buffer.str());

		//si llega al nivel deseado
		if (score % levelToAddBaby == 0)
		{

			//se añade un bebe
			AddNoUseBaby(true);

			//se incrementa el impulso
			boxAdvanceImpulse = boxAdvanceImpulse + 5;

			//se le suma 5 al proximo nivel al que se añadira un bebe
			levelToAddBaby = levelToAddBaby + 5;
		}

		//se printa 1 como representacion de haber conseguido un punto
		bool isFree = false;

		//busca el primer punto en la lista de puntos que no se este usando
		int indexPoint = 0;

		while(!isFree && indexPoint < maxSimultaneouPoints)
		{
			if(!pointsList[indexPoint].isActive)
			{
				isFree = true;
			}
			else
			{
				indexPoint++;
			}
		}

		//si ha encontrado uno libre:
		if(isFree) 
		{
			//establece sus valores de posicion y opacidad y lo activa
			pointsList[indexPoint].isActive = true;
			Vec2 positionPoint(box.sprite->getPosition().x * 1.2f, box.sprite->getPosition().y * 1.3f);
			pointsList[indexPoint].label->setPosition(positionPoint);
			pointsList[indexPoint].label->setOpacity(255);
			pointsList[indexPoint].startPosition = positionPoint;
		}
	}

    bool GameScene::touchBegan(const std::vector< Touch * > & touches, cocos2d::Event *)
    {
        if(state == PLAYING)
        {
            // Se comprueba si se ha pulsado alguno de los botones (guardando el id del touch si es así):
            checkButtonTouchBegin (buttonLeft,  touches);
            checkButtonTouchBegin (buttonRight, touches);
			checkButtonTouchBegin(buttonPause, touches);
        }
		else if(state == GAME_OVER)
		{
			checkButtonTouchBegin(buttonReset, touches);
			checkButtonTouchBegin(buttonMain, touches);
		}
		else if(state == PAUSE)
		{
			checkButtonTouchBegin(buttonPlay, touches);
			checkButtonTouchBegin(buttonMain, touches);
		}
        return true;
    }

    void GameScene::touchEnded(const std::vector< Touch * > & touches, cocos2d::Event *)
    {
        if(state == PLAYING)
        {
            // Se marcan como soltados los botones que se estén dejando de tocar:
            checkButtonTouchEnd (buttonLeft, touches);
            checkButtonTouchEnd (buttonRight, touches);
			checkButtonTouchEnd(buttonPause, touches);
        }
		else if(state == GAME_OVER)
		{
			checkButtonTouchEnd(buttonReset, touches);
			checkButtonTouchEnd(buttonMain, touches);
		}
		else if(state == PAUSE)
		{
			checkButtonTouchEnd(buttonPlay, touches);
			checkButtonTouchEnd(buttonMain, touches);
		}
    }

    bool GameScene::checkButtonTouchBegin(Button & button, const std::vector< Touch * > & touches)
    {
        // Para cada touch...
        for(auto touch : touches)
        {
            // ...se comprueba si el botón contiene en su área la posición de la escena tocada:
            if(button.touch == NO_TOUCH && button.sprite->getBoundingBox ().containsPoint (touch->getLocation ()))
            {
                // Si es así se vincula el id del touch al botón (para evitar que se suelte en un
                // botón distinto si se arrastra el dedo hacia otro botón):
                button.touch =  touch->getID ();
                return true;
            }
        }
        return false;
    }

	// Se busca si alguno de los toques que termina se vinculó a algún botón y, si es así, se desvincula:
	// Esto se hace así para que no se marque el inicio del toque en un botón y se desmarque en otro botón distinto.
    void GameScene::checkButtonTouchEnd(Button & button, const std::vector< Touch * > & touches)
    {
        for(auto touch : touches)
        {
            if(button.touch != NO_TOUCH && button.touch == touch->getID ())
            {
                button.touch  = NO_TOUCH;
                break;
            }
        }
    }

	// Se comprueba el código de la tecla pulsada para marcar el flag correspondiente
    void GameScene::keyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event *)
    {
            switch(keyCode)
            {
                case EventKeyboard::KeyCode::KEY_A:
                case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                    keyLeftIsPressed  = true;
                    break;
                case EventKeyboard::KeyCode::KEY_D:
                case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                    keyRightIsPressed = true;
                    break;
                case EventKeyboard::KeyCode::KEY_W:
                case EventKeyboard::KeyCode::KEY_UP_ARROW:
                    keyMoveIsPressed  = true;
                    break;
					// Si en Android se pulsa el botón back del terminal, se cierra el juego
                case EventKeyboard::KeyCode::KEY_BACK:     
                    Director::getInstance ()->end ();
                    break;
            }
    }

	// Se comprueba el código de la tecla pulsada para desmarcar el flag correspondiente
    void GameScene::keyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *)
    {
        switch(keyCode)            
        {
            case EventKeyboard::KeyCode::KEY_A:
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                keyLeftIsPressed  = false;
                break;
            case EventKeyboard::KeyCode::KEY_D:
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                keyRightIsPressed = false;
                break;
            case EventKeyboard::KeyCode::KEY_W:
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
                keyMoveIsPressed  = false;
                break;
        }
    }
}
