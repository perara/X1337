#include "Menu.h"
#include "Log.h"
#include <sstream>


Menu::Menu(sf::RenderWindow& window, GameState& state, std::shared_ptr<ResourceHandler>& resourceHandler) :
/// <summary>
/// Initializes a new instance of the <see cref="Menu"/> class.
/// </summary>
/// <param name="window">The window.</param>
/// <param name="state">The state.</param>
/// <param name="resourceHandler">The resource handler.</param>
Scene(window, resourceHandler),
state(state),
hardmodeSelected(false)
{
	this->init();
}
/// <summary>
/// Processes this instance.
/// </summary>
void Menu::process(){}


/// <summary>
/// Resets the menu instance
/// </summary>
void Menu::reset()
{
	// Stop Menu song
	resourceHandler->getSound(ResourceHandler::Sound::MENU_SONG).stop();
}

/// <summary>
/// Return the value selected on the menu weither hardmode was selected. 
/// </summary>
/// <returns>bool if hardmode is selected</returns>
bool Menu::getHardmodeSelected()
{
	return hardmodeSelected;
}

/// <summary>
/// Initializes the menu, This defines all of the Menu options in maps and adds this to an map .
/// </summary>
void Menu::init()
{

	// Retrieve all script names
	for (auto&i : resourceHandler->getScripts()) scripts.push_back(i);

	// Construct the "Main Options" map
	std::map<Menu::Options, std::string> mainMenu;
	{
		mainMenu[Menu::Options::NEW_GAME] = "New Game";
		//mainMenu[Menu::Options::LOAD_GAME] = "Load Existing Game";
		mainMenu[Menu::Options::HIGHSCORE] = "Highscore";
		mainMenu[Menu::Options::CREDITS] = "Credits";
		mainMenu[Menu::Options::EXIT_GAME] = "Exit Game";
	}

	// Construct the "Stage select Options" map
	std::map<Menu::Options, std::string> stageSelect;
	{
		stageSelect[Menu::Options::BACK] = "Back";
		stageSelect[Menu::Options::SELECT_STAGE] = "Select Stage";
	}

	// Construct the "Difficulty Options" map
	std::map<Menu::Options, std::string> difficultySelect;
	{
		difficultySelect[Menu::Options::NORMAL] = "Normal";
		difficultySelect[Menu::Options::HARD] = "HARD!!";
	}

	// Construct the "High Score Options" map
	std::map<Menu::Options, std::string> highScore;
	{
		highScore[Menu::Options::BACK] = "Back";
	}

	// Construct the "Pause Options" map
	std::map<Menu::Options, std::string> pause;
	{
		pause[Menu::Options::CONTINUE_GAME] = "Continue";
		pause[Menu::Options::TO_MAIN_MENU] = "Return to Main Menu";
	}

	// Construct the "Game Over Options" map
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

	// Checks weither current state has any options, if it does not, we set current option to the main menu.
	if (!option[state].empty())
	{
		setCurrentOption(option[state].begin()->first);
	}
	else
	{
		setCurrentOption(option[GameState::MAIN_MENU].begin()->first);
	}
}

/// <summary>
/// Updates the current option.
/// </summary>
void Menu::updateCurrentOption()
{
	this->setCurrentOption(option[state].begin()->first);
}
/////////////////////////////////////////////
////Preload the menu options into a map//////
/////////////////////////////////////////////
/// <summary>
/// Loads the menu options into the option map.
/// </summary>
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

/// <summary>
/// Input handler for menu. all events is handler here.
/// </summary>
/// <param name="event">The event</param>
/// ///////////////////////////////////////////
/// ////////Input handler for menu/////////////
/// ///////////////////////////////////////////
void Menu::input(sf::Event& event)
{

	// Handler for when clicking the UP key
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

	// Handler for when clicking the DOWN key
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

/// <summary>
/// Input handler for stage select.
/// </summary>
/// <param name="event">The event object</param>
void Menu::stageSelectInput(sf::Event& event)
{
	// Handler for when clicking the LEFT key
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

	// Handler for when clicking the RIGHT key
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
/// <summary>
/// Gets the currently selected option.
/// </summary>
/// <returns>Option as INT</returns>
int Menu::getCurrentOption()
{
	return this->currentOption;
}

/// <summary>
/// Sets the current selected option to the opt int
/// </summary>
/// <param name="opt">The option which is required selected</param>
void Menu::setCurrentOption(int opt)
{
	this->currentOption = opt;
}

/// <summary>
/// Gets the stage select option.
/// </summary>
/// <returns>Returns the selected value on stage</returns>
int Menu::getStageSelectOption()
{
	return this->stageSelectOption;
}
/// <summary>
/// Sets the stage select option.
/// </summary>
/// <param name="opt">The option which is required selected</param>
void Menu::setStageSelectOption(int opt)
{
	this->stageSelectOption = opt;
}

/////////////////////////////////////////////
///////Draw implementation for Menu//////////
/////////////////////////////////////////////
/// <summary>
/// Draws the menu
/// </summary>
void Menu::draw()
{
	// Draws the game title
	drawGameTitle();

	// A switch case which determine which menu is to be drawed.
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
/// <summary>
/// Draws the main menu.
/// </summary>
void Menu::drawMainMenu()
{
}

/////////////////////////////////////////////
////Draw menu options for current state//////
/////////////////////////////////////////////
/// <summary>
/// Draws all of the options for based on state.
/// </summary>
/// <param name="state">Current selected state..</param>
/// <param name="xOffset">The x offset position</param>
/// <param name="yOffset">The y offset position</param>
/// <param name="color">The color on the option</param>
void Menu::drawOptions(GameState state, int xOffset, int yOffset, sf::Color color)
{
	// Draw options
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
/// <summary>
/// Draws the game title.
/// </summary>
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
/// <summary>
/// Draws the stage select.
/// </summary>
void Menu::drawStageSelect()
{
	// Finds where the first frame should be drawed,
	sf::Vector2f frameStartPos((window.getView().getSize().x / 4) - 60, window.getView().getSize().y / 4);

	// Counter, and y multiplier for the frames ( this is increased when an modulo X is reached)
	int yMult = 1;
	int count = 1;
	int cnt = count;

	sf::FloatRect currentStageSelBounds; // Read as Current stage select bounds

	// Get each of the script names
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

		// This checks if the the frame width is reached (4 in a row)
		// If it is we increment the multiplier to reach next line
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
/// <summary>
/// Draws the credits.
/// </summary>
void Menu::drawCredits()
{
	// Draw Simple credits
	std::map<std::string, std::list<std::string>> credMap = resourceHandler->getCredits();

	// Start positions
	int startX = window.getView().getSize().x / 2;
	int startY = window.getView().getSize().y / 4;

	// Iterate though all of the credits categories
	for (auto& credCats : credMap)
	{
		// Draw the category
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
			// Draw credit text.
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

}

/////////////////////////////////////////////
///////////////Draw Pause////////////////////
/////////////////////////////////////////////
/// <summary>
/// Draws the pause.
/// </summary>
/// <param name="xOffSet">The x offset.</param>
/// <param name="yOffset">The y offset.</param>
void Menu::drawPause(int xOffSet, int yOffset)
{
	drawOptions(state, xOffSet, yOffset, sf::Color::White);

}


/////////////////////////////////////////////
/////////////Draw Highscore//////////////////
/////////////////////////////////////////////
/// <summary>
/// Draws the high score.
/// </summary>
void Menu::drawHighScore()
{
	// Get all of the highscores (Bad practice, needs work. Should not retrieve every frame)
	std::map<ResourceHandler::Scripts, std::list<std::shared_ptr<HighScoreItem>>> hScore = resourceHandler->getHighScores();

	// Determine the start y and x pos
	int xPos = window.getView().getSize().x / 8;
	int yPos = window.getView().getSize().y / 4;

	// Draw the score header
	sf::Text txtScore;
	txtScore.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	txtScore.setString(sf::String("Stage \t Playername \t Score \t Date"));
	txtScore.setPosition(xPos, yPos);
	window.draw(txtScore);

	// Draw each of top 1 highscores
	for (auto & i : hScore)
	{
		std::shared_ptr<HighScoreItem> item = i.second.front();
		yPos += 50;

		// Draw the text
		sf::Text txtScore;
		txtScore.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
		txtScore.setString(sf::String(std::to_string((int)item->stage) + "\t\t\t\t\t" + item->playerName + "\t\t\t\t" + std::to_string((int)item->score) + "\t" + item->date));
		txtScore.setPosition(xPos, yPos);
		window.draw(txtScore);
	}

}
