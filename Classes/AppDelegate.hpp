
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Modified Cocos2d-x example code by Ángel on january of 2016                *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef APP_DELEGATE_HEADER
#define APP_DELEGATE_HEADER

    #include <cocos2d.h>

    /**
    @brief The cocos2d Application.
    Note: The reason for implement as private inheritance is to hide some interface call by Director.
    */
    class AppDelegate : private cocos2d::Application
    {
    private:

        // Design Resolution es la resolución "virtual" que tendrán todas las escenas en cualquier dispositivo:

        static const int designResolutionWidth  = 720;             ///< Ancho virtual de todas las escenas
        static const int designResolutionHeight = 1280;             ///< Alto  virtual de todas las escenas

        // Assets Resolution es la resolución para la que se prepararon los assets.
        // En proyectos más elaborados normalmente conviene tener varios juegos de assets preparados para
        // diversas resoluciones reales para se tengan que escalar menos:

        static const int assetsResolutionWidth  = 720;
        static const int assetsResolutionHeight = 1280;

    public:

        /**
        @brief  Implement Director and Scene init code here.
        @return true    Initialize success, app continue.
        @return false   Initialize failed, app terminate.
        */
        virtual bool applicationDidFinishLaunching () override;

        /**
        @brief  The function be called when the application enter background
        @param  the pointer of the application
        */
        virtual void applicationDidEnterBackground () override;

        /**
        @brief  The function be called when the application enter foreground
        @param  the pointer of the application
        */
        virtual void applicationWillEnterForeground () override;

    };

#endif
