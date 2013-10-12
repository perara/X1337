#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "PlayerController.h"
#include "Renderer.h"
#include "World.h"
#include <boost/regex.h>

int main()
{
	Window* w = new Window();
	World *world = new World();

	Renderer* r = new Renderer(*w, world->p1); // world->p1 not good, fix
	PlayerController* i = new PlayerController(*w, world->p1, r->window);

	// run the program as long as the window is open
    while (r->isWindowOpen())
    {
		i->updateWorld();
		r->render();
    }
	
    return 0;
}