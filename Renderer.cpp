#include "Renderer.h"
#include "World.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

bool Renderer::isWindowOpen()
{
	return window.isOpen();
}
bool Renderer::pollEvent(sf::Event& e)
{
	return window.pollEvent(e);
}

void Renderer::render()
{
	window.clear(sf::Color::Black);
	window.draw(w.p1.playerShape);
	window.display();
}

Renderer::Renderer(World& w):
	w(w),
	window(sf::VideoMode(w.windowSize.x, w.windowSize.y), "X1337", sf::Style::Titlebar | sf::Style::Close)
{
	window.setMouseCursorVisible(false);
}


Renderer::~Renderer(void)
{
}