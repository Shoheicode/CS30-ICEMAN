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

		//Create Ice
		createIceMap();

		//Spawns Boulders
		spawnBoulders(bNum);

		spawnNuggets(gNum);

		int i = 0;

		//for (int i = 0; i <= 59; i++) {
		//	for (int j = 0; j <= 64; j++) {
		//		cout << numMap.at(i).at(j);
		//	}
		//	cout << "ENDLINE";
		//	cout << endl;
		//}

        //characterList.push_back(new IceMan(30, 60, this));
		player = new IceMan(30, 60, this);
		characterList.push_back(player);
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
		
		updateTextBox();

		for (Actor* a : characterList) {
			if (a->isAlive()) {
				a->doSomething();

				if (!player->isAlive()) {
					return GWSTATUS_PLAYER_DIED;
				}
				if(completeLevel()){
					return GWSTATUS_FINISHED_LEVEL;
				}
			}
		}

		removeDeadObjects();

		if (!player->isAlive()) {
			return GWSTATUS_PLAYER_DIED;
		}

        if (false) {
            decLives();
        }

        if (getLives() == 0) {
            return GWSTATUS_PLAYER_DIED;
        }

        return GWSTATUS_CONTINUE_GAME;
    }
    /*
    
    This method is called when Iceman:
        -Loses a life
        -Completes the current level

    */
    virtual void cleanUp()
    {
		for (int i = 0; i <= 59; i++) {
			for (int j = 0; j <= 64; j++) {
				delete iceMap.at(i).at(j); //deletes every ice in vector;
			}
		}
		iceMap.clear();
		cout << "ICEMAP" << iceMap.size() << endl;

		//Delete all the actors in character list
		for (Actor* a : characterList) {
			Actor* temp = a;
			a = nullptr;
			delete temp;
		}
		characterList.clear();
		cout << characterList.size() << endl;
		//delete characterList;
    }

	bool completeLevel() {
		return false;//for now
	}

	list<Actor*>& getCharacterList() {
		return characterList;
	}

	vector<vector<Ice*>>& getMap() {
		return iceMap;
	}

private:
	Actor* a;
	vector<vector<Ice*>> iceMap; //Used to keep track of ice on map
	list<Actor*> characterList;
	IceMan* player;

	void removeDeadObjects() {
		vector<list<Actor*>::iterator> it1;
		for (auto itr = characterList.begin(); itr != characterList.end(); itr++) {
			if ((*itr)->isAlive() == false) {
				Actor* a = *itr;
				*itr = nullptr;
				delete a;
				it1.push_back(itr);
			}
		}

		for (int i = 0; i < it1.size(); i++) {
			characterList.erase(it1.at(i));
		}
	}

	void createIceMap(){
		bool spawnIce = true;
		for (int i = 0; i <= 59; i++) {
			vector<Ice*> temp;
			for (int j = 0; j <= 64; j++) {
				if (!(j <= 33 && j >= 30 && i >= 4)) {
					temp.push_back(new Ice(j, i));
						// cout << 1;

				}
				else {
					temp.push_back(nullptr);
				}
			}
			iceMap.push_back(temp);
		}
	}

	void spawnBoulders(int bNum) {
		bool createBoulder = false;

		int x = rand() % 61;
		int y = rand() % 37 + 20;

		while (!createBoulder) {
			if (x <= 33 && x >= 27) {
				x = rand() % 60;
			}
			else {
				createBoulder = true;
			}
		}

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Ice* temp = iceMap.at(y + i).at(x + j);
				iceMap.at(y + i).at(x + j) = nullptr;
				delete temp;
			}
		}

		int currentNum = 0;

		characterList.push_back(new Boulder(x, y, this));

		currentNum++;

		while (currentNum != bNum) {

			createBoulder = true;

			x = rand() % 61;
			y = rand() % 37 + 20;

			if (x <= 33 && x >= 27) {
				createBoulder = false;
			}
			else {
				for (Actor* a : characterList) {
					if (!checkDistance(a, a->getX(), a->getY(), x, y)) {
						createBoulder = false;
						break;
					}

				}
			}
			if (createBoulder) {
				characterList.push_back(new Boulder(x, y, this));

				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						Ice* temp = iceMap.at(y + i).at(x + j);
						iceMap.at(y + i).at(x + j) = nullptr;
						delete temp;
					}
				}


				currentNum++;
			}
		}

	}

	void spawnNuggets(int num) {
		bool createNugget= false;

		int x = rand() % 61;
		int y = rand() % 57;

		while (!createNugget) {
			if (x <= 33 && x >= 27) {
				x = rand() % 60;
			}
			else {
				createNugget = true;
			}
		}

		int currentNum = 0;

		characterList.push_back(new Gold(x, y, this));

		currentNum++;

		while (currentNum != num) {

			createNugget = true;

			x = rand() % 61;
			y = rand() % 57;

			if (x <= 33 && x >= 27) {
				createNugget = false;
			}
			else {
				for (Actor* a : characterList) {
					if (!checkDistance(a, a->getX(), a->getY(), x, y)) {
						createNugget = false;
						break;
					}

				}
			}
			if (createNugget) {

				characterList.push_back(new Gold(x, y, this));
				currentNum++;
			}
		}
	}

	bool checkDistance(Actor* a, int obj1X, int obj1Y, int obj2X, int obj2Y) {
		double distance = pow(pow(obj1X - obj2X, 2) + pow(obj1Y - obj2Y, 2), 0.5);

		cout << a->getID() << endl;
		
		if (distance < 6) {
			return false;
		}

		cout << "DISTANCE: " << distance << endl;

		return true;

	}

	

	void updateTextBox() {
		int level = getLevel();
		int lives = getLives();
		int health = static_cast<int>((player->getHealth()/10.0) * 100);//percent of health 
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
