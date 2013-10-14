

#include <SFML\Graphics\RenderWindow.hpp>
#include "Player.h"
#include "World.h"
#include "main.h"
#include <iostream>

// Scene 1
World world1;
Player p1;
Player p2;


// Scene 2
World world2;
Player p3;
Player p4;



int main(){
	Config::getInstance().state = Config::getInstance().GAME;




	sf::RenderWindow window(sf::VideoMode(500,500),"X1337", sf::Style::Titlebar | sf::Style::Close);

	// World 1 (Scene1)
	world1 = World();
	{
		p1 = Player(sf::Vector2f(100,250), 10);
		p2 = Player(sf::Vector2f(100,300), 10);
		world1.addObject(&p1);
		world1.addObject(&p2);
	}

	// World 2 (Scene 2), this would be for example Menu in a game.
	world2 = World();
	{
		p3 = Player(sf::Vector2f(300,250), 10);
		p4 = Player(sf::Vector2f(333,300), 10);
		world2.addObject(&p3);
		world2.addObject(&p4);

	}


	while(window.isOpen()){
		// Clear The window
		window.clear(sf::Color::Black);

		switch(Config::getInstance().state){
		case Config::GAME:
			// Process The world
			world1.process();
			// Control player 1
			p1.inputHandler(window); // TODO, might make a vector which we pass to .process in Scene (if we have multiple inputHandlers
			// Draw World 1
			world1.draw(window);


			break;
		case Config::MENU: //Notice how exit button do not work anymore, (This is due to no imput handler in this Scene)
			world2.process();
			world2.draw(window);
			break;
		}




		// Display 
		window.display();
	}


	return 0;
}