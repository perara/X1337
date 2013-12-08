#include "Menu.h"
#include "Log.h"
#include <sstream>


Menu::Menu(sf::RenderWindow& window, GameState& state, std::unique_ptr<ResourceHandler>& resourceHandler) :
Scene(window, resourceHandler),
state(state),
hardmodeSelected(false)
{
	this->init();
}
void Menu::process(){}


void Menu::reset()
{
	resourceHandler->getSound(ResourceHandler::Sound::MENU_SONG).stop();
}

bool Menu::getHardmodeSelected()
{
	return hardmodeSelected;
}

void Menu::init()
{

	// Get script names
	for (auto&i : resourceHandler->getScripts()) scripts.push_back(i);


	std::map<Menu::Options, std::string> mainMenu;
	{
		mainMenu[Menu::Options::NEW_GAME] = "New Game";
		//mainMenu[Menu::Options::LOAD_GAME] = "Load Existing Game";
		mainMenu[Menu::Options::HIGHSCORE] = "Highscore";
		mainMenu[Menu::Options::CREDITS] = "Credits";
		mainMenu[Menu::Options::EXIT_GAME] = "Exit Game";
	}

	std::map<Menu::Options, std::string> stageSelect;
	{
		stageSelect[Menu::Options::BACK] = "Back";
		stageSelect[Menu::Options::SELECT_STAGE] = "Select Stage";
	}

	std::map<Menu::Options, std::string> difficultySelect;
	{
		difficultySelect[Menu::Options::NORMAL] = "Normal";
		difficultySelect[Menu::Options::HARD] = "HARD!!";
	}

	std::map<Menu::Options, std::string> highScore;
	{
		highScore[Menu::Options::BACK] = "Back";
	}

	std::map<Menu::Options, std::string> pause;
	{
		pause[Menu::Options::CONTINUE_GAME] = "Continue";
		pause[Menu::Options::TO_MAIN_MENU] = "Return to Main Menu";
	}

	std::map<Menu::Options, std::string> gameOver;
	{
		gameOver[Menu::Options::RESTART_STAGE] = "Restart Game";
		gameOver[Menu::Options::TO_MAIN_MENU2] = "Return to Main Menu";
	}


	// Create each of the state indexes and places options in the map
	optMap[GameState::MAIN_MENU] = mainMenu;
	optMap[GameState::STAGE_SELECT] = stageSelect;
	optMap[GameState::DIFFICULTY_SELECT] = difficultySelect;
	optMap[GameState::PAUSE] = pause;
	optMap[GameState::HIGHSCORE] = highScore;
	optMap[GameState::CREDITS] = highScore; // Use the same as highscore, we only need "Back"
	optMap[GameState::GAMEOVER] = gameOver;

	// Load all of the options
	loadMenuOptions();

	// Set default stage select to 1
	setStageSelectOption(1);

	if (!option[state].empty())
	{
		setCurrentOption(option[state].begin()->first);
	}
	else
	{
		setCurrentOption(option[GameState::MAIN_MENU].begin()->first);
	}
}

void Menu::updateCurrentOption()
{
	this->setCurrentOption(option[state].begin()->first);
}
/////////////////////////////////////////////
////Preload the menu options into a map//////
/////////////////////////////////////////////
void Menu::loadMenuOptions()
{
	for (auto &i : optMap) // Iterate through maps of options
	{
		int x = 20;
		int y = window.getView().getSize().y - 50;

		std::map<Menu::Options, std::string>::reverse_iterator rit;
		for (rit = i.second.rbegin(); rit != i.second.rend(); ++rit)
		{
			sf::Text txt;
			txt.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
			txt.setString(sf::String(rit->second));
			LOGD(rit->second);
			txt.setCharacterSize(30);
			txt.setPosition(20, y);
			txt.setColor(sf::Color(139, 137, 137));
			option[i.first][rit->first] = (txt);
			y -= 50;
		}
	}
}

/////////////////////////////////////////////
//////////Input handler for menu/////////////
/////////////////////////////////////////////
void Menu::input(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
	{
		resourceHandler->getSound(ResourceHandler::Sound::MENU_CLICK).play();

		if (currentOption == option[state].begin()->first)
		{
			currentOption = option[state].rbegin()->first;
			LOGD("Pressed up, do nothing");
		}
		else
		{
			currentOption--;
			LOGD("Pressed up, go up");
		}

	}

	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
	{
		resourceHandler->getSound(ResourceHandler::Sound::MENU_CLICK).play();
		if (currentOption == option[state].rbegin()->first)
		{
			currentOption = option[state].begin()->first;
			LOGD("Pressed down, do nothing");
		}
		else
		{
			currentOption++;
			LOGD("Pressed down, go down");
		}

	}
	// Each of the switch cases below is which action state that should be set whenever a user clicks enter on the selected 
	// menu choice. Lets say the user selects EXIT_GAME, we then want to exit the game. This is general for all menu's, hence
	// the mess.
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
	{
		resourceHandler->getSound(ResourceHandler::Sound::MENU_RETURN).play();

		switch (currentOption)
		{
			/////////////////////////////////////////////
			/////////////////Main Menu///////////////////
			/////////////////////////////////////////////
		case Menu::Options::NEW_GAME:
			state = GameState::STAGE_SELECT;
			this->setCurrentOption(option[state].begin()->first);
			break;
		case Menu::Options::LOAD_GAME:
			break;
		case Menu::Options::HIGHSCORE:
			state = GameState::HIGHSCORE;
			this->setCurrentOption(option[state].begin()->first);
			break;
		case Menu::Options::CREDITS:
			state = GameState::CREDITS;
			this->setCurrentOption(option[state].begin()->first);
			break;
		case Menu::Options::EXIT_GAME:
			exit(EXIT_SUCCESS);
			break;

			/////////////////////////////////////////////
			///////////////Stage Select//////////////////
			/////////////////////////////////////////////
		case Menu::Options::SELECT_STAGE:
			state = GameState::DIFFICULTY_SELECT;
			this->setCurrentOption(option[GameState::DIFFICULTY_SELECT].begin()->first);
			break;
		case Menu::Options::BACK:
			state = GameState::MAIN_MENU;
			this->setCurrentOption(option[state].begin()->first);
			break;

			/////////////////////////////////////////////
			///////////Difficulty Selection//////////////
			/////////////////////////////////////////////
		case Menu::Options::NORMAL:
			hardmodeSelected = false;
			state = GameState::INIT_GAME;
			break;
		case Menu::Options::HARD:
			hardmodeSelected = true;
			state = GameState::INIT_GAME;
			break;

			/////////////////////////////////////////////
			/////////////IN-GAME-Pause///////////////////
			/////////////////////////////////////////////
		case Menu::Options::CONTINUE_GAME:
			state = GameState::GAME;
			break;
		case Menu::Options::TO_MAIN_MENU:
			state = GameState::INIT_MAIN_MENU;
			this->setCurrentOption(option[GameState::MAIN_MENU].begin()->first);
			break;

			/////////////////////////////////////////////
			//////////IN-GAME- GAME OVER ////////////////
			/////////////////////////////////////////////
		case Menu::Options::RESTART_STAGE:
			state = GameState::INIT_GAME;
			break;
		case Menu::Options::TO_MAIN_MENU2:
			state = GameState::INIT_MAIN_MENU;
			this->setCurrentOption(option[GameState::MAIN_MENU].begin()->first);
			break;

		default:
			LOGD("Missing menu action!");
			break;
		}
	}

	// Do additional input for stage select (Two input handlers, horizontal + vertical
	if (state == GameState::STAGE_SELECT)	stageSelectInput(event);
}

void Menu::stageSelectInput(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
	{
		LOGD("Current Stage selected: " << getStageSelectOption());

		if (getStageSelectOption() == 1)
		{
			setStageSelectOption(numStages);
		}
		else
		{
			setStageSelectOption(getStageSelectOption() - 1);
		}

	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
	{
		LOGD("Current Stage selected: " << getStageSelectOption());

		if (getStageSelectOption() == numStages)
		{
			setStageSelectOption(1);
		}
		else
		{
			setStageSelectOption(getStageSelectOption() + 1);
		}
	}


}


//////////////////////////////////////////////
//Getter/Setter for current option variables//
//////////////////////////////////////////////
int Menu::getCurrentOption()
{
	return this->currentOption;
}

void Menu::setCurrentOption(int opt)
{
	this->currentOption = opt;
}

int Menu::getStageSelectOption()
{
	return this->stageSelectOption;
}
void Menu::setStageSelectOption(int opt)
{
	this->stageSelectOption = opt;
}

/////////////////////////////////////////////
///////Draw implementation for Menu//////////
/////////////////////////////////////////////
void Menu::draw()
{
	drawGameTitle();
	switch (state)
	{
	case GameState::MAIN_MENU:
		drawMainMenu();
		drawOptions(state);
		break;
	case GameState::STAGE_SELECT:
		drawStageSelect();
		drawOptions(state);
		break;
	case GameState::DIFFICULTY_SELECT:
		drawStageSelect();
		drawOptions(state);
		break;
	case GameState::HIGHSCORE:
		drawHighScore();
		drawOptions(state);
		break;
	case GameState::PAUSE:
		// Do nothing
		break;
	case GameState::GAMEOVER:
		// Do nothing
		break;
	case GameState::CREDITS:
		drawCredits();
		drawOptions(state);
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
void Menu::drawOptions(GameState state, int xOffset, int yOffset, sf::Color color)
{
	// Draw Options
	for (auto& i : option[state])
	{
		i.second.move(xOffset, yOffset); // Move 'x' offset and 'y' offset
		i.second.setColor(color);
		window.draw(i.second);
		i.second.move(-xOffset, -yOffset); // Move back  'x' offset and 'y' offset

	}

	// Draw Option overlay
	sf::FloatRect pos = option[state][(Menu::Options)currentOption].getGlobalBounds();
	sf::RectangleShape sh;
	sh.setFillColor(sf::Color(255, 255, 255, 150));
	sh.setSize(sf::Vector2f(pos.width + 20, pos.height / 2));
	sh.setPosition(pos.left - 10 + xOffset, pos.top + (pos.height / 4) + yOffset);
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
	gameTitle.setFont(resourceHandler->getFont(ResourceHandler::Fonts::COMICATE));
	gameTitle.setColor(sf::Color::Red);
	gameTitle.setCharacterSize(80);
	gameTitle.setPosition(sf::Vector2f(window.getView().getSize().x / 2 - (gameTitle.getGlobalBounds().width / 2), window.getView().getSize().y / 8 - (gameTitle.getGlobalBounds().height / 2)));
	window.draw(gameTitle);


	// Game version
	sf::Text gameVersion;
	gameVersion.setString(sf::String("v1.0"));
	gameVersion.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	gameVersion.setColor(sf::Color(139, 137, 137));
	gameVersion.setCharacterSize(15);
	gameVersion.setPosition(sf::Vector2f(window.getView().getSize().x - gameVersion.getGlobalBounds().width - 10, window.getView().getSize().y - gameVersion.getGlobalBounds().height * 2));
	window.draw(gameVersion);

}

/////////////////////////////////////////////
////////////Stage Selection//////////////////
/////////////////////////////////////////////
void Menu::drawStageSelect()
{
	sf::Vector2f frameStartPos((window.getView().getSize().x / 4) - 60, window.getView().getSize().y / 4);
	int yMult = 1;
	int count = 1;
	int cnt = count;

	sf::FloatRect currentStageSelBounds; // Read as Current stage select bounds
	for (Script& i : scripts)
	{
		// Ignore Game menu script 
		if (i.getScriptEnumVal() == ResourceHandler::Scripts::GAME_MENU)
		{
			cnt++;
			continue;
		}

		// Image display
		sf::RectangleShape frame(sf::Vector2f(window.getView().getSize().x / 8, window.getView().getSize().y / 8));
		frame.setFillColor(sf::Color(53, 24, 52));
		frame.setPosition((frameStartPos.x * count), frameStartPos.y * yMult);
		window.draw(frame);

		// Text Under image
		sf::Text txtName;
		txtName.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
		txtName.setString(i.getScriptTitle());
		txtName.setCharacterSize(20);
		txtName.setColor(sf::Color::White);
		txtName.setPosition(frame.getPosition().x, frame.getPosition().y + frame.getSize().y);
		window.draw(txtName);

		if (getStageSelectOption() == cnt) currentStageSelBounds = txtName.getGlobalBounds(); // Get current selected stage's text bounds

		count++;
		if (count % 4 == 1)
		{
			count = 1;
			yMult++;
		}
		cnt++;
	}
	numStages = cnt - 1; // Set number of stages

	// Draw Stage select overlay
	sf::RectangleShape sh = sf::RectangleShape();
	sh.setFillColor(sf::Color(255, 255, 255, 150));
	sh.setSize(sf::Vector2f(currentStageSelBounds.width + 20, currentStageSelBounds.height / 2));
	sh.setPosition(currentStageSelBounds.left - 10, currentStageSelBounds.top + (currentStageSelBounds.height / 4));
	window.draw(sh);
}

/////////////////////////////////////////////
///////////Credits draw function/////////////
/////////////////////////////////////////////
void Menu::drawCredits()
{
	// Draw Simple credits

	std::map<std::string, std::list<std::string>> credMap = resourceHandler->getCredits();

	int startX = window.getView().getSize().x / 2;
	int startY = window.getView().getSize().y / 4;

	// Iterate though all of the credits categories
	for (auto& credCats : credMap)
	{

		sf::Text txtCredCat;
		txtCredCat.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
		txtCredCat.setString(credCats.first);
		txtCredCat.setCharacterSize(30);
		txtCredCat.setColor(sf::Color::White);
		txtCredCat.setPosition(startX - (txtCredCat.getGlobalBounds().width / 2), startY);
		window.draw(txtCredCat);
		startY += 40;

		// Iterate through all of the credits for each of the categories
		for (std::string & credIt : credCats.second)
		{
			sf::Text txtCred;
			txtCred.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
			txtCred.setString(credIt);
			txtCred.setCharacterSize(20);
			txtCred.setColor(sf::Color::White);
			txtCred.setPosition(startX - (txtCred.getGlobalBounds().width / 2), startY);
			window.draw(txtCred);
			startY += 25;
		}

	}

	/*sf::Text txtCredits;
	txtCredits.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	txtCredits.setString(i.getScriptTitle());
	txtCredits.setCharacterSize(20);
	txtCredits.setColor(sf::Color::White);
	txtCredits.setPosition(frame.getPosition().x, frame.getPosition().y + frame.getSize().y);
	window.draw(txtName);*/

}

/////////////////////////////////////////////
///////////////Draw Pause////////////////////
/////////////////////////////////////////////
void Menu::drawPause(int xOffSet, int yOffset)
{
	drawOptions(state, xOffSet, yOffset, sf::Color::White);

}


/////////////////////////////////////////////
/////////////Draw Highscore//////////////////
/////////////////////////////////////////////
void Menu::drawHighScore()
{
	std::map<ResourceHandler::Scripts, std::list<std::shared_ptr<HighScoreItem>>> hScore = resourceHandler->getHighScores();
	int xPos = window.getView().getSize().x / 8;
	int yPos = window.getView().getSize().y / 4;

	sf::Text txtScore;
	txtScore.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	txtScore.setString(sf::String("Stage \t Playername \t Score \t Date"));
	txtScore.setPosition(xPos, yPos);

	window.draw(txtScore);


	for (auto & i : hScore)
	{
		std::shared_ptr<HighScoreItem> item = i.second.front();

		yPos += 50;

		sf::Text txtScore;
		txtScore.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
		txtScore.setString(sf::String(std::to_string((int)item->stage) + "\t\t\t\t\t" + item->playerName + "\t\t\t\t" + std::to_string((int)item->score) + "\t" + item->date));
		txtScore.setPosition(xPos, yPos);

		window.draw(txtScore);

	}

}
