//
// Created by per on 4/19/21.
//

#ifndef X1337_RENDERER_H
#define X1337_RENDERER_H


#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Image.h"
#include <queue>
#include <memory>

class Renderer {
private:
    sf::RenderWindow rw;
    sf::RenderTexture rt;

public:
    std::deque<std::shared_ptr<sf::Event>> injectedEvents;

    explicit Renderer(int width, int height, const std::string& title);

    void setView(const sf::View& view);
    void clear(const sf::Color& color=sf::Color::Transparent);
    void draw(const sf::Drawable& drawable);
    void create(sf::VideoMode mode, const sf::String& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
    Image capture();
    const sf::View &getView();

    void display();

    sf::Vector2u getSize();

    sf::RenderWindow & getWindow();

    bool isOpen();

    void close();

    bool pollEvent(sf::Event &event);
};


#endif //X1337_RENDERER_H
