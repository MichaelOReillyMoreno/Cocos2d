
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on january of 2016                                        *
 *  This is free software released into the public domain.                     *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_SIMPLE_SCENE_HEADER
#define GAME_SIMPLE_SCENE_HEADER

    #include <vector>
    #include <cocos2d.h>
	#include "MainMenu.hpp"
    namespace game
    {

        using cocos2d::Vec2;
        using cocos2d::Touch;
        using cocos2d::Sprite;

        class GameScene : public cocos2d::Scene
        {
        private:
            /// Esta enumeración define los posibles estados de la escena
            enum State
            {
                PLAYING, GAME_OVER, PAUSE
            };

            /// Se usa una enumeración para definir una constante entera que identifica un id de touch
            /// que se entiende como nulo
            enum
            {
                NO_TOUCH = -1
            };

            /// Esta es una estructura que se usa como base de otras que necesitan un puntero a Sprite
            /// y un operador de conversión automático a puntero a Sprite (operator Sprite *)
            struct SpriteWrapper
            {
                Sprite * sprite;
                operator Sprite * (){ return sprite; }
            };

            /// Esta estructura representa una entidad del juego (es básicamente un Sprite con información complementaria)
            struct Entity : SpriteWrapper
            {
                Vec2 speed;
				bool isEnemy;
            };

			/// Esta estructura representa una punto del juego
			struct labelPoint
			{
				Vec2 startPosition;
				cocos2d::Label * label;
				bool isActive;
				Vec2 speed;
			};

            /// Esta estructura representa un botón con el que el jugador puede interactuar para controlar la caja
            struct Button : SpriteWrapper
            {
                int touch;
            };

            /// Este typedef facilita que se pueda reemplazar std::vector por cualquier otro contenedor apropiado
            typedef std::vector< Entity > EntityList;
			typedef std::vector< labelPoint > LabelPointList;
            static const int framesPerSecondLimit     = 30;     ///< Velocidad de refresco de la escena
            static const int babyAdvanceSpeed     = 250;       ///< Velocidad a la que avanzan los asteroides
			static const int maxSimultaneouBabys = 10;        ///< Número maximo de bebes en escena
			static const int maxSimultaneouPoints = 10;		 ///< Número maximo de puntos en escena
			int levelToAddBaby = 5;						   ///proxima puntuacion del jugador en la que se añadira un nuevo bebe y aumentara el impulso
			int boxAdvanceImpulse = 10;					  ///< Fuerza con la que se incrementa la velocidad de la caja

        private:
			cocos2d::Label * labelScore;					   /// label usada para representar la score del jugador
			cocos2d::Size visibleSize;					  /// Se toma el tamaño visible del display para colocar los elementos gráficos
            State state;                                 ///< Estado de la escena
            Entity box;                                 ///< Sprite de la caja
            EntityList babyList;                       ///< Lista de sprites de bebes
            EntityList explosions;                    ///< Lista de sprites de explosiones
			LabelPointList pointsList;				 ///< Lista de sprites de puntos

            Sprite * gameOver;                                ///< Imagen con el texto 'game over'
			Sprite * background;                             ///< Imagen con BG

            Button buttonLeft;                              ///< Sprite del botón de izquierda
            Button buttonRight;                            ///< Sprite del botón de derecha
			Button buttonReset;                           ///< Sprite del botón de reset
			Button buttonMain;                           ///< Sprite del botón de main
			Button buttonPause;                         ///< Sprite del botón de pause
			Button buttonPlay;                         ///< Sprite del botón de play

            bool keyLeftIsPressed;                        ///< Flag que controla si está pulsada la tecla de 'izquierda'
            bool keyRightIsPressed;                      ///< Flag que controla si está pulsada la tecla de 'derecha'
            bool keyMoveIsPressed;                      ///< Flag que controla si está pulsada la tecla de 'impulso'

			int score;								    ///puntuacion del jugador
			static const int indexSprites = 8;		   ///numero de sprites
			static const int startEnemies = 4;		   ///a partir de que punto en el arrya son enemigos
			std::string sprites[indexSprites] =       ///lista de sprites disponibles
			{ "baby1.png", "baby2.png", "baby3.png",
				"baby4.png", "baby5.png", "esq1.png",
				"ork1.png", "ork2.png" };

        public:

            // Esta macro crea la implementación de un método create() predefinido que sirve para
            // crear instancias de esta clase:
            CREATE_FUNC(GameScene);

        private:

            /// Este método se llamará automáticamente para inicializar la escena.
            bool init() override;

            /// Función auxiliar que se llama desde init() para crear y configurar los botones.
            void createButtons(cocos2d::Layer * layer);

            /// Inicializa por completo el estado de la escena (para comenzar el juego).
            void resetGame();

            /// Establece el estado 'game over' cuando un bebe choca contra el suelo o un enemigo contra la cesta.
            void setGameOverState();

			/// Establece el estado 'pause' cuando se pulsa dicho boton.
			void setPauseStateOn();

			/// Desactiva el estado 'pause' cuando se pulsa dicho boton.
			void setPauseStateOff();

            /// Genera una explosión en las coordenadas que se indiquen.
            void makeExplosion(const Vec2 & position);

			//cambia la puntuacion del jugador y si llega a la puntuacion adecuada modifica los bebes en escena y el impulso de la cesta
			void ChangeScore();

        private:

            /// Este método se llamará automáticamente cada fotograma para actualizar el estado de la escena.
            /// Es una reimplementación de un método heredado de la clase Scene.
            void update(float delta) override;

            //  Métodos para actualizar diversos tipos de entidades del juego:
            void updateBox(float delta);
            void updateBabys(float delta);
			void updatePoints(float delta);
            void updateExplosions();
			void updateButtonsPause();
			void updateButtonsOnPlaying();
			void updateButtonsOnGameOver();

            /// Este método se usa para reposicionar la caja en el eje x al salirse de la pantalla
            void correctBoxLocation(Sprite * sprite);

			//comprueba la posicion de los bebes para realizar acciones en consecuencia dependiendo de si son enemigos o no
			void CheckBabyLocation(Entity& baby);

			//comprueba si los rectangulos de la cesta y un bebe han colisionado
            bool checkSquareCollision(const Sprite * asteroid, const Sprite * sprite);

            /// comprueban las colisiones de los bebe con la cesta
            void checkBabyBoxCollision();

			//selecciona un bebe de la lista que no este en uso y lo establece como visible, randY si se desea que tenga un aleatorio en este eje
			void AddNoUseBaby(bool randY);

        private:

            // Funciones para procesar los eventos de inicio y fin de toque en la pantalla:
            bool touchBegan(const std::vector< Touch * > & touches, cocos2d::Event *);
            void touchEnded(const std::vector< Touch * > & touches, cocos2d::Event *);

            /// Esta función pemite comprobar si se ha tocado con algún dedo el botón que se pasa por parámetro.
            /// Si se ha tocado retorna el valor true y guarda en el botón el id del puntero.
            bool checkButtonTouchBegin(Button & button,  const std::vector< Touch * > & touches);

            /// Esta función pemite comprobar si se ha dejado de tocar con algún dedo el botón que se pasa por parámetro.
            /// Si se ha dejado de tocar guarda en el botón el id 'NO_TOUCH'.
            void checkButtonTouchEnd(Button & button,  const std::vector< Touch * > & touches);

            // Funciones para procesar los eventos de tecla pulsada y soltada:
            // Se han añadido para probar con más comodidad el juego durante el desarrollo y para
            // tener en cuenta la pulsación del botón back en Android.
            void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);
            void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);

        };

    }

#endif
