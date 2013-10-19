#include "GameEngine.h"



GameEngine::GameEngine():
	window(sf::VideoMode(500, 500), "X1337", sf::Style::Titlebar | sf::Style::Close),
	world(window)
{
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
	
	// Button btn1 = Button(sf::String("Hello WOrld"), 30, sf::Vector2f(250,250), sf::Vector2f(200,100));
	// btn1.Draw(window);
	// btn1.isClicked();
	while(window.isOpen()){
		// Clear The window
		window.clear(sf::Color::Black);

		switch(Config::getInstance().state){
		case Config::GAME:
			// Process The world
			this->world.process();

			// Draw World 1
			this->world.draw();


			break;
		case Config::MENU: //Notice how exit button do not work anymore, (This is due to no imput handler in this Scene)

			break;
		}




		// Display 
		window.display();
	}

}