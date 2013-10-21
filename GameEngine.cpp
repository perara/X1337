#include "GameEngine.h"
#include "Log.h"

GameEngine::GameEngine():
	window(sf::VideoMode(500, 500), "X1337", sf::Style::Titlebar | sf::Style::Close),
	world(window)
{

	// Set mouse properties
	sf::Mouse::setPosition(sf::Vector2i((window.getSize().x / 2), (window.getSize().y / 2)), window); // Default mouse location
	window.setMouseCursorVisible(false);

	// Initial Configuration
	Config::getInstance().state = Config::getInstance().GAME;		// Set gamestate to Game
	if (!Config::getInstance().font.loadFromFile("COMICATE.TTF"))
		LOGE("Error, could not load font");

	// Start Gameloop
	this->runGame();
}


void GameEngine::runGame(){

	while(window.isOpen()){
		window.clear(sf::Color::Black);

		if(Config::getInstance().state == Config::GAME){
			// Process Scene
			this->world.process();
			Config::getInstance().elapsedTime = Config::getInstance().gameClock.getElapsedTime();

			// Draw Scene
			this->world.draw();
		}

		if(Config::getInstance().state == Config::MENU){

		}

		// Display 
		Config::getInstance().gameClock.restart();
		window.display();
	}

}

