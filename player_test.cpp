//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE_PLAYER
#include  <boost/test/unit_test.hpp>

#include "Player.h"
#include "World.h"
#include <iostream>




BOOST_AUTO_TEST_CASE(windowTest)
{
	std::cout << "Lol" << std::endl;
	World w;
	Player p;
	p.setPlayerPosX(100000);
	BOOST_CHECK(p.playerShape.getPosition().x < w.windowSize.x);
}