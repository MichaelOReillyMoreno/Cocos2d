
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Started by Ángel on january of 2016                                        *
*  This is free software released into the public domain.                     *
*  angel.rodriguez@esne.edu                                                   *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef INTRO_SIMPLE_SCENE_HEADER
#define INTRO_SIMPLE_SCENE_HEADER

#include <cocos2d.h>
#include "MainMenu.hpp"
namespace IntroGame
{
	using cocos2d::Vec2;
	using cocos2d::Sprite;

	class Intro : public cocos2d::Scene
	{
	private:
		Sprite * introImage;                       /// Imagen con intro
		cocos2d::Size visibleSize;				  /// Se toma el tamaño visible del display para colocar los elementos gráficos
		int delay;								 ///se crea un delay rudimentario para mantener un instante la cartela en pantalla

	public:
		CREATE_FUNC(Intro);

	private:
		/// Este método se llamará automáticamente para inicializar la escena.
		bool init() override;
		//carga el juego
		void LoadGame();

		/// Este método se llamará automáticamente cada fotograma para actualizar el estado de la escena.
		/// Es una reimplementación de un método heredado de la clase Scene.
		void update(float delta) override;
	};
}
#endif