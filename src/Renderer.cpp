//
// Created by per on 4/19/21.
//

#include "Renderer.h"

void Renderer::setView(const sf::View& view) {
    rw.setView(view);
    rt.setView(view);
}


void Renderer::clear(const sf::Color &color) {
    rw.clear(color);
    rt.clear(color);

}


void Renderer::draw(const sf::Drawable &drawable) {
    rw.draw(drawable);
    rt.draw(drawable);

}


const sf::View& Renderer::getView(){
    return rw.getView();
}

void Renderer::display(){
    rw.display();
}

sf::RenderWindow& Renderer::getWindow(){
    return rw;
}

sf::Vector2u Renderer::getSize(){
    return rw.getSize();
}

bool Renderer::isOpen(){
    // TODO return false when no window
    return rw.isOpen();
}

void Renderer::close(){

    return rw.close();
}

bool Renderer::pollEvent(sf::Event& event){

    if(!injectedEvents.empty()){
        event = *injectedEvents.front();
        injectedEvents.pop_front();
        return true;
    }

    return rw.pollEvent(event);
}

Renderer::Renderer(int width, int height, const std::string& title):
rw(sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize))
{
    rt.create(width, height);
    capture();
}

void Renderer::create(sf::VideoMode mode, const sf::String &title, sf::Uint32 style, const sf::ContextSettings &settings) {
    rw.create(mode, title, style, settings);

}

Image Renderer::capture() {
    auto x = rt.getTexture().copyToImage();
    auto sizes = x.getSize();
    return Image(sizes.y, sizes.x, 3, (unsigned char*)x.getPixelsPtr());
}


