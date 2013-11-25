#include "Menu.h"
#include "Globals.h"
#include "Log.h"

#include <SFML\Graphics\RectangleShape.hpp>


Menu::Menu(sf::RenderWindow& window):
	Scene(window)
{
	Globals::getInstance().getResourceHandler()->getSound(ResourceHandler::Sound::MENU_SONG).play();
	this->init();



}

void Menu::init()
{
	// Options
	{
		optList[Options::NEW_GAME] = "New Game";
		optList[Options::LOAD_GAME] = "Load Existing Game";
		optList[Options::CREDITS] = "Credits";
		optList[Options::EXIT_GAME] = "Exit Game";
	}
	this->loadMenuOptions();


	this->setInited(true);
}


void Menu::process()
{
	if(!this->getInited())
	{
		LOGI("Not inited!");
		return;
	}
}


void Menu::draw()
{
	if(!this->getInited())
	{
		LOGI("Not inited!");
		return;
	}

	// Draw Options
	for(auto& i: option) window.draw(*i);

	// Draw Option overlay
	sf::FloatRect pos = option[currentOption]->getGlobalBounds();
	sf::RectangleShape sh = sf::RectangleShape();
	sh.setFillColor(sf::Color(255,255,255,150));
	sh.setSize(sf::Vector2f(pos.width + 20, pos.height / 2));
	sh.setPosition(pos.left - 10,pos.top + (pos.height / 4));

	window.draw(sh);


}



void Menu::loadMenuOptions()
{
	int x = 20;
	int y = window.getSize().y - 50;

	std::map<Options, std::string>::reverse_iterator rit;
	for (rit=optList.rbegin(); rit != optList.rend(); ++rit)
	{
		sf::Text* txt = new sf::Text();
		txt->setFont(Globals::getInstance().getResourceHandler()->getFont(ResourceHandler::Fonts::SANSATION));
		txt->setString(sf::String(rit->second));
		txt->setCharacterSize(30);
		txt->setPosition(20,y);
		txt->setColor(sf::Color::White);
		option[rit->first] = txt;

		y-= 50;
	}


	this->setCurrentOption(0);
}

void Menu::input(sf::Event& event)
{
	int max = Menu::Options::MENU_OPT_COUNT - 1;

	if (event.type == sf::Event::KeyPressed && 
		event.key.code == sf::Keyboard::Up )
	{
		if(currentOption < 1)
		{
			currentOption = max;
		}
		else
		{
			currentOption--;
		}

	}
	else if (event.type == sf::Event::KeyPressed && 
		event.key.code == sf::Keyboard::Down )
	{
		if(currentOption == max)
		{
			currentOption = 0;
		}
		else
		{
			currentOption++;
		}

	}
	else if (event.type == sf::Event::KeyPressed && 
		event.key.code == sf::Keyboard::Return )
	{
		switch(currentOption)
		{
		case Menu::NEW_GAME:
			Globals::getInstance().setState(Globals::State::GAME);
			break;
		case Menu::LOAD_GAME:
			break;
		case Menu::CREDITS:
			break;
		case Menu::EXIT_GAME:
			exit(EXIT_SUCCESS);
			break;


		default:
			LOGD("Missing menu action!");
			break;
		}


	}



}

int Menu::getCurrentOption()
{
	return this->currentOption;
}
void Menu::setCurrentOption(int opt)
{
	this->currentOption = opt;
}


