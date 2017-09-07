
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Started by �ngel on january of 2016                                        *
*  This is free software released into the public domain.                     *
*  angel.rodriguez@esne.edu                                                   *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MAIN_SIMPLE_SCENE_HEADER
#define MAIN_SIMPLE_SCENE_HEADER

#include <cocos2d.h>

namespace mainGame
{
	using cocos2d::Vec2;
	using cocos2d::Touch;
	using cocos2d::Sprite;

	class Main : public cocos2d::Scene
	{
	private:
		cocos2d::Menu * menu;							 /// Menu del juego
		cocos2d::Size visibleSize;					    /// Se toma el tama�o visible del display para colocar los elementos gr�ficos
		Sprite * buttonPlay;                           /// Sprite del bot�n de jugar
		Sprite * buttonHelp;                          /// Sprite del bot�n de ayuda
		Sprite * buttonClose;                        /// Sprite del bot�n de cerrar
		Sprite * background;                        /// Imagen con BG
		Sprite * introImage;                       /// Imagen con BG
		cocos2d::Label *labelHelp;				  /// label usada para mostrar el mensaje de ayuda
		cocos2d::MenuItemImage * menu_item1;     /// item que se usara para convertir los sprites en botones
		cocos2d::MenuItemImage * menu_item2;    /// item que se usara para convertir los sprites en botones
		cocos2d::MenuItemImage * menu_item3;   /// item que se usara para convertir los sprites en botones

	public:
		CREATE_FUNC(Main);

	private:
		/// Este m�todo se llamar� autom�ticamente para inicializar la escena.
		bool init() override;

		/// Funci�n auxiliar que se llama desde init() para crear y configurar los botones.
		void createButtons(cocos2d::Layer * layer);

		// Callbacks
		//carga el juego
		void LoadGame(Ref * pSender);
		//activa la ayuda
		void LoadHelp(Ref * pSender);

		//cierra la ayuda
		void CloseHelp(Ref * pSender);
	};
}
#endif