#include "Menu.h"
#include "Log.h"



Menu::Menu(sf::RenderWindow& window):
	Scene(window)
{
	Globals::getInstance().getResourceHandler()->getSound(ResourceHandler::Sound::MENU_SONG).play();
	Globals::getInstance().getResourceHandler()->getSound(ResourceHandler::Sound::MENU_SONG).setLoop(true);
	this->init();

}

void Menu::reset()
{
	Globals::getInstance().getResourceHandler()->getSound(ResourceHandler::Sound::MENU_SONG).stop();
	this->setInited(false);
}

void Menu::init()
{

	std::map<Menu::Options, std::string> mainMenu;
	{
		mainMenu[Options::NEW_GAME] = "New Game";
		mainMenu[Options::LOAD_GAME] = "Load Existing Game";
		mainMenu[Options::CREDITS] = "Credits";
		mainMenu[Options::EXIT_GAME] = "Exit Game";
	}

	std::map<Menu::Options, std::string> stageSelect;
	{
		stageSelect[Options::BACK] = "Back";
	}

	optMap[Globals::State::MAIN_MENU] = mainMenu;
	optMap[Globals::State::STAGE_SELECT] = stageSelect;

	this->loadMenuOptions();
	this->setCurrentOption(option[GlobalState].begin()->first);
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

/////////////////////////////////////////////
////Preload the menu options into a map//////
/////////////////////////////////////////////
void Menu::loadMenuOptions()
{
	for(auto &i : optMap) // Iterate through maps of options
	{

		int x = 20;
		int y = window.getSize().y - 50;


		std::map<Options, std::string>::reverse_iterator rit;
		for (rit=i.second.rbegin(); rit != i.second.rend(); ++rit)
		{
			sf::Text txt;
			txt.setFont(Globals::getInstance().getResourceHandler()->getFont(ResourceHandler::Fonts::SANSATION));
			txt.setString(sf::String(rit->second));
			std::cout << rit->second << std::endl;
			txt.setCharacterSize(30);
			txt.setPosition(20,y);
			txt.setColor(sf::Color(139,137,137));
			option[i.first][rit->first] = (txt);
			y-= 50;
		}


	}
}

/////////////////////////////////////////////
//////////Input handler for menu/////////////
/////////////////////////////////////////////
void Menu::input(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up )
	{

		if(currentOption  == option[GlobalState].begin()->first)
		{
			currentOption = option[GlobalState].begin()->first;
			LOGD("Pressed up, do nothing");
		}
		else
		{
			currentOption--;
			LOGD("Pressed up, go down");
		}

	}

	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down )
	{

		if(currentOption == option[GlobalState].rbegin()->first)
		{
			currentOption = option[GlobalState].rbegin()->first;
			LOGD("Pressed down, do nothing");
		}
		else
		{
			currentOption++;
			LOGD("Pressed up, go up");
		}

	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return )
	{

		switch(currentOption)
		{
		case Menu::NEW_GAME:
			Globals::getInstance().setState(Globals::State::STAGE_SELECT);
			break;
		case Menu::LOAD_GAME:
			break;
		case Menu::BACK:
			Globals::getInstance().setState(Globals::State::MAIN_MENU);
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
		this->setCurrentOption(option[GlobalState].begin()->first);
	}

}


/////////////////////////////////////////////
////Getter/Setter for current option/////////
/////////////////////////////////////////////
int Menu::getCurrentOption()
{
	return this->currentOption;
}

void Menu::setCurrentOption(int opt)
{
	this->currentOption = opt;
}


/////////////////////////////////////////////
///////Draw implementation for Menu//////////
/////////////////////////////////////////////
void Menu::draw()
{
	if(!this->getInited())
	{
		LOGI("Not inited!");
		return;
	}

	drawGameTitle();
	drawOptions(Globals::getInstance().getState());

	switch(Globals::getInstance().getState())
	{
	case Globals::MAIN_MENU:
		drawMainMenu();
		break;
	case Globals::STAGE_SELECT:
		drawStageSelect();
		break;
	}
}

/////////////////////////////////////////////
//////////Main menu draw function////////////
/////////////////////////////////////////////
void Menu::drawMainMenu()
{
}

/////////////////////////////////////////////
////Draw menu options for current state//////
/////////////////////////////////////////////
void Menu::drawOptions(Globals::State state)
{
	// Draw Options
	for(auto& i: option[state]) window.draw(i.second);

	// Draw Option overlay
	sf::FloatRect pos = option[state][(Menu::Options)currentOption].getGlobalBounds();
	sf::RectangleShape sh = sf::RectangleShape();
	sh.setFillColor(sf::Color(255,255,255,150));
	sh.setSize(sf::Vector2f(pos.width + 20, pos.height / 2));
	sh.setPosition(pos.left - 10,pos.top + (pos.height / 4));
	window.draw(sh);
}



/////////////////////////////////////////////
///////////////Game title////////////////////
/////////////////////////////////////////////
void Menu::drawGameTitle()
{
	// Draw Game Title text
	sf::Text gameTitle;
	gameTitle.setString(sf::String("X1337"));
	gameTitle.setFont(Globals::getInstance().getResourceHandler()->getFont(ResourceHandler::Fonts::COMICATE));
	gameTitle.setColor(sf::Color::Red);
	gameTitle.setCharacterSize(80);
	gameTitle.setPosition(sf::Vector2f(window.getSize().x / 2 - (gameTitle.getGlobalBounds().width / 2), window.getSize().y / 8 - (gameTitle.getGlobalBounds().height / 2)));
	window.draw(gameTitle);

}

/////////////////////////////////////////////
////////////Stage Selection//////////////////
/////////////////////////////////////////////
void Menu::drawStageSelect()
{
	sf::Vector2f frameStartPos((window.getView().getSize().x/4) - 60 , window.getView().getSize().y / 4);
	int yMult = 1;
	int count = 1;
	for(Script& i : Globals::getInstance().getResourceHandler()->getScripts())
	{
		// Image display
		sf::RectangleShape frame(sf::Vector2f(window.getView().getSize().x / 8,window.getView().getSize().y / 8));
		frame.setFillColor(sf::Color(53,24,52));
		frame.setPosition((frameStartPos.x * count) ,frameStartPos.y * yMult);
		window.draw(frame);

		// Text Under image
		sf::Text txtName;
		txtName.setFont(Globals::getInstance().getResourceHandler()->getFont(ResourceHandler::Fonts::SANSATION));
		txtName.setString(i.getScriptName());
		txtName.setCharacterSize(20);
		txtName.setColor(sf::Color::White);
		txtName.setPosition(frame.getPosition().x, frame.getPosition().y + frame.getSize().y);
		window.draw(txtName);

		count++;
		if(count % 4 == 1)
		{
			count = 1;
			yMult++;
		}
	}
}