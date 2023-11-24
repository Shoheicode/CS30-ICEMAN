#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>
#include <stdlib.h> 
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

	/*
	init() method must:

	A. Initialize the data structures used to keep track of your game�s virtual world
	
	B. Construct a new oil field that meets the requirements stated in the section below
		(filled with Ice, Barrels of oil, Boulders, Gold Nuggets, etc.)
	
	C.Allocate and insert a valid Iceman object into the game world at the proper
		location
	*/

	virtual int init()
	{
		setGameStatText("HELLO");

		for (int i = 0; i <= 59; i++) {
			vector<Ice*> temp;
			for (int j = 0; j <= 64; j++) {
				if (!(j <= 33 && j >= 30 && j >= 4)) {
					temp.push_back(new Ice(j, i));
					// cout << 1;
					
				}
				else {
					//cout << 0;
					temp.push_back(nullptr);
				}
			}
			iceMap.push_back(temp);
		}
		//for (int i = 0; i <= 59; i++) {
		//	for (int j = 0; j <= 64; j++) {
		//		cout << numMap.at(i).at(j);
		//	}
		//	cout << "ENDLINE";
		//	cout << endl;
		//}

		characterList.push_back(new IceMan(30, 60, this));
		characterList.push_back(new Protester(60, 60));


		return GWSTATUS_CONTINUE_GAME;
	}

	/*
	move method() must:

	A: Update the Status Text on the top of the screen 

	B: Must ask all the active actors to do something (like move)
		-If an actor does something that makes Iceman give up, move should return GWSTATUS_PLAYER_DIED
		-If Iceman collects all the oil, move should play the sound  SOUND_FINISHED_LEVEL 
		and then return a value of GWSTATUS_FINISHED_LEVEL

	C:It must delete any actors that need be removed from the game and the STL container that tracks them
		-A Protester leaving the upper right hand corner
		-A Boulder hitting the ground
		-A Gold Nugget picked up by Iceman/Protester
		-Water Pool that has dried up
		-ETC
	
	*/

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		// cout << iceMap.size() << endl;
		
		updateTextBox();

		for (Actor* a : characterList) {
			a->doSomething();
		}

		if (false) {
			decLives();
		}

		if (getLives() == 0) {
			return GWSTATUS_PLAYER_DIED;
		}

		cout << "ENDLINE";
		cout << endl;

		return GWSTATUS_CONTINUE_GAME;
	}
	/*
	
	This method is called when Iceman:
		-Loses a life
		-Completes the current level

	*/
	virtual void cleanUp()
	{
	}

	vector<vector<Ice*>>& getMap() {
		return iceMap;
	}

private:
	Actor* a;
	vector<vector<Ice*>> iceMap;
	list<Actor*> characterList;

	void updateTextBox() {
		int level = getLevel();
		int lives = getLives();
		int health = 0;// getCurrentHealth();
		int squirts = 0; //getSquirtsLeftInSquirtGun();
		int gold = 0;//getPlayerGoldCount();
		int barrelsLeft = 0;// getNumberOfBarrelsRemainingToBePickedUp();
		
		int sonar = 0;// getPlayerSonarChargeCount();
		int score = getScore();

		string formatedString = formatText(level, lives,health,squirts,gold,barrelsLeft, sonar, score);

		setGameStatText(formatedString);
	}

	string formatText(int level, int lives, int health, int squirts, int gold, int barrels, int sonar, int score) {
		string levelString = to_string(level);
		string livesString = to_string(lives);
		string healthString = to_string(health);// getCurrentHealth();
		string squirtsString = to_string(squirts); //getSquirtsLeftInSquirtGun();
		string goldString = to_string(gold);//getPlayerGoldCount();
		string barrelsString = to_string(barrels);// getNumberOfBarrelsRemainingToBePickedUp();

		string sonarString = to_string(sonar);// getPlayerSonarChargeCount();
		string scoreString = to_string(score);

		string returnString = "Lvl: " + levelString + " Lives: " + livesString + " Hlth: " + healthString + "% Wtr: " + squirtsString + " Gld: " + goldString + " Oil Left: " +
			barrelsString + " Sonar: " + sonarString + " Scr: " + scoreString;

		return returnString;
	}
};

#endif // STUDENTWORLD_H_
