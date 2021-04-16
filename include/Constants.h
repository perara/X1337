//
// Created by per on 4/16/21.
//

#ifndef X1337_CONSTANTS_H
#define X1337_CONSTANTS_H

namespace Constants{


    namespace ResourceC{

        /// Texture enumerable which contains all resource "Links" its used to access the resource
        enum Texture
        {
            NOID=1, // Just default null texture
            BACKGROUND2=2,
            BACKGROUND3=3,
            HEART=4,
            BOSS=5,
            PLAYER_SHIP=6,
            ENEMY_SHIP=7,
            HEAVY_SHOT_TEXTURE=8,
            BOSS_DEATHSTAR_TEXTURE=9,
            CHUBBY_SHIP_TEXTURE=10,
            AUDIO_ON=11,
            AUDIO_OFF=12,
            PULSE_GUN=13,
            HEALTH_KIT=14,
            REGULAR_BULLET_1=15,
            PLAYER_BAR=16,
            MONITOR_ICON=17,

            PORTRAIT_TWINS=18,
            PORTRAIT_COUNCIL=19,
            PORTRAIT_DEATHSTAR=20,

            TEXTURECOUNT
        };

        /// Sound enumerable which contains all resource "Links" its used to access the resource
        enum Sound
        {
            MUSIC_DEATH_STAR_THEME = 1,
            MUSIC_MENU_SONG,
            MUSIC_COUNTDOWN,
            MUSIC_INGAME, // The usual ingame for all stages

            FX_STANDARD_SHOT,
            FX_HEAVY_SHOT,
            FX_ENEMY_DEATH,
            FX_PICKUP_HEALTH,
            FX_MENU_CLICK,
            FX_MENU_RETURN,
            FX_ERROR,
            FX_BOUNCE,

            EMOTE_DEATHSTAR_GREET,
            EMOTE_DEATHSTAR_BEWARE,
            EMOTE_DEATHSTAR_PERIODIC_1,
            EMOTE_DEATHSTAR_PERIODIC_2,
            EMOTE_DEATHSTAR_PERIODIC_3,
            EMOTE_DEATHSTAR_PERIODIC_4,
            EMOTE_DEATHSTAR_DEATH,

            STORY_DEATHSTAR_INTRO,
            STORY_TWINS_INTRO,
            STORY_COUNCIL_INTRO,

            SOUNDCOUNT

        };

        /// Scripts enumerable which contains all resource "Links" its used to access the resource
        enum Scripts
        {
            STAGE_ONE,
            STAGE_TWO,
            DEATH_STAR,
            GAME_MENU,

            SCRIPTSCOUNT
        };

        /// Fonts enumerable which contains all resource "Links" its used to access the resource
        enum Fonts
        {
            COMICATE,
            SANSATION,
            FONTCOUNT,
        };

    }


    namespace GameShapeC{
        /// ShapeType is an enumerable which contains all of the defined convex shapes in the GameShape implementation
        enum Type{
            CIRCLE=1,
            TRIANGLE=2,
            STARSHIP=3,
            BOSS=4,
            PLAYER_SHIP=5,
            SQUARE=6,
            SHAPECOUNT
        };
    }



    namespace MenuC{

        enum Options
        {
            // Main menu opts
            NEW_GAME,
            HIGHSCORE,
            CREDITS,
            EXIT_GAME,

            LOAD_GAME,

            // Stage Select opts
            STAGE_SELECT,
            BACK,

            // Difficulty Select opts
            NORMAL,
            HARD,

            // In-game pause opts
            CONTINUE_GAME,
            TO_MAIN_MENU,

            // In-game gameover opts
            RESTART_STAGE,
            TO_MAIN_MENU2,

            // Win game Options
            NEXT_STAGE,
            RESTART_STAGE_2,
            TO_MAIN_MENU_3,

            MENU_OPT_COUNT
        };
    }

    namespace EnemyC{
        /// Enumerable which contains all EnemyType 's implemented into the game.
        enum Type
        {
            REGULAR = 1,
            CHUBBY = 2,
            BOSS = 3,
            DEATHSTAR = 4,

            POWERUP_HEALTH = -10,
            NONE = -1,
            SHOOTERTYPECOUNT

        };
    }

    namespace EngineC{
        /// State Enumerable which contains all possible states for the GameEngine, World and Menu.
        enum State{
            GAME,
            INIT_GAME,
            INIT_MAIN_MENU,
            STAGE_SELECT,
            DIFFICULTY_SELECT,
            MAIN_MENU,
            OPTIONS,
            CREDITS,
            HIGHSCORE,
            PAUSE,
            GAMEOVER,


            GAMEWIN,
            INIT_NEXT_STAGE,

        };
    }

    /// An enumration which describes the ShooterType
    enum ShooterType
    {
        PLAYER,
        ENEMY,
        NONE
    };


    /// Defines the Bullet Type in an enumerable. This is all implemented bullets in the Game.
    enum BulletType {
        standardShot = 1,
        heavyShot = 5
    };

    /// PowerUpType is an enumerable which contains current implemented Powerup 's
    enum PowerUpType
    {
        HEALTH_INCREMENT = -10,
        PULSATING_GUN = -11
    };



#define GameState Constants::EngineC::State

}


#endif //X1337_CONSTANTS_H
