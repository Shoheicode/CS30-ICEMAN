#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>
#include <stdlib.h> 
#include <vector>
#include <cmath>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
        : GameWorld(assetDir)
    {
    }

	~StudentWorld() {
		for (int i = 0; i <= 59; i++) {
			for (int j = 0; j <= 64; j++) {
				delete iceMap.at(i).at(j); //deletes every ice in vector;
			}
		}
		iceMap.clear();
		//Delete all the actors in character list
		for (Actor* a : characterList) {
			Actor* temp = a;
			a = nullptr;
			delete temp;
		}
		characterList.clear();

	}

    /*
    init() method must:

    A. Initialize the data structures used to keep track of your game’s virtual world
    
    B. Construct a new oil field that meets the requirements stated in the section below
        (filled with Ice, Barrels of oil, Boulders, Gold Nuggets, etc.)
    
    C.Allocate and insert a valid Iceman object into the game world at the proper
        location
    */

    virtual int init()
    {   
		//Number of Boulders
		int bNum = min(static_cast<int>(getLevel())/ 2 + 2, 9);

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
		
		//Updates the Textbox
		updateTextBox();

		//Goes through each character and asks if it does something
		for (Actor* a : characterList) {
			if (a->isAlive()) {
				a->doSomething();

				if (!player->isAlive()) { //Checks if player dies and returns dies
					return GWSTATUS_PLAYER_DIED;
				}
				if(completeLevel()){ // Checks if player completes level
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
    /*
    
    This method is called when Iceman:
        -Loses a life
        -Completes the current level

    */
    virtual void cleanUp()
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

	bool completeLevel() {
		return false;//for now
	}

	//Returns the characterlist
	list<Actor*>& getCharacterList() {
		return characterList;
	}

	//Returns the map
	vector<vector<Ice*>>& getMap() {
		return iceMap;
	}

private:
	Actor* a;
	vector<vector<Ice*>> iceMap; //Used to keep track of ice on map
	list<Actor*> characterList;
	IceMan* player;

	void removeDeadObjects() {

		//Check through the character list for if the actors are alive
		vector<list<Actor*>::iterator> it1;
		for (auto itr = characterList.begin(); itr != characterList.end(); itr++) {
			if ((*itr)->isAlive() == false) { //If the actors are not alive
				//Delete the Actors and set the point at it to null
				Actor* a = *itr; 
				*itr = nullptr;
				delete a;
				it1.push_back(itr);
			}
		}

		//Delete the empty spots
		for (unsigned int i = 0; i < it1.size(); i++) {
			characterList.erase(it1.at(i));
		}
		
	}

	//Creates the icemap
	void createIceMap(){
		//Spawns ices for all 60 y spots
		for (int i = 0; i <= 59; i++) {
			vector<Ice*> temp;
			//Spawns Ice for all 65 x spots
			for (int j = 0; j <= 64; j++) {
				//Checks to ensure that the game has the gap
				if (!(j <= 33 && j >= 30 && i >= 4)) {
					temp.push_back(new Ice(j, i)); //Create the ice
						// cout << 1;

				}
				else {
					temp.push_back(nullptr); //Otherwise, set empty spots to nullptr
				}
			}
			iceMap.push_back(temp); //Pushes the row into the map.
		}
	}

	void spawnBoulders(int bNum) {

		//Sets the current number of boulders to 0
		int currentNum = 0;

		//Creates boulders until the number of boulders equals the number needed for the level. 
		while (currentNum != bNum) {

			//Sets create boulder to true
			bool createBoulder = true;

			//Creates the random numbers
			int x = rand() % 61;
			int y = rand() % 37 + 20;

			//Checks to make sure boulder isn't in the gap
			if (x <= 33 && x >= 27) {
				createBoulder = false;
			}
			//otherwise
			else {
				//For each character in character list
				for (Actor* a : characterList) {
					//Check if the distance is less than 6 and if not, don't create boulder
					if (!checkDistance(a, a->getX(), a->getY(), x, y)) {
						createBoulder = false;
						break;
					}

				}
			}
			//If the distance between each object is less than 6, create boulder
			if (createBoulder) {
				//Add to character list
				characterList.push_back(new Boulder(x, y, this));

				//Clear out map for boulder
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						Ice* temp = iceMap.at(y + i).at(x + j);
						iceMap.at(y + i).at(x + j) = nullptr;
						delete temp;
					}
				}

				//Increase the number of current boulders
				currentNum++;
			}
		}

	}

	//Spawns the gold nuggets
	void spawnNuggets(int num) {
		//Creates the nuggets
		bool createNugget= false;

		//Sets the current number of nuggets created to 0
		int currentNum = 0;

		while (currentNum != num) {

			createNugget = true;

			//Generate random positions
			int x = rand() % 61;
			int y = rand() % 57;

			//Make sure the hole in the center is empty
			if (x <= 33 && x >= 27) {
				createNugget = false;
			}
			else {
				//For each of the actors in character list
				for (Actor* a : characterList) {
					//Check to ensure the distance between all the characters is not less than 6. 
					if (!checkDistance(a, a->getX(), a->getY(), x, y)) {
						createNugget = false;
						break;
					}

				}
			}
			//If create nugget is true, create a nugget and increase current number of nuggets
			if (createNugget) {

				characterList.push_back(new Gold(x, y, this));
				currentNum++;
			}
		}
	}

	//Checks the distance between 2 objects
	bool checkDistance(Actor* a, int obj1X, int obj1Y, int obj2X, int obj2Y) {
		//Checks the distnace between 2 objects using distance formula
		double distance = pow(pow(obj1X - obj2X, 2) + pow(obj1Y - obj2Y, 2), 0.5);
		
		//Returns false if the distance is less than 6
		if (distance < 6) {
			return false;
		}

		//return true if the distance is greater than or equal to 6
		return true;

	}

	

	//Updates the text box
	void updateTextBox() {
		int level = getLevel(); // Gets the current level
		int lives = getLives(); //Gets the current lives
		int health = static_cast<int>((player->getHealth()/10.0) * 100);//percent of health 
		int squirts = 0; //getSquirtsLeftInSquirtGun();
		int gold = 0;//getPlayerGoldCount();
		int barrelsLeft = 0;// getNumberOfBarrelsRemainingToBePickedUp();
		
		int sonar = 0;// getPlayerSonarChargeCount();
		int score = getScore();

		string formatedString = formatText(level, lives,health,squirts,gold,barrelsLeft, sonar, score);

		//Sets the game text to the formated text. 
		setGameStatText(formatedString);
	}

	//Formats the text to look nice
	string formatText(int level, int lives, int health, int squirts, int gold, int barrels, int sonar, int score) {
		//Converts all values into strings
		string levelString = to_string(level); 
		string livesString = to_string(lives); 
		string healthString = to_string(health);
		string squirtsString = to_string(squirts); 
		string goldString = to_string(gold);
		string barrelsString = to_string(barrels);
		string sonarString = to_string(sonar);
		string scoreString = to_string(score);

		//Creates the format for the string
		string returnString = "Lvl: " + levelString + " Lives: " + livesString + " Hlth: " + healthString + "% Wtr: " + squirtsString + " Gld: " + goldString + " Oil Left: " +
			barrelsString + " Sonar: " + sonarString + " Scr: " + scoreString;

		//Returns the string
		return returnString;
	}
};

#endif // STUDENTWORLD_H_
