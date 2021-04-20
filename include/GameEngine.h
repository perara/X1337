#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include "World.h"
#include "Menu.h"
#include "Constants.h"
#include "Renderer.h"


/// <summary>
/// GameEngine runs the game. This class serves a game loop which runs until game end. All state is handler in this class.
/// </summary>
class GameEngine{
public:
	GameEngine();


	// State getter/setter
	void setState(GameState state);
	GameState& getState();

    void runGame();

private:

	void process();
	void draw();
	void input();

	void drawOpts(); // Function which draws the mute icon


	Renderer renderer;

	std::unique_ptr<World> world;
	std::unique_ptr<Menu> menu;
	std::shared_ptr<ResourceManager> resourceHandler;

	const sf::Time timeStep;
	sf::Time elapsedTime;
	sf::Clock gameClock;
	sf::Event event;
	bool mute; // State of the sound

	// VIEWS
	sf::View mainView;
	sf::View fullView;
	sf::View menuGameDemoView;
	sf::View playerBar;

	GameState state;
	bool fullscreen;

    void gameStep();

    void CreateInputEvent(Constants::Input inputEnum);
};