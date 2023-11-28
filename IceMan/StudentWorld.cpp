#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{
	//Number of Boulders
	int bNum = min(static_cast<int>(getLevel()) / 2 + 2, 9);

	//Number of Gold Nuggets
	int gNum = max(5 - static_cast<int>(getLevel()) / 2, 2);

	//Number of Barrels of Oil
	int oNum = min(static_cast<int>(getLevel()) + 2, 21);

	srand(time(NULL));

	//Creates the player
	player = new IceMan(30, 60, this);
	characterList.push_back(player);

	//Create Ice
	createIceMap();

	//Spawns Boulders
	spawnBoulders(bNum);

	//Spawns Nuggets
	spawnNuggets(gNum);

	//Adds a protestor
	characterList.push_back(new Protester(60, 60, this));


	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	// cout << iceMap.size() << endl;

	//Updates the Textbox
	updateTextBox();

	//Goes through each character and asks if it does something
	for (Actor* a : characterList) {
		if (a->isAlive()) {
			a->doSomething();

			if (!player->isAlive()) { //Checks if player dies and returns dies
				return GWSTATUS_PLAYER_DIED;
			}
			if (completeLevel()) { // Checks if player completes level
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
	}

	//Removes all unalive objects are screen
	removeDeadObjects();

	//Asks if player is alive
	if (!player->isAlive()) {
		return GWSTATUS_PLAYER_DIED;
	}

	//if (false) {
	//    decLives();
	//}

	//If the lives equal zero, then return the player has perished
	if (getLives() == 0) {
		return GWSTATUS_PLAYER_DIED;
	}
	//Otherwise return continue game
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	//Deletes all the stuff in icemap 
	for (int i = 0; i <= 59; i++) {
		for (int j = 0; j <= 64; j++) {
			delete iceMap.at(i).at(j); //deletes every ice in vector;
		}
	}
	//Clears the vector out
	iceMap.clear();

	//Delete all the actors in character list
	for (Actor* a : characterList) {
		Actor* temp = a;
		a = nullptr;
		delete temp;
	}
	characterList.clear();
	//delete characterList;
}

// while (The player has lives left) 
// { 
// Prompt_the_user_to_start_playing(); // “press a key to start” Initialize_the_game_world(); // you’re going to write this func 
// while (The player is still alive) 
// { 
// // each pass through this loop is a tick (1/20th of a sec) 
// // you’re going to write code to do the following 
// Ask_all_actors_to_do_something(); 
// If_any_actors_died_then_delete_them_from_the_world(); 
// // we write this code to handle the animation for  
// you Animate_all_of_the_alive_actors_to_the_screen();  
// Sleep_for_50ms_to_give_the_user_time_to_react(); 
// } 
// // the player died – you’re going to write this code  
// Cleanup_all_game_world_objects(); // you’re going to write this  if (The player has more lives) 
// Prompt_the_player_to_continue(); 
// } 
// Tell_the_user_the_game_is_over(); // “game over!”; we provide this 
