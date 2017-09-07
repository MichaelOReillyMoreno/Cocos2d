
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Modified Cocos2d-x example code by Ángel on january of 2016                *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <vector>
#include <string>
#include "Intro.hpp"
#include "AppDelegate.hpp"
#include "SimpleAudioEngine.h"
using namespace std;
using namespace cocos2d;

// Esta macro permite determinar si se está compilando para un entorno de escritorio:

#define IS_DESKTOP_COMPUTER (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || \
                            (CC_TARGET_PLATFORM == CC_PLATFORM_MAC  ) || \
                            (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

bool AppDelegate::applicationDidFinishLaunching ()
{
    // Se crea la ventana en la que se mostrarán todas las escenas:
    // En ordenadores de escritorio la ventana tiene un tamaño concreto y en dispositivos móviles
    // ocupa toda (o casi toda) la pantalla.

    auto director = Director::getInstance   ();
    auto glView   = director->getOpenGLView ();

    if (!glView)
    {
        #if IS_DESKTOP_COMPUTER
            glView = GLViewImpl::createWithRect ("OnlyBabysDude", Rect(0, 0, designResolutionWidth / 2, designResolutionHeight / 2));
        #else
            glView = GLViewImpl::create ("Simple Asteroids");
        #endif

        director->setOpenGLView (glView);
    }

    // Se establece la subcarpeta en la que se buscarán los recursos.
    // Establecer la subcarpeta apropiada según la resolución del dispositivo se deja pendiente como ejercicio.
    // Revisar esta documentación: http://www.cocos2d-x.org/wiki/Multi_resolution_support

    vector< string > searchPathList;

    searchPathList.push_back ("720p");

    FileUtils::getInstance ()->setSearchPaths (searchPathList);

    // Se establece la resolución de diseño de las escenas:

    glView->setDesignResolutionSize (designResolutionWidth, designResolutionHeight, ResolutionPolicy::NO_BORDER);

    // Se establece cuánto hay que escalar los assets para acaptarse a la resolución de diseño:

    director->setContentScaleFactor (float(assetsResolutionWidth) / designResolutionWidth);

    // Descomentar si se quieren mostrar estadísticas de rendimiento en pantalla:

    // director->setDisplayStats (true);

    // Se inicia la escena principal:

    director->runWithScene (IntroGame::Intro::create ());
    return true;
}

void AppDelegate::applicationDidEnterBackground ()
{
    Director::getInstance ()->stopAnimation ();

     CocosDenshion::SimpleAudioEngine::getInstance ()->pauseBackgroundMusic ();
}

void AppDelegate::applicationWillEnterForeground ()
{
    Director::getInstance ()->startAnimation ();

	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic ();
}
