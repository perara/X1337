#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "World.h"
#include "InputController.h"
#include "Renderer.h"
#include <boost/regex.h>

int main()
{
	World* w = new World();
	Renderer* r = new Renderer(*w);
	InputController* i = new InputController(*w, r->window);
	sf::Mouse::setPosition();

	// run the program as long as the window is open
    while (r->isWindowOpen())
    {
		i->updateWorld();
		r->render();
    }
	
    return 0;
}