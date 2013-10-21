#include "GameEngine.h"

GameEngine::GameEngine():
	window(sf::VideoMode(500, 500), "X1337", sf::Style::Titlebar | sf::Style::Close),
	world(window)
{

	sf::Mouse::setPosition(sf::Vector2i(250, 250), window); // Default mouse location
	window.setMouseCursorVisible(false);

	// Initial Configuration
	Config::getInstance().state = Config::getInstance().GAME;		// Set gamestate to Game
	if (!Config::getInstance().font.loadFromFile("COMICATE.TTF"))
	{
		std::cout << "Error, could not load font" << std::endl; // TODO - Change from cout to LOG
		system("pause");
		//return EXIT_FAILURE;
	}


	runGame();

}

void GameEngine::runGame(){

	while(window.isOpen()){
		window.clear(sf::Color::Black);

		switch(Config::getInstance().state)
		{
		case Config::GAME:
			// Process Scene
			this->world.process();
			Config::getInstance().elapsedTime = Config::getInstance().gameClock.getElapsedTime();
			
			// Draw Scene
			this->world.draw();


			break;
		case Config::MENU: 
			break;
		}




		// Display 
		Config::getInstance().gameClock.restart();
		window.display();
	}

}